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
    unsigned int *addr_1 = (unsigned int *) 0xd0000000;
    unsigned int *addr_2 = (unsigned int *) 0xc0000000;
    const unsigned int page_size = 0x1<<12;

    // printf("ding started.\n");
    // printf("ding: the value at address %x: %d\n", addr, *addr);
    // printf("ding: writing the value %d to the address %x\n", val, addr);
    // *addr = val;
    // yield();
    // printf("ding: the new value at address %x: %d\n", addr, *addr);


    pid_t pid;

    //unsigned int data = adv_alloc(1000);
    //printf("%d\n", data);

    unsigned int ptbl_a, ptbl_b;

    uintptr_t mptr = mmap(1024, PTE_P | PTE_W | PTE_U, SUPER, (unsigned int)addr_1);
    ptbl_a = adv_alloc(addr_1);
    ptbl_b = adv_alloc(((unsigned int)addr_1)+500*page_size);
    printf("addr_a: %x, addr_b: %x\n", addr_1, ((unsigned int)addr_1)+500*page_size);
    printf("ptbl_a: %x, ptbl_b: %x\n", ptbl_a, ptbl_b);

    uintptr_t mptr_2 = mmap(10, PTE_P | PTE_W | PTE_U, CONSECUTIVE, (unsigned int)addr_2);
    ptbl_a = adv_alloc(addr_2);
    ptbl_b = adv_alloc(((unsigned int)addr_2)+5*page_size);
    printf("addr_a: %x, addr_b: %x\n", addr_2, ((unsigned int)addr_2)+5*page_size);
    printf("ptbl_a: %x, ptbl_b: %x\n", ptbl_a, ptbl_b);

    *addr_1 = val_1;
    printf("before fork: the value at address %x: %d\n", addr_1, *addr_1);

    pid = sys_fork();

    if (pid == 0) {
        printf("child process: the original value at address %x: %d\n", addr_1, *addr_1);
        *addr_1 = val_2;
        printf("child process: the new value at address %x: %d\n", addr_1, *addr_1);
    }
    else {
        printf("parent process: the original value at address %x: %d\n", addr_1, *addr_1);
    }

    return 0;
}
