#ifndef INC_stm32F103xx_NVIC_H
#define INC_stm32F103xx_NVIC_H

#include<stdint.h>
#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"


/************************************************** NVIC Definitions Start **************************************************/
// NVIC ISERx Register Addresses
#define NVIC_ISER0            ((volatile uint32_t*)0xE000E100)
#define NVIC_ISER1            ((volatile uint32_t*)0xE000E104)
#define NVIC_ISER2            ((volatile uint32_t*)0xE000E108)
#define NVIC_ISER3            ((volatile uint32_t*)0xE000E10C)
#define NVIC_ISER4            ((volatile uint32_t*)0xE000E110)
#define NVIC_ISER5            ((volatile uint32_t*)0xE000E114)
#define NVIC_ISER6            ((volatile uint32_t*)0xE000E118)
#define NVIC_ISER7            ((volatile uint32_t*)0xE000E11C)

// NVIC ICERx Register Addresses
#define NVIC_ICER0            ((volatile uint32_t*)0XE000E180)
#define NVIC_ICER1            ((volatile uint32_t*)0XE000E184)
#define NVIC_ICER2            ((volatile uint32_t*)0XE000E188)
#define NVIC_ICER3            ((volatile uint32_t*)0XE000E18C)
#define NVIC_ICER4            ((volatile uint32_t*)0XE000E190)
#define NVIC_ICER5            ((volatile uint32_t*)0XE000E194)
#define NVIC_ICER6            ((volatile uint32_t*)0XE000E198)
#define NVIC_ICER7            ((volatile uint32_t*)0XE000E19C)

// NVIC Priority Register Address
#define NVIC_PR_BASE_ADDR     ((volatile uint32_t*)0xE000E400)

//IRQ Numbers
//USART/UART
#define USART1_IRQ_NO          37
#define USART2_IRQ_NO          38
#define USART3_IRQ_NO          39
#define UART4_IRQ_NO           52
#define UART5_IRQ_NO           53

//SPI
#define SPI1_IRQ_NO            35
#define SPI2_IRQ_NO            36
#define SPI3_IRQ_NO            51

//I2C
#define I2C1_EV_IRQ_NO         31
#define I2C1_ER_IRQ_NO         32
#define I2C2_EV_IRQ_NO         33
#define I2C3_ER_IRQ_NO         34

//TIM
#define TIM1_BRK_IRQ_NO        24
#define TIM1_UP_IRQ_NO         25
#define TIM1_TRG_COM_IRQ_NO    26
#define TIM1_CC_IRQ_NO         27
#define TIM2_IRQ_NO            28
#define TIM3_IRQ_NO            29
#define TIM4_IRQ_NO            30
#define TIM5_IRQ_NO            50
#define TIM6_IRQ_NO            54
#define TIM7_IRQ_NO            55

//DMA
#define DMA1_Channel1_IRQ_NO     11
#define DMA1_Channel2_IRQ_NO     12
#define DMA1_Channel3_IRQ_NO     13
#define DMA1_Channel4_IRQ_NO     14
#define DMA1_Channel5_IRQ_NO     15
#define DMA1_Channel6_IRQ_NO     16
#define DMA1_Channel7_IRQ_NO     17

#define DMA2_Channel1_IRQ_NO     56
#define DMA2_Channel2_IRQ_NO     57
#define DMA2_Channel3_IRQ_NO     58
#define DMA2_Channel4_5_IRQ_NO   59


/*************************************************** NVIC Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ NVIC API's Declarations Start *********************************************/

uint8_t nvic_irq_gpio_port_map(GPIO_Handle*);
void nvic_intrpt_enable(uint16_t);
void nvic_intrpt_disable(uint16_t);

/************************************************** NVIC API's Declarations End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/

#endif