#include <lib/x86.h>

#include "import.h"

/**
 * For each process from id 0 to NUM_IDS - 1,
 * set up the page directory entries so that the kernel portion of the map is
 * the identity map, and the rest of the page directories are unmapped.
 */
void pdir_init(unsigned int mbi_addr)
{
    // TODO: Define your local variables here.
    unsigned int userlo = 0x40000000>>22;
    unsigned int userhi = 0xF0000000>>22;
    unsigned int nps = 1024;

    idptbl_init(mbi_addr);

    // TODO
    for(unsigned int i=0; i < NUM_IDS; i++)
    {
        for(unsigned int j=0; j < nps; j++)
        {
            if(j < userlo || j >= userhi)
            {
                set_pdir_entry_identity(i, j);
            }
            else
            {
                rmv_pdir_entry(i, j);
            }
        }
    }
    return;
}

/**
 * Allocates a page (with container_alloc) for the page table,
 * and registers it in the page directory for the given virtual address,
 * and clears (set to 0) all page table entries for this newly mapped page table.
 * It returns the page index of the newly allocated physical page.
 * In the case when there's no physical page available, it returns 0.
 */
unsigned int alloc_ptbl(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    unsigned int page_index = container_alloc(proc_index);
    //dprintf("page_index = %d\n", page_index);
    if(page_index != 0)
    {
        set_pdir_entry_by_va(proc_index, vaddr, page_index);
        return page_index;
    }
    return 0;
}

// Reverse operation of alloc_ptbl.
// Removes corresponding the page directory entry,
// and frees the page for the page table entries (with container_free).
void free_ptbl(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    unsigned int pdir_entry = get_pdir_entry_by_va(proc_index, vaddr);
    if(pdir_entry != 0)
    {
        rmv_pdir_entry_by_va(proc_index, vaddr);
        container_free(proc_index, pdir_entry>>12);
    }
    return;
}
