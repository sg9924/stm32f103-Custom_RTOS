#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"

#define TASK_QUANTA_MS          500


void task1(void)
{
    while(1)
    {
        Serialprint("\r\nThis is Task 1 running...");
    }
}

void task2(void)
{
    while(1)
    {
        Serialprint("\r\nThis is Task 2 running...");
        taskYield();
    }
}

void task3(void)
{
    while(1)
    {
        Serialprint("\r\nThis is Task 3 running...");
    }
}



int main(void)
{
    board_init();

    __task_count_init();

    //Add the tasks
    taskAdd(&task1, "Task 1");
    taskAdd(&task2, "Task 2");
    taskAdd(&task3, "Task 3");

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);

    while(1)
    {
        Serialprint("\r\n The Tasks have stopped running for some reason...");
    }
}