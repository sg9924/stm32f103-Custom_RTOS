#ifndef INC_STM32F103XX_SYSTICK_H
#define INC_STM32F103XX_SYSTICK_H

#include "stm32f103xx.h"
#include "stm32f103xx_config.h"
#include "stm32f103xx_core.h"
#include "stm32f103xx_memory_map.h"

//Systick Peripheral Definition
#define SYSTICK_BASE_ADDR      (SCS_BASE_ADDR + 0x010U)
#define SYSTICK                ((SysTick_RegDef*)SYSTICK_BASE_ADDR)

//SYST_CSR Bit Field Definitions
#define SYST_CSR_ENABLE                  0
#define SYST_CSR_TICKINT                 1
#define SYST_CSR_CLKSOURCE               2
#define SYST_CSR_COUNTFLAG               16

#define SYSTICK_MAX_VALUE                0xFFFFFFFFU
#define SYSTICK_LOAD_VALUE_DEFAULT       ((8000000U)/1000)

//SYSTICK Configuration
#define SYSTICK_EXCEPTION_DISABLE        0
#define SYSTICK_EXCEPTION_ENABLE         1

#define SYSTICK_CLK_SRC_AHB_DIV_8        0
#define SYSTICK_CLK_SRC_AHB              1


//Macros
#define SYSTICK_CLEAR()                  (SYSTICK->CVR = 0)
#define SYSTICK_LOAD(load_value)         (SYSTICK->RVR = load_value)
#define SYSTICK_GET()                    (SYSTICK->CVR)
#define SYSTICK_CHECK_FLAG()             (CHECK_BIT(SYSTICK->CSR, SYST_CSR_COUNTFLAG))
#define SYSTICK_ENABLE()                 (SYSTICK->CSR |= 1 << SYST_CSR_ENABLE)
#define SYSTICK_DISABLE()                (SYSTICK->CSR &= ~(1 << SYST_CSR_ENABLE))
//#define SYSTICK_TICK_INC()               (current_tick += 1)
#define SYSTICK_ENABLE_INTERRUPT()       (SYSTICK->CSR |= 1<<SYST_CSR_TICKINT)
#define SYSTICK_DISABLE_INTERRUPT()      (SYSTICK->CSR &= ~(1<<SYST_CSR_TICKINT))
#define SYSTICK_EXCEPTION_PEND()         (SCB->ICSR |= 1<<SCB_ICSR_PENDSTSET)
#define SYSTICK_EXCEPTION_CLEAR()        (SCB->ICSR |= 1<<SCB_ICSR_PENDSTCLR)



//APi Declarations
void Systick_Configure(uint8_t clk_src, uint8_t exception);
void Systick_init();
uint32_t Systick_get_tick();
void Systick_Tick_Inc(void);
void Systick_delay(uint32_t delay);

#endif