#include <lib/x86.h>
#include <lib/debug.h>
#include <proc/PProc/export.h>
#include <pmm/MContainer/export.h>
#include <vmm/MPTOp/export.h>
#include <vmm/MPTNew/export.h>
#include "export.h"


int MPTFork_test()
{
    unsigned int vaddr = 4096 * 1024 * 300;
    const unsigned int page_size = 0x1<<12;
    const unsigned int P_SUPER = 1<<9;

    unsigned ch1 = container_split(0, 10000);

    alloc_super_page(ch1, vaddr, 7);

    unsigned int vaddr_now = vaddr;

    for(int i=0;i<1024;i++)
    {
        if (get_ptbl_entry_by_va(ch1, vaddr_now) == 0||(get_ptbl_entry_by_va(ch1, vaddr_now)&P_SUPER)==0) {
            dprintf("test 1.1 failed: (%d == 0)\n", get_ptbl_entry_by_va(ch1, vaddr_now));
            return 1;
        }
        if (get_pdir_entry_by_va(ch1, vaddr_now) == 0) {
            dprintf("test 1.2 failed: (%d == 0)\n", get_pdir_entry_by_va(ch1, vaddr_now));
            return 1;
        }
        vaddr_now += page_size;
    }

    dprintf("test 1.1 succeeded.\n");
    dprintf("test 1.2 succeeded.\n");

    unsigned ch2 = container_split(0, 10000);
    vmm_copy(ch1, ch2);

    dprintf("proc_fork succeed, chid:%d\n",ch2);    

    if (get_ptbl_entry_by_va(ch2, vaddr+500*page_size) == 0) {
        dprintf("test 1.3 failed: (%d == 0)\n", get_ptbl_entry_by_va(ch2, vaddr+500*page_size));
        return 1;
    }
    dprintf("test 1.3 succeeded.\n");
    if (get_pdir_entry_by_va(ch2, vaddr+500*page_size) == 0) {
        dprintf("test 1.4 failed: (%d == 0)\n", get_pdir_entry_by_va(ch2, vaddr+500*page_size));
        return 1;
    }
    dprintf("test 1.4 succeeded.\n");

    copy_on_write(ch2, vaddr);

    if (get_ptbl_entry_by_va(ch2, vaddr+500*page_size) == 0) {
        dprintf("test 1.5 failed: (%d == 0)\n", get_ptbl_entry_by_va(ch2, vaddr+500*page_size));
        return 1;
    }
    dprintf("test 1.5 succeeded.\n");
    if (get_pdir_entry_by_va(ch2, vaddr+500*page_size) == 0) {
        dprintf("test 1.6 failed: (%d == 0)\n", get_pdir_entry_by_va(ch2, vaddr+500*page_size));
        return 1;
    }
    dprintf("test 1.6 succeeded.\n");

    dprintf("test 1 passed.\n");
    return 0;
}

/**
 * Write Your Own Test Script (optional)
 *
 * Come up with your own interesting test cases to challenge your classmates!
 * In addition to the provided simple tests, selected (correct and interesting) test functions
 * will be used in the actual grading of the lab!
 * Your test function itself will not be graded. So don't be afraid of submitting a wrong script.
 *
 * The test function should return 0 for passing the test and a non-zero code for failing the test.
 * Be extra careful to make sure that if you overwrite some of the kernel data, they are set back to
 * the original value. O.w., it may make the future test scripts to fail even if you implement all
 * the functions correctly.
 */
int MPTFork_test_own()
{
    // TODO (optional)
    // dprintf("own test passed.\n");
    return 0;
}

int test_MPTFork()
{
    return MPTFork_test() + MPTFork_test_own();
}
