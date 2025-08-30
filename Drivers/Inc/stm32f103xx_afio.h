#ifndef INC_stm32F103xx_AFIO_H
#define INC_stm32F103xx_AFIO_H


#include "stm32F103xx.h"
#include "stm32F103xx_gpio.h"
/************************************************** AFIO Definitions Start **************************************************/

// AFIO Peripheral
#define AFIO                                  ((AFIO_RegDef*)AFIO_BASE_ADDR)

// AFIO_EVCR Bit Definitions
#define AFIO_EVCR_PIN0                        0
#define AFIO_EVCR_PIN1                        1
#define AFIO_EVCR_PIN2                        2
#define AFIO_EVCR_PIN3                        3
#define AFIO_EVCR_PORT0                       4
#define AFIO_EVCR_PORT1                       5
#define AFIO_EVCR_PORT2                       6
#define AFIO_EVCR_EVOE                        7


// AFIO_MAPR Bit Definitions
#define AFIO_MAPR_SPI1_REMAP                  0
#define AFIO_MAPR_I2C1_REMAP                  1
#define AFIO_MAPR_USART1_REMAP                2
#define AFIO_MAPR_USART2_REMAP                3
#define AFIO_MAPR_USART3_REMAP0               4
#define AFIO_MAPR_USART3_REMAP1               5
#define AFIO_MAPR_TIM1_REMAP0                 6
#define AFIO_MAPR_TIM1_REMAP1                 7
#define AFIO_MAPR_TIM2_REMAP0                 8
#define AFIO_MAPR_TIM2_REMAP1                 9
#define AFIO_MAPR_TIM3_REMAP0                 10
#define AFIO_MAPR_TIM3_REMAP1                 11
#define AFIO_MAPR_TIM4_REMAP                  12
#define AFIO_MAPR_CAN_REMAP0                  13
#define AFIO_MAPR_CAN_REMAP1                  14
#define AFIO_MAPR_PD01_REMAP                  15
#define AFIO_MAPR_TIM5CH4_REMAP               16
#define AFIO_MAPR_ADC1_ETRGINJ_REMAP          17
#define AFIO_MAPR_ADC1_ETRGREG_REMAP          18
#define AFIO_MAPR_ADC2_ETRGINJ_REMAP          19
#define AFIO_MAPR_ADC2_ETRGREG_REMAP          20
#define AFIO_MAPR_SWJ_CFG0                    24
#define AFIO_MAPR_SWJ_CFG1                    25
#define AFIO_MAPR_SWJ_CFG2                    26


// AFIO_MAPR2 Bit Definitions
#define AFIO_MAPR2_TIM9_REMAP                 5
#define AFIO_MAPR2_TIM10_REMAP                6
#define AFIO_MAPR2_TIM11_REMAP                7
#define AFIO_MAPR2_TIM13_REMAP                8
#define AFIO_MAPR2_TIM14_REMAP                9
#define AFIO_MAPR2_FSMC_NADV                  10

//AFIO EVENT Output Port Selection
#define AFIO_EVENT_PORTA                      0
#define AFIO_EVENT_PORTB                      1
#define AFIO_EVENT_PORTC                      2
#define AFIO_EVENT_PORTD                      3
#define AFIO_EVENT_PORTE                      4

//AFIO EVENT Output Pin Selection
#define AFIO_EVENT_PIN0                       0
#define AFIO_EVENT_PIN1                       1
#define AFIO_EVENT_PIN2                       2
#define AFIO_EVENT_PIN3                       3
#define AFIO_EVENT_PIN4                       4
#define AFIO_EVENT_PIN5                       5
#define AFIO_EVENT_PIN6                       6
#define AFIO_EVENT_PIN7                       7
#define AFIO_EVENT_PIN8                       8
#define AFIO_EVENT_PIN9                       9
#define AFIO_EVENT_PIN10                      10
#define AFIO_EVENT_PIN11                      11
#define AFIO_EVENT_PIN12                      12
#define AFIO_EVENT_PIN13                      13
#define AFIO_EVENT_PIN14                      14
#define AFIO_EVENT_PIN15                      15

/*************************************************** AFIO Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ AFIO Macros Definitions Start *********************************************/

// AFIO Enable
#define AFIO_PLCK_EN()          (RCC->APB2ENR |= 1<<0)

/************************************************** AFIO Macros Definitions End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ AFIO API's Declarations Start *********************************************/

void AFIO_EXTI_Config(GPIO_RegDef* port_code, uint8_t pin_no, uint8_t mode);
void AFIO_EVENT_Config(uint8_t port, uint8_t pin_no, uint8_t mode);

/************************************************** AFIO API's Declarations End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#endif /* INC_stm32F103xx_AFIO_H */