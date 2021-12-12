#include <proc.h>
#include <stdio.h>
#include <syscall.h>

#define PTE_P 0x001  /* Present */
#define PTE_W 0x002  /* Writeable */
#define PTE_U 0x004  /* User-accessible */

#define SUPER 200
#define CONSECUTIVE 300

int main(int argc, char **argv)
{
    unsigned int val_1 = 300;
    unsigned int val_2 = 600;
    unsigned int *addr = (unsigned int *) 0xe0000000;

    // printf("ding started.\n");
    // printf("ding: the value at address %x: %d\n", addr, *addr);
    // printf("ding: writing the value %d to the address %x\n", val, addr);
    // *addr = val;
    // yield();
    // printf("ding: the new value at address %x: %d\n", addr, *addr);


    pid_t pid;

    uintptr_t mptr = mmap(1024, PTE_P | PTE_W | PTE_U, SUPER, (unsigned int)addr);
    *addr = val_1;
    printf("before fork: the value at address %x: %d\n", addr, *addr);

    pid = sys_fork();

    if (pid == 0) {
        printf("child process: the original value at address %x: %d\n", addr, *addr);
        *addr = val_2;
        printf("child process: the new value at address %x: %d\n", addr, *addr);
    }
    else {
        printf("parent process: the original value at address %x: %d\n", addr, *addr);
    }

    return 0;
}
