#include"stm32f103xx_adc.h"


static void ADC_GPIO_Config(ADC_Handle* pADCHandle)
{
    if(pADCHandle->pGPIOHandle->pGPIOx == GPIOA && pADCHandle->ADCx_Config.channel <= ADC_CHANNEL_7)
    {
        GPIO_Config(pADCHandle->pGPIOHandle, pADCHandle->pGPIOHandle->pGPIOx, GPIO_MODE_IP, GPIO_CONFIG_ANALOG, pADCHandle->ADCx_Config.channel, GPIO_OP_SPEED_0);
        GPIO_Init(pADCHandle->pGPIOHandle);
    }
    else if(pADCHandle->pGPIOHandle->pGPIOx == GPIOB && pADCHandle->ADCx_Config.channel > ADC_CHANNEL_7)
    {
        GPIO_Config(pADCHandle->pGPIOHandle, pADCHandle->pGPIOHandle->pGPIOx, GPIO_MODE_IP, GPIO_CONFIG_ANALOG, pADCHandle->ADCx_Config.channel, GPIO_OP_SPEED_0);
        GPIO_Init(pADCHandle->pGPIOHandle);
    }
}



void ADC_PClk_init(ADC_Handle* pADCHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pADCHandle->pADCx == ADC1)
            ADC1_PCLK_EN();
        else if(pADCHandle->pADCx == ADC2)
            ADC3_PCLK_EN();
        else if(pADCHandle->pADCx == ADC3)
            ADC3_PCLK_EN();
    }
    else if(mode == DISABLE)
    {
        if(pADCHandle->pADCx == ADC1)
            ADC1_PCLK_DISABLE();
        else if(pADCHandle->pADCx == ADC2)
            ADC1_PCLK_DISABLE();
        else if(pADCHandle->pADCx == ADC3)
            ADC1_PCLK_DISABLE();
    }
}



void ADC_Configure(ADC_Handle* pADCHandle, ADC_RegDef* ADCx, uint8_t mode, uint8_t scan, uint8_t event, uint8_t dma_mode)
{
    pADCHandle->pADCx                        = ADCx;
    pADCHandle->ADCx_Config.mode             = mode;
    pADCHandle->ADCx_Config.scan             = scan;
    pADCHandle->ADCx_Config.external_event   = event;
    pADCHandle->ADCx_Config.dma_mode         = dma_mode;
}


void ADC_Sequence_Configure(ADC_Seq_Handle* pADCSeqHandle, ADC_Handle* pADCHandle, uint8_t seq_type, uint8_t no_of_conv, uint8_t no_of_chnls, uint8_t* channel_list)
{
    pADCSeqHandle->pADCHandle                    = pADCHandle;
    pADCSeqHandle->ADCx_Seq_Config.seq_type      = seq_type;
    pADCSeqHandle->ADCx_Seq_Config.no_of_conv    = no_of_conv;
    pADCSeqHandle->ADCx_Seq_Config.no_of_chnls   = no_of_chnls;
    pADCSeqHandle->ADCx_Seq_Config.channel_list  = channel_list;
}




void ADC_init(ADC_Handle* pADCHandle, GPIO_Handle* pGPIOHandle, GPIO_RegDef* GPIOx)
{
    pADCHandle->pGPIOHandle             = pGPIOHandle;
    pADCHandle->pGPIOHandle->pGPIOx     = GPIOx;

    //Configure GPIO for ADC
    ADC_GPIO_Config(pADCHandle);

    //Enable ADC Peripheral Clock
    ADC_PClk_init(pADCHandle, ENABLE);

    //ADC Modes
    if(pADCHandle->ADCx_Config.mode == ADC_MODE_SINGLE)
        pADCHandle->pADCx->CR2 &= ~(1<<ADC_CR2_CONT);
    else if(pADCHandle->ADCx_Config.mode == ADC_MODE_CONT)
        pADCHandle->pADCx->CR2 |= 1<<ADC_CR2_CONT;
    
    if(pADCHandle->ADCx_Config.scan == ADC_SCAN_ENABLE)
        pADCHandle->pADCx->CR1 |= 1<<ADC_CR1_SCAN;

    //ADC DMA
    if(pADCHandle->ADCx_Config.dma_mode == ADC_DMA_ENABLE)
        pADCHandle->pADCx->CR2 |= 1<<ADC_CR2_DMA;
    else if(pADCHandle->ADCx_Config.dma_mode == ADC_DMA_DISABLE)
        pADCHandle->pADCx->CR2 &= ~(1<<ADC_CR2_DMA);

    //Configure the Event type
    if(pADCHandle->ADCx_Config.external_event == ADC_EVENT_SWSTART)
        pADCHandle->pADCx->CR2 |= ADC_EVENT_SWSTART << ADC_CR2_EXTSEL0;
}


void ADC_Channel_init(ADC_Handle* pADCHandle, uint8_t channel)
{
    pADCHandle->ADCx_Config.channel = channel;
    //Set Channel Number in Regular Sequence
    ADC_Reg_Seq_Config(pADCHandle->pADCx,pADCHandle->ADCx_Config.channel, ADC_SEQ_NO_1);
}




void ADC_Sequence_init(ADC_Seq_Handle* pADCSeqHandle, GPIO_Handle* pGPIOHandle, GPIO_RegDef* GPIOx)
{
    uint8_t no_of_chnls = pADCSeqHandle->ADCx_Seq_Config.no_of_chnls;

    //initialize ADC
    ADC_init(pADCSeqHandle->pADCHandle, pGPIOHandle, GPIOx);

    //set no of conversions
    ADC_No_Of_Conv(pADCSeqHandle->pADCHandle->pADCx, pADCSeqHandle->ADCx_Seq_Config.no_of_conv);

    //set channel numbers in sequence registers according to the order of the channels
    while(--no_of_chnls >= 0)
    {
        ADC_Reg_Seq_Config(pADCSeqHandle->pADCHandle->pADCx, pADCSeqHandle->ADCx_Seq_Config.channel_list[no_of_chnls], (no_of_chnls+1));
    }
}


void ADC_IT_Config()
{
    
}


void ADC_Reg_Seq_Config(ADC_RegDef* pADCx, uint8_t channel, uint8_t seq_no)
{
    uint8_t SQR_Reg_bit_offset = (((seq_no%16)-1)*5)%30;

    if(seq_no>=1 && seq_no<=6)
    {
        if(channel == ADC_CHANNEL_0)
            pADCx->SQR3 &= ~(0x1F << SQR_Reg_bit_offset);
        else if(channel >= ADC_CHANNEL_1)
            pADCx->SQR3 |= (channel) << SQR_Reg_bit_offset;
    }
    else if(seq_no>=7 && seq_no<=12)
    {
        if(channel == ADC_CHANNEL_0)
            pADCx->SQR2 &= ~(0x1F  << SQR_Reg_bit_offset);
        else if(channel >= ADC_CHANNEL_1)
            pADCx->SQR2 |= (channel) << SQR_Reg_bit_offset;
    }
    else if(seq_no>=13 && seq_no<=16)
    {
        if(channel == ADC_CHANNEL_0)
            pADCx->SQR1 &= ~(0x1F  << SQR_Reg_bit_offset);
        else if(channel >= ADC_CHANNEL_1)
            pADCx->SQR1 |= (channel) << SQR_Reg_bit_offset;
    }
}




void ADC_No_Of_Conv(ADC_RegDef* pADCx, uint8_t no_of_conversions)
{
    if(no_of_conversions != 1)
        pADCx->SQR1 |= (no_of_conversions - 1) << ADC_SQR1_L;
    else if(no_of_conversions == 1)
        pADCx->SQR1 &= ~(1<<ADC_SQR1_L);
}



static void _ADC_Start()
{
    //Enable ADC to wake from sleep
    ADC1_ENABLE();
    //!!delay should be provided here!!
    //Enable ADC after Wake
    ADC1_ENABLE();
}

uint16_t ADC_Start(ADC_Handle* pADCHandle)
{
    uint16_t adc_data = 0;

    _ADC_Start();

    //Wait till ADC has completed conversion (EOC becomes 1)
    while(!(pADCHandle->pADCx->SR & 1<<ADC_SR_EOC));

    //Extract ADC Data (lower 16 bits only)
    adc_data = pADCHandle->pADCx->DR & 0xFFFF;
    
    return adc_data;
}


//needs to be expanded to include DMA here
void ADC_DMA_Start()
{
    _ADC_Start();
}