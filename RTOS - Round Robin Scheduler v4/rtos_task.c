#include "rtos_task.h"
#include "rtos_port.h"

tcb_t TCBS[NO_OF_TASKS+1];  //declare an array of TCB's
tcb_t *pcurrent;          //current pointer to a tcb

static ptask_t ptask_list[NO_OF_TASKS + 1];
static uint8_t task_count;

extern uint32_t current_tick;

static void taskAdd_Check(uint8_t task_count);

void __task_count_init(void)
{
    task_count=1;
}


static void taskAdd_Check(uint8_t task_count)
{
    if(task_count>NO_OF_TASKS)
    {
        Serialprintln("Incorrect Task Add!! | Configured Tasks: %d | Current Task No.: %d", FATAL, NO_OF_TASKS, task_count);
        SERIAL_NL();
        __asm("BKPT #0");
    }
}


void taskAdd(ptask_t func_ptr, char* task_desc, tcb_t* ptask_handle)
{
    taskAdd_Check(task_count);

    ptask_list[task_count]             = func_ptr;
    
    TCBS[task_count].ptask_func        = func_ptr;
    TCBS[task_count].task_id           = task_count;
    TCBS[task_count].task_state        = TASK_STATE_READY;
    TCBS[task_count].task_desc         = task_desc;
    TCBS[task_count].block_tick        = 0;

    Serialprintln("Task %d added", INFO, task_count);
    ptask_handle = &TCBS[task_count++];
}



void taskAdd_Weighted(ptask_t func_ptr, char* task_desc, uint8_t task_weight, tcb_t* ptask_handle)
{
    taskAdd_Check(task_count);

    ptask_list[task_count]             = func_ptr;
    
    TCBS[task_count].ptask_func        = func_ptr;
    TCBS[task_count].task_id           = task_count;
    TCBS[task_count].task_state        = TASK_STATE_READY;
    TCBS[task_count].task_desc         = task_desc;
    TCBS[task_count].task_weight       = task_weight;
    TCBS[task_count].block_tick        = 0;

    Serialprintln("Task %d added", INFO, task_count);
    ptask_handle = &TCBS[task_count++];
}



void taskReset_Quota()
{
    tcb_t* tcb = TCBS;
    if(!tcb) return;
    do
    {
        if(tcb->task_state == TASK_STATE_READY || tcb->task_state == TASK_STATE_RUNNING)
             tcb->task_quota = tcb->task_weight;
        tcb = tcb->pnext;
    }while(tcb != TCBS);
}



void taskAdd_Idle()
{
    TCBS[0].ptask_func  = &taskIdle;
    TCBS[0].task_desc   = "Idle Task";
    TCBS[0].task_id     = 0;
    TCBS[0].task_weight = 0;
}



void taskDelay(uint32_t tick)
{
    //for all tasks other than idle task
    if(pcurrent->task_id)
    {
        pcurrent->block_tick = current_tick + tick;
        pcurrent->task_state = TASK_STATE_BLOCKED;

        //Pend the systick Exception to switch to next task
        SYSTICK_EXCEPTION_PEND();
    }
}



void taskIdle(void)
{
    while(1)
    {
        Serialprint("\r\nNo Tasks to run...", INFO);
    }
}



void taskUnblock(void)
{
    tcb_t* temp = pcurrent;
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        if(temp->task_state == TASK_STATE_BLOCKED)
        {
            if(temp->block_tick == current_tick)
                temp->task_state = TASK_STATE_READY;
        }
        temp = temp->pnext;
    }
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

tcb_t* getIdleTask_TCB()
{
    return &TCBS[0];
}


uint8_t getTaskCount()
{
    return task_count-1;
}