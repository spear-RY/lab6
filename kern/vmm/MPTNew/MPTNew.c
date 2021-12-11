#include <lib/x86.h>

#include "import.h"

/**
 * This function will be called when there's no mapping found in the page structure
 * for the given virtual address [vaddr], e.g., by the page fault handler when
 * a page fault happened because the user process accessed a virtual address
 * that is not mapped yet.
 * The task of this function is to allocate a physical page and use it to register
 * a mapping for the virtual address with the given permission.
 * It should return the physical page index registered in the page directory, the
 * return value from map_page.
 * In the case of error, it should return the constant MagicNumber.
 */
unsigned int alloc_page(unsigned int proc_index, unsigned int vaddr,
                        unsigned int perm)
{
    // TODO
    unsigned int page_index = container_alloc(proc_index);
    if(page_index == 0)
    {
        return MagicNumber;
    }
    unsigned int reg_page_index = map_page(proc_index, vaddr, page_index, perm);
    if(reg_page_index == MagicNumber)
    {
        return MagicNumber;
    }
    return reg_page_index;
}

unsigned int alloc_consecutive_page(unsigned int proc_index, unsigned int vaddr, unsigned int perm, unsigned int len)
{
    unsigned int page_index = container_consecutive_alloc(proc_index, len);
    if(page_index == 0)
    {
        dprintf("alloc_consecutive_page 1: MAGIC\n");
        return MagicNumber;
    }
    unsigned int reg_page_index = map_consecutive_page(proc_index, vaddr, page_index, perm, len);
    if(reg_page_index == MagicNumber)
    {
        dprintf("alloc_consecutive_page 2: MAGIC\n");
        return MagicNumber;
    }
    return reg_page_index;
}

unsigned int alloc_super_page(unsigned int proc_index, unsigned int vaddr, unsigned int perm)
{
    const unsigned int len = 1024;
    const unsigned int P_SUPER = 1<<9;
    const unsigned int page_size = 0x1<<12;

    unsigned int page_index = container_consecutive_alloc(proc_index, len);
    if(page_index == 0)
    {
        //dprintf("alloc_super_page 1: MAGIC\n");
        return MagicNumber;
    }
    
    unsigned int vaddr_now = vaddr;

    unsigned int reg_page_index = map_consecutive_page(proc_index, vaddr, page_index, perm|P_SUPER, len);

    if(reg_page_index == MagicNumber)
    {
        return MagicNumber;
    }

    return reg_page_index;
}


/**
 * Designate some memory quota for the next child process.
 */
unsigned int alloc_mem_quota(unsigned int id, unsigned int quota)
{
    //dprintf("enter alloc_mem_quota, id: %d, quota: %d\n", id, quota);

    unsigned int child;
    child = container_split(id, quota);
    return child;
}
