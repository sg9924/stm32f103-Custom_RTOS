#include "rtos_task.h"
#include "rtos_port.h"

tcb_t TCBS[NO_OF_TASKS];  //declare an array of TCB's
tcb_t *pcurrent;          //current pointer to a tcb

static ptask_t ptask_list[NO_OF_TASKS];
static uint8_t task_count;

void __task_count_init(void)
{
    task_count=0;
}


ptask_t* getTaskList()
{
    return ptask_list;  
}


void taskAdd(ptask_t func_ptr, char* task_desc)
{
    ptask_list[task_count]             = func_ptr;
    
    TCBS[task_count].ptask_func        = func_ptr;
    TCBS[task_count].task_desc         = task_desc;
    TCBS[task_count].period_tick       = 0;
    TCBS[task_count].next_release_tick = 0;
    task_count++;
}


void taskAdd_Periodic(ptask_t func_ptr, uint32_t period, char* task_desc)
{
    ptask_list[task_count]             = func_ptr;

    TCBS[task_count].ptask_func        = func_ptr;
    TCBS[task_count].task_desc         = task_desc;
    TCBS[task_count].period_tick       = period;
    TCBS[task_count].next_release_tick = 0;
    task_count++;
}



void taskYield(void)
{
    SYSTICK_CLEAR();
    SYSTICK_EXCEPTION_PEND();
}