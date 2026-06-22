#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "Semaphore.h"



tcb_t* tcb_list[NO_OF_TASKS];


void gpio_led_init()
{
    GPIO_Handle G;
    //Red LED - Pin A4
    GPIO_Config(&G, GPIOA, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, GPIO_PIN4, GPIO_OP_SPEED_2);
    GPIO_Init(&G);
    //Yellow LED - Pin A5
    GPIO_Config(&G, GPIOA, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, GPIO_PIN5, GPIO_OP_SPEED_2);
    GPIO_Init(&G);
    //Green LED - Pin A6
    GPIO_Config(&G, GPIOA, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, GPIO_PIN6, GPIO_OP_SPEED_2);
    GPIO_Init(&G);
}

void task1(void)
{
    while(1)
    {
        uint32_t last_wake = Systick_get_tick();
        Serialprintln("[Tick: %x] [ID: %d] Task 1", INFO, Systick_get_tick(), tcb_list[0]->task_id, tcb_list[0]->task_priority);
        GPIO_OpToggle(GPIOA, GPIO_PIN4);
        taskDelayAbs(&last_wake, MS_TO_TICK(10));
    }
}

void task2(void)
{
    while(1)
    {
        uint32_t last_wake = Systick_get_tick();
        Serialprintln("[Tick: %x] [ID: %d] Task 2", INFO, Systick_get_tick(), tcb_list[1]->task_id, tcb_list[1]->task_priority);
        GPIO_OpToggle(GPIOA, GPIO_PIN5);
        taskDelayAbs(&last_wake, MS_TO_TICK(20));
    }
}


void task3(void)
{
    while(1)
    {
        uint32_t last_wake = Systick_get_tick();
        Serialprintln("[Tick: %x] [ID: %d] Task 3", INFO, Systick_get_tick(), tcb_list[2]->task_id, tcb_list[2]->task_priority);
        GPIO_OpToggle(GPIOA, GPIO_PIN6);
        taskDelayAbs(&last_wake, MS_TO_TICK(30));
    }
}




int main(void)
{
    board_init();
    rtosKernel_Init();
    gpio_led_init();

    //Round Robin
    //Add the tasks
    tcb_list[0] = taskAdd(&task1, "Task 1", 60);
    tcb_list[1] = taskAdd(&task2, "Task 2", 60);
    tcb_list[2] = taskAdd(&task3, "Task 3", 60);

    /*
    //Round Robin Weighted
    tcb_list[0] = taskAdd_Weighted(&task1, "Task 1", 1);
    tcb_list[1] = taskAdd_Weighted(&task2, "Task 2", 1);
    tcb_list[2] = taskAdd_Weighted(&task3, "Task 3", 1);
    */

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);
}