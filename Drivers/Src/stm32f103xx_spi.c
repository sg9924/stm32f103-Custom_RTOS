#include "stm32f103xx_exti.h"
#include "stm32f103xx_nvic.h"
#include "stm32f103xx_gpio.h"
#include "stm32f103xx_spi.h"


uint8_t SPI_Flag_Check(SPI_Handle* pSPIHandle, uint8_t flag)
{
    return (pSPIHandle->pSPIx->SR & 1<<flag);
}

void SPI_PClk_init(SPI_Handle* pSPIHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if (pSPIHandle->pSPIx == SPI1)
            SPI1_PCLK_EN();
        else if (pSPIHandle->pSPIx == SPI2)
            SPI2_PCLK_EN();
        else if (pSPIHandle->pSPIx == SPI3)
            SPI3_PCLK_EN();
    }
    else if(mode == DISABLE)
    {
        if (pSPIHandle->pSPIx == SPI1)
            SPI1_PCLK_DIS();
        else if (pSPIHandle->pSPIx == SPI2)
            SPI2_PCLK_DIS();
        else if (pSPIHandle->pSPIx == SPI3)
            SPI3_PCLK_DIS();
    }
}

void SPI_Pinit(SPI_Handle* pSPIHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if (pSPIHandle->pSPIx == SPI1)
            SPI1_ENABLE();
        else if (pSPIHandle->pSPIx == SPI2)
            SPI2_ENABLE();
        else if (pSPIHandle->pSPIx == SPI3)
            SPI3_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if (pSPIHandle->pSPIx == SPI1)
            SPI1_DISABLE();
        else if (pSPIHandle->pSPIx == SPI2)
            SPI2_DISABLE();
        else if (pSPIHandle->pSPIx == SPI3)
            SPI3_DISABLE();
    }
}

void SPI_Configure(SPI_Handle* pSPIHandle, uint8_t mode, uint8_t bus, uint8_t dff, uint8_t clk_div, uint8_t clk_phase, uint8_t clk_polarity, uint8_t ssm)
{
    pSPIHandle->SPIConfig.mode              = mode;
    pSPIHandle->SPIConfig.bus_mode          = bus;
    pSPIHandle->SPIConfig.data_frame_format = dff;
    pSPIHandle->SPIConfig.clock_div         = clk_div;
    pSPIHandle->SPIConfig.clock_phase       = clk_phase;
    pSPIHandle->SPIConfig.clock_polarity    = clk_polarity;
    pSPIHandle->SPIConfig.SSM               = ssm;

    pSPIHandle->prxbuffer = 0;
    pSPIHandle->ptxbuffer = 0;
    pSPIHandle->rx_len    = 0;
    pSPIHandle->tx_len    = 0;
    pSPIHandle->rx_state  = SPI_RDY;
    pSPIHandle->tx_state  = SPI_RDY;
}


void SPI_init(SPI_Handle* pSPIHandle, GPIO_Handle* pGPIOHandle, SPI_RegDef* pSPIx)
{
    pSPIHandle->pSPIx = pSPIx;
    pSPIHandle->pGPIOHandle = pGPIOHandle;

    //1. Enable Peripheral Clock
    SPI_PClk_init(pSPIHandle, ENABLE);

    //2. Configure GPIO pins for the SPI peripheral
    if(pSPIHandle->pSPIx == SPI1)
    {
        //SPI1: NSS  - PA4, SCK  - PA5, MISO - PA6, MOSI - PA7
        pSPIHandle->pGPIOHandle->pGPIOx = GPIOA;

        //NSS pin is used only when SSM is not configured
        if(pSPIHandle->SPIConfig.SSM == SPI_SSM_DISABLE)
        {
            //NSS
            GPIO_Config(pGPIOHandle, GPIOA, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_PP, GPIO_PIN4, GPIO_OP_SPEED_2);
            GPIO_Init(pGPIOHandle);
        }
        //SCK
        GPIO_Config(pGPIOHandle, GPIOA, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_PP, GPIO_PIN5, GPIO_OP_SPEED_2);
        GPIO_Init(pGPIOHandle);
        //MOSI
        GPIO_Config(pGPIOHandle, GPIOA, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_PP, GPIO_PIN7, GPIO_OP_SPEED_2);
        GPIO_Init(pGPIOHandle);
        //MISO
        GPIO_Config(pGPIOHandle, GPIOA, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_PP, GPIO_PIN6, GPIO_OP_SPEED_2);
        GPIO_Init(pGPIOHandle);
    }

    //3. configure SPI mode - Master / Slave
    if(pSPIHandle->SPIConfig.mode == SPI_MODE_MASTER)
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_MSTR;
    else if(pSPIHandle->SPIConfig.mode == SPI_MODE_SLAVE)
        pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_MSTR);

    //4. Configure SPI Bus
    if(pSPIHandle->SPIConfig.bus_mode == SPI_BUS_FD)
        pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_BIDI_MODE);   //Full Duplex - 2 unidirectional lines
    else if(pSPIHandle->SPIConfig.bus_mode == SPI_BUS_HD_TX)
    {
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_BIDI_MODE;      //Half Duplex - single bidirectional line
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_BIDIOE;         //output enable for TX
    }
    else if(pSPIHandle->SPIConfig.bus_mode == SPI_BUS_HD_RX)
    {
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_BIDI_MODE ;     //Half Duplex - single bidirectional line
        pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_BIDIOE);      //output disable for RX (default)
    }
    else if(pSPIHandle->SPIConfig.mode == SPI_BUS_FD_RXONLY)
    {
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_BIDI_MODE ;     //Full Duplex - single bidirectional line
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_RX_ONLY;        //RX only
    }

    //5. configure SPI Clock
    if(pSPIHandle->SPIConfig.clock_div == SPI_CLK_DIV2)
        pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_BR0);         //default
    else 
        pSPIHandle->pSPIx->CR1 |= (pSPIHandle->SPIConfig.clock_div)<<SPI_CR1_BR0;

    //6. configure DFF
    if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_8BIT)
        pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_DFF); 
    else if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_8BIT)
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_DFF;

    //7. configure CPOL
    if(pSPIHandle->SPIConfig.clock_polarity == SPI_CPOL_0)
        pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_CPOL); 
    else if(pSPIHandle->SPIConfig.clock_polarity == SPI_CPOL_1)
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_CPOL;
    
    //8. configure CPHA
    if(pSPIHandle->SPIConfig.clock_polarity == SPI_CPHA_0)
        pSPIHandle->pSPIx->CR1 &= ~(1<<SPI_CR1_CPHA); 
    else if(pSPIHandle->SPIConfig.clock_polarity == SPI_CPHA_1)
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_CPHA;
    
    //9. configure for SSM
    if(pSPIHandle->SPIConfig.SSM == SPI_SSM_MASTER)
    {
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_SSM;    //enable SSM
        pSPIHandle->pSPIx->CR1 |= 1<<SPI_CR1_SSI;    //Set SSI to 1 to force this value in NSS pin
    }

    //10. Enable SPI Peripheral
    SPI_Pinit(pSPIHandle, ENABLE);
}


// SPI Send Data (Blocking)
void SPI_TX(SPI_Handle* pSPIHandle, uint8_t *pbuffer, uint32_t size)
{
    pSPIHandle->tx_state = SPI_TX_BUSY;
    pSPIHandle->tx_len = size;
    pSPIHandle->ptxbuffer = pbuffer;

    if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_16BIT)
    {
        while(size>0)
        {
            while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_TXE));
            pSPIHandle->pSPIx->DR = *((uint16_t*)pbuffer);
            pbuffer+=2;
            size--;
        }
    }
    else if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_8BIT)
    {
        while(size>0)
        {
            while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_TXE));
            pSPIHandle->pSPIx->DR = *(pbuffer);
            pbuffer++;
            size--;
        }
    }

    //while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_BSY));
    pSPIHandle->tx_state = SPI_RDY;
}



// SPI Receive Data (Blocking)
void SPI_RX(SPI_Handle* pSPIHandle, uint8_t *pbuffer, uint32_t size)
{
    pSPIHandle->tx_state = SPI_RX_BUSY;
    pSPIHandle->tx_len = size;
    pSPIHandle->ptxbuffer = pbuffer;

    if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_16BIT)
    {
        while(size>0)
        {
            while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_RXNE));
            *((uint16_t*)pbuffer) = pSPIHandle->pSPIx->DR;
            pbuffer+=2;
            size--;
        }
    }
    else if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_8BIT)
    {
        while(size>0)
        {
            while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_RXNE));
            *pbuffer = pSPIHandle->pSPIx->DR;
            pbuffer++;
            size--;
        }
    }

    //while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_BSY));
    pSPIHandle->tx_state = SPI_RDY;
}


void SPI_IT_Config(SPI_Handle* pSPIHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pSPIHandle->pSPIx == SPI1)
            nvic_intrpt_enable(SPI1_IRQ_NO);
        else if(pSPIHandle->pSPIx == SPI2)
            nvic_intrpt_enable(SPI2_IRQ_NO);
        else if(pSPIHandle->pSPIx == SPI3)
            nvic_intrpt_enable(SPI3_IRQ_NO);
    }
    else if(mode == DISABLE)
    {
        if(pSPIHandle->pSPIx == SPI1)
            nvic_intrpt_disable(SPI1_IRQ_NO);
        else if(pSPIHandle->pSPIx == SPI2)
            nvic_intrpt_disable(SPI2_IRQ_NO);
        else if(pSPIHandle->pSPIx == SPI3)
            nvic_intrpt_disable(SPI3_IRQ_NO);
    }
}



uint8_t SPI_TX_IT(SPI_Handle* pSPIHandle, uint8_t *pbuffer, uint32_t size)
{
    //1. get current state of SPI
    uint8_t tx_state = pSPIHandle->tx_state;

    //For SPI not busy in TX
    if(tx_state!=SPI_TX_BUSY)
    {
        //2. store bufer address, data length 
        pSPIHandle->ptxbuffer = pbuffer;
        pSPIHandle->tx_len    = size;

        //3. set state as busy in TX
        pSPIHandle->tx_state  = SPI_TX_BUSY;

        //4. enable interrupt for SPI in NVIC
        SPI_IT_Config(pSPIHandle, ENABLE);

        //5. Enable TXE interrupt in SPI
        pSPIHandle->pSPIx->CR2 |= 1<<SPI_CR2_TXEIE;
    }

    return tx_state;
}


void SPI_IRQ_Handler(SPI_Handle* pSPIHandle)
{

    /********* TXE Flag *********/
    uint32_t temp1 = pSPIHandle->pSPIx->CR2 & 1<<SPI_CR2_TXEIE;
    uint32_t temp2 = pSPIHandle->pSPIx->SR & 1<<SPI_SR_TXE;

    //if TXEIE and TXE are set
    if(temp1 && temp2)
    {
        if(pSPIHandle->tx_state == SPI_TX_BUSY)
        {
            if(pSPIHandle->tx_len == 0)
            {
                //clear TXE flag
                pSPIHandle->pSPIx->SR &= ~(1<<SPI_SR_TXE);
                //disable TXEIE
                pSPIHandle->pSPIx->CR2 &= ~(1<<SPI_CR2_TXEIE);
                //reset SPI state
                pSPIHandle->tx_state = SPI_RDY;
                //reset buffer address
                pSPIHandle->ptxbuffer = 0;
                //reset data length
                pSPIHandle->tx_len = 0;
            }
            else
            {
                while(pSPIHandle->tx_len > 0)
                {
                    if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_16BIT)
                    {
                            while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_TXE));
                            pSPIHandle->pSPIx->DR = *((uint16_t*)(pSPIHandle->ptxbuffer));
                            (pSPIHandle->ptxbuffer)+=2;
                            (pSPIHandle->tx_len)--;
                    }
                    else if(pSPIHandle->SPIConfig.data_frame_format == SPI_DFF_8BIT)
                    {
                            while(!(pSPIHandle->pSPIx->SR & 1<<SPI_SR_TXE));
                            pSPIHandle->pSPIx->DR = *(pSPIHandle->ptxbuffer);
                            (pSPIHandle->ptxbuffer)++;
                            (pSPIHandle->tx_len)--;
                    } 
                }
            }
        }
    }
}