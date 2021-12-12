#include <proc.h>
#include <syscall.h>
#include <types.h>

pid_t spawn(uintptr_t exec, unsigned int quota)
{
    return sys_spawn(exec, quota);
}

void yield(void)
{
    sys_yield();
}

uintptr_t mmap(unsigned int size, unsigned int perm, unsigned int flag, unsigned int vaddr)
{
    return sys_mmap(size, perm, flag, vaddr);
}

unsigned int adv_alloc(unsigned int vaddr)
{
    return sys_adv_alloc(vaddr);
}

// void write(unsigned int vaddr, char data)
// {
//     sys_write(vaddr, data);
// }

// char read(unsigned int vaddr)
// {
//     return sys_read(vaddr);
// }