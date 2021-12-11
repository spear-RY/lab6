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

uintptr_t mmap(unsigned int size, unsigned int perm, unsigned int flag)
{
    return sys_mmap(size, perm, flag);
}
