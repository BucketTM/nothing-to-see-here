/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 6
 */
/* Embedded XINU, Copyright (C) 2008,2024.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);

/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */
syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */
    int highest_prio;

    oldproc = &proctab[currpid];

#if AGING
    // TODO: Implement aging.
    //
    // Reference include/clock.h to find more information
    // about the quantums and how aging should behave.
    promote_medium--;
    if (promote_medium == 0)
    {
        promote_medium = QUANTUM;

        if (nonempty(readylist[PRIORITY_MED]))
        {
            pid_typ pid = dequeue(readylist[PRIORITY_MED]);
            proctab[pid].priority = PRIORITY_HIGH;
            enqueue(pid, readylist[PRIORITY_HIGH]);

            promote_low--;
            if (promote_low == 0)
            {
                promote_low = QUANTUM;

                if (nonempty(readylist[PRIORITY_LOW]))
                {
                    pid_typ lowpid = dequeue(readylist[PRIORITY_LOW]);
                    proctab[lowpid].priority = PRIORITY_MED;
                    enqueue(lowpid, readylist[PRIORITY_MED]);
                }
            }
        }
    }
#endif

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist[oldproc->priority]);
    }

    /* remove first process in highest priority ready queue */
    // determine queue to pick from
    for (highest_prio = PRIORITY_HIGH; highest_prio >= PRIORITY_LOW; highest_prio--)
        if (nonempty(readylist[highest_prio]))
            break;

    currpid = dequeue(readylist[highest_prio]);
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running */

#if PREEMPT
    preempt = QUANTUM;
#endif

    ctxsw(&oldproc->ctx, &newproc->ctx);

    /* The OLD process returns here when resumed. */
    return OK;
}
