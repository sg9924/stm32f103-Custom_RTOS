#ifndef __RTOSPORT_H__
#define __RTOSPORT_H__

#include "stm32f103xx.h"
#include "stm32f103xx_systick.h"
#include "stm32f103xx_core.h"
#include "stm32f103xx_config.h"

#include "stm32f103xx_init.h"
#include "stm32f103xx_serial.h"
#include "stm32f103xx_utilities.h"


#define SYSCLOCK                        (RCC_Get_SYSCLK())

#define DISABLE_IRQ()                   __asm volatile ("CPSID I"::: "memory")
#define ENABLE_IRQ()                    __asm volatile ("CPSIE I"::: "memory")

//System Control Block ICSR Register Address and PendSV bit
#define INTCTRL                         (*(volatile uint32_t *)0xE000ED04)
#define PENDSVSET                       (1UL << 28)

#define WORD_ALIGN_8BYTE(value)         (((value) + 1) & ~1)
#define MS_TO_TICK(ms_value)            (((ms_value * TICK_RATE_HZ) + (1000-1))/1000)




#endif