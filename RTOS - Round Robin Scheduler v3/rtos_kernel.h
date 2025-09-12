#ifndef __RTOSKERNEL_H__
#define __RTOSKERNEL_H__
#include <stdint.h>

#define DISABLE_IRQ()            __asm("CPSID I")
#define ENABLE_IRQ()             __asm("CPSIE I")


void rtosKernel_TaskStackInit(uint8_t task_num);
void rtosKernel_StackInit(void);
void rtosKernel_TCBInit(void);
void rtosKernel_TaskInit(void);
void rtosKernel_Launch(uint32_t quanta);
__attribute__((naked)) void rtosScheduler_Launch(void);

#endif