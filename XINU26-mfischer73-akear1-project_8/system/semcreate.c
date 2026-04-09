/**
 * @file semcreate.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <interrupt.h>
#include <semaphore.h>
#include <xinu.h>

static semaphore semalloc(void);

/**
 * @ingroup semaphores
 *
 * Creates a semaphore that initially has the specified count.
 *
 * @param count
 *      Initial count of the semaphore (often the number of some resource that
 *      is available).  Must be non-negative.
 *
 * @return
 *      On success, returns the new semaphore; otherwise returns ::SYSERR.  The
 *      new semaphore must be freed with semfree() when no longer needed.  This
 *      function can only fail if the system is out of semaphores or if @p count
 *      was negative.
 */
semaphore semcreate(int count)
{
    semaphore sem;

    if (count < 0)              /* Don't allow starting with negative count.  */
    {
        return SYSERR;
    }

    sem = semalloc();           /* Allocate semaphore.  */
    if (SYSERR != sem)          /* If semaphore was allocated, set count.  */
    {

        semtab[sem].count = count;
    }
    return OK;
}

semaphore semalloc(void)
{
    // kprintf("Semalloc call\r\n");
    int i;
    static int nextsem = 0;

    /* check all NSEM slots, starting at 1 past the last slot searched.  */
    for (i = 0; i < NSEM; i++)
    {
        nextsem = (nextsem + 1) % NSEM;
        if (SFREE == semtab[nextsem].state)
        {
            semtab[nextsem].state = SUSED;
            // kprintf("next: %d\r\n", nextsem);
            return nextsem;
        }
    }

    // kprintf("returns a syserr\r\n");
    return SYSERR;
}
