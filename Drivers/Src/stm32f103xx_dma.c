#include"stm32f103xx_dma.h"


void DMA_PClk_init(DMA_Handle* pDMAHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
            DMA1_PCLK_ENABLE();
        else if(pDMAHandle->pDMAx == DMA2)
            DMA2_PCLK_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
            DMA1_PCLK_DISABLE();
        else if(pDMAHandle->pDMAx == DMA2)
            DMA2_PCLK_DISABLE();
    }
}


void DMA_P_init(DMA_Handle* pDMAHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
            DMA1_ENABLE(pDMAHandle->channel);
        else if(pDMAHandle->pDMAx == DMA2)
            DMA2_ENABLE(pDMAHandle->channel);
    }
    else if(mode == DISABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
            DMA1_DISABLE(pDMAHandle->channel);
        else if(pDMAHandle->pDMAx == DMA2)
            DMA2_DISABLE(pDMAHandle->channel);
    }
}

//DMA Config
void DMA_Configure(DMA_Handle* pDMAHandle, DMA_RegDef* pDMAx, uint8_t channel, uint8_t channel_priority, uint8_t direction, uint16_t no_of_data, uint8_t peri_size, uint8_t mem_size,
                   uint8_t peri_inc, uint8_t mem_inc, uint8_t mem_to_mem_mode, uint8_t circ_mode)
{
    pDMAHandle->pDMAx   = pDMAx;
    pDMAHandle->channel = channel;
    
    pDMAHandle->DMAx_Config.channel_priority = channel_priority;
    pDMAHandle->DMAx_Config.direction        = direction;
    pDMAHandle->DMAx_Config.no_of_data       = no_of_data;
    pDMAHandle->DMAx_Config.peripheral_size  = peri_size;
    pDMAHandle->DMAx_Config.memory_size      = mem_size;
    pDMAHandle->DMAx_Config.peripheral_inc   = peri_inc;
    pDMAHandle->DMAx_Config.memory_inc       = mem_inc;
    pDMAHandle->DMAx_Config.mem_to_mem_mode  = mem_to_mem_mode;
    pDMAHandle->DMAx_Config.circular_mode    = circ_mode;
}


//DMA Address Init
void DMA_Address_Config(DMA_Handle* pDMAHandle, uint32_t peri_addr, uint32_t mem_addr)
{
    pDMAHandle->memory_addr_1 = mem_addr;

    if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_ENABLE)
        pDMAHandle->memory_addr_2 = peri_addr;
    else if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_DISABLE)
        pDMAHandle->peripheral_addr = peri_addr;
}


//DMA Initialization
void DMA_init(DMA_Handle* pDMAHandle)
{
    DMA_PClk_init(pDMAHandle, ENABLE);  //enable DMA peripheral clock
    DMA_P_init(pDMAHandle, DISABLE);    //disable DMA peripheral for the specified channel
    
    //get channel
    uint8_t chnl = pDMAHandle->channel;

    //set direction
    if(pDMAHandle->DMAx_Config.direction == DMA_DIR_FROM_MEMORY)
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_DIR;
    else if(pDMAHandle->DMAx_Config.direction == DMA_DIR_FROM_PERIPHERAL)
        pDMAHandle->pDMAx->Channel[chnl].CCR &= ~(1<<DMA_CCRx_DIR);

    //Mem Inc 
    if(pDMAHandle->DMAx_Config.memory_inc == DMA_MEM_INC_ENABLE)
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_MINC;

    //Peri Inc
    if(pDMAHandle->DMAx_Config.peripheral_inc == DMA_PERI_INC_ENABLE)
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_PINC;
        

    //Circular Mode
    if(pDMAHandle->DMAx_Config.circular_mode == DMA_CIRC_MODE_ENABLE)
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_CIRC;
    
    //Peripheral Size
    pDMAHandle->pDMAx->Channel[chnl].CCR |= (pDMAHandle->DMAx_Config.peripheral_size)<<DMA_CCRx_PSIZE0;

    //Memory Size
    pDMAHandle->pDMAx->Channel[chnl].CCR |= (pDMAHandle->DMAx_Config.memory_size)<<DMA_CCRx_MSIZE0;

    //No of Data Items
    pDMAHandle->pDMAx->Channel[chnl].CNDTR = pDMAHandle->DMAx_Config.no_of_data;

    if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_ENABLE)
    {
        pDMAHandle->pDMAx->Channel[chnl].CCR |= 1<<DMA_CCRx_MEM2MEM;
        pDMAHandle->pDMAx->Channel[chnl].CPAR = pDMAHandle->memory_addr_1;
        pDMAHandle->pDMAx->Channel[chnl].CMAR = pDMAHandle->memory_addr_2;
    }
    else if(pDMAHandle->DMAx_Config.mem_to_mem_mode == DMA_MEM_TO_MEM_DISABLE)
    {
        pDMAHandle->pDMAx->Channel[chnl].CPAR = pDMAHandle->peripheral_addr;
        pDMAHandle->pDMAx->Channel[chnl].CMAR = pDMAHandle->memory_addr_1;
    }
}


//DMA Interrupt Configure
void DMA_IT_Config(DMA_Handle* pDMAHandle, uint8_t interrupt, uint8_t mode)
{
    DMA_PClk_init(pDMAHandle, ENABLE);  //enable DMA peripheral clock
    
    uint8_t chnl = pDMAHandle->channel;

    if(mode == ENABLE)
    {
        if(interrupt == DMA_INTRPT_TC_ENABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR |= DMA_INTRPT_TC_ENABLE;
        else if(interrupt == DMA_INTRPT_HT_ENABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR |= DMA_INTRPT_HT_ENABLE;
        else if(interrupt == DMA_INTRPT_TE_ENABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR |= DMA_INTRPT_TE_ENABLE;
    }
    else if(mode == DISABLE)
    {
        if(interrupt == DMA_INTRPT_TC_DISABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR &= DMA_INTRPT_TC_DISABLE;
        else if(interrupt == DMA_INTRPT_HT_DISABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR &= DMA_INTRPT_HT_DISABLE;
        else if(interrupt == DMA_INTRPT_TE_DISABLE)
            pDMAHandle->pDMAx->Channel[chnl].CCR &= DMA_INTRPT_TE_DISABLE;
    }
}


void DMA_IRQ_Config(DMA_Handle* pDMAHandle, uint8_t mode, uint8_t channel)
{
    if(mode == ENABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
        {
            if(channel == DMA_CHANNEL_1)
                nvic_intrpt_enable(DMA1_Channel1_IRQ_NO);
            else if(channel == DMA_CHANNEL_2)
                nvic_intrpt_enable(DMA1_Channel2_IRQ_NO);
            else if(channel == DMA_CHANNEL_3)
                nvic_intrpt_enable(DMA1_Channel3_IRQ_NO);
            else if(channel == DMA_CHANNEL_4)
                nvic_intrpt_enable(DMA1_Channel4_IRQ_NO);
            else if(channel == DMA_CHANNEL_5)
                nvic_intrpt_enable(DMA1_Channel5_IRQ_NO);
            else if(channel == DMA_CHANNEL_6)
                nvic_intrpt_enable(DMA1_Channel6_IRQ_NO);
            else if(channel == DMA_CHANNEL_7)
                nvic_intrpt_enable(DMA1_Channel7_IRQ_NO);
        }
        else if(pDMAHandle->pDMAx == DMA2)
        {
            if(channel == DMA_CHANNEL_1)
                nvic_intrpt_enable(DMA2_Channel1_IRQ_NO);
            else if(channel == DMA_CHANNEL_2)
                nvic_intrpt_enable(DMA2_Channel2_IRQ_NO);
            else if(channel == DMA_CHANNEL_3)
                nvic_intrpt_enable(DMA2_Channel3_IRQ_NO);
            else if(channel == DMA_CHANNEL_4 || channel == DMA_CHANNEL_5)
                nvic_intrpt_enable(DMA2_Channel4_5_IRQ_NO);
        }
    }
    else  if(mode == DISABLE)
    {
        if(pDMAHandle->pDMAx == DMA1)
        {
            if(channel == DMA_CHANNEL_1)
                nvic_intrpt_disable(DMA1_Channel1_IRQ_NO);
            else if(channel == DMA_CHANNEL_2)
                nvic_intrpt_disable(DMA1_Channel2_IRQ_NO);
            else if(channel == DMA_CHANNEL_3)
                nvic_intrpt_disable(DMA1_Channel3_IRQ_NO);
            else if(channel == DMA_CHANNEL_4)
                nvic_intrpt_disable(DMA1_Channel4_IRQ_NO);
            else if(channel == DMA_CHANNEL_5)
                nvic_intrpt_disable(DMA1_Channel5_IRQ_NO);
            else if(channel == DMA_CHANNEL_6)
                nvic_intrpt_disable(DMA1_Channel6_IRQ_NO);
            else if(channel == DMA_CHANNEL_7)
                nvic_intrpt_disable(DMA1_Channel7_IRQ_NO);
        }
        else if(pDMAHandle->pDMAx == DMA2)
        {
            if(channel == DMA_CHANNEL_1)
                nvic_intrpt_disable(DMA2_Channel1_IRQ_NO);
            else if(channel == DMA_CHANNEL_2)
                nvic_intrpt_disable(DMA2_Channel2_IRQ_NO);
            else if(channel == DMA_CHANNEL_3)
                nvic_intrpt_disable(DMA2_Channel3_IRQ_NO);
            else if(channel == DMA_CHANNEL_4 || channel == DMA_CHANNEL_5)
                nvic_intrpt_disable(DMA2_Channel4_5_IRQ_NO);
        }
    }
}