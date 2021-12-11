#ifndef _USER_PROC_H_
#define _USER_PROC_H_

#include <types.h>

#define SUPER 200
#define CONSECUTIVE 300
#define MagicNumber 1048577

pid_t spawn(unsigned int elf_id, unsigned int quota);
void yield(void);
uintptr_t mmap(unsigned int size, unsigned int perm, unsigned int flag, unsigned int vaddr);
void write(unsigned int vaddr, char data);
char read(unsigned int vaddr);

#endif  /* !_USER_PROC_H_ */
