#include <lib/x86.h>

#include "import.h"

void vmm_copy(unsigned int from_id, unsigned int to_id)
{
    const unsigned int P_SUPER = 1<<9;

    unsigned int from_ptbl_entry;
    unsigned int from_pdir_entry;
    dprintf("enter vmm_copy.\n");
    for(unsigned int pde_index=256; pde_index<960; pde_index++)
    {
        from_pdir_entry = get_pdir_entry(from_id, pde_index);
        if(!(from_pdir_entry&PTE_P))//if the page not present, continue
            {
                continue;
            }
        unsigned int page_index = container_alloc(to_id);
        if(page_index == 0)//if the page alloc failed
        {
            dprintf("the page alloc failed.\n");
            return;
        }
        set_pdir_entry(to_id, pde_index, page_index);

        dprintf("pde_index:%d\n",pde_index);

        for(unsigned int pte_index=0; pte_index<1024; pte_index++)
        {
            from_ptbl_entry = get_ptbl_entry(from_id, pde_index, pte_index);
            if(!(from_ptbl_entry&PTE_P))//if the page not present, continue
            {
                continue;
            }
            dprintf("pid: %d, super: %d, pde_idx: %d, pte_idx: %d\n", from_id, from_ptbl_entry&P_SUPER, pde_index, pte_index);

            if(from_ptbl_entry&P_SUPER)
            {
                dprintf("vmm set (super)\n");
                set_ptbl_entry(from_id, pde_index, pte_index, from_ptbl_entry>>12, PTE_P | PTE_U | PTE_COW | P_SUPER);
                set_ptbl_entry(to_id, pde_index, pte_index, from_ptbl_entry>>12, PTE_P | PTE_U | PTE_COW | P_SUPER);
            }
            else
            {
                dprintf("vmm set (common)\n");
                set_ptbl_entry(from_id, pde_index, pte_index, from_ptbl_entry>>12, PTE_P | PTE_U | PTE_COW);
                set_ptbl_entry(to_id, pde_index, pte_index, from_ptbl_entry>>12, PTE_P | PTE_U | PTE_COW);
            }
        }
    }
    //dprintf("vmm_copy exit.\n");
    return;
}

void copy_on_write(unsigned int pid, unsigned int vaddr)
{
    const unsigned int len = 1024;
    const unsigned int P_SUPER = 1<<9;
    const unsigned int page_size = 0x1<<12;

    unsigned int from_ptbl_entry = get_ptbl_entry_by_va(pid, vaddr);


    if(from_ptbl_entry&P_SUPER)//copy a super page
    {
        dprintf("enter cow (super)\n");
        unsigned int vaddr_now = vaddr;
        while(get_ptbl_entry_by_va(pid, vaddr-page_size)==from_ptbl_entry)
        {
            vaddr_now -= page_size;
        }
        unsigned int page_index = alloc_super_page(pid, vaddr_now, PTE_P | PTE_W | PTE_U);

        if(page_index == 0)//if failed to allocate a new super page
        {
            return;
        }

        unmap_page(pid, vaddr_now);
        map_consecutive_page(pid, vaddr_now, page_index, PTE_P | PTE_W | PTE_U | P_SUPER, len);

        unsigned int to_ptbl_entry = get_ptbl_entry_by_va(pid, vaddr_now);

        unsigned char* from;
        unsigned char* to;

        for(int i=0;i<4096*1024;i++)
        {
            from = (unsigned char*)((from_ptbl_entry&0xFFFFF000) + i);
            to = (unsigned char*)((to_ptbl_entry&0xFFFFF000) + i);
            //dprintf("from: %d\n",*from);
            *to = *from;
            //dprintf("to: %d\n",*to);
        }
        return;
    }
    else//copy a common page
    {
        unsigned int page_index = container_alloc(pid);

        dprintf("enter cow (common)\n");

        //dprintf("enter cow, pid = %d, vaddr = %08x, perm: %08x\n",pid,vaddr,PTE_P | PTE_W | PTE_U);

        if(page_index == 0)//if failed to allocate a new page
        {
            return;
        }

        rmv_ptbl_entry_by_va(pid, vaddr);
        set_ptbl_entry_by_va(pid, vaddr, page_index, PTE_P | PTE_W | PTE_U);//set the new table entry as writeable
        unsigned int to_ptbl_entry = get_ptbl_entry_by_va(pid, vaddr);

        unsigned char* from;
        unsigned char* to;

        for(int i=0;i<4096;i++)
        {
            from = (unsigned char*)((from_ptbl_entry&0xFFFFF000) + i);
            to = (unsigned char*)((to_ptbl_entry&0xFFFFF000) + i);
            //dprintf("from: %d\n",*from);
            *to = *from;
            //dprintf("to: %d\n",*to);
        }
        return;
    }
}