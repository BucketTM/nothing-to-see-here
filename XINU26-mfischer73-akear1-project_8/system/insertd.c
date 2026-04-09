/**
 * @file insertd.c
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
 * Insert a process in delta queue in ascending order
 * @param pid    proc id to insert
 * @param q      queue in which prcoess should be inserted
 * @param key    delta key
 * @return OK
 */
ulong insertd(pid_typ pid, qid_typ q, ulong key)
{
    ulong next;                 /* runs through list                  */
    ulong prev;                 /* follows next through list          */

    if (isbadqueue(q) || isbadpid(pid))
    {
        return SYSERR;
    }

    prev = queuehead(q);
    next = queuetab[queuehead(q)].next;
    while ((queuetab[next].key <= key) && (next != queuetail(q)))
    {
        key -= queuetab[next].key;
        prev = next;
        next = queuetab[next].next;
    }
    queuetab[pid].next = next;
    queuetab[pid].prev = prev;
    queuetab[pid].key = key;
    queuetab[prev].next = pid;
    queuetab[next].prev = pid;
    if (next != queuetail(q))
    {
        queuetab[next].key -= key;
    }

    return OK;
}
