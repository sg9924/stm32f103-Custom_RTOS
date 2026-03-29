#ifndef INC_stm32F103xx_RCC_H
#define INC_stm32F103xx_RCC_H


#include"stm32f103xx.h"

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
#define RCC_CFGR_PLLXTPRE      17
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

//RCC_BDCR Bit Fields
#define RCC_BDCR_LSEON              0
#define RCC_BDCR_LSERDY             1
#define RCC_BDCR_LSEBYP             2
#define RCC_BDCR_RTCSEL0            8
#define RCC_BDCR_RTCSEL1            9
#define RCC_BDCR_RTCEN              15
#define RCC_BDCR_BDRST              16

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

//System CLock
#define SYSCLK_HSI                  0
#define SYSCLK_HSE                  1
#define SYSCLK_PLL                  2

//PLL
#define PLL_CLK_SRC_HSI_DIV_2       0
#define PLL_CLK_SRC_HSE             1

//MCO Clock Source
#define MCO_NO_CLK                  0
#define MCO_SYSCLK                  4
#define MCO_HSICLK                  5
#define MCO_HSECLK                  6
#define MCO_PLLCLK_DIV_2            7

//AHB Prescalar
#define AHB_PRESCALAR_1             1
#define AHB_PRESCALAR_2             2
#define AHB_PRESCALAR_4             4
#define AHB_PRESCALAR_8             8
#define AHB_PRESCALAR_16            16
#define AHB_PRESCALAR_32            32
#define AHB_PRESCALAR_64            64
#define AHB_PRESCALAR_128           128
#define AHB_PRESCALAR_256           256
#define AHB_PRESCALAR_512           512

//APB1 Prescalar
#define APB1_PRESCALAR_1            1
#define APB1_PRESCALAR_2            4
#define APB1_PRESCALAR_4            5
#define APB1_PRESCALAR_8            6
#define APB1_PRESCALAR_16           7

//APB2 Prescalar
#define APB2_PRESCALAR_1            1
#define APB2_PRESCALAR_2            4
#define APB2_PRESCALAR_4            5
#define APB2_PRESCALAR_8            6
#define APB2_PRESCALAR_16           7

//
#define PLL_HSE_DIV_NONE            0
#define PLL_HSE_DIV_2               1

//
#define PLL_MUL_2                   2
#define PLL_MUL_3                   3
#define PLL_MUL_4                   4
#define PLL_MUL_5                   5
#define PLL_MUL_6                   6
#define PLL_MUL_7                   7
#define PLL_MUL_8                   8
#define PLL_MUL_9                   9
#define PLL_MUL_10                  10
#define PLL_MUL_11                  11
#define PLL_MUL_12                  12
#define PLL_MUL_13                  13
#define PLL_MUL_14                  14
#define PLL_MUL_15                  15
#define PLL_MUL_16                  16


//
#define SWITCH_STATUS_HSI           0
#define SWITCH_STATUS_HSE           1
#define SWITCH_STATUS_PLL           2
 

//Defaults
#define RCC_CLK_SRC_DEFAULT         RCC_CLK_HSI
#define RCC_SYSCLK_DEFAULT          SYSCLK_HSI
#define RCC_SYSCLK_FREQ_DEFAULT     8000000

/**************************************************** RCC Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************* RCC Macros Definitions Start *********************************************/
#define PWR_PLCK_ENABLE()           (RCC->APB1ENR |= 1<<RCC_APB1ENR_PWREN)
#define PWR_PLCK_DISABLE()          (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_PWREN))



#define RCC_HSE_ENABLE()            (RCC->CR |= 1<<RCC_CR_HSEON)
#define RCC_HSI_ENABLE()            (RCC->CR |= 1<<RCC_CR_HSION)
#define RCC_PLL_ENABLE()            (RCC->CR |= 1<<RCC_CR_PLLON)

#define RCC_HSE_DISABLE()           (RCC->CR &= ~(1<<RCC_CR_HSEON))
#define RCC_HSI_DISABLE()           (RCC->CR &= ~(1<<RCC_CR_HSION))
#define RCC_PLL_DISABLE()           (RCC->CR &= ~(1<<RCC_CR_PLLON))


#define RCC_HSIRDY_WAIT()           while (!(RCC->CR & 1<<RCC_CR_HSIRDY))
#define RCC_HSERDY_WAIT()           while (!(RCC->CR & 1<<RCC_CR_HSERDY))
#define RCC_PLLRDY_WAIT()           while (!(RCC->CR & 1<<RCC_CR_PLLRDY))

//System Clock Switch
#define RCC_SYSCLK_SWITCH_CLEAR()   (RCC->CFGR &= ~(3<<RCC_CFGR_SW))
#define RCC_SYSCLK_SWITCH_HSI()     (RCC_SYSCLK_SWITCH_CLEAR())
#define RCC_SYSCLK_SWITCH_HSE()     (RCC->CFGR |= 1<<RCC_CFGR_SW)
#define RCC_SYSCLK_SWITCH_PLL()     (RCC->CFGR |= 2<<RCC_CFGR_SW)
#define RCC_GET_SWS()               ((RCC->CFGR & 0x3<<RCC_CFGR_SWS)>>RCC_CFGR_SWS)

//System Clock Switch Status
#define RCC_SYSCLK_GET_SWSTATUS()  ((RCC->CFGR & 3<<RCC_CFGR_SWS)>>RCC_CFGR_SWS)

//PLL
#define RCC_GET_PLLMUL()           ((RCC->CFGR & 0xF<<RCC_CFGR_PLLMUL)>>RCC_CFGR_PLLMUL + 2)
#define RCC_GET_PLLXPTRE()         ((RCC->CFGR & 0x3<<RCC_CFGR_PLLXTPRE)>>RCC_CFGR_PLLXTPRE)

#define RCC_GET_HPRE()             ((RCC->CFGR & 0xF<<RCC_CFGR_HPRE)>>RCC_CFGR_HPRE)
#define RCC_GET_PPRE1()            ((RCC->CFGR & 0x7<<RCC_CFGR_PLLXTPRE)>>RCC_CFGR_PLLXTPRE)
#define RCC_GET_PPRE2()            ((RCC->CFGR & 0x7<<RCC_CFGR_PLLXTPRE)>>RCC_CFGR_PLLXTPRE)




/************************************************* RCC Macros Definitions End ************************************************/
/*---------------------------------------------------------------------------------------------------------------------------*/
// RCC Config Structure
typedef struct 
{
    uint32_t System_Clock;
    uint32_t High_Clock;
    uint32_t P_Clock_1;
    uint32_t P_Clock_2;
}RCC_Clock;


typedef struct
{
    uint8_t  PLL_source;
    uint8_t  PLL_HSE_divider;
    uint8_t  PLL_multiplier;
    int32_t PLL_clock;
}RCC_PLL;


// RCC Handler Structure
typedef struct
{
    RCC_RegDef    *pRCC;                 /*<>*/
    RCC_Clock     RCC_Clocks;
    RCC_PLL       RCC_PLL_Config;
    uint8_t       Clock_Source;
    uint8_t       SYSCLK_Source;
    uint8_t       AHB_Prescalar;
    uint8_t       APB1_Prescalar;
    uint8_t       APB2_Prescalar;
    uint8_t       MCO_Clock_Source;
}RCC_Handle;


uint8_t RCC_Get_Clock_Source();
uint8_t RCC_Get_SYSCLK_Source();
uint32_t RCC_Get_HCLK();
uint32_t RCC_Get_PCLK1();
uint32_t RCC_Get_PCLK2();
uint32_t RCC_Get_SYSCLK();

void RCC_Update_Clocks();

void RCC_Config_HCLK(uint8_t ahb_prescalar);
void RCC_Config_PCLK1(uint8_t apb1_prescalar);
void RCC_Config_PCLK2(uint8_t apb2_prescalar);
void RCC_Config_PLL(uint8_t pll_clk_src, uint8_t pll_mul_factor, uint8_t pll_hse_div);

void RCC_Select_Clock_Source(uint8_t clk_src);
void RCC_Select_SYSCLK(uint8_t sysclock);

void RCC_init(uint8_t clk_src, uint8_t sysclk_src, uint8_t AHB_prescalar, uint8_t APB1_prescalar, uint8_t APB2_prescalar);

#endif