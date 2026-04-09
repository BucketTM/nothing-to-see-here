/**
 * @file signal.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <proc.h>
#include <semaphore.h>
#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Signal a semaphore, releasing up to one waiting process.
 *
 * signal() may reschedule the currently running process.  As a result, signal()
 * should not be called from non-reentrant interrupt handlers unless ::resdefer
 * is set to a positive value at the start of the interrupt handler.
 *
 * @param sem
 *      Semaphore to signal.
 *
 * @return
 *      ::OK on success, ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore.
 */
syscall signal(semaphore sem)
{
    register struct sement *semptr;

    // TODO: signal the semaphore


	//if value of semaphore not in semaphore table, break(error)
	//return SYSERROR instead of OK
	//
	//handle empty list, it shouldn't ready a new process, but does still increment
	//if isempty(semptr->queue){count++}
	//
	if (semtab[sement]==NULL){
		return SYSERR;
	}
	if (isempty(semptr->queue)){
		;
	}

	else{

		ready(dequeue(semptr->queue),RESCH_YES);  
	}
	semptr->count++;	
	return OK;
}
