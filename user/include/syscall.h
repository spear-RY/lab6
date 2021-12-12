#ifndef _USER_SYSCALL_H_
#define _USER_SYSCALL_H_

#include <lib/syscall.h>

#include <debug.h>
#include <gcc.h>
#include <proc.h>
#include <types.h>
#include <x86.h>

static gcc_inline void sys_puts(const char *s, size_t len)
{
    asm volatile ("int %0"
                  :: "i" (T_SYSCALL),
                     "a" (SYS_puts),
                     "b" (s),
                     "c" (len)
                  : "cc", "memory");
}

static gcc_inline pid_t sys_spawn(unsigned int elf_id, unsigned int quota)
{
    int errno;
    pid_t pid;

    asm volatile ("int %2"
                  : "=a" (errno), "=b" (pid)
                  : "i" (T_SYSCALL),
                    "a" (SYS_spawn),
                    "b" (elf_id),
                    "c" (quota)
                  : "cc", "memory");

    return errno ? -1 : pid;
}

static gcc_inline void sys_yield(void)
{
    asm volatile ("int %0"
                  :: "i" (T_SYSCALL),
                     "a" (SYS_yield)
                  : "cc", "memory");
}

static gcc_inline pid_t sys_fork(void)
{
    int errno;
    pid_t pid;

    asm volatile ("int %2"
                  : "=a" (errno), "=b" (pid)
                  : "i" (T_SYSCALL),
                    "a" (SYS_fork)
                  : "cc", "memory");

    return pid;
}

static gcc_inline uint32_t sys_get_ptbl_entry(unsigned int vaddr)
{
    int errno;
    uint32_t data;

    asm volatile ("int %2"
                  : "=a" (errno), "=b" (data)
                  : "i" (T_SYSCALL),
                    "a" (SYS_ADV),
                    "b" (vaddr)
                  : "cc", "memory");

    return errno ? -1 : data;
}

static gcc_inline uintptr_t sys_mmap(unsigned int size, unsigned int perm, unsigned int flag, unsigned int vaddr)
{
    int errno;
    uintptr_t ptr;

    asm volatile ("int %2"
                  : "=a" (errno), "=b" (ptr)
                  : "i" (T_SYSCALL),
                    "a" (SYS_mmap),
                    "b" (size),
                    "c" (perm),
                    "d" (flag),
                    "S" (vaddr)
                  : "cc", "memory");

    return errno ? -1 : ptr;
}

// static gcc_inline void sys_write(unsigned int vaddr, char data)
// {
//     asm volatile ("int %2"
//                   : "=a" (errno), "=b" (pid)
//                   : "i" (T_SYSCALL),
//                     "a" (SYS_write),
//                     "b" (vaddr),
//                     "c" (data)
//                   : "cc", "memory");
// }

// static gcc_inline char sys_read(unsigned int vaddr)
// {
//     int errno;
//     char data;

//     asm volatile ("int %2"
//                   : "=a" (errno), "=b" (data)
//                   : "i" (T_SYSCALL),
//                     "a" (SYS_read),
//                     "b" (vaddr)
//                   : "cc", "memory");

//     return errno ? -1 : data;
// }

#endif  /* !_USER_SYSCALL_H_ */
