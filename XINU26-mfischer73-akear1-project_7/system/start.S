/* *
 * @file start.S
 *
 * Initialization code for Embedded Xinu on the RISC-V Virt device.
 *
 * Embedded Xinu, Copyright (C) 2022.  All rights reserved.
 *
 */
#include <riscv.h>

.section .init
.globl   _start

/* _start:  Entry point of the Xinu kernel.  This will be the very first
/* byte of the kernel image and on the will be loaded at address 0x80000000.  */
.func _start

_start:
	// Continue at reset handler.
	j reset_handler
	.endfunc

	.section .text
/* reset_handler: Reset handler routine executed to start up the kernel,
 * when the processor is reset, or (currently) when an unhandled
* exception occurs. */
	.func reset_handler

reset_handler:

	la  a1, _end
	li  a3, NULLUSER_STACK_SIZE
	add sp, a1, a3

	la   a0, _bss
	la   t0, _end
	sub  a1, t0, a0
	call bzero

	la a4, memheap
	sw sp, 0(a4)

	// Tell the CPU to handle all exceptions in S-Mode
	li    t1, RISCV_ENABLE_ALL_SMODE_INTR
	csrrs x0, sie, t1

	// Allow S mode to access U mode pages
	li    t1, RISCV_MSTATUS_SUM
	csrrs x0, sstatus, t1

	// Loads address of the function interrupt in t1
	la t1, interrupt

	// Sets stvec to the address of interrupt
	csrw stvec, t1

	sfence.vma zero, zero

	j nulluser
	.endfunc
