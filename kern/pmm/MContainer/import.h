#ifndef _KERN_PMM_MCONTAINER_H_
#define _KERN_PMM_MCONTAINER_H_

#ifdef _KERN_

unsigned int get_nps(void);
unsigned int at_is_norm(unsigned int page_index);
unsigned int at_is_allocated(unsigned int page_index);
void pmem_init(unsigned int mbi_addr);
unsigned int palloc(void);
unsigned int consecutive_palloc(unsigned int page_num);
void pfree(unsigned int pfree_index);
void consecutive_pfree(unsigned int pfree_index, unsigned int len);

#endif  /* _KERN_ */

#endif  /* !_KERN_PMM_MCONTAINER_H_ */
