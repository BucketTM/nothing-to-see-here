/**
 * @file clock.h
 * Definitions relating to the hardware clock and the Xinu clock interrupt.
 */
/* Embedded Xinu, Copyright (C) 2009, 2013, 2023.  All rights reserved. */

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <stddef.h>

/**
 * @ingroup timer
 *
 * Timer resolution.  This refers to the maximum number of times that Xinu
 * schedules the timer interrupt per second, @a not to the number of native
 * clock cycles per second.
 */
#define CLKTICKS_PER_SEC  1000

#define QUANTUM 	3       /* clock ticks until preemption */

/**
 * The "preempt" quantum indicates when the scheduler should preempt.
 * For example, if the QUANTUM is 3, then after 3 clock ticks, the
 * scheduler will preempt and then reset the value of the preempt
 * quantum to QUANTUM.
 *
 * The "promote_medium" quantum indicates when a medium priority process ("PRIORITY_MED")
 * should get promoted and become a high priority process ("PRIORITY_HIGH").
 * The promote_medium quantum should decrement when resched() is called and AGING is TRUE.
 *
 * The "promote_low" quantum indicates when a low priority process ("PRIORITY_LOW")
 * should get promoted and become a medium prority process. This quantum should be decremented
 * every time a medium priority process is promoted.
 *
 *
 * NOTE1: The "promote_medium" and "promote_low" quantum's should only be used
 *        when AGING is set to TRUE.
 *
 * NOTE2: All of theses quantum's have a reset value of QUANTUM (#defined above this).
 *        Therefore, when these reach 0, they should be reset to that value.
 */
extern volatile ulong preempt;
extern volatile ulong promote_medium;
extern volatile ulong promote_low;

/**
 * clkticks and clktime keep track of information about the clock.
 */
extern volatile ulong clkticks;
extern volatile ulong clktime;

/* Clock function prototypes. */
void clkinit(void);
interrupt clkhandler(void);

#endif                          /* _CLOCK_H_ */
