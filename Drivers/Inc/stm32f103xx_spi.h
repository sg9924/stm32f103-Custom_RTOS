#ifndef INC_stm32F103xx_SPI_H
#define INC_stm32F103xx_SPI_H

#include<stdint.h>
#include"stm32F103xx.h"
#include"stm32F103xx_gpio.h"
#include"stm32F103xx_rcc.h"

/********************************************* SPI Structure Definitions Start *********************************************/
typedef struct
{
    uint8_t    mode;
    uint8_t    bus_mode;
    uint8_t    clock_polarity;
    uint8_t    clock_phase;
    uint8_t    clock_div;
    uint8_t    data_frame_format;
    uint8_t    SSM;
}SPI_Config;


typedef struct
{
    SPI_RegDef  *pSPIx;
    SPI_Config  SPIConfig;
    GPIO_Handle *pGPIOHandle;
    uint8_t     *ptxbuffer;
    uint8_t     *prxbuffer;
    uint8_t     tx_state;
    uint8_t     rx_state;
    uint32_t    tx_len;
    uint32_t    rx_len;
}SPI_Handle;

/********************************************* SPI Structure Definitions Start *********************************************/

/************************************************** SPI Definitions Start **************************************************/

#define SPI1                   ((SPI_RegDef*)SPI1_BASE_ADDR)
#define SPI2                   ((SPI_RegDef*)SPI2_BASE_ADDR)
#define SPI3                   ((SPI_RegDef*)SPI3_BASE_ADDR)

//SPI CR1 Bit Field Definitions
#define SPI_CR1_CPHA          0
#define SPI_CR1_CPOL          1
#define SPI_CR1_MSTR          2
#define SPI_CR1_BR0           3
#define SPI_CR1_BR1           4
#define SPI_CR1_BR2           5
#define SPI_CR1_SPE           6
#define SPI_CR1_LSB_FIRST     7
#define SPI_CR1_SSI           8
#define SPI_CR1_SSM           9
#define SPI_CR1_RX_ONLY       10
#define SPI_CR1_DFF           11
#define SPI_CR1_CRC_NEXT      12
#define SPI_CR1_CRCEN         13
#define SPI_CR1_BIDIOE        14
#define SPI_CR1_BIDI_MODE     15

//SPI CR2 Bit Fields Definitions
#define SPI_CR2_RXDMAEN       0
#define SPI_CR2_TXDMAEN       1
#define SPI_CR2_SSOE          2
#define SPI_CR2_ERRIE         5
#define SPI_CR2_RXNEIE        6
#define SPI_CR2_TXEIE         7

//SPI SR Bit Field Definitions
#define SPI_SR_RXNE           0
#define SPI_SR_TXE            1
#define SPI_SR_CHSIDE         2
#define SPI_SR_UDR            3
#define SPI_SR_CRCERR         4
#define SPI_SR_MODF           5
#define SPI_SR_OVR            6
#define SPI_SR_BSY            7

//SPI DR Bit Field Definitions
#define SPI_DR_BIT0           0
#define SPI_DR_BIT1           1
#define SPI_DR_BIT2           2
#define SPI_DR_BIT3           3
#define SPI_DR_BIT4           4
#define SPI_DR_BIT5           5
#define SPI_DR_BIT6           6
#define SPI_DR_BIT7           7
#define SPI_DR_BIT8           8
#define SPI_DR_BIT9           9
#define SPI_DR_BIT10          10
#define SPI_DR_BIT11          11
#define SPI_DR_BIT12          12
#define SPI_DR_BIT13          13
#define SPI_DR_BIT14          14
#define SPI_DR_BIT15          15

//SPI Modes
#define SPI_MODE_MASTER       0
#define SPI_MODE_SLAVE        1

//SPI Clock Phases
#define SPI_CPHA_DEFAULT      SPI_CPHA_0
#define SPI_CPHA_0            0
#define SPI_CPHA_1            1

//SPI Clock Polarities
#define SPI_CPOL_DEFAULT      SPI_CPOL_0
#define SPI_CPOL_0            0
#define SPI_CPOL_1            1

//SPI Data Frame Formats
#define SPI_DFF_DEFAULT       SPI_DFF_8BIT
#define SPI_DFF_8BIT          0
#define SPI_DFF_16BIT         1

//SPI Bus Modes
#define SPI_BUS_FD            0
#define SPI_BUS_FD_RXONLY     1
#define SPI_BUS_HD_TX         2
#define SPI_BUS_HD_RX         3
#define SPI_BUS_SIMPLEX_TX    4
#define SPI_BUS_SIMPLEX_RX    5

//SPI Clock Speed
#define SPI_CLK_DIV2          0
#define SPI_CLK_DIV4          1
#define SPI_CLK_DIV8          2
#define SPI_CLK_DIV16         3
#define SPI_CLK_DIV32         4
#define SPI_CLK_DIV64         5
#define SPI_CLK_DIV128        6
#define SPI_CLK_DIV256        7

//SPI SSM
#define SPI_SSM_DISABLE       0 
#define SPI_SSM_MASTER        1
#define SPI_SSM_SLAVE         2

//SPI States
#define SPI_RDY               0
#define SPI_TX_BUSY           1
#define SPI_RX_BUSY           2

/************************************************** SPI Definitions End ****************************************************/

/**************************************************** SPI Macros Start *****************************************************/

//SPI Peripheral Clock Enable
#define SPI1_PCLK_EN()       (RCC->APB2ENR |= 1<<RCC_APB2ENR_SPI1EN)
#define SPI2_PCLK_EN()       (RCC->APB2ENR |= 1<<RCC_APB1ENR_SPI2EN)
#define SPI3_PCLK_EN()       (RCC->APB2ENR |= 1<<RCC_APB1ENR_SPI3EN)

//SPI Peripheral Clock Disable
#define SPI1_PCLK_DIS()       (RCC->APB2ENR &= ~(1<<RCC_APB2ENR_SPI1EN))
#define SPI2_PCLK_DIS()       (RCC->APB2ENR &= ~(1<<RCC_APB1ENR_SPI2EN))
#define SPI3_PCLK_DIS()       (RCC->APB2ENR &= ~(1<<RCC_APB1ENR_SPI3EN))

//SPI Peripheral Enable
#define SPI1_ENABLE()        (SPI1->CR1 |= 1<< SPI_CR1_SPE)
#define SPI2_ENABLE()        (SPI2->CR1 |= 1<< SPI_CR1_SPE)
#define SPI3_ENABLE()        (SPI3->CR1 |= 1<< SPI_CR1_SPE)

//SPI Peripheral Disable
#define SPI1_DISABLE()        (SPI1->CR1 &= ~(1<< SPI_CR1_SPE))
#define SPI2_DISABLE()        (SPI2->CR1 &= ~(1<< SPI_CR1_SPE))
#define SPI3_DISABLE()        (SPI3->CR1 &= ~(1<< SPI_CR1_SPE))

/**************************************************** SPI Macros End *******************************************************/

uint8_t SPI_Flag_Check(SPI_Handle* pSPIHandle, uint8_t flag);
void SPI_PClk_init(SPI_Handle* pSPIHandle, uint8_t mode);
void SPI_Pinit(SPI_Handle* pSPIHandle, uint8_t mode);
void SPI_Configure(SPI_Handle* pSPIHandle, uint8_t mode, uint8_t bus,uint8_t dff, uint8_t clk_div, uint8_t clk_phase, uint8_t clk_polarity, uint8_t ssm);
void SPI_init(SPI_Handle* pSPIHandle, GPIO_Handle* pGPIOHandle, SPI_RegDef* pSPIx);
void SPI_TX(SPI_Handle* pSPIHandle, uint8_t *pbuffer, uint32_t size);

void SPI_IT_Config(SPI_Handle* pSPIHandle, uint8_t mode);
uint8_t SPI_TX_IT(SPI_Handle* pSPIHandle, uint8_t *pbuffer, uint32_t size);
void SPI_IRQ_Handler(SPI_Handle* pSPIHandle);



#endif