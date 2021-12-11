#include <lib/elf.h>
#include <lib/debug.h>
#include <lib/gcc.h>
#include <lib/seg.h>
#include <lib/trap.h>
#include <lib/x86.h>
#include <lib/syscall.h>
#include <lib/string.h>

#include "import.h"

extern tf_t uctx_pool[NUM_IDS];
extern char STACK_LOC[NUM_IDS][PAGESIZE];

void proc_start_user(void)
{
    unsigned int cur_pid = get_curid();
    tss_switch(cur_pid);
    set_pdir_base(cur_pid);

    trap_return((void *) &uctx_pool[cur_pid]);
}

unsigned int proc_create(void *elf_addr, unsigned int quota)
{
    unsigned int pid, id;

    id = get_curid();

    pid = thread_spawn((void *) proc_start_user, id, quota);

    if (pid != NUM_IDS) {
        elf_load(elf_addr, pid);

        uctx_pool[pid].es = CPU_GDT_UDATA | 3;
        uctx_pool[pid].ds = CPU_GDT_UDATA | 3;
        uctx_pool[pid].cs = CPU_GDT_UCODE | 3;
        uctx_pool[pid].ss = CPU_GDT_UDATA | 3;
        uctx_pool[pid].esp = VM_USERHI;
        uctx_pool[pid].eflags = FL_IF;
        uctx_pool[pid].eip = elf_entry(elf_addr);
    }

    return pid;
}

unsigned int proc_fork(void)
{
    unsigned int pid, id;
    id = get_curid();

    //dprintf("enter proc_fork.\n");
    
    unsigned int quota = container_get_quota(id);
    unsigned int usage = container_get_usage(id);
    unsigned int assign = (quota-usage)/2;

    pid = thread_spawn((void *) proc_start_user, id, assign);

    if (pid != NUM_IDS) 
    {
        memcpy(&uctx_pool[pid], &uctx_pool[id], sizeof(uctx_pool[id]));

        dprintf("memcpy succeed.\n");

        vmm_copy(id, pid);//copy pdir and ptbl

        dprintf("proc and vmm_copy succeed.\n");
        return pid;
    }
    else
    {
        //dprintf("proc_fork failed.\n");
        return NUM_IDS;
    }

}
