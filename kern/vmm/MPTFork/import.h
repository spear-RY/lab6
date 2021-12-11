#ifndef _KERN_VMM_MPTFork_H_
#define _KERN_VMM_MPTFork_H_

#ifdef _KERN_

unsigned int get_pdir_entry(unsigned int proc_index, unsigned int pde_index);
void set_pdir_entry(unsigned int proc_index, unsigned int pde_index,
                    unsigned int page_index);
unsigned int get_ptbl_entry(unsigned int proc_index, unsigned int pde_index,
                            unsigned int pte_index);
void set_ptbl_entry(unsigned int proc_index, unsigned int pde_index,
                    unsigned int pte_index, unsigned int page_index,
                    unsigned int perm);

unsigned int get_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr);
void rmv_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr);
void set_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr,
                          unsigned int page_index, unsigned int perm);


unsigned int container_alloc(unsigned int id);

unsigned int unmap_page(unsigned int proc_index, unsigned int vaddr);
unsigned int map_consecutive_page(unsigned int proc_index, unsigned int vaddr,
                      unsigned int page_index, unsigned int perm, unsigned int len);
unsigned int alloc_super_page(unsigned int proc_index, unsigned int vaddr, unsigned int perm);



#endif  /* _KERN_ */

#endif  /* !_KERN_VMM_MPTFork_H_ */
