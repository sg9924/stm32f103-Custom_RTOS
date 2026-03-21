#include "rtos_task.h"
#include "rtos_kernel.h"
#include "rtos_port.h"


//external variables
extern uint32_t current_tick;
extern tcb_t* ready_queue[TASK_MAX_PRIORITY];
extern tcb_t* blocked_queue[TASK_MAX_PRIORITY];

tcb_t TCBS[NO_OF_TASKS+1];  //declare an array of TCB's
tcb_t *pcurrent;            //current pointer to a tcb

static ptask_t ptask_list[NO_OF_TASKS + 1];
static uint8_t task_count;

static void taskIdle(void);




void __task_count_init(void)
{
    task_count=1;
}



void taskAdd_Priority(ptask_t func_ptr, char* task_desc, uint8_t task_priority)
{
    if(task_count>NO_OF_TASKS)
    {
        Serialprintln("Incorrect Task Add!! | Configured Tasks: %d | Current Task No.: %d", FATAL, NO_OF_TASKS, task_count);
        SERIAL_NL();
        __asm("BKPT #0");
    }
    
    ptask_list[task_count]               = func_ptr;
    
    TCBS[task_count].ptask_func          = func_ptr;
    TCBS[task_count].task_id             = task_count;
    TCBS[task_count].task_state          = TASK_STATE_READY;
    TCBS[task_count].task_desc           = task_desc;
    TCBS[task_count].task_priority       = task_priority;
    TCBS[task_count].block_tick          = 0;
    ready_queue_add(&TCBS[task_count]);

    Serialprintln("'%s' task has been added", INFO, TCBS[task_count].task_desc);
    task_count++;
}



void taskAdd_Idle()
{
    TCBS[0].ptask_func    = &taskIdle;
    TCBS[0].task_desc     = "Idle Task";
    TCBS[0].task_id       = 0;
    TCBS[0].task_priority = TASK_MAX_PRIORITY; //lowest priority
}




void taskDelay(uint32_t timeout_tick)
{   
    //task should NOT be blocked already and it should NOT be the idle task
    if(pcurrent->task_id != 0 && pcurrent->task_state != TASK_STATE_BLOCKED)
    {
        //set state as blocked
        pcurrent->task_state = TASK_STATE_BLOCKED;

        //set block ticks
        pcurrent->block_tick = current_tick + timeout_tick;

        //insert into blocked queue
        blocked_queue_add(pcurrent);

        //Pend the systick Exception to switch to next task
        SYSTICK_EXCEPTION_PEND();
    }
}


//Idle Task Definition
static void taskIdle(void)
{
    while(1)
    {
        Serialprint("[Tick: %d] [ID: %d] [Priority: %d] Idle Task\r\n", INFO, current_tick, TCBS[0].task_id, TCBS[0].task_priority);
    }
}



void taskUnblock(void)
{
    uint8_t priority;
    #if SCHEDULER == SCHEDULER_PRIORITY
    priority = TASK_MAX_PRIORITY;
    #endif

    //iterate through each pirority from highest to lowest
    for(uint8_t i=0; i<priority; i++)
    {
        //get the starting task of the priority
        tcb_t* t = blocked_queue[i];

        //go through the tasks in the queue
        while(t != NULL)
        {
            //check block tick
            if(t->task_state == TASK_STATE_BLOCKED)
            {
                //if block tick matches current tick, block duration is over
                if(t->block_tick == current_tick)
                {
                    //set task as ready
                    t->task_state = TASK_STATE_READY;
                    t->block_tick = 0;

                    //dequeue from blocked queue
                    blocked_queue[i] = t->pnext;
                    t->pnext = NULL;

                    return;
                }
            }
            t = t->pnext;
        }
    }
    return;
}



void taskYield(void)
{
    SYSTICK_CLEAR();
    SYSTICK_EXCEPTION_PEND();
}






ptask_t getTaskFunc(uint8_t task_num)
{
    return ptask_list[task_num];
}

ptask_t* getTaskFunc_List()
{
    return ptask_list;  
}

tcb_t* getTask_List()
{
    return TCBS;
}

tcb_t* getTask_Idle()
{
    return &TCBS[0];
}


uint8_t getTask_Count()
{
    return task_count-1;
}


tcb_t* getTask_Priority(uint8_t priority)
{
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        if(TCBS[i].task_priority == priority)
            return &TCBS[i];
    }

    return (getTask_Idle());
}