/**
 * @file signaln.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <proc.h>
#include <semaphore.h>
#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Signal a semaphore @p count times, releasing @p count waiting threads.
 *
 * signaln() may reschedule the currently running process.  As a result,
 * signaln() should not be called from non-reentrant interrupt handlers unless
 * ::resdefer is set to a positive value at the start of the interrupt handler.
 *
 * @param sem
 *      Semaphore to signal.
 * @param count
 *      Number of times to signal, which must be positive.
 *
 * @return
 *      ::OK on success, ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore of if @p count was not positive.
 */
syscall signaln(semaphore sem, int count)
{
    register struct sement *semptr;

    // TODO: signal the semaphore with n

    return OK;
}
