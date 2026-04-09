/**
 * @file wait.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009.  All rights reserved. */

#include <proc.h>
#include <semaphore.h>
#include <xinu.h>

/**
 * @ingroup semaphores
 *
 * Wait on a semaphore.
 *
 * If the semaphore's count is positive, it will be decremented and this
 * function will return immediately.  Otherwise, the currently running process
 * will be put to sleep until the semaphore is signaled with signal() or
 * signaln(), or freed with semfree().
 *
 * @param sem
 *      Semaphore to wait on.
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail if @p
 *      sem did not specify a valid semaphore.
 */
syscall wait(semaphore sem)
{
    
register struct sement *semptr;
    register struct pentry *ppcb;

semptr->count--;
    if(semptr->count < 0){
ppcb = &proctab[currpid];
	    (ppcb->state)=PRWAIT;
enqueue(currpid,semptr->queue);
resched();
}else{
	return SYSERR;
}
    return OK;
}
