#include "rtos_kernel.h"
#include "rtos_task.h"
#include "stm32f103xx_init.h"
#include "stm32f103xx_serial.h"

#define TASK_QUANTA     500


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
    addTask(&task1);
    addTask(&task2);
    addTask(&task3);

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA);

    while(1)
    {

    }
}