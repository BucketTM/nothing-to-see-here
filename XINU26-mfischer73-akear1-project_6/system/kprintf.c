/** * COSC 3250 - Project 3 
* A UART Console I/O for Xinu.
* @authors Arthur Kear and Maggie Fischer 
* Credit to The C Programming Language - 2nd Edition, Google Gemini and ChatGPT for help with git, C issues and formatting. 
* Instructor Dennis Brylow 
* TA-BOT:MAILTO arthur.kear@marquette.edu */
#include <xinu.h>

#define UNGETMAX 10              /* Can un-get at most 10 characters. */
static unsigned char ungetArray[UNGETMAX];
unsigned int bufp;

syscall kgetc()
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    if (bufp > 0) {
        bufp--;
        return ungetArray[bufp];
    }

    while ((regptr->lsr & UART_LSR_DR) == 0) {
        ;
    }

    unsigned char c = regptr->rbr;

    return c;
}

/**
 * kcheckc - check to see if a character is available.
 * @return true if a character is available, false otherwise.
 */
syscall kcheckc(void)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    /* Check if the software unget buffer has data OR if the 
       UART hardware reports data is ready in the LSR */
    if (bufp > 0 || (regptr->lsr & UART_LSR_DR)) {
        return TRUE;
    }

    return FALSE;
}

/**
 * kungetc - put a serial character "back" into a local buffer.
 * @param c character to unget.
 * @return c on success, SYSERR on failure.
 */
syscall kungetc(unsigned char c)
{
    /* Check if there is room in the buffer */
    if (bufp >= UNGETMAX) {
        return SYSERR;
    }

    /* Put the character in the next available slot */
    ungetArray[bufp] = c;

    /* Increment the buffer pointer */
    bufp++;

    return c;
}

syscall kputc(uchar c)
{
    volatile struct ns16550_uart_csreg *regptr;
    regptr = (struct ns16550_uart_csreg *)UART_BASE;

    /* Wait until the Transmitter Holding Register (THR) is empty */
    while ((regptr->lsr & UART_LSR_THRE) == 0) {
        ; /* Do nothing until the UART is ready */
    }

    /* Send the character by writing it to the THR */
    regptr->thr = c;

    return c;
}

syscall kprintf(const char *format, ...)
{
    int retval;
    va_list ap;

    va_start(ap, format);
    retval = _doprnt(format, ap, (int (*)(long, long))kputc, 0);
    va_end(ap);
    return retval;
}
