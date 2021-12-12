#include <lib/debug.h>
#include <lib/pmap.h>
#include <lib/types.h>
#include <lib/x86.h>
#include <lib/trap.h>
#include <lib/syscall.h>

#include <pmm/MContainer/export.h>
#include <vmm/MPTFork/export.h>
#include <vmm/MPTOp/export.h>

#include "import.h"

#define SUPER 200
#define CONSECUTIVE 300

static char sys_buf[NUM_IDS][PAGESIZE];

/**
 * Copies a string from user into buffer and prints it to the screen.
 * This is called by the user level "printf" library as a system call.
 */
void sys_puts(void)
{
    unsigned int cur_pid;
    unsigned int str_uva, str_len;
    unsigned int remain, cur_pos, nbytes;

    cur_pid = get_curid();
    str_uva = syscall_get_arg2();
    str_len = syscall_get_arg3();

    if (!(VM_USERLO <= str_uva && str_uva + str_len <= VM_USERHI)) {
        syscall_set_errno(E_INVAL_ADDR);
        return;
    }

    remain = str_len;
    cur_pos = str_uva;

    while (remain) {
        if (remain < PAGESIZE - 1)
            nbytes = remain;
        else
            nbytes = PAGESIZE - 1;

        if (pt_copyin(cur_pid, cur_pos, sys_buf[cur_pid], nbytes) != nbytes) {
            syscall_set_errno(E_MEM);
            return;
        }

        sys_buf[cur_pid][nbytes] = '\0';
        KERN_INFO("%s", sys_buf[cur_pid]);

        remain -= nbytes;
        cur_pos += nbytes;
    }

    syscall_set_errno(E_SUCC);
}

extern uint8_t _binary___obj_user_pingpong_ping_start[];
extern uint8_t _binary___obj_user_pingpong_pong_start[];
extern uint8_t _binary___obj_user_pingpong_ding_start[];
extern uint8_t _binary___obj_user_fork_test_fork_test_start[];

/**
 * Spawns a new child process.
 * The user level library function sys_spawn (defined in user/include/syscall.h)
 * takes two arguments [elf_id] and [quota], and returns the new child process id
 * or NUM_IDS (as failure), with appropriate error number.
 * Currently, we have three user processes defined in user/pingpong/ directory,
 * ping, pong, and ding.
 * The linker ELF addresses for those compiled binaries are defined above.
 * Since we do not yet have a file system implemented in mCertiKOS,
 * we statically load the ELF binaries into the memory based on the
 * first parameter [elf_id].
 * For example, ping, pong, ding and fork_test correspond to the elf_ids
 * 1, 2, 3, and 4, respectively.
 * If the parameter [elf_id] is none of these, then it should return
 * NUM_IDS with the error number E_INVAL_PID. The same error case apply
 * when the proc_create fails.
 * Otherwise, you should mark it as successful, and return the new child process id.
 */
void sys_spawn(void)
{
    // TODO
    unsigned int elf_id = syscall_get_arg2();
    unsigned int quota = syscall_get_arg3();
    //dprintf("elf_id: %d\n",elf_id);
    uint8_t *elf_addr;
    if(elf_id == 1)
    {
        elf_addr = _binary___obj_user_pingpong_ping_start;
    }
    else if(elf_id == 2)
    {
        elf_addr = _binary___obj_user_pingpong_pong_start;
    }
    else if(elf_id == 3)
    {
        elf_addr = _binary___obj_user_pingpong_ding_start;
    }
    else if(elf_id == 4)
    {
        elf_addr = _binary___obj_user_fork_test_fork_test_start;
    }
    else
    {
        syscall_set_errno(E_INVAL_PID);
        syscall_set_retval1(NUM_IDS);
        return;
    }

    unsigned int pid = proc_create(elf_addr, quota);
    if(pid == NUM_IDS)
    {
        syscall_set_errno(E_INVAL_PID);
        syscall_set_retval1(NUM_IDS);
        return;
    }
    else
    {
        syscall_set_errno(E_SUCC);
        syscall_set_retval1(pid);
        return;
    }
}

/**
 * Yields to another thread/process.
 * The user level library function sys_yield (defined in user/include/syscall.h)
 * does not take any argument and does not have any return values.
 * Do not forget to set the error number as E_SUCC.
 */
void sys_yield(void)
{
    // TODO
    thread_yield();
    syscall_set_errno(E_SUCC);
    return;
}

// Your implementation of fork
void sys_fork()
{
    // TODO
    //dprintf("enter sys_fork.\n");
    unsigned int cur_pid;
    cur_pid = get_curid();
    unsigned int pid = proc_fork();
    if(pid == NUM_IDS)
    {
        syscall_set_errno(E_INVAL_PID);
        syscall_set_retval1(NUM_IDS);
        return;
    }
    else
    {
        syscall_set_errno(E_SUCC);
        syscall_set_retval1(pid);
        return;
    }
    return;
}


void sys_mmap(void)
{
    unsigned int curid = get_curid();
    unsigned int size = syscall_get_arg2();
    unsigned int perm = syscall_get_arg3();
    unsigned int flag = syscall_get_arg4();
    unsigned int vaddr = syscall_get_arg5();

    unsigned int quota = container_get_quota(curid);
    unsigned int usage = container_get_usage(curid);

    KERN_INFO("In process %d, size: %d, perm: %d, flag: %d\n",curid, size, perm, flag);

    unsigned int reg_page;

    if(flag == SUPER)//Request a super page
    {
        reg_page = alloc_super_page(curid, vaddr, perm);
    }
    else if(flag == CONSECUTIVE)//Request consecutive pages
    {
        reg_page = alloc_consecutive_page(curid, vaddr, perm, size);
    }
    else
    {
        reg_page = alloc_page(curid, vaddr, perm);
    }

    if(reg_page == MagicNumber)
    {
        syscall_set_errno(E_MEM);
        syscall_set_retval1(MagicNumber);
        return;
    }

    syscall_set_errno(E_SUCC);
    syscall_set_retval1(reg_page); //我看你alloc_super_page的返回值就是get_ptb_entry所以我就直接在这里return了
}

//我把write和read给你连回user process了，在那边直接write，read就行
void sys_write(void)//only support writing char
{
    const unsigned int pte_w = 0x002;

    unsigned int curid = get_curid();
    unsigned int vaddr = syscall_get_arg2();
    char data = syscall_get_arg3();

    // unsigned int quota = container_get_quota(curid);
    // unsigned int usage = container_get_usage(curid);

    // KERN_INFO("In process %d, size: %d, perm: %d, flag: %d\n",curid, size, perm, flag);

    // unsigned int ptbl_entry = get_ptbl_entry_by_va(curid, vaddr);

    // if((ptbl_entry&pte_w)==0)//unwritable. call trap -> pagefault handler
    // {
    //     //不确定是不是能直接叫 void pgflt_handler(void);
    //     pgflt_handler();
    // }

    // char* target = (ptbl_entry&0xFFFFF000)+(vaddr&0x00000FFF);
    // *target = data;

    syscall_set_errno(E_SUCC);
    return;
}

void sys_read(void)//only support reading char
{
    const unsigned int pte_w = 0x002;

    unsigned int curid = get_curid();
    unsigned int vaddr = syscall_get_arg2();

    // unsigned int ptbl_entry = get_ptbl_entry_by_va(curid, vaddr);

    // if((ptbl_entry&PTE_W)==0)//unwritable. call trap -> pagefault handler
    // {
    //     pgflt_handler();
    // }

    // char* target = (ptbl_entry&0xFFFFF000)+(vaddr&0x00000FFF);
    // char res = *target;//return res;

    char res = 0;

    syscall_set_errno(E_SUCC);
    syscall_set_retval1(res);
}

void sys_get_ptbl_entry(void)//这个改一下名，叫sys_get_ptbl_entry
{
    unsigned int curid = get_curid();
    unsigned int vaddr = syscall_get_arg2();

    unsigned int ptbl_entry = get_ptbl_entry_by_va(curid, vaddr);

    KERN_INFO("get_ptble_entry In process %d, vaddr: %x\n", curid, vaddr);

    syscall_set_errno(E_SUCC);
    syscall_set_retval1(ptbl_entry);
}
