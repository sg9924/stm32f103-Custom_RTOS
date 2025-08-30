#ifndef INC_STM32F103XX_EXCEPTIONS_H
#define INC_STM32F103XX_EXCEPTIONS_H

#include "stm32f103xx.h"
#include "stm32f103xx_core.h"






//Macros
#define PENDSV_PEND()                          (SCB->ICSR |= 1<<SCB_ICSR_PENDSVSET)
#define PENDSV_CLEAR()                         (SCB->ICSR |= 1<<SCB_ICSR_PENDSVCLR)
#define PENDSV_CHECK()                         (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_PENDSVACT))

#define NMI_PEND()                             (SCB->ICSR |= 1<<SCB_ICSR_NMIPENDSET)
#define NMI_CHECK()                            (CHECK_BIT(SCB->ICSR, SCB_ICSR_NMIPENDSET))

#endif