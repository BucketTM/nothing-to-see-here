/**
 * @file sleep.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <kernel.h>
#include <stddef.h>
#include <interrupt.h>
#include <proc.h>
#include <queue.h>
#include <clock.h>

/**
 * Yields the processor for the specified number of milliseconds, allowing other
 * processes to be scheduled.
 *
 * @param ms number of milliseconds to sleep
 *
 * @return
 *      If successful, the process will sleep for the specified number of
 *      milliseconds, then return ::OK.  Otherwise, ::SYSERR will be returned.
 *      If a system timer is not supported, ::SYSERR will always returned.
 */
syscall sleep(ulong ms)
{
#if CLOCK

    // TODO: sleep the process

    return OK;
#else
    return SYSERR;
#endif
}

/**
 * Wakeup and ready all procs that have no more time to sleep
 */
ulong wakeup(void)
{
#if CLOCK
    irqmask im;

    im = disable();

    while (nonempty(sleepq) && firstkey(sleepq) <= 0)
    {
        ready(dequeue(sleepq), RESCHED_NO);
    }

    restore(im);
    return OK;
#else
    return SYSERR;
#endif
}
