#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "rtos_queue.h"
#include"memmgmt.h"


tcb_t* tcb_list;
Queue_t q;
uint8_t buf[100];


void task1(void)
{
    uint8_t r = 0;
    
    while(1)
    {
        Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] [Consumer] Waiting for Data...", INFO, Systick_get_tick(), tcb_list[1].task_id, tcb_list[1].task_priority);

        if(queueReceive(&q, &r, 10) == true)
        {
            Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] [Consumer] Received %d", INFO, Systick_get_tick(), tcb_list[1].task_id, tcb_list[1].task_priority, r);
        }
        else
            Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] [Consumer] Not able to receive data from queue...", INFO);
    }
}

void task3(void)
{
    uint8_t peek_value;
    while(1)
    {
        queuePeek(&q, (void*)&peek_value);
        Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] [Monitor] Queue Front: %d, Queue Rear: %d, Queue Peek: %d, Buffer Base Addr: %x, Buffer Current Addr: %x", INFO, Systick_get_tick(), tcb_list[3].task_id, tcb_list[3].task_priority, q.front, q.rear, peek_value, q.buffer, queueBufferAddr(&q, 0));
        taskDelay(3);
    }
}


void task2(void)
{
    uint8_t v = 0;
    while(1)
    {
        v++;
        Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] [Producer] Sending %d", INFO, Systick_get_tick(), tcb_list[2].task_id, tcb_list[2].task_priority, v);

        if(queueSend(&q, &v, 10) == true)
        {
            Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] [Producer] Sent %d", INFO, Systick_get_tick(), tcb_list[2].task_id, tcb_list[2].task_priority,v);
        }
        else
            Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] [Producer] Not able to send %d...", INFO, Systick_get_tick(), tcb_list[2].task_id, tcb_list[2].task_priority, v);
        taskDelay(3);
    }
}



int main(void)
{
    board_init();
    rtosKernel_Init();

    //reset structure and buffer values to 0
    xmemset(&q, 0, sizeof(q));
    xmemset(&buf, 0, sizeof(buf[100]));


    //create the static queue structure
    if(queueCreateStatic(sizeof(uint8_t), 10, &q, (uint8_t*)&buf) == &q)
        Serialprintln("[main] Static Queue created successfully", INFO);
    else
        Serialprintln("[main] Static Queue was not created successfully...", FATAL);

    //Lower numbers are higher priority
    taskAdd_Priority(&task1, "T1 - Consumer", 2);
    taskAdd_Priority(&task2, "T2 - Producer", 0);
    taskAdd_Priority(&task3, "T3 - Monitor", 1);

    tcb_list = getTask_List();

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);
}