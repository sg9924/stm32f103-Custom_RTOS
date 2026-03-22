#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "Semaphore.h"


tcb_t* tcb_list;
uint8_t flag = 1;

void task1(void)
{
    uint32_t noti_value;
    while(1)
    {
        Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] This is Task 1 running...", INFO, Systick_get_tick(), tcb_list[1].task_id, tcb_list[1].task_priority);
        if(flag)
        {
            if(taskNotify_Wait(0, 0, &noti_value, 10))
                Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] TASK NOTIFY RECEIVED FROM TASK 2 | VALUE: %d", INFO, Systick_get_tick(), tcb_list[1].task_id,tcb_list[1].task_priority, noti_value);
        }
    }
}

void task2(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] This is Task 2 running...", INFO, Systick_get_tick(), tcb_list[2].task_id, tcb_list[2].task_priority);
        if(flag)
        {
            taskNotify_Send(&tcb_list[1], 1, TASK_NOTIFY_ACTION_INC);
            Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] TASK NOTIFY SENT TO TASK 1", INFO, Systick_get_tick(), tcb_list[2].task_id, tcb_list[2].task_priority);
        }
        flag = 0;
    }
}


void task3(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] This is Task 3 running...", INFO, Systick_get_tick(), tcb_list[3].task_id, tcb_list[3].task_priority);
        //taskDelay(3);
    }
}




int main(void)
{
    board_init();
    rtosKernel_Init();

    //4 is for the idle task
    //Lower numbers are higher priority
    taskAdd_Priority(&task1, "Task 1", 0);
    taskAdd_Priority(&task2, "Task 2", 0);
    taskAdd_Priority(&task3, "Task 3", 3);

    tcb_list = getTask_List();

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);

    while(1)
    {

    }
}