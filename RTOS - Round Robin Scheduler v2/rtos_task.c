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
    TCBS[task_count].ptask_func = func_ptr;

    TCBS[task_count].ptask_func        = func_ptr;
    TCBS[task_count].task_desc         = task_desc;
}


void taskYield(void)
{
    SYSTICK_CLEAR();
    SYSTICK_EXCEPTION_PEND();
}