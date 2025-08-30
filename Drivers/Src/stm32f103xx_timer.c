#include"stm32f103xx_timer.h"


void TIM_PClk_init(TIM_Handle* pTIMHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pTIMHandle->pTIMx == TIM2)
            TIM2_PCLK_ENABLE();
        else if(pTIMHandle->pTIMx == TIM3)
            TIM3_PCLK_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pTIMHandle->pTIMx == TIM2)
            TIM2_PCLK_DISABLE();
        else if(pTIMHandle->pTIMx == TIM3)
            TIM3_PCLK_DISABLE();
    }
}


void TIM_P_init(TIM_Handle* pTIMHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pTIMHandle->pTIMx == TIM2)
            TIM2_ENABLE();
        else if(pTIMHandle->pTIMx == TIM3)
            TIM3_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pTIMHandle->pTIMx == TIM2)
            TIM2_DISABLE();
        else if(pTIMHandle->pTIMx == TIM3)
            TIM3_DISABLE();
    }
}


void TIM_Base_Configure(TIM_Handle* pTIMHandle, TIM_RegDef* pTIMx, uint8_t count_direction, uint32_t prescale_value, uint32_t autoreload_value, uint8_t ar_preload)
{
    pTIMHandle->pTIMx                                = pTIMx;
    pTIMHandle->TIMx_Base_Config.ar_preload          = ar_preload;
    pTIMHandle->TIMx_Base_Config.autoreload_value    = autoreload_value;
    pTIMHandle->TIMx_Base_Config.count_direction     = count_direction;
    pTIMHandle->TIMx_Base_Config.prescale_value      = prescale_value;
}

void TIM_Base_init(TIM_Handle* pTIMHandle)
{
    //enable Timer peripheral clock
    TIM_PClk_init(pTIMHandle, ENABLE);

    //configure count direction
    if(pTIMHandle->TIMx_Base_Config.count_direction == TIM_COUNT_DIR_DOWN)
        pTIMHandle->pTIMx->CR1 |= 1<<TIM_CR1_DIR;
    else if(pTIMHandle->TIMx_Base_Config.count_direction == TIM_COUNT_DIR_UP)
        pTIMHandle->pTIMx->CR1 &= ~(1<<TIM_CR1_DIR); //default direction
    
    //configure prescale value
    TIM_Prescaler_Load(pTIMHandle, pTIMHandle->TIMx_Base_Config.prescale_value);

    //configure auto reload value
    TIM_AutoReload_Load(pTIMHandle, pTIMHandle->TIMx_Base_Config.autoreload_value);

    //configure auto reload preload type
    if(pTIMHandle->TIMx_Base_Config.ar_preload == TIM_AR_PRELOAD_ENABLE)
        pTIMHandle->pTIMx->CR1 |= 1<<TIM_CR1_ARPE;
    else if(pTIMHandle->TIMx_Base_Config.ar_preload == TIM_AR_PRELOAD_DISABLE)
        pTIMHandle->pTIMx->CR1 &= ~(1<<TIM_CR1_ARPE);
}



void TIM_OC_Configure(TIM_Handle* pTIMHandle, TIM_OC_Handle* pTIMOCHandle, uint8_t oc_mode, uint8_t oc_channel, uint8_t oc_polarity, uint8_t oc_preload, uint16_t oc_value)
{
    pTIMOCHandle->pTIMHandle                      = pTIMHandle;
    pTIMOCHandle->pTIMHandle->pTIMx               = pTIMHandle->pTIMx;

    pTIMOCHandle->TIMx_OC_Config.oc_channel       = oc_channel;
    pTIMOCHandle->TIMx_OC_Config.oc_preload       = oc_preload;
    pTIMOCHandle->TIMx_OC_Config.oc_mode          = oc_mode;
    pTIMOCHandle->TIMx_OC_Config.oc_polarity      = oc_polarity;
    pTIMOCHandle->TIMx_OC_Config.oc_value         = oc_value;
}



void TIM_OC_Channel_init(TIM_OC_Handle* pTIMOCHandle)
{
    uint8_t channel         = pTIMOCHandle->TIMx_OC_Config.oc_channel;
    uint8_t mode            = pTIMOCHandle->TIMx_OC_Config.oc_mode;
    uint8_t polarity        = pTIMOCHandle->TIMx_OC_Config.oc_polarity;

    uint8_t word_offset     = channel/2;
    uint8_t ch_bit_offset   = (channel%2)*8;
    uint8_t mode_bit_offset = (channel%2)?4:12;
    uint8_t pol_bit_offset  = 1+(channel*4);

    //set channel as output
    pTIMOCHandle->pTIMHandle->pTIMx->CCMR[word_offset] |= ~(1<<ch_bit_offset);

    //set oc mode
    if(mode == TIM_OC_MODE_FROZEN)
        pTIMOCHandle->pTIMHandle->pTIMx->CCMR[word_offset] |= ~(1<<mode_bit_offset);
    else 
        pTIMOCHandle->pTIMHandle->pTIMx->CCMR[word_offset] |= 1<<mode_bit_offset;

    //set oc polarity
    if(polarity == TIM_OC_POL_ACTIVE_HIGH)
        pTIMOCHandle->pTIMHandle->pTIMx->CCER |= ~(1<<pol_bit_offset);
    else if(polarity == TIM_OC_POL_ACTIVE_LOW)
        pTIMOCHandle->pTIMHandle->pTIMx->CCER |= 1<<pol_bit_offset;

    //oc preload enable
    pTIMOCHandle->pTIMHandle->pTIMx->CCMR[word_offset] |= 1<<((channel%2)?3:11);

    //trigger update event to load registers for preload enable
    if(pTIMOCHandle->TIMx_OC_Config.oc_preload == TIM_OC_PRELOAD_ENABLE || pTIMOCHandle->pTIMHandle->TIMx_Base_Config.ar_preload == TIM_AR_PRELOAD_ENABLE)
        TIM_Update_Event_Trigger(pTIMOCHandle->pTIMHandle);
}


void TIM_OC_init(TIM_OC_Handle* pTIMOCHandle)
{
    //Base init
    TIM_Base_init(pTIMOCHandle->pTIMHandle);

    //Output Channel init
    TIM_OC_Channel_init(pTIMOCHandle);

    //Enable Interrupts if any
    TIM_Interrupt_init(pTIMOCHandle->pTIMHandle->pTIMx,
                       pTIMOCHandle->pTIMHandle->TIMx_Intrpt_Config.interrupt_type,
                       pTIMOCHandle->TIMx_OC_Config.oc_channel,
                       pTIMOCHandle->pTIMHandle->TIMx_Intrpt_Config.interrupt_mode);
}






void TIM_Base_Start(TIM_Handle* pTIMHandle)
{
    TIM_P_init(pTIMHandle, ENABLE);
}

void TIM_Base_Stop(TIM_Handle* pTIMHandle)
{
    TIM_P_init(pTIMHandle, DISABLE);
}

void TIM_OC_Start(TIM_OC_Handle* pTIMOCHandle, uint8_t channel)
{
    pTIMOCHandle->pTIMHandle->pTIMx->CCER |= 1<<(channel*4);
    TIM_Base_Start(pTIMOCHandle->pTIMHandle);
}

void TIM_OC_Stop(TIM_OC_Handle* pTIMOCHandle, uint8_t channel)
{
    pTIMOCHandle->pTIMHandle->pTIMx->CCER |= ~(1<<(channel*4));
    TIM_Base_Start(pTIMOCHandle->pTIMHandle);
}

void TIM_Prescaler_Load(TIM_Handle* pTIMHandle, uint32_t prescale_value)
{
    pTIMHandle->TIMx_Base_Config.prescale_value = prescale_value;
    pTIMHandle->pTIMx->PSC = prescale_value;
}

void TIM_AutoReload_Load(TIM_Handle* pTIMHandle, uint32_t auto_reload_value)
{
    pTIMHandle->TIMx_Base_Config.autoreload_value = auto_reload_value;
    pTIMHandle->pTIMx->ARR = auto_reload_value;
}

void TIM_Count_Reset(TIM_Handle* pTIMHandle)
{
    pTIMHandle->TIMx_Base_Config.count = 0;
    pTIMHandle->pTIMx->CNT = 0;
}

void TIM_Update_Event_Check(TIM_Handle* pTIMHandle)
{
    //wait till UIF becomes 1
    while(!(pTIMHandle->pTIMx->SR & 1<<TIM_SR_UIF));

    //clear UIF flag
    pTIMHandle->pTIMx->SR &= ~(1<<TIM_SR_UIF);
}

void TIM_Update_Event_Trigger(TIM_Handle* pTIMHandle)
{
    pTIMHandle->pTIMx->EGR |= 1<<TIM_EGR_UG;
    pTIMHandle->pTIMx->SR |= ~(1<<TIM_SR_UIF);
}

void TIM_Interrupt_init(TIM_RegDef* pTIMx, uint8_t interrupt_type, uint8_t channel, uint8_t mode)
{
    uint8_t bit_offset = (interrupt_type == TIM_UPDATE_INTRPT)?TIM_UPDATE_INTRPT:(channel + interrupt_type);
    if(mode == ENABLE)
    {
        //Enable interrupt in Timer
        pTIMx->DIER |= 1<<bit_offset;

        //Enable Interrupt in NVIC
        if(pTIMx == TIM2)
            nvic_intrpt_enable(TIM2_IRQ_NO);
        else if(pTIMx == TIM3)
            nvic_intrpt_enable(TIM3_IRQ_NO);
        else if(pTIMx == TIM4)
            nvic_intrpt_enable(TIM4_IRQ_NO);
    }
    else if(mode == DISABLE)
    {
        //Enable interrupt in Timer
        pTIMx->DIER |= ~(1<<bit_offset);

        //Enable Interrupt in NVIC
        if(pTIMx == TIM2)
            nvic_intrpt_disable(TIM2_IRQ_NO);
        else if(pTIMx == TIM3)
            nvic_intrpt_disable(TIM3_IRQ_NO);
        else if(pTIMx == TIM4)
            nvic_intrpt_disable(TIM4_IRQ_NO);
    }

}

void TIM_DMA_Init(TIM_RegDef* pTIMx, uint8_t dma_type, uint8_t channel, uint8_t mode)
{
    uint8_t bit_offset = (dma_type == TIM_TRIG_DMA)?TIM_TRIG_DMA:(channel + dma_type);
    if(mode == ENABLE)
        pTIMx->DIER |= 1<<bit_offset;
    else if(mode == DISABLE)
        pTIMx->DIER |= ~(1<<bit_offset);
}