/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University. All rights reserved. */
/*
 * TA-BOT:MAILTO arthur.kear@marquette.edu and margarette.fischer@marquette.edu
 *
 * and
 *
 */

#include <xinu.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    struct fbcnode *fbc;
    struct fbcnode *fbc2;
    struct dirblock *swizzle;
    struct dentry *phw;
    int diskfd;

    if ((NULL == psuper) || (block <= 0))
    {
        return SYSERR;
    }

    phw = psuper->sb_disk;
    if (NULL == phw)
    {
        return SYSERR;
    }
    diskfd = phw - devtab;

    /* Updating the free list must be mutually exclusive. */
    wait(psuper->sb_freelock);

    fbc = psuper->sb_freelst;

    /* If there is no free list yet, create one using this returned block. */
    if (NULL == fbc)
    {
        fbc = (struct fbcnode *)malloc(sizeof(struct fbcnode));
        if (NULL == fbc)
        {
            signal(psuper->sb_freelock);
            return SYSERR;
        }

        fbc->fbc_blocknum = block;
        fbc->fbc_count = 0;
        fbc->fbc_next = NULL;

        psuper->sb_freelst = fbc;

        /* Update superblock record on disk. */
        swizzle = psuper->sb_dirlst;
        psuper->sb_dirlst = (struct dirblock *)swizzle->db_blocknum;
        seek(diskfd, psuper->sb_blocknum);
        if (SYSERR == write(diskfd, psuper, sizeof(struct superblock)))
        {
            psuper->sb_dirlst = swizzle;
            signal(psuper->sb_freelock);
            return SYSERR;
        }
        psuper->sb_dirlst = swizzle;

        /* Update this new free block record on disk. */
        seek(diskfd, fbc->fbc_blocknum);
        if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode)))
        {
            signal(psuper->sb_freelock);
            return SYSERR;
        }

        signal(psuper->sb_freelock);
        return OK;
    }

    /* If the first free block collector has room, add the block there. */
    if (fbc->fbc_count < FREEBLOCKMAX)
    {
        fbc->fbc_free[fbc->fbc_count] = block;
        fbc->fbc_count++;

        /* Update this free block record on disk. */
        fbc2 = fbc->fbc_next;
        if (NULL == fbc->fbc_next)
        {
            fbc->fbc_next = 0;
        }
        else
        {
            fbc->fbc_next = (struct fbcnode *)fbc->fbc_next->fbc_blocknum;
        }

        seek(diskfd, fbc->fbc_blocknum);
        if (SYSERR == write(diskfd, fbc, sizeof(struct fbcnode)))
        {
            fbc->fbc_next = fbc2;
            signal(psuper->sb_freelock);
            return SYSERR;
        }

        fbc->fbc_next = fbc2;
        signal(psuper->sb_freelock);
        return OK;
    }

    /* Otherwise the returned block itself becomes the new head collector node. */
    fbc2 = (struct fbcnode *)malloc(sizeof(struct fbcnode));
    if (NULL == fbc2)
    {
        signal(psuper->sb_freelock);
        return SYSERR;
    }

    fbc2->fbc_blocknum = block;
    fbc2->fbc_count = 0;
    fbc2->fbc_next = fbc;

    psuper->sb_freelst = fbc2;

    /* Update superblock record on disk. */
    swizzle = psuper->sb_dirlst;
    psuper->sb_dirlst = (struct dirblock *)swizzle->db_blocknum;
    seek(diskfd, psuper->sb_blocknum);
    if (SYSERR == write(diskfd, psuper, sizeof(struct superblock)))
    {
        psuper->sb_dirlst = swizzle;
        signal(psuper->sb_freelock);
        return SYSERR;
    }
    psuper->sb_dirlst = swizzle;

    /* Update this new free block record on disk. */
    fbc = fbc2->fbc_next;
    if (NULL == fbc2->fbc_next)
    {
        fbc2->fbc_next = 0;
    }
    else
    {
        fbc2->fbc_next = (struct fbcnode *)fbc2->fbc_next->fbc_blocknum;
    }

    seek(diskfd, fbc2->fbc_blocknum);
    if (SYSERR == write(diskfd, fbc2, sizeof(struct fbcnode)))
    {
        fbc2->fbc_next = fbc;
        signal(psuper->sb_freelock);
        return SYSERR;
    }

    fbc2->fbc_next = fbc;

    signal(psuper->sb_freelock);
    return OK;
}
