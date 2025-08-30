#ifndef INC_STM32F103XX_FAULTS_H
#define INC_STM32F103XX_FAULTS_H

#include "stm32f103xx.h"
#include "stm32f103xx_core.h"



//Macros

//Faults
//Get status of Faults
#define MMFSR_READ()                        (SCB->CFSR & 0xFF)
#define BFSR_READ()                         ((SCB->CFSR >> 8) & 0xFF)
#define UFSR_READ()                         ((SCB->CFSR >> 16) & 0xFFFF)
//Enable Faults
#define MEMFAULT_ENABLE()                   (SCB->SHCSR |= 1<<SCB_SHCSR_MEMFAULTENA)
#define BUSFAULT_ENABLE()                   (SCB->SHCSR |= 1<<SCB_SHCSR_BUSFAULTENA)
#define USGFAULT_ENABLE()                   (SCB->SHCSR |= 1<<SCB_SHCSR_USGFAULTENA)
//Disable Faults
#define MEMFAULT_DISABLE()                  (SCB->SHCSR &= ~(1<<SCB_SHCSR_MEMFAULTENA))
#define BUSFAULT_DISABLE()                  (SCB->SHCSR &= ~(1<<SCB_SHCSR_BUSFAULTENA))
#define USGFAULT_DISABLE()                  (SCB->SHCSR &= ~(1<<SCB_SHCSR_USGFAULTENA))
//Check Pending Faults
#define USGFAULT_PENDING_CHECK()            (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_USGFAULTPENDED))
#define MEMFAULT_PENDING_CHECK()            (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_MEMFAULTPENDED))
#define BUSFAULT_PENDING_CHECK()            (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_BUSFAULTPENDED))
//Check Active Exceptions/Faults
#define MEMFAULT_ACTIVE_CHECK()             (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_MEMFAULTACT))
#define BUSFAULT_ACTIVE_CHECK()             (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_BUSFAULTACT))
#define USGFAULT_ACTIVE_CHECK()             (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_USGFAULTACT))
#define SVCALL_ACTIVE_CHECK()               (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_SVCALLACT))
#define MONITOR_ACTIVE_CHECK()              (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_MONITORACT))
#define PENDSV_ACTIVE_CHECK()               (CHECK_BIT(SCB->SHCSR, SCB_SHCSR_PENDSVACT))
 
//Traps
//Enable Traps
#define UNALIGN_TRP_ENABLE()                (SCB->CCR |= 1<<SCB_CCR_UNALIGN_TRP)
#define DIV_0_TRP_ENABLE()                  (SCB->CCR |= 1<<SCB_CCR_DIV_0_TRP)
//Disable Traps
#define UNALIGN_TRP_DISABLE()               (SCB->CCR &= ~(1<<SCB_CCR_UNALIGN_TRP))
#define DIV_0_TRP_DISABLE()                 (SCB->CCR &= ~(1<<SCB_CCR_DIV_0_TRP))

//Fault Address Registers
//Enable Fault Address Registers
#define MMFAR_ENABLE()                      (SCB->CFSR |= 1<<SCB_CFSR_MMAR_VALID)
#define BFAR_ENABLE()                       (SCB->CFSR |= 1<<SCB_CFSR_BFAR_VALID)
//Disable Fault Address Registers
#define MMFAR_DISABLE()                     (SCB->CFSR &= ~(1<<SCB_CFSR_MMAR_VALID))
#define BFAR_DISABLE()                      (SCB->CFSR &= ~(1<<SCB_CFSR_BFAR_VALID))
//Get Address where Faults occurred
#define MMFAR_READ()                        (SCB->MMFAR)
#define BFAR_READ()                         (SCB->BFAR)




//API Declarations
void enable_faults(void);




#endif