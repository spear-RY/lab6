#include <lib/x86.h>

/**
 * Kernel thread context.
 * When you switch to another kernel thread, you need to save
 * the current thread's states and restore the new thread's states.
 */
struct kctx {
    void *esp;
    unsigned int edi;
    unsigned int esi;
    unsigned int ebx;
    unsigned int ebp;
    void *eip;
};

// Memory to save the NUM_IDS kernel thread states.
struct kctx kctx_pool[NUM_IDS];

void kctx_set_esp(unsigned int pid, void *esp)
{
    kctx_pool[pid].esp = esp;
}

void kctx_set_eip(unsigned int pid, void *eip)
{
    kctx_pool[pid].eip = eip;
}

extern void cswitch(struct kctx *from_kctx, struct kctx *to_kctx);

/**
 * Saves the states for thread # [from_pid] and restores the states
 * for thread # [to_pid].
 */
void kctx_switch(unsigned int from_pid, unsigned int to_pid)
{
    //dprintf("enter cswitch from %d to %d.\n",from_pid,to_pid);
    //dprintf("from: esp: %08x, edi: %08x, esi: %08x, ebx: %08x, ebp: %08x, eip: %08x\n", (unsigned int) kctx_pool[from_pid].esp, kctx_pool[from_pid].edi, kctx_pool[from_pid].esi, kctx_pool[from_pid].ebx, kctx_pool[from_pid].ebp, (unsigned int) kctx_pool[from_pid].eip);
    //dprintf("to: esp: %08x, edi: %08x, esi: %08x, ebx: %08x, ebp: %08x, eip: %08x\n", (unsigned int) kctx_pool[to_pid].esp, kctx_pool[to_pid].edi, kctx_pool[to_pid].esi, kctx_pool[to_pid].ebx, kctx_pool[to_pid].ebp, (unsigned int) kctx_pool[to_pid].eip);
    cswitch(&kctx_pool[from_pid], &kctx_pool[to_pid]);
    //dprintf("cswitch succeed.\n");
}
