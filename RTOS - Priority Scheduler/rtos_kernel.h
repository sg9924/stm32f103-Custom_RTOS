#ifndef __RTOSKERNEL_H__
#define __RTOSKERNEL_H__
#include <stdint.h>

#include "rtos_task.h"


#define DISABLE_IRQ()            __asm("CPSID I")
#define ENABLE_IRQ()             __asm("CPSIE I")

#define MS_TO_TICK(ms_value)     ((ms_value*TICK_RATE_HZ) + (1000-1)/1000)

void rtosKernel_TaskStackInit(uint8_t task_num);
void rtosKernel_StackInit(void);
void rtosKernel_TCBInit(void);
void rtosKernel_TaskInit(void);
void rtosKernel_Init();
void rtosKernel_Launch(uint32_t quanta);
__attribute__((naked)) void rtosScheduler_Launch(void);
uint8_t assert(uint8_t condition, char* assert_msg);

void add_to_ready_queue(tcb_t* task);
void ready_queue_init();
uint8_t check_ready_queue_empty();
void ready_queue_reset();



#endif