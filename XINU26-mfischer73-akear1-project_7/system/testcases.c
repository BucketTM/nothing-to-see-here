/**
 * @file testcases.c
 * @provides testcases
 *
 *
 * Modified by:	
 *
 * TA-BOT:MAILTO arthur.kear@marquette.edu AND margarette.fischer@marquette.edu
 *
 */
/* Embedded XINU, Copyright (C) 2023.  All rights reserved. */

#include <xinu.h>

/* This fake page table will allow you to test your printPageTable function
 * without having paging completely working.
 */
pgtbl createFakeTable(void){
	pgtbl root = pgalloc();
	pgtbl lvl1 = pgalloc();
	pgtbl lvl0 = pgalloc();

	if (root == NULL || lvl1 == NULL || lvl0 == NULL)
	{
		return NULL;
	}

	memset(root, 0, PAGE_SIZE);
	memset(lvl1, 0, PAGE_SIZE);
	memset(lvl0, 0, PAGE_SIZE);

	volatile ulong *pte = &(root[5]);
	*pte = PA2PTE(lvl1) | PTE_V;

	ulong *lvl1pte = &(lvl1[145]);
	*lvl1pte = PA2PTE(lvl0) | PTE_V;

	ulong *lvl0pte = &(lvl0[343]);
	*lvl0pte = PA2PTE(0x1000) | PTE_W | PTE_R | PTE_V;

	ulong *lvl0pte1 = &(lvl0[120]);
	*lvl0pte1 = PA2PTE(0x4000) | PTE_X | PTE_R | PTE_V;

	ulong *lvl0pte2 = &(lvl0[45]);
	*lvl0pte2 = PA2PTE(0x8000) | PTE_X | PTE_R | PTE_V;

	return root;
}

static void printPageTableLevel(pgtbl pagetable, int level)
{
	int i;

	if (pagetable == NULL)
	{
		return;
	}

	for (i = 0; i < PTE_MAX; i++)
	{
		ulong pte = pagetable[i];

		if ((pte & PTE_V) == 0)
		{
			continue;
		}

		if ((pte & (PTE_R | PTE_W | PTE_X)) == 0)
		{
			kprintf("L%d[%3d] = 0x%08X\r\n", level, i, pte);

			if (level > 0)
			{
				printPageTableLevel((pgtbl)PTE2PA(pte), level - 1);
			}
		}
		else
		{
			kprintf("L%d[%3d] = 0x%08X -> PA 0x%08X\r\n",
			        level, i, pte, PTE2PA(pte));
		}
	}
}

void printPageTable(pgtbl pagetable)
{
	/*
	* TODO: Write a function that prints out the page table.
	* Your function should print out all *valid* page table entries in
	* the page table.  If any of the entries are a link (if the
	* Read/Write/Execute bits aren't set), recursively print that page
	* table.  If it is a leaf, print the page table entry and the
	* physical address is maps to. 
	*/
	printPageTableLevel(pagetable, 2);

	//if valid
	//printPageTableLevel()
	//printInfo <--- print all the stored leaf info recursively 
	//need the adresses printed so we need PTE_R , PTE_W , PTE_X 
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
	pgtbl fake;

	kprintf("===TEST BEGIN===\r\n");

	// TODO: Test your operating system!

	fake = createFakeTable();
	printPageTable(fake);

	kprintf("===TEST END===\r\n");
	return;
}
