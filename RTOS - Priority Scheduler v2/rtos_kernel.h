#ifndef __RTOSKERNEL_H__
#define __RTOSKERNEL_H__
#include <stdint.h>

#include "rtos_task.h"


#define DISABLE_IRQ()            __asm("CPSID I")
#define ENABLE_IRQ()             __asm("CPSIE I")

#define MS_TO_TICK(ms_value)     ((ms_value*TICK_RATE_HZ) + (1000-1)/1000)


void rtosKernel_Init();
void rtosKernel_Launch(uint32_t quanta);


uint8_t assert(uint8_t condition, char* assert_msg);



#endif