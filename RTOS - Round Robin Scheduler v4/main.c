#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "Semaphore.h"

//Set quanta in milliseconds for the round robin scheduler
#define TASK_QUANTA_MS          500

tcb_t* tcb_list;

void task1(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [Quota: %d] This is Task 1 running...", INFO, Systick_get_tick(), tcb_list[1].task_quota);
    }
}

void task2(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [Quota: %d] This is Task 2 running...", INFO, Systick_get_tick(), tcb_list[2].task_quota); 
    }
}


void task3(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [Quota: %d] This is Task 3 running...", INFO, Systick_get_tick(), tcb_list[3].task_quota); 
    }
}




int main(void)
{
    board_init();

    rtosKernel_Init();

    //Add the tasks with weights
    taskAdd_Weighted(&task1, "Task 1", 4, &tcb_list[1]);
    taskAdd_Weighted(&task2, "Task 2", 2, &tcb_list[2]);
    taskAdd_Weighted(&task3, "Task 3", 5, &tcb_list[3]);

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);
}