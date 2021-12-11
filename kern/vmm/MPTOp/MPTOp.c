#include <lib/x86.h>

#include "import.h"

/**
 * Returns the page table entry corresponding to the virtual address,
 * according to the page structure of process # [proc_index].
 * Returns 0 if the mapping does not exist.
 */
unsigned int get_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    unsigned int pdir_entry = get_pdir_entry(proc_index, (vaddr & 0xFFC00000) >> 22);
    if(!(pdir_entry & 1))
    {
        return 0;
    }
    unsigned int ptbl_entry = get_ptbl_entry(proc_index, (vaddr & 0xFFC00000) >> 22, (vaddr & 0x003FF000) >> 12);
    if(!(ptbl_entry & 1))
    {
        return 0;
    }
    return ptbl_entry;
}

// Returns the page directory entry corresponding to the given virtual address.
unsigned int get_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    unsigned int pdir_entry = get_pdir_entry(proc_index, (vaddr & 0xFFC00000) >> 22);
    if(!(pdir_entry & 1))
        return 0;
    return pdir_entry;
}

// Removes the page table entry for the given virtual address.
void rmv_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    rmv_ptbl_entry(proc_index, (vaddr & 0xFFC00000) >> 22, (vaddr & 0x003FF000) >> 12);
    return;
}

// Removes the page directory entry for the given virtual address.
void rmv_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr)
{
    // TODO
    rmv_pdir_entry(proc_index, (vaddr & 0xFFC00000) >> 22);
    return;
}

// Maps the virtual address [vaddr] to the physical page # [page_index] with permission [perm].
// You do not need to worry about the page directory entry. just map the page table entry.
void set_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr,
                          unsigned int page_index, unsigned int perm)
{
    // TODO
    set_ptbl_entry(proc_index, (vaddr & 0xFFC00000) >> 22, (vaddr & 0x003FF000) >> 12, page_index, perm);
    return;
}

// Registers the mapping from [vaddr] to physical page # [page_index] in the page directory.
void set_pdir_entry_by_va(unsigned int proc_index, unsigned int vaddr,
                          unsigned int page_index)
{
    // TODO
    //dprintf("enter set_pdir_entry_by_va\n");
    set_pdir_entry(proc_index, (vaddr & 0xFFC00000) >> 22, page_index);
    return;
}

// Initializes the identity page table.
// The permission for the kernel memory should be PTE_P, PTE_W, and PTE_G,
// While the permission for the rest should be PTE_P and PTE_W.
void idptbl_init(unsigned int mbi_addr)
{
    // TODO: Define your local variables here.
    unsigned int userlo = 0x40000000/4096;
    unsigned int userhi = 0xF0000000/4096;
    unsigned int nps = 0xFFFFFFFF/4096;
    unsigned int pagesize = 4096;
    unsigned int vm_addr;

    container_init(mbi_addr);

    // TODO
    for(unsigned int i=0; i < nps; i++)
    {
        vm_addr = i * pagesize;
        if(i < userlo || i >= userhi)
        {
            set_ptbl_entry_identity((vm_addr & 0xFFC00000) >> 22, (vm_addr & 0x003FF000) >> 12, PTE_P | PTE_W | PTE_G);
        }
        else
        {
            set_ptbl_entry_identity((vm_addr & 0xFFC00000) >> 22, (vm_addr & 0x003FF000) >> 12, PTE_P | PTE_W);
        }
    }
    return;
}
