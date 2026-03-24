#include "rtos_config.h"
#include "rtos_kernel.h"
#include "rtos_task.h"
#include "Semaphore.h"



void Semaphore_Init(int8_t* semaphore, int8_t value)
{
    *semaphore = value;
}


void Semaphore_Give(int8_t* semaphore)
{
    DISABLE_IRQ();
    *semaphore += 1;
    ENABLE_IRQ(); 
}


void Semaphore_Take(int8_t* semaphore)
{
    DISABLE_IRQ();
    while(*semaphore<=0)
    {
        DISABLE_IRQ();
        ENABLE_IRQ();
    }
    *semaphore -= 1;
    ENABLE_IRQ();
}



void Semaphore_CooperativeTake(int8_t *semaphore)
{
    DISABLE_IRQ();

    while(*semaphore<=0)
    {
        DISABLE_IRQ();
        taskYield();
        ENABLE_IRQ();
    }

    *semaphore -= 1;
    ENABLE_IRQ();
}