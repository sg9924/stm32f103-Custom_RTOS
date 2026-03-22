#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "Semaphore.h"


tcb_t* tcb_list[NO_OF_TASKS+1];

void task1(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [ID: %d] This is Task 1 running...", INFO, Systick_get_tick(), tcb_list[1]->task_id);
        taskDelay(5);
    }
}

void task2(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [ID: %d] This is Task 2 running...", INFO, Systick_get_tick(), tcb_list[2]->task_id);
        taskDelay(2);
    }
}


void task3(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [ID: %d] This is Task 3 running...", INFO, Systick_get_tick(), tcb_list[3]->task_id); 
    }
}




int main(void)
{
    board_init();

    rtosKernel_Init();

    //Add the tasks
    taskAdd(&task1, "Task 1", &tcb_list[1]);
    taskAdd(&task2, "Task 2", &tcb_list[2]);
    taskAdd(&task3, "Task 3", &tcb_list[3]);

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);

    while(1)
    {

    }
}