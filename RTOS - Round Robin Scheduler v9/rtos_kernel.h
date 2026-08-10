#ifndef __RTOSKERNEL_H__
#define __RTOSKERNEL_H__
#include <stdint.h>

#include "rtos_task.h"

#define DISABLE_IRQ()                   __asm volatile ("CPSID I"::: "memory")
#define ENABLE_IRQ()                    __asm volatile ("CPSIE I"::: "memory")

#define MS_TO_TICK(ms_value)            (((ms_value * TICK_RATE_HZ) + (1000-1))/1000)

//System Control Block ICSR Register Address and PendSV bit
#define INTCTRL                         (*(volatile uint32_t *)0xE000ED04)
#define PENDSVSET                       (1UL << 28)

#define WORD_ALIGN_8BYTE(value)         (((value) + 1) & ~1)

#define SCHEDULER_SWITCH_CONTEXT()      INTCTRL = PENDSVSET


//Critical Section APIs
static inline uint32_t primask_read(void)
{
    uint32_t primask;
    __asm volatile (
        "MRS %0, PRIMASK"
        : "=r" (primask)
        :
        : "memory"
    );
    return primask;
}



static inline void primask_write(uint32_t primask)
{
    __asm volatile (
        "MSR PRIMASK, %0"
        :
        : "r" (primask)
        : "memory"
    );
}



//ISR
//Enter Critical Section inside ISR
static inline uint32_t enterCriticalISR(void)
{
    //Read the recent primask value
    uint32_t primask = primask_read();
    //Disable all Interrupts
    DISABLE_IRQ();
    return primask;
}


//Exit Critical Section inside ISR
static inline void exitCriticalISR(uint32_t primask)
{
    //restore the PRIMASK state we had before
    primask_write(primask);
    //clobber to indicate that memory has been modified
    __asm volatile ("" ::: "memory");
}


//Macro versions
#define CRITICAL_SECTION_ENTER(key)       uint32_t key = enterCriticalISR()
#define CRITICAL_SECTION_EXIT(key)        exitCriticalISR(key)


//Kernel
void rtosKernel_Init();
void rtosKernel_Launch(uint32_t quanta);

//Assert
uint8_t assert(uint8_t condition, char* assert_msg);

//Stack Allocate
uint32_t* Stack_Allocate(uint32_t size_in_words);


//Queue Operations
void ready_queue_add(tcb_t* task);
uint8_t ready_queue_remove(tcb_t* task, uint8_t state);

void blocked_queue_add(tcb_t* task);
uint8_t blocked_queue_remove(tcb_t* task, uint8_t state);



#endif