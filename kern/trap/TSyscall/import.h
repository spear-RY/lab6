#ifndef _KERN_TRAP_TSYSCALL_H_
#define _KERN_TRAP_TSYSCALL_H_

#ifdef _KERN_

unsigned int get_curid(void);
unsigned int syscall_get_arg1(void);
unsigned int syscall_get_arg2(void);
unsigned int syscall_get_arg3(void);
unsigned int syscall_get_arg4(void);
unsigned int syscall_get_arg5(void);
unsigned int syscall_get_arg6(void);

void syscall_set_errno(unsigned int errno);
void syscall_set_retval1(unsigned int retval);
void syscall_set_retval2(unsigned int retval);
void syscall_set_retval3(unsigned int retval);
void syscall_set_retval4(unsigned int retval);
void syscall_set_retval5(unsigned int retval);

unsigned int proc_create(void *elf_addr, unsigned int quota);
unsigned int proc_fork(void);
void thread_yield(void);
void pgflt_handler(void);

unsigned int container_get_quota(unsigned int id);

unsigned int alloc_super_page(unsigned int proc_index, unsigned int vaddr, unsigned int perm);
unsigned int alloc_consecutive_page(unsigned int proc_index, unsigned int vaddr, unsigned int perm, unsigned int len);
unsigned int alloc_page(unsigned int proc_index, unsigned int vaddr, unsigned int perm);

unsigned int get_ptbl_entry_by_va(unsigned int proc_index, unsigned int vaddr);

#endif  /* _KERN_ */

#endif  /* !_KERN_TRAP_TSYSCALL_H_ */
