#ifndef INC_stm32F103xx_RCC_H
#define INC_stm32F103xx_RCC_H


#include "stm32f103xx.h"

/*************************************************** RCC Definitions Start **************************************************/

// RCC Definition
#define RCC				    ((RCC_RegDef*)RCC_BASE_ADDR)


//RCC_CR Bit Fields
#define RCC_CR_HSION           0
#define RCC_CR_HSIRDY          1
#define RCC_CR_HSITRIM         3
#define RCC_CR_HSICAL          8
#define RCC_CR_HSEON           16
#define RCC_CR_HSERDY          17
#define RCC_CR_HSEBYP          18
#define RCC_CR_CSSON           19
#define RCC_CR_PLLON           24
#define RCC_CR_PLLRDY          25

//RCC_CFGR Bit fields
#define RCC_CFGR_SW            0
#define RCC_CFGR_SWS           2
#define RCC_CFGR_HPRE          4
#define RCC_CFGR_PPRE1         8
#define RCC_CFGR_PPRE2         11
#define RCC_CFGR_ADCPRE        14
#define RCC_CFGR_PLLSRC        16
#define RCC_CFGR_XTPRE         17
#define RCC_CFGR_PLLMUL        18
#define RCC_CFGR_USBPRE        22
#define RCC_CFGR_MCO           24

//RCC_AHBENR Bit fields
#define RCC_AHBENR_DMA1EN           0
#define RCC_AHBENR_DMA2EN           1
#define RCC_AHBENR_SRAMEN           2
#define RCC_AHBENR_FLITFEN          4
#define RCC_AHBENR_CRCEN            6
#define RCC_AHBENR_FSMCEN           8
#define RCC_AHBENR_SDIOEN           10

//RCC_APB2ENR Bit Fields
#define RCC_APB2ENR_AFIOEN          0
#define RCC_APB2ENR_IOPAEN          2
#define RCC_APB2ENR_IOPBEN          3
#define RCC_APB2ENR_IOPCEN          4
#define RCC_APB2ENR_IOPDEN          5
#define RCC_APB2ENR_IOPEEN          6
#define RCC_APB2ENR_IOPFEN          7
#define RCC_APB2ENR_IOPGEN          8
#define RCC_APB2ENR_ADC1EN          9
#define RCC_APB2ENR_ADC2EN          10
#define RCC_APB2ENR_TIM1EN          11
#define RCC_APB2ENR_SPI1EN          12
#define RCC_APB2ENR_TIM8EN          13
#define RCC_APB2ENR_USART1EN        14
#define RCC_APB2ENR_ADC3EN          15
#define RCC_APB2ENR_TIM9EN          19
#define RCC_APB2ENR_TIM10EN         20
#define RCC_APB2ENR_TIM11EN         21

//RCC_APB1ENR Bit Fields
#define RCC_APB1ENR_TIM2EN          0
#define RCC_APB1ENR_TIM3EN          1
#define RCC_APB1ENR_TIM4EN          2
#define RCC_APB1ENR_TIM5EN          3
#define RCC_APB1ENR_TIM6EN          4
#define RCC_APB1ENR_TIM7EN          5
#define RCC_APB1ENR_TIM12EN         6
#define RCC_APB1ENR_TIM13EN         7 
#define RCC_APB1ENR_TIM14EN         8
#define RCC_APB1ENR_WWDGEN          11
#define RCC_APB1ENR_SPI2EN          14
#define RCC_APB1ENR_SPI3EN          15
#define RCC_APB1ENR_USART2EN        17
#define RCC_APB1ENR_USART3EN        18
#define RCC_APB1ENR_UART4EN         19
#define RCC_APB1ENR_USART5EN        20
#define RCC_APB1ENR_I2C1EN          21
#define RCC_APB1ENR_I2C2EN          22
#define RCC_APB1ENR_USBEN           23
#define RCC_APB1ENR_CANEN           25
#define RCC_APB1ENR_BKPEN           27
#define RCC_APB1ENR_PWREN           28
#define RCC_APB1ENR_DACEN           29

//RCC_CSR Bit Fields
#define RCC_CSR_LSION               0
#define RCC_CSR_LSIRDY              1
#define RCC_CSR_RMVF                24
#define RCC_CSR_PINRSTF             26
#define RCC_CSR_PORRSTF             27
#define RCC_CSR_SFTRSTF             28
#define RCC_CSR_IWDGRSTF            29
#define RCC_CSR_WWDGRSTF            30
#define RCC_CSR_LPWRRSTF            31


//Clock Source
#define RCC_CLK_HSI                 0
#define RCC_CLK_HSE                 1
#define RCC_CLK_PLL                 2

//MCO Clock Source
#define MCO_NO_CLK                  0
#define MCO_SYSCLK                  4
#define MCO_HSICLK                  5
#define MCO_HSECLK                  6
#define MCO_PLLCLK_DIV_2            7

/**************************************************** RCC Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************* RCC Macros Definitions Start *********************************************/



/************************************************* GPIO Macros Definitions Start *********************************************/
/*---------------------------------------------------------------------------------------------------------------------------*/
// RCC Config Structure
typedef struct 
{
    uint8_t  Clock_Source;
    uint32_t System_Clock;
    uint32_t High_Clock;
    uint32_t P_Clock_1;
    uint32_t P_Clock_2;
    uint8_t  MCO_Clock_Source;
}RCC_Config;


// RCC Handler Structure
typedef struct
{
    RCC_RegDef    *pRCC;                 /*<>*/
    RCC_Config    RCC_State;
}RCC_Handle;


uint8_t RCC_Get_Clock_Source(RCC_Handle* pRCCHandle);
uint32_t RCC_Get_HCLK(RCC_Handle* pRCCHandle);
uint32_t RCC_Get_PCLK1(RCC_Handle* pRCCHandle);
uint32_t RCC_Get_PCLK2(RCC_Handle* pRCCHandle);
uint8_t RCC_Update_Clock_Source(RCC_Handle* pRCCHandle);
uint32_t RCC_Update_HCLK(RCC_Handle* pRCCHandle);
uint32_t RCC_Update_PCLK1(RCC_Handle* pRCCHandle);
uint32_t RCC_Update_PCLK2(RCC_Handle* pRCCHandle);
void RCC_Select_Clock_Source(RCC_Handle* pRCCHandle);
void RCC_init(RCC_Handle* pRCCHandle, uint8_t clk_src);

#endif /*INC_stm32F103xx_RCC_H*/