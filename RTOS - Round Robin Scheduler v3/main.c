#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "Semaphore.h"

//Set quanta in milliseconds for the round robin scheduler
#define TASK_QUANTA_MS          500

extern tcb_t TCBS[NO_OF_TASKS+1];  //an array of TCB's

void task1(void)
{
    while(1)
    {
        Serialprintln("[%x] This is Task 1 running...", INFO, Systick_get_tick());
        //taskDelay(100);
    }
}

void task2(void)
{
    while(1)
    {
        Serialprintln("[%x] This is Task 2 running...", INFO, Systick_get_tick()); 
    }
}


void task3(void)
{
    while(1)
    {
        Serialprintln("[%x] This is Task 3 running...", INFO, Systick_get_tick()); 
    }
}




int main(void)
{
    board_init();

    char a[50] = "Hello";
    char b[50] = " There";

    __task_count_init();

    //Add the tasks
    taskAdd(&task1, "Task 1");
    taskAdd(&task2, "Task 2");
    taskAdd(&task3, "Task 3");

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);
}