#ifndef INC_STM32F103XX_H
#define INC_STM32F103XX_H
/*--------------------------------------------------------------------------------------------------------------------------*/
// standard header files
#include<stdint.h>

//custom header files
#include"stm32f103xx_memory_map.h"
/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* Generic Macros Definitions Start *********************************************/
#define SYSCORE_CLK          8000000

#define ENABLE              1
#define DISABLE             0

#define SET                 ENABLE
#define RESET               DISABLE
#define GPIO_PIN_SET        SET
#define GPIO_PIN_RESET      RESET
#define FLAG_RESET          RESET
#define FLAG_SET            SET

// Bit Manipulations !!Currently not used!!
#define READ_BIT(reg,bit_field)              (reg>>bit_field)
#define SET_BIT(reg,bit_field)               (reg |= 1U<<bit_field)
#define RESET_BIT(reg,bit_field)             (reg &= ~(1U<<bit_field))
#define CHECK_BIT(reg,bit_position)          (reg & (1U<<bit_position) >> bit_position)

/********************************************** Generic Macros Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************* Peripheral Registers Definition Structures Start *************************************/

//CRC Register Definition Structure
typedef struct 
{
    volatile uint32_t DR;
    volatile uint32_t IDR;
    volatile uint32_t CR;
}CRC_RegDef;

//Systick Register Definition Structure
typedef struct
{
    volatile uint32_t CSR;
    volatile uint32_t RVR;
    volatile uint32_t CVR;
    volatile uint32_t CALIB;
}SysTick_RegDef;

// GPIO Register Definition Structure
typedef struct
{
    volatile uint32_t CR[2];        /*<Address Offset: 0x00> <Configuration: Mode & Config Type>*/
    volatile uint32_t IDR;          /*<Address Offset: 0x08> <Input Data>*/
    volatile uint32_t ODR;          /*<Address Offset: 0x0C> <Output Data>*/
    volatile uint32_t BSRR;         /*<Address Offset: 0x10> <Bit Set Reset (atomic operation)>*/
    volatile uint32_t BRR;          /*<Address Offset: 0x14> <Bit Reset>*/
    volatile uint32_t LCKR;         /*<Address Offset: 0x18> <Lock>*/
}GPIO_RegDef;

// AFIO Register Definition Structure
typedef struct
{
    volatile uint32_t EVCR;         /*<Address Offset: 0x00> <Event Control*/
    volatile uint32_t MAPR;         /*<Address Offset: 0x04> <Remap>*/
    volatile uint32_t EXTICR[4];    /*<Address Offset: 0x08> <Output Data>*/
    volatile uint32_t MAPR2;        /*<Address Offset: 0x18> <Remap>*/
}AFIO_RegDef;

// RCC Register Definition Structure
typedef struct
{
    volatile uint32_t CR;           /*<Address offset: 0x00> <Control>*/
    volatile uint32_t CFGR;         /*<Address offset: 0x04> <Configuration>*/
    volatile uint32_t CIR;          /*<Address offset: 0x08> <Clock Interrupt>*/
    volatile uint32_t APB2RSTR;     /*<Address offset: 0x0C> <APB2 Reset>*/
    volatile uint32_t APB1RSTR;     /*<Address offset: 0x10> <APB1 Reset>*/
    volatile uint32_t AHBENR;       /*<Address offset: 0x14> <AHB Enable>*/
    volatile uint32_t APB2ENR;      /*<Address offset: 0x18> <APB2 Enable>*/
    volatile uint32_t APB1ENR;      /*<Address offset: 0x1C> <APB1 Enable>*/
    volatile uint32_t BDCR;         /*<Address offset: 0x20> <Backup Domain Clock>*/
    volatile uint32_t CSR;          /*<Address offset: 0x24> <Control/Status>*/
}RCC_RegDef;

// EXTI Register Definition Structure
typedef struct
{
    volatile uint32_t IMR;          /*<Address Offset: 0x00> <Interrupt Mask>*/
    volatile uint32_t EMR;          /*<Address Offset: 0x04> <Event Mask>*/
    volatile uint32_t RTSR;         /*<Address Offset: 0x08> <Rising Trigger Selection>*/
    volatile uint32_t FTSR;         /*<Address Offset: 0x0C> <Falling Trigger Selection>*/
    volatile uint32_t SWIER;        /*<Address Offset: 0x0C> <Software Interrupt Event>*/
    volatile uint32_t PR;           /*<Address Offset: 0x0C> <Pending>*/
}EXTI_RegDef;

// USART Register Definition Structure
typedef struct
{
    volatile uint32_t SR;          /*<Address Offset: 0x00> <>*/
    volatile uint32_t DR;          /*<Address Offset: 0x04> <>*/
    volatile uint32_t BRR;         /*<Address Offset: 0x08> <>*/
    volatile uint32_t CR1;         /*<Address Offset: 0x0C> <>*/
    volatile uint32_t CR2;         /*<Address Offset: 0x10> <>*/
    volatile uint32_t CR3;         /*<Address Offset: 0x14> <>*/
    volatile uint32_t GTPR;        /*<Address Offset: 0x18> <>*/
}USART_RegDef;


//SPI
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
}SPI_RegDef;


//ADC
typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR[4];
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR[4];
    volatile uint32_t DR;
}ADC_RegDef;



//TIMER
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR[2];
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t CCR[4];
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
}TIM_RegDef;

//I2C
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
}I2C_RegDef;

//DMA
typedef struct
{
    volatile uint32_t CCR;
    volatile uint32_t CNDTR;
    volatile uint32_t CPAR;
    volatile uint32_t CMAR;
}DMA_Channel_RegDef;

typedef struct
{
    volatile uint32_t ISR;
    volatile uint32_t IFCR;
    DMA_Channel_RegDef Channel[7];
}DMA_RegDef;

/*************************************** Peripheral Registers Definition Structures End ***************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/

#endif