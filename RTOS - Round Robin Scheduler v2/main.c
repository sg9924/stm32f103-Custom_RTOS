#include "rtos_kernel.h"
#include "rtos_task.h"
#include "stm32f103xx_init.h"
#include "stm32f103xx_serial.h"

#define TASK_QUANTA          500


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

    //Add the tasks
    taskAdd(&task1, "Task 1");
    taskAdd(&task2, "Task 2");
    taskAdd(&task3, "Task 3");

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA);

    while(1)
    {
        Serialprint("\r\n The Tasks have stopped running for some reason...");
    }
}