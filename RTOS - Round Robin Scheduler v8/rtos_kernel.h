#ifndef __RTOSKERNEL_H__
#define __RTOSKERNEL_H__
#include <stdint.h>

#include "rtos_task.h"

#define DISABLE_IRQ()            __asm("CPSID I")
#define ENABLE_IRQ()             __asm("CPSIE I")

#define MS_TO_TICK(ms_value)     (((ms_value * TICK_RATE_HZ) + (1000-1))/1000)

//System Control Block ICSR Register Address and PendSV bit
#define INTCTRL                  (*(volatile uint32_t *)0xE000ED04)
#define PENDSVSET                (1UL << 28)

#define WORD_ALIGN_8BYTE(value)         (((value) + 1) & ~1)



//Critical Section APIs
//ISR
//Enter Critical Section inside ISR
inline uint32_t enterCriticalISR(void)
{
    uint32_t temp;

    //Read PRIMASk value into temp
    //then disable the interrupts
    __asm volatile(
        "MRS %0, PRIMASK\n"
        "CPSID I\n"
        : "=r" (temp)
        :
        : "memory"
    );

    return temp;
}


//Exit Critical Section inside ISR
inline uint32_t exitCriticalISR(uint32_t prev_mask)
{
    //restore the PRIMASK state we had before
    __asm volatile (
        "MSR PRIMASK, %0\n"
        : 
        : "r" (prev_mask) 
        : "memory"
    );
}


//Kernel
void rtosKernel_Init();
void rtosKernel_Launch(uint32_t quanta);

//Assert
uint8_t assert(uint8_t condition, char* assert_msg);

//Stack Allocate
#if STACK_TYPE == STACK_TYPE_INDIVIDUAL
uint32_t* Stack_Allocate(uint32_t size_in_words);
#endif


//Queue Operations
void ready_queue_add(tcb_t* task);
uint8_t ready_queue_remove(tcb_t* task, uint8_t state);

void blocked_queue_add(tcb_t* task);
uint8_t blocked_queue_remove(tcb_t* task, uint8_t state);



#endif