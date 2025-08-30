#include "stm32f103xx.h"
#include "stm32f103xx_gpio.h"
#include "stm32f103xx_usart.h"
#include "stm32f103xx_afio.h"
#include "stm32f103xx_exti.h"
#include "stm32f103xx_nvic.h"

//extern GPIO_Pins GPIO_Used;

/*USART Pins
-USART2
--TX   PA2
--RX   PA3
--CK   PA4
--CTS  PA0
--RTS  PA1
-USART3
--TX   PB10
--RX   PB11
--CK   PB12
--CTS  PB13
--RTS  PB14
*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** USART API's Definitions Start **********************************************/

// USART - Peripheral Clock Enable
void USART_PClk_init(USART_RegDef *pUSARTx, uint8_t mode)
{
	if(mode == ENABLE)
	{
		if(pUSARTx == USART1)
			USART1_PCLK_EN();
        else if (pUSARTx == USART2)
			USART2_PCLK_EN();
        else if (pUSARTx == USART3)
			USART3_PCLK_EN();
		else if (pUSARTx == UART4)
			UART4_PCLK_EN();
        else if (pUSARTx == UART5)
			UART5_PCLK_EN();
	}
	else
	{
		//To Do
	}

}

//Set Default USART Configuration
void USART_Config_Default(USART_Handle* pUSARTHandle)
{
    pUSARTHandle->USARTx_Config.mode           = USART_MODE_TX;
    pUSARTHandle->USARTx_Config.baudrate       = USART_BAUDRATE_9600;
    pUSARTHandle->USARTx_Config.clock_phase    = USART_CPHA_LOW;
    pUSARTHandle->USARTx_Config.clock_polarity = USART_CPOL_LOW;
    pUSARTHandle->USARTx_Config.word_length    = USART_WORD_8BIT;
    pUSARTHandle->USARTx_Config.parity_control = USART_PARITY_DISABLE;
    pUSARTHandle->USARTx_Config.stop_bits      = USART_STOPBIT_1;

    pUSARTHandle->pRXBuffer = 0;
    pUSARTHandle->pTXBuffer = 0;
    pUSARTHandle->TXState = USART_READY;
    pUSARTHandle->RXState = USART_READY;
}

//Set USART Configuration
void USART_Configure(USART_Handle* pUSARTHandle, uint8_t mode, uint32_t baudrate, uint8_t clk_phase,
                    uint8_t clk_polarity, uint8_t word_len, uint8_t parity_ctrl, uint8_t parity_type, uint8_t stop_bits)
{
    pUSARTHandle->USARTx_Config.mode           = mode;
    pUSARTHandle->USARTx_Config.baudrate       = baudrate;
    pUSARTHandle->USARTx_Config.clock_phase    = clk_phase;
    pUSARTHandle->USARTx_Config.clock_polarity = clk_polarity;
    pUSARTHandle->USARTx_Config.word_length    = word_len;
    pUSARTHandle->USARTx_Config.parity_control = parity_ctrl;
    pUSARTHandle->USARTx_Config.parity_type    = parity_type;
    pUSARTHandle->USARTx_Config.stop_bits      = stop_bits;

    pUSARTHandle->pRXBuffer = 0;
    pUSARTHandle->pTXBuffer = 0;
    pUSARTHandle->TXState = USART_READY;
    pUSARTHandle->RXState = USART_READY;
}

//USART Set Baudrate
void USART_SetBaudRate(USART_Handle* pUSARTHandle)
{
    uint16_t uartdiv = SYSCORE_CLK/(pUSARTHandle->USARTx_Config.baudrate);
    pUSARTHandle->pUSARTx->BRR |= ((uartdiv/16) << USART_BRR_DIV_MANTISSA) | ((uartdiv%16) << USART_BRR_DIV_FRACTION);
}                                                                                               

//USART Init
void USART_init(USART_Handle* pUSARTHandle, GPIO_Handle* pGPIOHandle,  USART_RegDef* pUSARTx)
{
    pUSARTHandle->pUSARTx = pUSARTx;
    pUSARTHandle->pGPIOHandle = pGPIOHandle;

    //USART2
    if(pUSARTHandle->pUSARTx == USART2)
    {
        pUSARTHandle->pGPIOHandle->pGPIOx = GPIOA;

        //Initialize GPIOA pins
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed = GPIO_OP_SPEED_10;
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinMode = GPIO_MODE_AF;
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinConfigType = GPIO_CONFIG_AF_OP_PP;

        //PA2
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinNo = 2;
        GPIO_Init(pUSARTHandle->pGPIOHandle);

        //PA3
        pUSARTHandle->pGPIOHandle->GPIOx_PinConfig.PinNo = 3;
        GPIO_Init(pUSARTHandle->pGPIOHandle);

        //uint8_t p[2] = {2, 3};
        //GPIO_Used_Update(GPIOA_BASE_ADDR, p, 2);
    }

    //1. Enable GPIOA Peripheral Clock
    GPIO_PClk_init(pUSARTHandle->pGPIOHandle->pGPIOx, ENABLE);

    //2. Enable USART2 Peripheral Clock
    USART_PClk_init(pUSARTHandle->pUSARTx, ENABLE);

    //3. Configure baudrate
    USART_SetBaudRate(pUSARTHandle);

    //CR1 Configuration
    uint32_t temp = 0;
    //4. Enable USART for the configured mode
    if(pUSARTHandle->USARTx_Config.mode == USART_MODE_TX)
        temp |= 1<< USART_CR1_TE;
    else if(pUSARTHandle->USARTx_Config.mode == USART_MODE_RX)
        temp |= 1<< USART_CR1_RE;
    else if(pUSARTHandle->USARTx_Config.mode == USART_MODE_TXRX)
        temp |= (1<< USART_CR1_TE) | (1<<USART_CR1_RE);

    //5. Configure Word Length
    temp |= pUSARTHandle->USARTx_Config.word_length<<USART_CR1_M;

    //6. Configure Parity
    if(pUSARTHandle->USARTx_Config.parity_control == USART_PARITY_ENABLE)
    {
        //Enable Parity
        temp |= 1<<USART_CR1_PCE;
        if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_ODD)
            temp |= 1<<USART_CR1_PS;
        //by default even parity will be selected when parity is enabled
    }

    //7. Enable USART2
    temp |= 1<< USART_CR1_UE;

    //load the configurated values into CR1
    pUSARTHandle->pUSARTx->CR1 = temp;


    //CR2 Configuration
    //8. Configure Stop Bits
    temp = 0;
    temp |= pUSARTHandle->USARTx_Config.stop_bits<<USART_CR2_STOP;
}

//USART Send Data (Blocking)
void USART_TX(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    pUSARTHandle->pTXBuffer = pbuffer;
    pUSARTHandle->TXLen = size;
    pUSARTHandle->TXState = USART_TX_BUSY;

    while(size--)
    {
        //wait till USART_SR->TXE becomes 1 to indicate that USART_DR is empty
        while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_TXE));

        if(pUSARTHandle->USARTx_Config.word_length == USART_WORD_9BIT)
        {
            uint16_t *pdata = (uint16_t*)pbuffer;
            pUSARTHandle->pUSARTx->DR = (*pdata && (uint16_t)0x01FF);

            if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_DISABLE)
                pbuffer += 2; //increment buffer address twice
            else
                pbuffer++;    //increment buffer address once
        }
        else //8 bit word length
        {
            pUSARTHandle->pUSARTx->DR = *pbuffer;
            pbuffer++;
        }
    }

    //wait till TC flag is set
    while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_TC));
}

//USART Receive Data (Blocking)
void USART_RX(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    pUSARTHandle->pRXBuffer = pbuffer;
    pUSARTHandle->RXLen = size;
    pUSARTHandle->RXState = USART_RX_BUSY;

    while(size--)
    {
        //wait till USART_SR->RXNE becomes 1 to indicate that USART_DR is not empty
        while(!(pUSARTHandle->pUSARTx->SR & 1<<USART_SR_RXNE));

        if(pUSARTHandle->USARTx_Config.word_length == USART_WORD_9BIT)
        {
            if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_DISABLE)
            {
                *((uint16_t*)pbuffer) = (pUSARTHandle->pUSARTx->DR & (uint16_t)0x01FF);
                pbuffer += 2;
            }
            else
            {
                *(pbuffer) = pUSARTHandle->pUSARTx->DR;
                pbuffer++;
            }
        }
        else //8 bit word length
        {
            *(pbuffer) = pUSARTHandle->pUSARTx->DR;
            pbuffer++;
        }
        
        pUSARTHandle->RXState = USART_READY;
    }
}

//USART Interrupt Configuration
void USART_IT_Config(USART_Handle* pUSARTHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pUSARTHandle->pUSARTx == USART1)
            nvic_intrpt_enable(USART1_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART2)
            nvic_intrpt_enable(USART2_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART3)
            nvic_intrpt_enable(USART3_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART4)
            nvic_intrpt_enable(UART4_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART5)
            nvic_intrpt_enable(UART5_IRQ_NO);
    }
    else if(mode == DISABLE)
    {
        if(pUSARTHandle->pUSARTx == USART1)
            nvic_intrpt_disable(USART1_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART2)
            nvic_intrpt_disable(USART2_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == USART3)
            nvic_intrpt_disable(USART3_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART4)
            nvic_intrpt_disable(UART4_IRQ_NO);
        else if(pUSARTHandle->pUSARTx == UART5)
            nvic_intrpt_disable(UART5_IRQ_NO);
    }
}

//USART Send Data through Interrupts (Non-Blocking)
uint8_t USART_TX_IT(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    //1. Get Current state of USART
    uint8_t TX_State = pUSARTHandle->TXState;

    //USART not busy in TX
    if(TX_State != USART_TX_BUSY)
    {
        //2. Get Data size and buffer address
        pUSARTHandle->TXLen = size;
        pUSARTHandle->pTXBuffer = pbuffer;

        //3. Set state as busy in TX
        pUSARTHandle->TXState = USART_TX_BUSY;

        //4. Enable TX Interrupt
        pUSARTHandle->pUSARTx->CR1 |= 1<<USART_CR1_TXEIE;

        //5. Enable TC Interrupt
        pUSARTHandle->pUSARTx->CR1 |= 1<<USART_CR1_TCIE;
    }

    return TX_State;
}

//USART Receive Data through Interrupts (Non-Blocking)
uint8_t USART_RX_IT(USART_Handle* pUSARTHandle, uint8_t* pbuffer, uint32_t size)
{
    //1. Get Current state of USART
    uint8_t RX_State = pUSARTHandle->RXState;

    //USART not busy in TX
    if(RX_State != USART_RX_BUSY)
    {
        //2. Get Data size and buffer address
        pUSARTHandle->RXLen = size;
        pUSARTHandle->pRXBuffer = pbuffer;

        //3. Set state as busy in TX
        pUSARTHandle->TXState = USART_RX_BUSY;

        //4. Enable TX Interrupt
        pUSARTHandle->pUSARTx->CR1 |= 1<<USART_CR1_RXNEIE;
    }

    return RX_State;
}

void USART_IRQ_Handler(USART_Handle* pUSARTHandle)
{
    //data buffer
    //uint8_t* pdata;

    /********* TC Flag *********/
    uint32_t temp1 = pUSARTHandle->pUSARTx->SR & (1<<USART_SR_TC);
    uint32_t temp2 = pUSARTHandle->pUSARTx->CR1 & (1<<USART_CR1_TCIE);

    //If TC and TCIE are set
    if(temp1 && temp2)
    {
        //1. Check if USART is busy in TX
        if(pUSARTHandle->TXState == USART_TX_BUSY)
        {
            //2. if data length is zero
            if(pUSARTHandle->TXLen == 0)
            {
                //clear TC flag
                pUSARTHandle->pUSARTx->SR &= ~(1<<USART_SR_TC);

                //disable TC Interrupt
                pUSARTHandle->pUSARTx->CR1 &= ~(1<<USART_CR1_TCIE);
                
                //Reset USART State
                pUSARTHandle->TXState = USART_READY;

                //Reset USART Buffer Address
                pUSARTHandle->pTXBuffer = 0;

                //Reset Data Length
                pUSARTHandle->TXLen = 0;
            }
        }
    }

    /********* TX Flag *********/
    temp1 = pUSARTHandle->pUSARTx->SR & (1<<USART_SR_TXE);
    temp2 = pUSARTHandle->pUSARTx->CR1 & (1<<USART_CR1_TXEIE);

    if(temp1 && temp2)
    {
        //1.Check state of USART
        if(pUSARTHandle->TXState == USART_TX_BUSY)
        {
            //2. Send Data until its length becomes 0
            if(pUSARTHandle->TXLen > 0)
            {
                if(pUSARTHandle->USARTx_Config.word_length == USART_WORD_9BIT)
                {
                    uint16_t *pdata = (uint16_t*)pUSARTHandle->pTXBuffer;
                    pUSARTHandle->pUSARTx->DR = (*pdata && (uint16_t)0x01FF);

                    if(pUSARTHandle->USARTx_Config.parity_type == USART_PARITY_DISABLE)
                    {
                        pUSARTHandle->pTXBuffer += 2; //increment buffer address twice
                        pUSARTHandle->TXLen -= 2;
                    }
                    else
                    {
                        pUSARTHandle->pTXBuffer++;    //increment buffer address once
                        pUSARTHandle->TXLen--;
                    }
                }
                else //8 bit word length
                {
                    pUSARTHandle->pUSARTx->DR = *pUSARTHandle->pTXBuffer;
                    pUSARTHandle->pTXBuffer++;
                    pUSARTHandle->TXLen--;
                }
            }

            //Disable interrupt for TXE when TX length becomes 0
            if(pUSARTHandle->TXLen == 0)
                pUSARTHandle->pUSARTx->CR1 &= ~(1<<USART_CR1_TXEIE);
        }
    }
}

/*********************************************** USART API's Definitions End ************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/