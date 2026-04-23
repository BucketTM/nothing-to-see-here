/**
 * @file xsh_test.c
 * @provides xsh_test
 */
/* Embedded XINU, Copyright (C) 2009. All rights reserved. */

#include <xinu.h>

/**
 * Shell command (test) is testing hook.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_test(int nargs, char *args[])
{
    int fd;
    int i;
    char *name = "testf";
    char msg[] = "hello";

    //TODO: Test your O/S.
    printf("Starting filesystem test.\n");

    fd = fileCreate(name);
    if (SYSERR == fd)
    {
        printf("fileCreate failed.\n");
        return SYSERR;
    }
    printf("Created file '%s' with fd %d.\n", name, fd);

    for (i = 0; msg[i] != '\0'; i++)
    {
        if (SYSERR == filePutChar(fd, msg[i]))
        {
            printf("filePutChar failed.\n");
            fileClose(fd);
            return SYSERR;
        }
    }
    printf("Wrote \"%s\" to file.\n", msg);

    if (SYSERR == fileClose(fd))
    {
        printf("fileClose failed.\n");
        return SYSERR;
    }
    printf("Closed file.\n");

    fd = fileOpen(name);
    if (SYSERR == fd)
    {
        printf("fileOpen failed.\n");
        return SYSERR;
    }
    printf("Re-opened file '%s' with fd %d.\n", name, fd);

    if (SYSERR == fileDelete(fd))
    {
        printf("fileDelete failed.\n");
        fileClose(fd);
        return SYSERR;
    }
    printf("Deleted file.\n");

    fd = fileOpen(name);
    if (SYSERR == fd)
    {
        printf("Confirmed deleted file can no longer be opened.\n");
    }
    else
    {
        printf("ERROR: deleted file was opened again.\n");
        fileClose(fd);
        return SYSERR;
    }

    printf("Filesystem test complete.\n");
    return OK;
}
