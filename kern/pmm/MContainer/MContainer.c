#include <lib/debug.h>
#include <lib/x86.h>
#include "import.h"

struct SContainer {
    int quota;      // maximum memory quota of the process
    int usage;      // the current memory usage of the process
    int parent;     // the id of the parent process
    int nchildren;  // the number of child processes
    int used;       // whether current container is used by a process
};

// mCertiKOS supports up to NUM_IDS processes
static struct SContainer CONTAINER[NUM_IDS];

/**
 * Initializes the container data for the root process (the one with index 0).
 * The root process is the one that gets spawned first by the kernel.
 */
void container_init(unsigned int mbi_addr)
{
    unsigned int real_quota;
    // TODO: define your local variables here.

    unsigned int nps;

    pmem_init(mbi_addr);
    real_quota = 0;

    /**
     * TODO: Compute the available quota and store it into the variable real_quota.
     * It should be the number of the unallocated pages with the normal permission
     * in the physical memory allocation table.
     */

    nps = 0xFFFFFFFF/4096;

    for(unsigned int i=0; i<nps; i++)
    {
        if(at_is_norm(i) && !at_is_allocated(i))
        {
            real_quota++;
        }
    }

    KERN_DEBUG("\nreal quota: %d\n\n", real_quota);

    CONTAINER[0].quota = real_quota;
    CONTAINER[0].usage = 0;
    CONTAINER[0].parent = 0;
    CONTAINER[0].nchildren = 0;
    CONTAINER[0].used = 1;
}

// Get the id of parent process of process # [id].
unsigned int container_get_parent(unsigned int id)
{
    // TODO
    if(id<NUM_IDS && CONTAINER[id].used)
    {
        return (unsigned int)CONTAINER[id].parent;
    }
    return 0;
}

// Get the number of children of process # [id].
unsigned int container_get_nchildren(unsigned int id)
{
    // TODO
    if(id<NUM_IDS && CONTAINER[id].used)
    {
        return (unsigned int)CONTAINER[id].nchildren;
    }
    return 0;
}

// Get the maximum memory quota of process # [id].
unsigned int container_get_quota(unsigned int id)
{
    // TODO
    if(id<NUM_IDS && CONTAINER[id].used)
    {
        return (unsigned int)CONTAINER[id].quota;
    }
    return 0;
}

// Get the current memory usage of process # [id].
unsigned int container_get_usage(unsigned int id)
{
    // TODO
    if(id<NUM_IDS)
    {
        return (unsigned int)CONTAINER[id].usage;
    }
    return 0;
}

// Determines whether the process # [id] can consume an extra
// [n] pages of memory. If so, returns 1, otherwise, returns 0.
unsigned int container_can_consume(unsigned int id, unsigned int n)
{
    // TODO
    if(id<NUM_IDS && CONTAINER[id].used)
    {
        if(n <= (CONTAINER[id].quota - CONTAINER[id].usage))
        {
            return (unsigned int)1;
        }
    }
    return 0;
}

/**
 * Dedicates [quota] pages of memory for a new child process.
 * You can assume it is safe to allocate [quota] pages
 * (the check is already done outside before calling this function).
 * Returns the container index for the new child process.
 */
unsigned int container_split(unsigned int id, unsigned int quota)
{
    unsigned int child, nc;

    nc = CONTAINER[id].nchildren;
    if(nc >= MAX_CHILDREN)
    {
        return NUM_IDS;
    }
    child = id * MAX_CHILDREN + 1 + nc;  // container index for the child process

    if (NUM_IDS <= child) {
        return NUM_IDS;
    }

    /**
     * TODO: Update the container structure of both parent and child process appropriately.
     */
    //dprintf("enter container_split, id: %d, quota: %d, child: %d\n", id, quota, child);

    CONTAINER[id].nchildren++;
    CONTAINER[id].usage += quota;

    CONTAINER[child].used = 1;
    CONTAINER[child].quota = quota;
    CONTAINER[child].usage = 0;
    CONTAINER[child].parent = id;
    CONTAINER[child].nchildren = 0;

    return child;
}

/**
 * Allocates one more page for process # [id], given that this will not exceed the quota.
 * The container structure should be updated accordingly after the allocation.
 * Returns the page index of the allocated page, or 0 in the case of failure.
 */
unsigned int container_alloc(unsigned int id)
{
    /*
     * TODO: Implement the function here.
     */
    int page_index = palloc();
    if(page_index && CONTAINER[id].usage < CONTAINER[id].quota)
    {
        CONTAINER[id].usage++;
        return page_index;
    }
    else
    {
        return 0;
    }
}

unsigned int container_consecutive_alloc(unsigned int id, unsigned int len)
{

    if((CONTAINER[id].usage+len)>CONTAINER[id].quota)
    {

        dprintf("container_consecutive_alloc: insufficient quota: %d\n", CONTAINER[id].quota);
        return 0;
    }

    int page_index = consecutive_palloc(len);

    if(page_index && CONTAINER[id].usage < CONTAINER[id].quota)
    {
        CONTAINER[id].usage += len;
        return page_index;
    }
    else
    {
        //dprintf("container_consecutive_alloc: consecutive_palloc failed\n");
        return 0;
    }
}

// Frees the physical page and reduces the usage by 1.
void container_free(unsigned int id, unsigned int page_index)
{
    // TODO
    if(CONTAINER[id].usage > 0)
    {
        CONTAINER[id].usage--;
        pfree(page_index);
    }
    return;
}

void container_consecutive_free(unsigned int id, unsigned int page_index, unsigned int len)
{
    // TODO
    CONTAINER[id].usage-=len;
    consecutive_pfree(page_index, len);
    return;
}

void container_super_free(unsigned int id, unsigned int page_index)
{
    const unsigned int len = 1024;
    container_consecutive_free(id, page_index, len);
    return;
}
