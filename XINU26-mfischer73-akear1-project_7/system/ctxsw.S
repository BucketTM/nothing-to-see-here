/**
 * @file     ctxsw.s
 * @provides ctxsw
 * Credit to The C Programming Language - 2nd Edition, ChatGPT, and Google Gemini
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2025.  All rights reserved. */

#include <riscv.h>

.section .ctxswsec
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldcontext, &newcontext)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the program counter for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldcontext address of outgoing process context save area
 * @param  &newcontext address of incoming process context save area
 * @return special case -- see above
 */
	.func ctxsw
ctxsw:

// TODO: Save process state, including at least the 
//	callee-save ("non-volatile") registers.

sd a0, CTX_A0*8(a0)
sd a1, CTX_A1*8(a0)
sd a2, CTX_A2*8(a0)
sd a3, CTX_A3*8(a0)
sd a4, CTX_A4*8(a0)
sd a5, CTX_A5*8(a0)
sd a6, CTX_A6*8(a0)
sd a7, CTX_A7*8(a0)

sd s0,  CTX_S0*8(a0)
sd s1,  CTX_S1*8(a0)
sd s2,  CTX_S2*8(a0)
sd s3,  CTX_S3*8(a0)
sd s4,  CTX_S4*8(a0)
sd s5,  CTX_S5*8(a0)
sd s6,  CTX_S6*8(a0)
sd s7,  CTX_S7*8(a0)
sd s8,  CTX_S8*8(a0)
sd s9,  CTX_S9*8(a0)
sd s10, CTX_S10*8(a0)
sd s11, CTX_S11*8(a0)

sd ra, CTX_RA*8(a0)
sd sp, CTX_SP*8(a0)
sd gp, CTX_GP*8(a0)
sd tp, CTX_TP*8(a0)
sd t0, CTX_T0*8(a0)
sd t1, CTX_T1*8(a0)
sd t2, CTX_T2*8(a0)
sd t3, CTX_T3*8(a0)
sd t4, CTX_T4*8(a0)
sd t5, CTX_T5*8(a0)
sd t6, CTX_T6*8(a0)

sd ra, CTX_PC*8(a0)

// TODO: Switch from old process stack to new process stack.
ld sp, CTX_SP*8(a1)

// TODO: Restore process state, including at least the
//	callee-save ("non-volatile") registers.
//	These call and restore segments must match the register data
//	layout you choose in create().
// TODO: Jump to next function.
//	Note: there are two important values at play here --
//	the address of the process's function that we want to end up in
//	once we leave this ctxsw(), and the value we want to have in the
//	return address register.  Those are almost always the same value, except
//	for the very first time we ctxsw() to a new process, when they
//	are different.  See create() for more details.

ld t0, CTX_PC*8(a1)
ld ra, CTX_RA*8(a1)

ld s0,  CTX_S0*8(a1)
ld s1,  CTX_S1*8(a1)
ld s2,  CTX_S2*8(a1)
ld s3,  CTX_S3*8(a1)
ld s4,  CTX_S4*8(a1)
ld s5,  CTX_S5*8(a1)
ld s6,  CTX_S6*8(a1)
ld s7,  CTX_S7*8(a1)
ld s8,  CTX_S8*8(a1)
ld s9,  CTX_S9*8(a1)
ld s10, CTX_S10*8(a1)
ld s11, CTX_S11*8(a1)

ld gp, CTX_GP*8(a1)
ld tp, CTX_TP*8(a1)
ld t1, CTX_T1*8(a1)
ld t2, CTX_T2*8(a1)
ld t3, CTX_T3*8(a1)
ld t4, CTX_T4*8(a1)
ld t5, CTX_T5*8(a1)
ld t6, CTX_T6*8(a1)

ld a0, CTX_A0*8(a1)
ld a2, CTX_A2*8(a1)
ld a3, CTX_A3*8(a1)
ld a4, CTX_A4*8(a1)
ld a5, CTX_A5*8(a1)
ld a6, CTX_A6*8(a1)
ld a7, CTX_A7*8(a1)
ld a1, CTX_A1*8(a1)

/* Project 7: install target page table before returning */
ld t5, CTX_KERNSATP*8(a1)
sfence.vma zero, zero
csrw satp, t5
sfence.vma zero, zero

beq t0, ra, switch

li t4, SSTATUS_S_MODE
csrc sstatus, t4
csrw sepc, t0
sret

switch:
	ret

	.end ctxsw
