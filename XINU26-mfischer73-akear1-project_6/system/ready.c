/**
 * @file ready.c
 * @provides ready
 *
 * COSC 3250 Assignment 6
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/**
 * Make a process eligible for CPU service.
 * @param pid process id of the process to move to the ready state
 * @param resch if TRUE, reschedule will be called
 * @return OK if the process has been added to the ready list, else SYSERR
 */
syscall ready(pid_typ pid, bool resch)
{
    register pcb *ppcb;
    ASSERT(!isbadpid(pid));

    ppcb = &proctab[pid];
    ppcb->state = PRREADY;

    // TODO: enqueue the process onto the proper readylist based on priority.

    if (resch)
    {
        resched();
    }
    return OK;
}
