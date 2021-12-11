#ifndef _KERN_VMM_MPTKERN_H_
#define _KERN_VMM_MPTKERN_H_

#ifdef _KERN_

void pdir_init_kern(unsigned int mbi_addr);
unsigned int map_page(unsigned int proc_index, unsigned int vaddr,
                      unsigned int page_index, unsigned int perm);
unsigned int unmap_page(unsigned int proc_index, unsigned int vaddr);
unsigned int map_consecutive_page(unsigned int proc_index, unsigned int vaddr,
                      unsigned int page_index, unsigned int perm, unsigned int len);
unsigned int unmap_consecutive_page(unsigned int proc_index, unsigned int vaddr, unsigned int len);

#endif  /* _KERN_ */

#endif  /* !_KERN_VMM_MPTKERN_H_ */
