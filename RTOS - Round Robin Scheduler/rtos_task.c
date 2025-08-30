#include "rtos_task.h"


static ptask_t ptask_list[NO_OF_TASKS]; 
static uint8_t task_count;

void __task_count_init()
{
    task_count=0;
}


void addTask(ptask_t func_ptr)
{
    ptask_list[task_count++] = func_ptr;
}

ptask_t* getTaskList()
{
    return ptask_list;  
}