#ifndef INC_STM32F103XX_MEMORY_MAP_H
#define INC_STM32F103XX_MEMORY_MAP_H


/********************************************* Memory Address Definitions Start *********************************************/
//Memory Base Addresses
#define FLASH_BASE_ADDR     0x08000000U
#define SRAM_BASE_ADDR      0x20000000U
#define PERIP_BASE_ADDR     0x40000000U

//Bus Domain Bases Addresses
#define APB1_BASE_ADDR      PERIP_BASE_ADDR
#define APB2_BASE_ADDR      0x40010000U
#define AHB_BASE_ADDR       0x40018000U

//CRC
#define CRC_BASE_ADDR       0x40023000U

//Memory Protection Unit
#define MPU_BASE_ADDR       0xE000ED90U

//RCC Base Address
#define RCC_BASE_ADDR       0x40021000U

//AHB1 Peripherals Base Addresses
//AHB1-GPIO Base Addresses
#define GPIO_BASE_ADDR      0x40010800U
#define GPIOA_BASE_ADDR     (GPIO_BASE_ADDR) 
#define GPIOB_BASE_ADDR     (GPIO_BASE_ADDR + 0x0400)
#define GPIOC_BASE_ADDR     (GPIO_BASE_ADDR + 0x0800)
#define GPIOD_BASE_ADDR     (GPIO_BASE_ADDR + 0x0C00)
#define GPIOE_BASE_ADDR     (GPIO_BASE_ADDR + 0x1000)
#define GPIOF_BASE_ADDR     (GPIO_BASE_ADDR + 0x1400)
#define GPIOG_BASE_ADDR     (GPIO_BASE_ADDR + 0x1800)

//AHB1-AFIO Base Address
#define AFIO_BASE_ADDR      APB2_BASE_ADDR

//AHB1-EXTI Base Address
#define EXTI_BASE_ADDR      (APB2_BASE_ADDR + 0x400)

//USART/USART Base Addresses
#define USART1_BASE_ADDR    (APB2_BASE_ADDR + 0x3800)
#define USART2_BASE_ADDR    (APB1_BASE_ADDR + 0x4400)
#define USART3_BASE_ADDR    (APB1_BASE_ADDR + 0x4800)
#define UART4_BASE_ADDR     (APB1_BASE_ADDR + 0x4C00)
#define UART5_BASE_ADDR     (APB1_BASE_ADDR + 0x5000)

//SPI Base Addresses
#define SPI1_BASE_ADDR      (APB2_BASE_ADDR + 0x3000)
#define SPI2_BASE_ADDR      (APB1_BASE_ADDR + 0x3800)
#define SPI3_BASE_ADDR      (APB1_BASE_ADDR + 0x3C00)

//I2C Base Addresses
#define I2C1_BASE_ADDR      (PERIP_BASE_ADDR + 0x5400)
#define I2C2_BASE_ADDR      (PERIP_BASE_ADDR + 0x5800)

//ADC Base Addresses
#define ADC1_BASE_ADDR      (APB2_BASE_ADDR + 0x2400)
#define ADC2_BASE_ADDR      (APB2_BASE_ADDR + 0x2800)
#define ADC3_BASE_ADDR      (APB2_BASE_ADDR + 0x3C00)

//TIMER Base Addresses
#define TIM1_BASE_ADDR      (APB2_BASE_ADDR + 0x2C00)
#define TIM2_BASE_ADDR      (PERIP_BASE_ADDR)
#define TIM3_BASE_ADDR      (PERIP_BASE_ADDR + 0x0400)
#define TIM4_BASE_ADDR      (PERIP_BASE_ADDR + 0x0800)
#define TIM5_BASE_ADDR      (PERIP_BASE_ADDR + 0x0C00)
#define TIM6_BASE_ADDR      (PERIP_BASE_ADDR + 0x1000)
#define TIM7_BASE_ADDR      (PERIP_BASE_ADDR + 0x1400)
#define TIM12_BASE_ADDR     (PERIP_BASE_ADDR + 0x1800)
#define TIM13_BASE_ADDR     (PERIP_BASE_ADDR + 0x1C00)
#define TIM14_BASE_ADDR     (PERIP_BASE_ADDR + 0x2000)

//DMA
#define DMA1_BASE_ADDR      (0x40020000)
#define DMA2_BASE_ADDR      (0x40020400)

/********************************************** Memory Address Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/

#endif