#include"rtos_queue.h"
#include"rtos_port.h"



//queues for tasks with priority - one linked list per priority
tcb_t* ready_queue[1];
tcb_t* blocked_queue[1];

extern tcb_t TCBS[NO_OF_TASKS+1];                 //an array of TCB's

void ready_queue_add(tcb_t* task)
{
    //set state as ready
    task->task_state = TASK_STATE_READY;
    task->pnext      = NULL;

    if(ready_queue[0] == NULL)
    {
        ready_queue[0] = task;
    }
    else
    {
        //get the starting task of the specific priority
        tcb_t* i = ready_queue[0];

        //iterate till the end of the linked list
        while(i->pnext != NULL) i = i->pnext;

        //add the task to the last node in the queue
        i->pnext = task;
    }
}



void blocked_queue_add(tcb_t* task)
{
    //set state as blocked
    task->task_state = TASK_STATE_BLOCKED;
    task->pnext      = NULL;

    if(blocked_queue[0] == NULL)
        blocked_queue[0] = task;
    else
    {
        //get the starting task of the specific priority
        tcb_t* i = blocked_queue[0];

        //iterate till the end of the linked list
        while(i->pnext != NULL) i = i->pnext;

        //add the task to the last node in the queue
        i->pnext = task;
    }
}



void ready_queue_init()
{
    for(uint8_t i=0; i<1; i++)
        ready_queue[i] = NULL;
}


void blocked_queue_init()
{
    for(uint8_t i=0; i<1; i++)
        blocked_queue[i] = NULL;
}


uint8_t ready_queue_check_empty()
{
    for(uint8_t i=0; i<1; i++)
    {
        if(ready_queue[i] == NULL)
            continue;
        else
            return 0;
    }
    return 1;
}


uint8_t blocked_queue_check_empty()
{
    for(uint8_t i=0; i<1; i++)
    {
        if(blocked_queue[i] == NULL)
            continue;
        else
            return 0;
    }
    return 1;
}


uint8_t blocked_queue_check(tcb_t* task)
{
    tcb_t* t = blocked_queue[0];

    while(t != NULL)
    {
        if(t == task) return 1;
        else break;

        t = t->pnext;
    }
    return 0;
}



void ready_queue_reset()
{
    for(uint8_t i=1; i<NO_OF_TASKS+1; i++)
    {
        if(blocked_queue_check(&TCBS[i]) != 1)
            ready_queue_add(&TCBS[i]);
    }
}



uint8_t ready_queue_remove(tcb_t* task, uint8_t state)
{
    if(ready_queue[0] != NULL)
    {
        //get the starting task of the specific priority
        tcb_t* i = ready_queue[0];

        //starting task of queue matches
        if(i == task)
        {
            i->task_state = state;
            blocked_queue[0] = i->pnext;
        }
        //first task didn't match
        else
        {   
            //iterate
            while(i->pnext != NULL)
            {
                if(i->pnext == task)
                {
                    (i->pnext)->task_state = state;
                    i = (i->pnext)->pnext;
                    return 1;
                }
                i = i->pnext;
            }
        }
    }
    return 0;
}


uint8_t blocked_queue_remove(tcb_t* task, uint8_t state)
{
    if(blocked_queue[0] != NULL)
    {
        //get the starting task of the specific priority
        tcb_t* i = blocked_queue[0];

        //starting task of queue matches
        if(i == task)
        {
            i->task_state = state;
            i->block_tick = 0;
            blocked_queue[0] = i->pnext;
        }
        //first task didn't match
        else
        {   
            //iterate
            while(i->pnext != NULL)
            {
                if(i->pnext == task)
                {
                    (i->pnext)->task_state = state;
                    (i->pnext)->block_tick = 0;
                    i = (i->pnext)->pnext;
                    return 1;
                }
                i = i->pnext;
            }
        }
    }
    return 0;
}