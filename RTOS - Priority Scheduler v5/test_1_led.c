#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "rtos_queue.h"
#include "memmgmt.h"


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
        GPIO_OpToggle(GPIOA, GPIO_PIN4);
        taskDelay(MS_TO_TICK(500));
    }
}


void task2(void)
{
    while(1)
    {
        GPIO_OpToggle(GPIOA, GPIO_PIN5);
        taskDelay(MS_TO_TICK(510));
    }
}


void task3(void)
{
    while(1)
    {
        GPIO_OpToggle(GPIOA, GPIO_PIN6);
        taskDelay(MS_TO_TICK(520));
    }
}



int main(void)
{
    board_init();
    rtosKernel_Init();
    gpio_led_init();

    //Lower numbers are higher priority
    taskAdd_Priority(&task1, "Task 1 - Red LED", 0);
    taskAdd_Priority(&task2, "Task 2 - Yellow LED", 1);
    taskAdd_Priority(&task3, "Task 3 - Green LED", 2);

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);
}