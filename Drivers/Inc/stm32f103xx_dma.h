#ifndef INC_stm32F103xx_DMA_H
#define INC_stm32F103xx_DMA_H
#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_rcc.h"
#include"stm32f103xx_nvic.h"

enum DMA_Channel {
    _ADC1 = 0,
    _TIM2_CH3 = 0,
    _TIM4_CH1 = 0,
    _USART3_TX = 1,
    _TIM1_CH1 = 1,
    _TIM2_UP = 1,
    _TIM3_CH3 = 1,
    _SPI1_RX = 1
};

typedef struct
{
    uint8_t   direction;
    uint8_t   peripheral_inc;
    uint8_t   memory_inc;
    uint8_t   circular_mode;
    uint8_t   channel_priority;
    uint8_t   memory_size;
    uint8_t   peripheral_size;
    uint16_t  no_of_data;
    uint8_t   mem_to_mem_mode;
}DMA_Config;

typedef struct 
{
    DMA_RegDef* pDMAx;
    DMA_Config  DMAx_Config;
    uint8_t     channel;
    uint32_t    peripheral_addr;
    uint32_t    memory_addr_1;
    uint32_t    memory_addr_2;
}DMA_Handle;


//DMA Peripheral Definitions
#define DMA1              ((DMA_RegDef*)0x40020000)
#define DMA2              ((DMA_RegDef*)0x40020400)



//DMA_ISR Register Bit Field Definitions
#define DMA_ISR_GIF1                    0
#define DMA_ISR_TCIF1                   1
#define DMA_ISR_HTIF1                   2
#define DMA_ISR_TEIF1                   3
#define DMA_ISR_GIF2                    4
#define DMA_ISR_TCIF2                   5
#define DMA_ISR_HTIF2                   6
#define DMA_ISR_TEIF2                   7
#define DMA_ISR_GIF3                    8
#define DMA_ISR_TCIF3                   9
#define DMA_ISR_HTIF3                   10
#define DMA_ISR_TEIF3                   11
#define DMA_ISR_GIF4                    12
#define DMA_ISR_TCIF4                   13
#define DMA_ISR_HTIF4                   14
#define DMA_ISR_TEIF4                   15
#define DMA_ISR_GIF5                    16
#define DMA_ISR_TCIF5                   17
#define DMA_ISR_HTIF5                   18
#define DMA_ISR_TEIF5                   19
#define DMA_ISR_GIF6                    20
#define DMA_ISR_TCIF6                   21
#define DMA_ISR_HTIF6                   22
#define DMA_ISR_TEIF6                   23
#define DMA_ISR_GIF7                    24
#define DMA_ISR_TCIF7                   25
#define DMA_ISR_HTIF7                   26
#define DMA_ISR_TEIF7                   27

//DMA_IFCR Register Bit Field Definitions
#define DMA_IFCR_CGIF1                  0
#define DMA_IFCR_CTCIF1                 1
#define DMA_IFCR_CHTIF1                 2
#define DMA_IFCR_CTEIF1                 3
#define DMA_IFCR_CGIF2                  4
#define DMA_IFCR_CTCIF2                 5
#define DMA_IFCR_CHTIF2                 6
#define DMA_IFCR_CTEIF2                 7
#define DMA_IFCR_CGIF3                  8
#define DMA_IFCR_CTCIF3                 9
#define DMA_IFCR_CHTIF3                 10
#define DMA_IFCR_CTEIF3                 11
#define DMA_IFCR_CGIF4                  12
#define DMA_IFCR_CTCIF4                 13
#define DMA_IFCR_CHTIF4                 14
#define DMA_IFCR_CTEIF4                 15
#define DMA_IFCR_CGIF5                  16
#define DMA_IFCR_CTCIF5                 17
#define DMA_IFCR_CHTIF5                 18
#define DMA_IFCR_CTEIF5                 19
#define DMA_IFCR_CGIF6                  20
#define DMA_IFCR_CTCIF6                 21
#define DMA_IFCR_CHTIF6                 22
#define DMA_IFCR_CTEIF6                 23
#define DMA_IFCR_CGIF7                  24
#define DMA_IFCR_CTCIF7                 25
#define DMA_IFCR_CHTIF7                 26
#define DMA_IFCR_CTEIF7                 27

// DMA_CCRx Register Bit Field Definitions
#define DMA_CCRx_EN                     0
#define DMA_CCRx_TCIE                   1
#define DMA_CCRx_HTIE                   2
#define DMA_CCRx_TEIE                   3
#define DMA_CCRx_DIR                    4
#define DMA_CCRx_CIRC                   5
#define DMA_CCRx_PINC                   6
#define DMA_CCRx_MINC                   7
#define DMA_CCRx_PSIZE0                 8
#define DMA_CCRx_PSIZE1                 9
#define DMA_CCRx_MSIZE0                 10
#define DMA_CCRx_MSIZE1                 11
#define DMA_CCRx_PL0                    12
#define DMA_CCRx_PL1                    13
#define DMA_CCRx_MEM2MEM                14


//DMA Direction
#define DMA_DIR_FROM_PERIPHERAL         0
#define DMA_DIR_FROM_MEMORY             1

//DMA Memory to Memory
#define DMA_MEM_TO_MEM_ENABLE           1
#define DMA_MEM_TO_MEM_DISABLE          0

//DMA Channel
#define DMA_CHANNEL_1                   0
#define DMA_CHANNEL_2                   1
#define DMA_CHANNEL_3                   2
#define DMA_CHANNEL_4                   3
#define DMA_CHANNEL_5                   4
#define DMA_CHANNEL_6                   5
#define DMA_CHANNEL_7                   6

//DMA Peripheral Size
#define DMA_PERI_SIZE_8BIT              0
#define DMA_PERI_SIZE_16BIT             1
#define DMA_PERI_SIZE_32BIT             2

//DMA Memory Size
#define DMA_MEM_SIZE_8BIT               0
#define DMA_MEM_SIZE_16BIT              1
#define DMA_MEM_SIZE_32BIT              2

//DMA Peripheral Increment
#define DMA_PERI_INC_ENABLE             1
#define DMA_PERI_INC_DISABLE            0

//DMA Memory Increment
#define DMA_MEM_INC_ENABLE              1
#define DMA_MEM_INC_DISABLE             0

//DMA Channel Priority
#define DMA_CHANNEL_PRIORITY_LOW        0
#define DMA_CHANNEL_PRIORITY_MEDIUM     1
#define DMA_CHANNEL_PRIORITY_HIGH       2
#define DMA_CHANNEL_PRIORITY_VERY_HIGH  3

//DMA Circular Mode
#define DMA_CIRC_MODE_DISABLE           0
#define DMA_CIRC_MODE_ENABLE            1

//DMA Interrupts
#define DMA_INTRPT_TC_DISABLE           (~(1<<DMA_CCRx_TCIE))
#define DMA_INTRPT_TC_ENABLE            (1<<DMA_CCRx_TCIE)

#define DMA_INTRPT_TE_DISABLE           (~(1<<DMA_CCRx_TEIE))
#define DMA_INTRPT_TE_ENABLE            (1<<DMA_CCRx_TEIE)

#define DMA_INTRPT_HT_DISABLE           (~(1<<DMA_CCRx_HTIE))
#define DMA_INTRPT_HT_ENABLE            (1<<DMA_CCRx_HTIE)



//DMA Peripheral Clock Enable
#define DMA1_PCLK_ENABLE()              RCC->AHBENR |= 1<<RCC_AHBENR_DMA1EN
#define DMA2_PCLK_ENABLE()              RCC->AHBENR |= 1<<RCC_AHBENR_DMA2EN

//DMA Peripheral Clock Disable
#define DMA1_PCLK_DISABLE()             RCC->AHBENR &= ~(1<<RCC_AHBENR_DMA1EN)
#define DMA2_PCLK_DISABLE()             RCC->AHBENR &= ~(1<<RCC_AHBENR_DMA2EN)

//DMA Channel Enable
#define DMA1_ENABLE(channel)            (DMA1->Channel[channel].CCR |= 1<<DMA_CCRx_EN)
#define DMA2_ENABLE(channel)            (DMA2->Channel[channel].CCR |= 1<<DMA_CCRx_EN)

//DMA Channel Disable
#define DMA1_DISABLE(channel)           (DMA1->Channel[channel].CCR &= ~(1<<DMA_CCRx_EN))
#define DMA2_DISABLE(channel)           (DMA2->Channel[channel].CCR &= ~(1<<DMA_CCRx_EN))


//Function API Declarations
void DMA_PClk_init(DMA_Handle* pDMAHandle, uint8_t mode);
void DMA_Configure(DMA_Handle* pDMAHandle, DMA_RegDef* pDMAx, uint8_t channel, uint8_t channel_priority, uint8_t direction, uint16_t no_of_data, uint8_t peri_size, uint8_t mem_size,
                   uint8_t peri_inc, uint8_t mem_inc, uint8_t mem_to_mem_mode, uint8_t circ_mode);
void DMA_Address_Config(DMA_Handle* pDMAHandle, uint32_t peri_addr, uint32_t mem_addr);
void DMA_init(DMA_Handle* pDMAHandle);

//DMA Interrupts
void DMA_IT_Config(DMA_Handle* pDMAHandle, uint8_t interrupt, uint8_t mode);
void DMA_IRQ_Config(DMA_Handle* pDMAHandle, uint8_t mode, uint8_t channel);


#endif