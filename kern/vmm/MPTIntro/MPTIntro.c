#include <lib/gcc.h>
#include <lib/x86.h>
#include <lib/debug.h>

#include "import.h"

#define PT_PERM_UP  0
#define PT_PERM_PTU (PTE_P | PTE_W | PTE_U)
#define PAGESIZE 4096
#define PDir_SUPER 1<<9

/**
 * Page directory pool for NUM_IDS processes.
 * mCertiKOS maintains one page structure for each process.
 * Each PDirPool[index] represents the page directory of the page structure
 * for the process # [index].
 * In mCertiKOS, we statically allocate page directories, and maintain the second
 * level page tables dynamically.
 * The unsigned int * type is meant to suggest that the contents of the array
 * are pointers to page tables. In reality they are actually page directory
 * entries, which are essentially pointers plus permission bits. The functions
 * in this layer will require casting between integers and pointers anyway and
 * in fact any 32-bit type is fine, so feel free to change it if it makes more
 * sense to you with a different type.
 */
unsigned int *PDirPool[NUM_IDS][1024] gcc_aligned(PAGESIZE);

/**
 * In mCertiKOS, we use identity page table mappings for the kernel memory.
 * IDPTbl is an array of statically allocated identity page tables that will be
 * reused for all the kernel memory.
 * That is, in every page directory, the entries that fall into the range of
 * addresses reserved for the kernel will point to an entry in IDPTbl.
 */
unsigned int IDPTbl[1024][1024] gcc_aligned(PAGESIZE);

// Sets the CR3 register with the start address of the page structure for process # [index].
void set_pdir_base(unsigned int index)
{
    // TODO
    //dprintf("PDirPool[%d]: %p\n", index, (unsigned int)PDirPool[index]);
    set_cr3(PDirPool[index]);
}

// Returns the page directory entry # [pde_index] of the process # [proc_index].
// This can be used to test whether the page directory entry is mapped.
unsigned int get_pdir_entry(unsigned int proc_index, unsigned int pde_index)
{
    // TODO
    if(proc_index < NUM_IDS && pde_index < 1024)
    {
        unsigned int pdir_entry = (unsigned int)PDirPool[proc_index][pde_index];
        return pdir_entry;
    }
    else
    {
        return 0;
    }
}

// Sets the specified page directory entry with the start address of physical
// page # [page_index].
// You should also set the permissions PTE_P, PTE_W, and PTE_U.
void set_pdir_entry(unsigned int proc_index, unsigned int pde_index,
                    unsigned int page_index)
{
    // TODO
    //dprintf("enter set_pdir_entry, proc: %d, pde: %p, page: %d\n", proc_index, pde_index, page_index);
    if(proc_index < NUM_IDS && pde_index < 1024)
    {
        PDirPool[proc_index][pde_index] = (unsigned int *)(page_index * PAGESIZE + PT_PERM_PTU);
        //dprintf("PDirPool[proc_index][pde_index]: %p\n", (unsigned int)PDirPool[proc_index][pde_index]);
    }
    return;
}

// Sets the page directory entry # [pde_index] for the process # [proc_index]
// with the initial address of page directory # [pde_index] in IDPTbl.
// You should also set the permissions PTE_P, PTE_W, and PTE_U.
// This will be used to map a page directory entry to an identity page table.
void set_pdir_entry_identity(unsigned int proc_index, unsigned int pde_index)
{
    // TODO
    if(proc_index < NUM_IDS && pde_index < 1024)
    {
        PDirPool[proc_index][pde_index] = (unsigned int *)((unsigned int)IDPTbl[pde_index] | PT_PERM_PTU);
    }
    return;
}

// Removes the specified page directory entry (sets the page directory entry to 0).
// Don't forget to cast the value to (unsigned int *).
void rmv_pdir_entry(unsigned int proc_index, unsigned int pde_index)
{
    // TODO
    if(proc_index < NUM_IDS && pde_index < 1024)
    {
        PDirPool[proc_index][pde_index] = (unsigned int *)0;
    }
    return;
}

// Returns the specified page table entry.
// Do not forget that the permission info is also stored in the page directory entries.
unsigned int get_ptbl_entry(unsigned int proc_index, unsigned int pde_index,
                            unsigned int pte_index)
{
    // TODO
    if(proc_index < NUM_IDS && pde_index < 1024 && pte_index < 1024)
    {
        unsigned int *ptbl_entry_ptr = (unsigned int *)(((unsigned int)PDirPool[proc_index][pde_index] & 0xFFFFF000) + pte_index * 4);
        return *ptbl_entry_ptr;
    }
    return 0;
}

// Sets the specified page table entry with the start address of physical page # [page_index]
// You should also set the given permission.
void set_ptbl_entry(unsigned int proc_index, unsigned int pde_index,
                    unsigned int pte_index, unsigned int page_index,
                    unsigned int perm)
{
    // TODO
    //dprintf("enter set_ptbl_entry, pde_index: %p, pte_index: %p\n",pde_index,pte_index);
    if(proc_index < NUM_IDS && pde_index < 1024 && pte_index < 1024)
    {
        //dprintf("enter IF in set_ptbl_entry, addr: 0x%08x, perm: %p\n",page_index * PAGESIZE,perm);
        unsigned int *ptbl_entry_ptr = (unsigned int *)(((unsigned int)PDirPool[proc_index][pde_index] & 0xFFFFF000) + pte_index * 4);
        *ptbl_entry_ptr = (unsigned int)(page_index * PAGESIZE + perm);
    }
    return;
}

// Sets up the specified page table entry in IDPTbl as the identity map.
// You should also set the given permission.
void set_ptbl_entry_identity(unsigned int pde_index, unsigned int pte_index,
                             unsigned int perm)
{
    // TODO
    if(pde_index < 1024 && pte_index < 1024)
    {
        IDPTbl[pde_index][pte_index] = (unsigned int)((pde_index << 22) + (pte_index << 12) + perm);
    }
    return;
}

// Sets the specified page table entry to 0.
void rmv_ptbl_entry(unsigned int proc_index, unsigned int pde_index,
                    unsigned int pte_index)
{
    // TODO
    if(proc_index < NUM_IDS && pde_index < 1024 && pte_index < 1024)
    {
        unsigned int *ptbl_entry_ptr = (unsigned int *)(((unsigned int)PDirPool[proc_index][pde_index] & 0xFFFFF000) + pte_index * 4);
        *ptbl_entry_ptr = 0;
    }
    return;
}
