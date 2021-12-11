#ifndef _KERN_VMM_MPTNEW_H_
#define _KERN_VMM_MPTNEW_H_

#ifdef _KERN_

unsigned int container_alloc(unsigned int id);
void container_free(unsigned int id, unsigned int page_index);
unsigned int container_split(unsigned int id, unsigned int quota);
unsigned int map_page(unsigned int proc_index, unsigned int vaddr,
                      unsigned int page_index, unsigned int perm);

unsigned int container_consecutive_alloc(unsigned int id, unsigned int len);
void container_consecutive_free(unsigned int id, unsigned int page_index, unsigned int len);
unsigned int map_consecutive_page(unsigned int proc_index, unsigned int vaddr,
                      unsigned int page_index, unsigned int perm, unsigned int len);

#endif  /* _KERN_ */

#endif  /* !_KERN_VMM_MPTKERN_H_ */
