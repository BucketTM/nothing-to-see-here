/**
 * @file     clkhandler.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013, 2024.  All rights reserved. */

#include <xinu.h>

int resched(void);
void wakeup(void);

/**
 * @ingroup timer
 *
 * Interrupt handler function for the timer interrupt.  This schedules a new
 * timer interrupt to occur at some point in the future, then updates ::clktime
 * and ::clkticks, then wakes sleeping processes if there are any, otherwise
 * reschedules the processor.
 */
interrupt clkhandler(void)
{
    volatile struct timer *t = (volatile struct timer *)TIMER_BASE;

    /* Another clock tick passes. */
    clkticks++;

    // Clear the pending interrupt by setting it to 1
    t->irq_sta = TMR0_IRQ_PEND;

    // Reload the timer
    t->t0_ctrl |= TMR0_RELOAD;

    // Wait until bit is cleared
    while (t->t0_ctrl & TMR0_RELOAD)
    {

    }

    // Reenable the timer
    t->t0_ctrl |= TMR0_EN;

    // Check if 1000 milliseconds have passed
    if (clkticks == CLKTICKS_PER_SEC)
    {
        clktime++;
        clkticks = 0;
//      kputc('.');
    }

    /* If sleepq is not empty, decrement first key.   */
    /* If key reaches zero, call wakeup.              */
    if (nonempty(sleepq) && (--firstkey(sleepq) <= 0))
    {
        wakeup();
    }
    else
    {
#if PREEMPT
        /* check to see if this proc should be preempted. */
        preempt = preempt - 1;
        if (preempt <= 0)
        {
            //      kputc('+');
            resched();
        }
#endif
    }

}
