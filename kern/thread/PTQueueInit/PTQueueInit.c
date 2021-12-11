#include "lib/x86.h"

#include "import.h"

/**
 * Initializes all the thread queues with tqueue_init_at_id.
 */
void tqueue_init(unsigned int mbi_addr)
{
    // TODO: define your local variables here.

    tcb_init(mbi_addr);

    // TODO
    for(unsigned int i=0; i<NUM_IDS+1; i++)
    {
        tqueue_init_at_id(i);
    }
    return;
}

/**
 * Insert the TCB #pid into the tail of the thread queue #chid.
 * Recall that the doubly linked list is index based.
 * So you only need to insert the index.
 * Hint: there are multiple cases in this function.
 */
void tqueue_enqueue(unsigned int chid, unsigned int pid)
{
    // TODO
    if(tqueue_get_tail(chid)==NUM_IDS) //the queue is empty
    {
        tqueue_set_head(chid, pid);
        tqueue_set_tail(chid, pid);
        tcb_set_prev(pid, NUM_IDS);
        tcb_set_next(pid, NUM_IDS);
    }
    else //the queue is not empty
    {
        unsigned int target = tqueue_get_tail(chid);
        tcb_set_next(target, pid);
        tcb_set_prev(pid, target);
        tcb_set_next(pid, NUM_IDS);
        tqueue_set_tail(chid, pid);
    }
    return;
}

/**
 * Reverse action of tqueue_enqueue, i.e. pops a TCB from the head of the specified queue.
 * It returns the popped thread's id, or NUM_IDS if the queue is empty.
 * Hint: there are multiple cases in this function.
 */
unsigned int tqueue_dequeue(unsigned int chid)
{
    // TODO
    if(tqueue_get_head(chid)==NUM_IDS)
    {
        return NUM_IDS;
    }
    else
    {
        unsigned int res = tqueue_get_head(chid);
        if(tqueue_get_head(chid) == tqueue_get_tail(chid)) //the queue will be empty after pop
        {
            tcb_set_prev(res, NUM_IDS);
            tcb_set_next(res, NUM_IDS);
            tqueue_set_head(chid, NUM_IDS);
            tqueue_set_tail(chid, NUM_IDS);
        }
        else //the queue will not be empty
        {
            unsigned int next_head = tcb_get_next(res);
            tqueue_set_head(chid, next_head);
            tcb_set_prev(next_head, NUM_IDS);
            tcb_set_prev(res, NUM_IDS);
            tcb_set_next(res, NUM_IDS);
        }
        return res;
    }
    return 0;
}

/**
 * Removes the TCB #pid from the queue #chid.
 * Hint: there are many cases in this function.
 */
void tqueue_remove(unsigned int chid, unsigned int pid)
{
    // TODO
    unsigned int target = NUM_IDS;
    unsigned int next = tqueue_get_head(chid);
    while(next != NUM_IDS)
    {
        if(next == pid)
        {
            target = pid;
            break;
        }
        else
        {
            next = tcb_get_next(next);
        }
    }
    if(target == NUM_IDS)
    {
        return;
    }

    unsigned int nx = tcb_get_next(target);
    unsigned int prev = tcb_get_prev(target);

    if(tqueue_get_head(chid) == target)
    {
        tqueue_set_head(chid, nx);
    }
    if(tqueue_get_tail(chid) == target)
    {
        tqueue_set_tail(chid, prev);
    }

    if(nx != NUM_IDS)
    {
        tcb_set_prev(nx, prev);
        tcb_set_next(target, NUM_IDS);
    }
    if(prev != NUM_IDS)
    {
        tcb_set_next(prev, nx);
        tcb_set_prev(target, NUM_IDS);
    }
}
