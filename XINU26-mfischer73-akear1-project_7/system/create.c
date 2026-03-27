
/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, uint priority, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */
    page stackpage;             /* physical stack page          */

    if (ssize < MINSTK)
        ssize = MINSTK;

    ssize = (ulong)((((ulong)(ssize + 3)) >> 2) << 2);

    /* Project 7: allocate a physical page for the user stack */
    stackpage = pgalloc();
    if ((void *)stackpage == (void *)SYSERR || stackpage == NULL)
    {
        return SYSERR;
    }

    memset(stackpage, 0, PAGE_SIZE);
    saddr = (ulong *)((ulong)stackpage + PAGE_SIZE);

    /* allocate new pid */
    pid = newpid();

    /* a little error checking */
    if (SYSERR == pid)
    {
        pgfree(stackpage);
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];

    // TODO: Setup PCB entry for new process.
    ppcb->state = PRSUSP;
    ppcb->priority = priority;
    ppcb->stklen = PAGE_SIZE;
    ppcb->stkbase = (ulong *)stackpage;
    strncpy(ppcb->name, name, PNMLEN - 1);
    ppcb->name[PNMLEN - 1] = '\0';

    /* Create user page table and swap mapping */
    ppcb->pagetable = vm_userinit(pid, stackpage);
    if (ppcb->pagetable == NULL)
    {
        pgfree(stackpage);
        ppcb->state = PRFREE;
        numproc--;
        return SYSERR;
    }

    /* Initialize stack with accounting block. */
    *--saddr = STACKMAGIC;
    *--saddr = pid;
    *--saddr = ppcb->stklen;
    *--saddr = (ulong)ppcb->stkbase;

    /* Handle variable number of arguments passed to starting function   */
    if (nargs)
    {
        pads = ((nargs - 1) / ARG_REG_MAX) * ARG_REG_MAX;
    }

    /* If more than 8 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }

    // TODO: Initialize process context.
    //
    // TODO:  Place arguments into context and/or activation record.
    //        See K&R 7.3 for example using va_start, va_arg and
    //        va_end macros for variable argument functions.

    {
        ulong a[ARG_REG_MAX] = {0,0,0,0,0,0,0,0};
        ulong *argsp = saddr;

        va_start(ap, nargs);

        for (i = 0; (i < nargs) && (i < ARG_REG_MAX); i++)
        {
            a[i] = va_arg(ap, ulong);
        }

        if (nargs > ARG_REG_MAX)
        {
            ulong extra = nargs - ARG_REG_MAX;
            for (i = 0; i < extra; i++)
            {
                argsp[i] = va_arg(ap, ulong);
            }
        }

        va_end(ap);

        memset(ppcb->ctx, 0, sizeof(ppcb->ctx));

        ppcb->ctx[CTX_A0] = a[0];
        ppcb->ctx[CTX_A1] = a[1];
        ppcb->ctx[CTX_A2] = a[2];
        ppcb->ctx[CTX_A3] = a[3];
        ppcb->ctx[CTX_A4] = a[4];
        ppcb->ctx[CTX_A5] = a[5];
        ppcb->ctx[CTX_A6] = a[6];
        ppcb->ctx[CTX_A7] = a[7];

        /* virtual user stack top, adjusted by how far argsp is below top */
        ppcb->ctx[CTX_SP] = PROCSTACKVADDR + ((ulong)argsp - (ulong)stackpage);
        ppcb->ctx[CTX_RA] = (ulong)userret;
        ppcb->ctx[CTX_PC] = (ulong)funcaddr;

        /* process page table satp and SP offset for trap path */
        ppcb->ctx[CTX_KERNSATP] = MAKE_SATP(pid, ppcb->pagetable);
        ppcb->ctx[CTX_SPOFFSET] = ppcb->swaparea[CTX_SPOFFSET];
    }

    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    // ASSIGNMENT 5 TODO: Replace the call to kill(); with user_kill();
    // when you believe your trap handler is working in Assignment 5
    // user_kill();
    kill(currpid);
}
