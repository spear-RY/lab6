#include <lib/x86.h>
#include <lib/debug.h>

#include "import.h"

/**
 * Sets the entire page map for process 0 as the identity map.
 * Note that part of the task is already completed by pdir_init.
 */
void pdir_init_kern(unsigned int mbi_addr)
{
    // TODO: Define your local variables here.
    unsigned int userlo = 0x40000000>>22;
    unsigned int userhi = 0xF0000000>>22;

    pdir_init(mbi_addr);

    //TODO
    for(unsigned int i=userlo; i < userhi; i++)
    {
        set_pdir_entry_identity(0, i);
    }
    return;
}

/**
 * Maps the physical page # [page_index] for the given virtual address with the given permission.
 * In the case when the page table for the page directory entry is not set up,
 * you need to allocate the page table first.
 * In the case of error, it returns the constant MagicNumber defined in lib/x86.h,
 * otherwise, it returns the physical page index registered in the page directory,
 * (the return value of get_pdir_entry_by_va or alloc_ptbl).
 */
unsigned int map_page(unsigned int proc_index, unsigned int vaddr,
                      unsigned int page_index, unsigned int perm)
{
    // TODO
    unsigned int pdir_entry = get_pdir_entry_by_va(proc_index, vaddr);
    if(pdir_entry == 0)
    {
        //dprintf("enter pdir_entry == 0\n");
        if(alloc_ptbl(proc_index, vaddr) == 0)
        {
            //dprintf("alloc failed\n");
            return MagicNumber;
        }
    }
    set_ptbl_entry_by_va(proc_index, vaddr, page_index, perm);
    return get_pdir_entry_by_va(proc_index, vaddr);
}

unsigned int map_consecutive_page(unsigned int proc_index, unsigned int vaddr,
                      unsigned int page_index, unsigned int perm, unsigned int len)
{
    unsigned int vaddr_now = vaddr;
    const unsigned int page_size = 0x1<<12;
    const unsigned int P_SUPER = 1<<9;
    unsigned int page_now = page_index;

    for(unsigned int i=0;i<len;i++)
    {
        unsigned int pdir_entry = get_pdir_entry_by_va(proc_index, vaddr_now);
        if(pdir_entry == 0)
        {
            //dprintf("enter pdir_entry == 0\n");
            if(alloc_ptbl(proc_index, vaddr_now) == 0)
            {
                //dprintf("alloc failed\n");
                return MagicNumber;
            }
        }
        set_ptbl_entry_by_va(proc_index, vaddr_now, page_now, perm);
        dprintf("pid: %d, len: %d, super: %d, pde_idx: %d, pte_idx: %d\n", proc_index, i, get_ptbl_entry_by_va(proc_index, vaddr_now)&P_SUPER, vaddr_now>>22, (vaddr_now&0x003FF000)>>12);
        vaddr_now += page_size;
        page_now++;
    }
    return get_pdir_entry_by_va(proc_index, vaddr);
}

/**
 * Remove the mapping for the given virtual address (with rmv_ptbl_entry_by_va).
 * You need to first make sure that the mapping is still valid,
 * e.g., by reading the page table entry for the virtual address.
 * Nothing should be done if the mapping no longer exists.
 * You do not need to unmap the page table from the page directory.
 * It should return the corresponding page table entry.
 */
unsigned int unmap_consecutive_page(unsigned int proc_index, unsigned int vaddr, unsigned int len);

unsigned int unmap_page(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    const unsigned int P_SUPER = 1<<9;
    const unsigned int page_size = 0x1<<12;

    unsigned int ptbl_entry = get_ptbl_entry_by_va(proc_index, vaddr);
    if(ptbl_entry == 0)
    {
        return 0;
    }

    if(ptbl_entry&P_SUPER)//release a super page
    {
        unsigned int vaddr_now = vaddr;
        while(get_ptbl_entry_by_va(proc_index, vaddr-page_size)==ptbl_entry)
        {
            vaddr_now -= page_size;
        }
        unmap_consecutive_page(proc_index, vaddr_now, 1024);
    }
    else
    {
        rmv_ptbl_entry_by_va(proc_index, vaddr);
    }
    return ptbl_entry;
}

unsigned int unmap_consecutive_page(unsigned int proc_index, unsigned int vaddr, unsigned int len)
{
    // TODO
    unsigned int vaddr_now = vaddr;
    const unsigned int page_size = 0x1<<12;
    for(unsigned int i=0;i<len;i++)
    {
        unsigned int ptbl_entry = get_ptbl_entry_by_va(proc_index, vaddr_now);
        if(ptbl_entry == 0)
        {
            //dprintf("alloc failed\n");
            return MagicNumber;
        }
        rmv_ptbl_entry_by_va(proc_index, vaddr_now);
        vaddr_now += page_size;
    }

    return 1;
}

