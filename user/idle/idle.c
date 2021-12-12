#include <proc.h>
#include <stdio.h>
#include <syscall.h>
#include <x86.h>

int main(int argc, char **argv)
{
    printf("idle\n");

    pid_t ping_pid, pong_pid, ding_pid, fork_pid;

    // if ((ping_pid = spawn(1, 1000)) != -1)
    //     printf("ping in process %d.\n", ping_pid);
    // else
    //     printf("Failed to launch ping.\n");

    // if ((pong_pid = spawn(2, 1000)) != -1)
    //     printf("pong in process %d.\n", pong_pid);
    // else
    //     printf("Failed to launch pong.\n");

    if ((ding_pid = spawn(3, 30000)) != -1)
        printf("ding in process %d.\n", ding_pid);
    else
        printf("Failed to launch ding.\n");

    // int32_t ss = mmap(10, 100, SUPER, 1000);
    // // int32_t ss = mmap(10, 100, CONSECUTIVE, 1000);
    // printf("s: %d\n", ss);

    // if ((fork_pid = spawn(4, 1000)) != -1)
    //     printf("fork in process %d.\n", fork_pid);
    // else
    //     printf("Failed to launch fork.\n");*/

    return 0;
}
