/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:
 *
 * TA-BOT:MAILTO
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar c;

    kprintf("\r\n===TEST BEGIN===\r\n\n");

    c = kgetc();
    switch (c)
    {

    case '0':

        // TODO: test your operating system

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
