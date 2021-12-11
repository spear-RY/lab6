#ifndef _KERN_VMM_MPTFork_H_
#define _KERN_VMM_MPTFork_H_

#ifdef _KERN_

void vmm_copy(unsigned int from_id, unsigned int to_id);
void copy_on_write(unsigned int pid, unsigned int vaddr);

#endif  /* _KERN_ */

#endif  /* !_KERN_VMM_MPTFork_H_ */
