/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University. All rights reserved. */
/*
 * Modified by
 *
 * and
 *
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    struct dirblock *dirblk;
    struct dirblock *nextblk;
    struct filenode *fnode;
    struct dentry *phw;
    int diskfd;
    int i;
    int index;
    int blocknum;
    int result;

    // TODO: Unlink this file from the master directory index,
    // and return its space to the free disk block list.

    // Use the superblock's locks to guarantee mutually exclusive
    // access to the directory index.

    if ((NULL == supertab) || (NULL == filetab) || (fd < 0))
    {
        return SYSERR;
    }

    phw = supertab->sb_disk;
    if (NULL == phw)
    {
        return SYSERR;
    }
    diskfd = phw - devtab;

    wait(supertab->sb_dirlock);

    dirblk = supertab->sb_dirlst;
    index = fd;

    /* Walk directory blocks until we find the block containing this fd. */
    while ((dirblk != NULL) && (index >= DIRENTRIES))
    {
        dirblk = dirblk->db_next;
        index -= DIRENTRIES;
    }

    if ((NULL == dirblk) || (index < 0) || (index >= DIRENTRIES))
    {
        signal(supertab->sb_dirlock);
        return SYSERR;
    }

    fnode = &dirblk->db_fnodes[index];

    if (FILE_FREE == fnode->fn_state)
    {
        signal(supertab->sb_dirlock);
        return SYSERR;
    }

    blocknum = fnode->fn_blocknum;

    /* If file data is currently buffered in memory, release it. */
    if ((FILE_OPEN & fnode->fn_state) && (NULL != fnode->fn_data))
    {
        free(fnode->fn_data);
    }

    /* Reset this file node to the free state. */
    fnode->fn_state = FILE_FREE;
    fnode->fn_length = 0;
    fnode->fn_cursor = 0;
    fnode->fn_blocknum = 0;
    fnode->fn_data = NULL;
    for (i = 0; i < FNAMLEN; i++)
    {
        fnode->fn_name[i] = '\0';
    }

    /* Write the updated directory block back to disk. */
    nextblk = dirblk->db_next;
    if (NULL == nextblk)
    {
        dirblk->db_next = 0;
    }
    else
    {
        dirblk->db_next = (struct dirblock *)nextblk->db_blocknum;
    }

    seek(diskfd, dirblk->db_blocknum);
    result = write(diskfd, dirblk, sizeof(struct dirblock));

    dirblk->db_next = nextblk;

    signal(supertab->sb_dirlock);

    if (SYSERR == result)
    {
        return SYSERR;
    }

    /* Return the file's data block to the free list. */
    if (blocknum != 0)
    {
        return sbFreeBlock(supertab, blocknum);
    }

    return OK;
}
