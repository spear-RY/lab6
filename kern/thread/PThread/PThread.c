#include <lib/x86.h>
#include <lib/thread.h>

#include "import.h"

void thread_init(unsigned int mbi_addr)
{
    tqueue_init(mbi_addr);
    set_curid(0);
    tcb_set_state(0, TSTATE_RUN);
}

/**
 * Allocates a new child thread context, sets the state of the new child thread
 * to ready, and pushes it to the ready queue.
 * It returns the child thread id.
 */
unsigned int thread_spawn(void *entry, unsigned int id, unsigned int quota)
{
    // TODO
    unsigned int child_id = kctx_new(entry, id, quota);
    if(child_id == NUM_IDS)
    {
        return NUM_IDS;
    }
    tcb_set_state(child_id, TSTATE_READY);
    tqueue_enqueue(NUM_IDS, child_id);
    return child_id;
}

/**
 * Yield to the next thread in the ready queue.
 * You should set the currently running thread state as ready,
 * and push it back to the ready queue.
 * Then set the state of the popped thread as running, set the
 * current thread id, and switch to the new kernel context.
 * Hint: If you are the only thread that is ready to run,
 * do you need to switch to yourself?
 */
void thread_yield(void)
{
    // TODO
    unsigned int next_id = tqueue_dequeue(NUM_IDS);
    if(next_id == NUM_IDS)//if no in ready thread, return
    {
        return;
    }
    unsigned int now_id = get_curid();
    tcb_set_state(now_id, TSTATE_READY);
    tqueue_enqueue(NUM_IDS, now_id);

    tcb_set_state(next_id, TSTATE_RUN);
    set_curid(next_id);
    kctx_switch(now_id, next_id);
    return;
}
