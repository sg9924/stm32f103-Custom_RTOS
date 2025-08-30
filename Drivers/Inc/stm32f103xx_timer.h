#ifndef INC_stm32F103xx_TIMER_H
#define INC_stm32F103xx_TIMER_H
#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_rcc.h"
#include"stm32f103xx_nvic.h"

//Timer Base Unit Configuration (maybe split into 2 config structures? -> Frequently used + rarely used -> use memset)
typedef struct
{
    uint8_t  count_direction;
    uint32_t count;
    uint8_t  center_aligned_mode;
    uint8_t  one_pulse_mode;
    uint8_t  ar_preload;
    uint8_t  clock_div;
    uint16_t autoreload_value;
    uint16_t prescale_value;
}TIM_Base_Config;

//Timer Interrupt Config
typedef struct
{
    uint8_t interrupt_mode;
    uint8_t interrupt_type;
}TIM_Intrpt_Config;

//Timer DMA Config
typedef struct
{
    uint8_t dma_mode;
    uint8_t dma_type;
}TIM_DMA_Config;

//Timer Output Channel Configuration
typedef struct
{
    uint8_t  oc_mode;
    uint8_t  oc_channel;
    uint16_t oc_value;
    uint8_t  oc_polarity;
    uint8_t  oc_preload;
}TIM_OC_Config;

//Timer Base Handle
typedef struct
{
    TIM_RegDef*        pTIMx;
    TIM_Base_Config    TIMx_Base_Config;
    TIM_Intrpt_Config  TIMx_Intrpt_Config;
    TIM_DMA_Config     TIMx_DMA_Config;
    uint8_t            timer_state;
}TIM_Handle;

//Timer Output Channel Handle
typedef struct
{
    //TIM_RegDef*      pTIMx;
    TIM_Handle*      pTIMHandle;
    TIM_OC_Config    TIMx_OC_Config;
    uint8_t          timer_state;
}TIM_OC_Handle;


//TIM_CR1 Register Bit Field Definitions
#define TIM_CR1_CEN               0
#define TIM_CR1_UDIS              1
#define TIM_CR1_URS               2
#define TIM_CR1_OPM               3
#define TIM_CR1_DIR               4
#define TIM_CR1_CMS               5
#define TIM_CR1_ARPE              7
#define TIM_CR1_CKD0              8
#define TIM_CR1_CKD1              9

//TIM_CR2 Register Bit Field Definitions
#define TIM_CR2_CCDS              3
#define TIM_CR2_MMS0              4
#define TIM_CR2_MMS1              5
#define TIM_CR2_MMS2              6
#define TIM_CR2_TI1S              7

//TIM_SMCR Register Bit Field Definitions
#define TIM_SMCR_SMS0             0
#define TIM_SMCR_SMS1             1
#define TIM_SMCR_SMS2             2
#define TIM_SMCR_TS0              4
#define TIM_SMCR_TS1              5
#define TIM_SMCR_TS2              6
#define TIM_SMCR_MSM              7
#define TIM_SMCR_ETF0             8
#define TIM_SMCR_ETF1             9
#define TIM_SMCR_ETF2             10
#define TIM_SMCR_ETF3             11
#define TIM_SMCR_ETPS0            12
#define TIM_SMCR_ETPS1            13
#define TIM_SMCR_ECE              14
#define TIM_SMCR_ETP              15

//TIM_DIER Register Bit Field Definitions
#define TIM_DIER_UIE              0
#define TIM_DIER_CC1IE            1
#define TIM_DIER_CC2IE            2
#define TIM_DIER_CC3IE            3
#define TIM_DIER_CC4IE            4
#define TIM_DIER_TIE              6
#define TIM_DIER_UDE              8
#define TIM_DIER_CC1DE            9
#define TIM_DIER_CC2DE            10
#define TIM_DIER_CC3DE            11
#define TIM_DIER_CC4DE            12
#define TIM_DIER_TDE              14

//TIM_SR Register Bit Field Definitions
#define TIM_SR_UIF                0
#define TIM_SR_CC1IF              1
#define TIM_SR_CC2IF              2
#define TIM_SR_CC3IF              3
#define TIM_SR_CC4IF              4
#define TIM_SR_TIF                6
#define TIM_SR_CC1OF              9
#define TIM_SR_CC2OF              10
#define TIM_SR_CC3OF              11
#define TIM_SR_CC4OF              12

//TIM_EGR Register Bit Field Definitions
#define TIM_EGR_UG                0
#define TIM_EGR_CC1G              1
#define TIM_EGR_CC2G              2
#define TIM_EGR_CC3G              3
#define TIM_EGR_CC4G              4
#define TIM_EGR_TG                6

//TIM_CCMR1 Register Bt Field Definitions
//for input capture
#define TIM_CCMR1_CC1S0           0
#define TIM_CCMR1_CC1S1           1
#define TIM_CCMR1_IC1PSC0         2
#define TIM_CCMR1_IC1PSC1         3
#define TIM_CCMR1_IC1F0           4
#define TIM_CCMR1_IC1F1           5
#define TIM_CCMR1_IC1F2           6
#define TIM_CCMR1_IC1F3           7
#define TIM_CCMR1_CC2S0           8
#define TIM_CCMR1_CC2S1           9
#define TIM_CCMR1_IC2PSC0         10
#define TIM_CCMR1_IC2PSC1         11
#define TIM_CCMR1_IC2F0           12
#define TIM_CCMR1_IC2F1           13
#define TIM_CCMR1_IC2F2           14
#define TIM_CCMR1_IC2F3           15

//for output capture
#define TIM_CCMR1_OC1FE           2
#define TIM_CCMR1_OC1PE           3
#define TIM_CCMR1_OC1M0           4
#define TIM_CCMR1_OC1M1           5
#define TIM_CCMR1_OC1M2           6
#define TIM_CCMR1_OC1CE           9
#define TIM_CCMR1_OC2FE           10
#define TIM_CCMR1_OC2PE           11
#define TIM_CCMR1_OC2M0           12
#define TIM_CCMR1_OC2M1           13
#define TIM_CCMR1_OC2M2           14
#define TIM_CCMR1_OC2CE           15

//TIM_CCMR2 Register Bit Field Definitions
//for input capture
#define TIM_CCMR2_CC3S0           0
#define TIM_CCMR2_CC3S1           1
#define TIM_CCMR2_IC3PSC0         2
#define TIM_CCMR2_IC3PSC1         3
#define TIM_CCMR2_IC3F0           4
#define TIM_CCMR2_IC3F1           5
#define TIM_CCMR2_IC3F2           6
#define TIM_CCMR2_IC3F3           7
#define TIM_CCMR2_CC4S0           8
#define TIM_CCMR2_CC4S1           9
#define TIM_CCMR2_IC4PSC0         10
#define TIM_CCMR2_IC4PSC1         11
#define TIM_CCMR2_IC4F0           12
#define TIM_CCMR2_IC4F1           13
#define TIM_CCMR2_IC4F2           14
#define TIM_CCMR2_IC4F3           15

//for output capture
#define TIM_CCMR2_OC3FE           2
#define TIM_CCMR2_OC3PE           3
#define TIM_CCMR2_OC3M0           4
#define TIM_CCMR2_OC3M1           5
#define TIM_CCMR2_OC3M2           6
#define TIM_CCMR2_OC3CE           9
#define TIM_CCMR2_OC4FE           10
#define TIM_CCMR2_OC4PE           11
#define TIM_CCMR2_OC4M0           12
#define TIM_CCMR2_OC4M1           13
#define TIM_CCMR2_OC4M2           14
#define TIM_CCMR2_OC4CE           15

//TIM_CCER Register Bit Field Definitions
#define TIM_CCER_CC1E             0
#define TIM_CCER_CC1P             1
#define TIM_CCER_CC2E             4
#define TIM_CCER_CC2P             5
#define TIM_CCER_CC3E             8
#define TIM_CCER_CC3P             9
#define TIM_CCER_CC4E             12
#define TIM_CCER_CC4P             13

//TIM_DCR Register Bit Field Definitions
#define TIM_DCR_DBA0              0
#define TIM_DCR_DBA1              1
#define TIM_DCR_DBA2              2
#define TIM_DCR_DBA3              3
#define TIM_DCR_DBA4              4
#define TIM_DCR_DBL1              8
#define TIM_DCR_DBL2              9
#define TIM_DCR_DBL3              10
#define TIM_DCR_DBL4              11
#define TIM_DCR_DBL5              12



//Timer Count Direction
#define TIM_COUNT_DIR_UP          0
#define TIM_COUNT_DIR_DOWN        1
#define TIM_COUNT_DIR_DEFAULT     TIM_COUNT_DIR_UP

//Timer Center Aligned Mode Types
#define TIM_CMS_EDGE              0
#define TIM_CMS_MODE_1            1
#define TIM_CMS_MODE_2            2
#define TIM_CMS_MODE_3            3
#define TIM_CMS_DEFAULT            TIM_CMS_EDGE

//Timer One Pulse Mode
#define TIM_OPM_DISABLE           0
#define TIM_OPM_ENABLE            1
#define TIM_OPM_DEFAULT           TIM_OPM_DISABLE

//Timer Auto Reload Preload
#define TIM_AR_PRELOAD_DISABLE    0                        //the ARR value is loaded into the shadow register permanently
#define TIM_AR_PRELOAD_ENABLE     1                        //the ARR value is loaded into the shadow register on each Update Event (UEV)
#define TIM_AR_PRELOAD_DEFAULT    TIM_AR_PRELOAD_DISABLE

//Output Compare Settings
//Output Compare Mode
#define TIM_OC_MODE_FROZEN          0
#define TIM_OC_MODE_ACTIVE_MATCH    1
#define TIM_OC_MODE_INACTIVE_MATCH  2
#define TIM_OC_MODE_TOGGLE          3
#define TIM_OC_MODE_FORCE_INACTIVE  4
#define TIM_OC_MODE_FORCE_ACTIVE    5
#define TIM_OC_MODE_PWM_1           6
#define TIM_OC_MODE_PWM_2           7

//Output Compare Polarity
#define TIM_OC_POL_ACTIVE_HIGH    0
#define TIM_OC_POL_ACTIVE_LOW     1

//Output Compare Preload
#define TIM_OC_PRELOAD_DISABLE    0
#define TIM_OC_PRELOAD_ENABLE     1

//Timer Interrupts
#define TIM_UPDATE_INTRPT         0
#define TIM_CC_INTRPT             1
#define TIM_TRIG_INTRPT           5

//Timer DMA
#define TIM_UPDATE_DMA            8
#define TIM_CC_DMA                9
#define TIM_TRIG_DMA              13


//Timer Channel
#define TIM_CHANNEL1              0
#define TIM_CHANNEL2              1
#define TIM_CHANNEL3              2
#define TIM_CHANNEL4              3

//Timer State
#define TIM_STATE_READY           0
#define TIM_STATE_RUNNING         1



//TIMER Peripherals Definition
#define TIM1                         ((TIM_RegDef*)TIM1_BASE_ADDR)
#define TIM2                         ((TIM_RegDef*)TIM2_BASE_ADDR)
#define TIM3                         ((TIM_RegDef*)TIM3_BASE_ADDR)
#define TIM4                         ((TIM_RegDef*)TIM4_BASE_ADDR)
#define TIM5                         ((TIM_RegDef*)TIM5_BASE_ADDR)
#define TIM6                         ((TIM_RegDef*)TIM6_BASE_ADDR)

//Timer Peripheral Clock Enable
#define TIM1_PCLK_ENABLE()           (RCC->APB2ENR |= 1<<RCC_APB2ENR_TIM1EN)
#define TIM2_PCLK_ENABLE()           (RCC->APB1ENR |= 1<<RCC_APB1ENR_TIM2EN)
#define TIM3_PCLK_ENABLE()           (RCC->APB1ENR |= 1<<RCC_APB1ENR_TIM3EN)
#define TIM4_PCLK_ENABLE()           (RCC->APB1ENR |= 1<<RCC_APB1ENR_TIM4EN)
#define TIM5_PCLK_ENABLE()           (RCC->APB1ENR |= 1<<RCC_APB1ENR_TIM5EN)
#define TIM6_PCLK_ENABLE()           (RCC->APB1ENR |= 1<<RCC_APB1ENR_TIM6EN)

//Timer Peripheral Clock Disable
#define TIM1_PCLK_DISABLE()          (RCC->APB2ENR &= ~(1<<RCC_APB2ENR_TIM1EN))
#define TIM2_PCLK_DISABLE()          (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_TIM2EN))
#define TIM3_PCLK_DISABLE()          (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_TIM3EN))
#define TIM4_PCLK_DISABLE()          (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_TIM4EN))
#define TIM5_PCLK_DISABLE()          (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_TIM5EN))
#define TIM6_PCLK_DISABLE()          (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_TIM6EN))

//Timer Peripheral Enable
#define TIM1_ENABLE()                (TIM1->CR1 |= 1<<TIM_CR1_CEN)
#define TIM2_ENABLE()                (TIM2->CR1 |= 1<<TIM_CR1_CEN)
#define TIM3_ENABLE()                (TIM3->CR1 |= 1<<TIM_CR1_CEN)
#define TIM4_ENABLE()                (TIM4->CR1 |= 1<<TIM_CR1_CEN)
#define TIM5_ENABLE()                (TIM5->CR1 |= 1<<TIM_CR1_CEN)
#define TIM6_ENABLE()                (TIM6->CR1 |= 1<<TIM_CR1_CEN)

//Timer Peripheral Disable
#define TIM1_DISABLE()               (TIM1->CR1 &= ~(1<<TIM_CR1_CEN))
#define TIM2_DISABLE()               (TIM2->CR1 &= ~(1<<TIM_CR1_CEN))
#define TIM3_DISABLE()               (TIM3->CR1 &= ~(1<<TIM_CR1_CEN))
#define TIM4_DISABLE()               (TIM4->CR1 &= ~(1<<TIM_CR1_CEN))
#define TIM5_DISABLE()               (TIM5->CR1 &= ~(1<<TIM_CR1_CEN))
#define TIM6_DISABLE()               (TIM6->CR1 &= ~(1<<TIM_CR1_CEN))


//Function Declarations

//Timer Peripheral Clock Init
void TIM_PClk_init(TIM_Handle* pTIMHandle, uint8_t mode);
//Timer Peripheral Init
void TIM_P_init(TIM_Handle* pTIMHandle, uint8_t mode);

void TIM_Interrupt_init(TIM_RegDef* pTIMx, uint8_t interrupt_type, uint8_t channel, uint8_t mode);

//Base Timer Functions
//Timer Configure
void TIM_Base_Configure(TIM_Handle* pTIMHandle, TIM_RegDef* pTIMx, uint8_t count_direction, uint32_t prescale_value, uint32_t autoreload_value, uint8_t ar_preload);
//Timer Init
void TIM_Base_init(TIM_Handle* pTIMHandle);
//Timer Start
void TIM_Base_Start(TIM_Handle* pTIMHandle);
//Timer Start
void TIM_Base_Stop(TIM_Handle* pTIMHandle);

//General Timer Functions


//DMA
void TIM_DMA_Init(TIM_RegDef* pTIMx, uint8_t dma_type, uint8_t channel, uint8_t mode);

//
void TIM_Prescaler_Load(TIM_Handle* pTIMHandle, uint32_t prescale_value);
void TIM_AutoReload_Load(TIM_Handle* pTIMHandle, uint32_t auto_reload_value);
void TIM_Count_Reset(TIM_Handle* pTIMHandle);
void TIM_Update_Event_Check(TIM_Handle* pTIMHandle);
void TIM_Update_Event_Check(TIM_Handle* pTIMHandle);
void TIM_Update_Event_Trigger(TIM_Handle* pTIMHandle);


#endif