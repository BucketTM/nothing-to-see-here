/**
 * @file dispatch.c
 * @provides dispatch
 *
 */
/* Embedded XINU, Copyright (C) 2008, 2023.  All rights reserved. */


#include <xinu.h>

/**
 * @ingroup process
 * Dispatch the trap or exception handler, called via interrupt.S
 * @param cause  The value of the scause register 
 * @param stval  The value of the stval register  
 * @param frame  The stack pointer of the process that caused the interupt 
 * @param program_counter  The value of the sepc register 
 */

void dispatch(ulong cause, ulong val, ulong *frame, ulong *program_counter) {
    ulong swi_opcode;
    ulong pc;

    /* interrupt.S passes sepc in a3 as a VALUE, but interrupt.h declares it as a pointer.
     * Convert once here so we can do correct arithmetic. */
    pc = (ulong)program_counter;
    
    if((long)cause > 0) {
        cause = cause << 1;
        cause = cause >> 1;

       /**
	* TODO:
	* Check to ensure the trap is an environment call from U-Mode
	* Find the system call number that's triggered
	* Pass the system call number and any arguments into syscall_dispatch.
	* Make sure to set the return value in the appropriate spot.
	* Update the program counter appropriately with set_sepc
	*
	* If the trap is not an environment call from U-Mode call xtrap
	*/
	
	if (cause == E_ENVCALL_FROM_UMODE) { //this is the inturrupt for cause register
		/* syscall number is placed in a7 by the SYSCALL() macro */
		swi_opcode = frame[CTX_A7];

		/* args begin at a0 in the saved trap frame */
		frame[CTX_A0] = syscall_dispatch((int)swi_opcode, &frame[CTX_A0]);

		/* advance past the ecall instruction */
		set_sepc(pc + 4);
	}
	else
	{
		/* for non-ecall traps */
		xtrap(frame, cause, val, (ulong *)pc);
	}
    }
    else
    {
        /* Interrupts (MSB set in scause) are not handled here for this assignment. */
        cause = cause << 1;
        cause = cause >> 1;
        xtrap(frame, cause, val, (ulong *)pc);
    }
}
