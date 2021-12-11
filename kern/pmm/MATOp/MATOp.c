#include <lib/debug.h>
#include "import.h"

#define PAGESIZE     4096
#define VM_USERLO    0x40000000
#define VM_USERHI    0xF0000000
#define VM_USERLO_PI (VM_USERLO / PAGESIZE)
#define VM_USERHI_PI (VM_USERHI / PAGESIZE)

/**
 * Allocate a physical page.
 *
 * 1. First, implement a naive page allocator that scans the allocation table (AT)
 *    using the functions defined in import.h to find the first unallocated page
 *    with normal permissions.
 *    (Q: Do you have to scan the allocation table from index 0? Recall how you have
 *    initialized the table in pmem_init.)
 *    Then mark the page as allocated in the allocation table and return the page
 *    index of the page found. In the case when there is no available page found,
 *    return 0.
 * 2. Optimize the code using memoization so that you do not have to
 *    scan the allocation table from scratch every time.
 */
unsigned int palloc()
{
    // TODO
    for(unsigned int i = VM_USERLO_PI; i < VM_USERHI_PI; i++)
    {
        if(at_is_norm(i) == 0)
            continue;
        if(at_is_allocated(i) == 1)
            continue;
        at_set_allocated(i, 1);
        return i;
    }
    return 0;
}

unsigned int consecutive_palloc(unsigned int page_num)
{
    unsigned int cnt = page_num;
    unsigned int i = VM_USERLO_PI;

    while(i<VM_USERHI_PI)
    {
        unsigned int end = i;
        while(end<VM_USERHI_PI&&cnt>0)
        {
            if(at_is_norm(i) == 0 || at_is_allocated(i) == 1)
            {
                cnt = page_num;
                i = end+1;
                break;
            }
            else
            {
                cnt--;
                end++;
            }
        }
        if(cnt==0)
        {
            unsigned start = i;
            while(start<end)
            {
                at_set_allocated(start, 1);
                start++;
            }
            return i;
        }
    }
    
    return 0;//return 0 if fail
}

/**
 * Free a physical page.
 *
 * This function marks the page with given index as unallocated
 * in the allocation table.
 *
 * Hint: Simple.
 */
void pfree(unsigned int pfree_index)
{
    // TODO
    at_set_allocated(pfree_index, 0);
}

void consecutive_pfree(unsigned int pfree_index, unsigned int len)
{
    for(int i=0;i<len;i++)
    {
        at_set_allocated(pfree_index+i, 0);
    }
}