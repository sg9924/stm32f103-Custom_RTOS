#ifndef INC_stm32F103xx_EXTI_H
#define INC_stm32F103xx_EXTI_H


#include"stm32F103xx.h"
#include "stm32F103xx_gpio.h"
/************************************************** EXTI Definitions Start **************************************************/

// EXTI Peripheral
#define EXTI               ((EXTI_RegDef*)EXTI_BASE_ADDR)

/*************************************************** EXTI Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ EXTI API's Declarations Start *********************************************/

void EXTI_Intrpt_Config(uint8_t pin_no, uint8_t intrpt_mode, uint8_t mode);
void EXTI_Pend_Clear(uint8_t exti_no);
uint8_t EXTI_Pend_Check(uint8_t exti_no);
void EXTI_Intrpt_Mask(uint8_t exti_no, uint8_t mode);
void EXTI_Event_Mask(uint8_t exti_no, uint8_t mode);
void EXTI_SWIE(uint8_t exti_no, uint8_t mode);

/************************************************** EXTI API's Declarations End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#endif /* INC_stm32F103xx_EXTI_H */