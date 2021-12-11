#ifndef _KERN_PMM_MATOP_H_
#define _KERN_PMM_MATOP_H_

#ifdef _KERN_

unsigned int palloc(void);
void pfree(unsigned int pfree_index);
unsigned int consecutive_palloc(unsigned int page_num);
void consecutive_pfree(unsigned int pfree_index, unsigned int len);

#endif  /* _KERN_ */

#endif  /* !_KERN_PMM_MATOP_H_ */
