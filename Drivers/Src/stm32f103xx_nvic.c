#include"stm32f103xx_nvic.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** NVIC API's Definitions Start ***********************************************/

//Get IRQ Number for GPIO Pins
uint8_t nvic_irq_gpio_port_map(GPIO_Handle* pGPIOHandle)
{
    if(pGPIOHandle->GPIOx_PinConfig.PinNo<=4)
        return (pGPIOHandle->GPIOx_PinConfig.PinNo+6);
    else if(pGPIOHandle->GPIOx_PinConfig.PinNo>4 && pGPIOHandle->GPIOx_PinConfig.PinNo<=9)
        return 23;
    else if(pGPIOHandle->GPIOx_PinConfig.PinNo>9 && pGPIOHandle->GPIOx_PinConfig.PinNo<=15)
        return 40;
    else 
        return 0;
}

// NVIC Interrupt Enable
void nvic_intrpt_enable(uint16_t irq_no)
{    
    if(irq_no<=31)
    {
        *NVIC_ISER0 |= (1<<irq_no);
    }
    else if(irq_no<=63)
    {
        *NVIC_ISER1 |= (1<<irq_no%32);
    }
    else if(irq_no<=95)
    {
        *NVIC_ISER2 |= (1<<irq_no%64);
    }
    else if(irq_no<=127)
    {
        *NVIC_ISER3 |= (1<<irq_no%96);
    }
    else if(irq_no<=159)
    {
        *NVIC_ISER4 |= (1<<irq_no%128);
    }
    else if(irq_no<=191)
    {
        *NVIC_ISER5 |= (1<<irq_no%160);
    }
    else if(irq_no<=223)
    {
        *NVIC_ISER6 |= (1<<irq_no%192);
    }
    else if(irq_no<=255)
    {
        *NVIC_ISER7 |= (1<<irq_no%224);
    }
}


// NVIC Interrupt Disable
void nvic_intrpt_disable(uint16_t irq_no)
{
    if(irq_no<=31)
    {
        *NVIC_ICER0 |= (1<<irq_no);
    }
    else if(irq_no<=63)
    {
        *NVIC_ICER1 |= (1<<irq_no%32);
    }
    else if(irq_no<=95)
    {
        *NVIC_ICER2 |= (1<<irq_no%64);
    }
    else if(irq_no<=127)
    {
        *NVIC_ICER3 |= (1<<irq_no%96);
    }
    else if(irq_no<=159)
    {
        *NVIC_ICER4 |= (1<<irq_no%128);
    }
    else if(irq_no<=191)
    {
        *NVIC_ICER5 |= (1<<irq_no%160);
    }
    else if(irq_no<=223)
    {
        *NVIC_ICER6 |= (1<<irq_no%192);
    }
    else if(irq_no<=255)
    {
        *NVIC_ICER7 |= (1<<irq_no%224);
    }
}

/*********************************************** NVIC API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/