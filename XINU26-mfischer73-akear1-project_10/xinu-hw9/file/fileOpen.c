/* fileOpen.c - fileOpen */
/* Copyright (C) 2007, Marquette University. All rights reserved. */
/*
 * Modified by
 *
 * and
 *
 */

#include <file.h>

/*------------------------------------------------------------------------
 * fileOpen - Open a file, return a file descriptor.
 *------------------------------------------------------------------------
 */
devcall fileOpen(char *name)
{
    struct dirblock *dirblk;
    struct filenode *fnode;
    int fd = 0;
    int i;
    int curblock;

    // A little error checking.
    if ((NULL == supertab) || (NULL == filetab) || (NULL == name))
    {
        return SYSERR;
    }

    // Run through the file descriptors, looking for a match.
    dirblk = supertab->sb_dirlst;
    while (dirblk != NULL)
    {
        for (i = 0; i < DIRENTRIES; i++, fd++)
        {
            fnode = &dirblk->db_fnodes[i];

            if ((FILE_USED & fnode->fn_state) &&
                (0 == strncmp(fnode->fn_name, name, FNAMLEN)))
            {
                // Check to see if file is already open.
                if (FILE_OPEN & fnode->fn_state)
                {
                    return fd;
                }

                // Open file by reading disk block into memory.
                curblock = (int)fnode->fn_blocknum;
                fnode->fn_data = (char *)malloc(DISKBLOCKLEN);
                if (NULL == fnode->fn_data)
                {
                    return SYSERR;
                }
                seek(DISK0, curblock);
                read(DISK0, fnode->fn_data, DISKBLOCKLEN);
                fnode->fn_state |= FILE_OPEN;
                fnode->fn_cursor = 0;

                // Return the descriptor for this file.
                return fd;
            }
        }

        dirblk = dirblk->db_next;
    }

    return SYSERR;
}
