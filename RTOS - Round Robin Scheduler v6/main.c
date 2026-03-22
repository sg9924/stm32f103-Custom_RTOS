#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "Semaphore.h"


uint8_t flag = 1;
tcb_t* tcb_list[NO_OF_TASKS+1];


void task1(void)
{
    uint32_t noti_value;
    while(1)
    {
        Serialprintln("[Tick: %x] [Task ID: %d] This is Task 1 running...", INFO, Systick_get_tick(), tcb_list[1]->task_id);
        
        if(flag)
        {
            if(taskNotify_Wait(0, 0, &noti_value, 10)) //wait for noti from task 2
                Serialprintln("[Tick: %x] [Task ID: %d] Notification Received from Task 2! with Value %d", INFO, Systick_get_tick(), tcb_list[1]->task_id, noti_value);
        }
    }
}

void task2(void)
{
    while(1)
   {
        Serialprintln("[Tick: %x] [Task ID: %d] This is Task 2 running...", INFO, Systick_get_tick(), tcb_list[2]->task_id);

        if(flag)
        {
            taskNotify_Send(tcb_list[1], 1, TASK_NOTIFY_ACTION_INC); //send noti to task 1
            Serialprintln("[Tick: %x] [Task ID: %d] Notification Sent to Task 1!", INFO, Systick_get_tick(), tcb_list[2]->task_id);
        }
        flag = 0;
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
}