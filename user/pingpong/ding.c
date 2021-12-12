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

    //unsigned int data = get_ptbl_entry(1000);
    //printf("%d\n", data);

    unsigned int ptbl_a, ptbl_b, ptbl_c, ptbl_d;

    uintptr_t mptr = mmap(1024, PTE_P | PTE_W | PTE_U, SUPER, (unsigned int)addr_1);
    ptbl_a = get_ptbl_entry(addr_1);
    ptbl_b = get_ptbl_entry(((unsigned int)addr_1)+500*page_size);

    uintptr_t mptr_2 = mmap(10, PTE_P | PTE_W | PTE_U, CONSECUTIVE, (unsigned int)addr_2);
    ptbl_c = get_ptbl_entry(addr_2);
    ptbl_d = get_ptbl_entry(((unsigned int)addr_2)+5*page_size);

    *addr_1 = val_1;
    printf("before fork: the value at address %x: %d\n", addr_1, *addr_1);

    pid_t pid;

    pid = sys_fork();
    printf("after fork pid: %d\n", pid);

    if (pid == 0) {
        printf("child process: the original value at address %x: %d\n", addr_1, *addr_1);
        *addr_1 = val_2;
        printf("child process: the new value at address %x: %d\n", addr_1, *addr_1);
    }
    else {
        printf("parent process: the original value at address %x: %d\n", addr_1, *addr_1);
    }

    printf("Alloc Super Page: getting ptbl entries at addr_a and addr_a + 500 * page_size\n");
    printf("addr_a: %x, addr_b: %x\n", addr_1, ((unsigned int)addr_1)+500*page_size);
    printf("ptbl_a: %x, ptbl_b: %x\n", ptbl_a, ptbl_b);

    printf("Alloc Consecutive Page: getting ptbl entries at addr_a and addr_a + 5 * page_size\n");
    printf("addr_a: %x, addr_b: %x\n", addr_2, ((unsigned int)addr_2)+5*page_size);
    printf("ptbl_a: %x, ptbl_b: %x\n", ptbl_c, ptbl_d);

    return 0;
}
