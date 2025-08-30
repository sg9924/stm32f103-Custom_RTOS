#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_rcc.h"
#include"stm32f103xx_serial.h"


/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ RCC API's Definitions Start ***********************************************/

uint8_t RCC_Get_Clock_Source(RCC_Handle* pRCCHandle)
{
    return pRCCHandle->RCC_State.Clock_Source;
}

uint32_t RCC_Get_HCLK(RCC_Handle* pRCCHandle)
{
    return pRCCHandle->RCC_State.High_Clock;
}

uint32_t RCC_Get_PCLK1(RCC_Handle* pRCCHandle)
{
    return pRCCHandle->RCC_State.P_Clock_1;
}

uint32_t RCC_Get_PCLK2(RCC_Handle* pRCCHandle)
{
    return pRCCHandle->RCC_State.P_Clock_2;
}



uint8_t RCC_Update_Clock_Source(RCC_Handle* pRCCHandle)
{
    if(pRCCHandle->pRCC->CR & 1<< RCC_CR_HSEON)
        pRCCHandle->RCC_State.Clock_Source = RCC_CLK_HSE;
    else if(pRCCHandle->pRCC->CR & 1<< RCC_CR_HSION)
        pRCCHandle->RCC_State.Clock_Source = RCC_CLK_HSI;
    else if(pRCCHandle->pRCC->CR & 1<< RCC_CR_PLLON)
        pRCCHandle->RCC_State.Clock_Source = RCC_CLK_PLL;

    return pRCCHandle->RCC_State.Clock_Source;
}


uint32_t RCC_Update_HCLK(RCC_Handle* pRCCHandle)
{
    uint8_t temp = (pRCCHandle->pRCC->CFGR & 0xF<<RCC_CFGR_HPRE);

    if(pRCCHandle->pRCC->CFGR >> RCC_CFGR_HPRE != 1)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock;
    else if (temp == 8)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/2;
    else if (temp == 9)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/4;
    else if (temp == 10)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/8;
    else if (temp == 11)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/16;
    else if (temp == 12)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/64;
    else if (temp == 13)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/128;
    else if (temp == 14)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/256;
    else if (temp == 15)
        pRCCHandle->RCC_State.High_Clock = pRCCHandle->RCC_State.System_Clock/512;

    return pRCCHandle->RCC_State.High_Clock;
}


uint32_t RCC_Update_PCLK1(RCC_Handle* pRCCHandle)
{
    uint8_t temp = (pRCCHandle->pRCC->CFGR & 0x7<<RCC_CFGR_PPRE1);
    if(pRCCHandle->pRCC->CFGR >> RCC_CFGR_PPRE1 != 1)
        pRCCHandle->RCC_State.P_Clock_1 = pRCCHandle->RCC_State.System_Clock;
    else if (temp == 4)
        pRCCHandle->RCC_State.P_Clock_1 = pRCCHandle->RCC_State.High_Clock/2;
    else if (temp == 5)
        pRCCHandle->RCC_State.P_Clock_1 = pRCCHandle->RCC_State.High_Clock/4;
    else if (temp == 6)
        pRCCHandle->RCC_State.P_Clock_1 = pRCCHandle->RCC_State.High_Clock/8;
    else if (temp == 7)
        pRCCHandle->RCC_State.P_Clock_1 = pRCCHandle->RCC_State.High_Clock/16;

    return pRCCHandle->RCC_State.P_Clock_1;
}


uint32_t RCC_Update_PCLK2(RCC_Handle* pRCCHandle)
{
    uint8_t temp = (pRCCHandle->pRCC->CFGR & 0x7<<RCC_CFGR_PPRE2);
    if(pRCCHandle->pRCC->CFGR >> RCC_CFGR_PPRE2 != 1)
        pRCCHandle->RCC_State.P_Clock_2 = pRCCHandle->RCC_State.System_Clock;
    else if (temp == 4)
        pRCCHandle->RCC_State.P_Clock_2 = pRCCHandle->RCC_State.High_Clock/2;
    else if (temp == 5)
        pRCCHandle->RCC_State.P_Clock_2 = pRCCHandle->RCC_State.High_Clock/4;
    else if (temp == 6)
        pRCCHandle->RCC_State.P_Clock_2 = pRCCHandle->RCC_State.High_Clock/8;
    else if (temp == 7)
        pRCCHandle->RCC_State.P_Clock_2 = pRCCHandle->RCC_State.High_Clock/16;

    return pRCCHandle->RCC_State.P_Clock_2;
}



void RCC_Select_Clock_Source(RCC_Handle* pRCCHandle)
{
    if(pRCCHandle->RCC_State.Clock_Source == RCC_CLK_HSI)
    {
        pRCCHandle->pRCC->CR |= 1<<RCC_CR_HSION;
        while(!(pRCCHandle->pRCC->CR & 1<<RCC_CR_HSIRDY));
        pRCCHandle->RCC_State.System_Clock = 8000000;
    }
    else if(pRCCHandle->RCC_State.Clock_Source == RCC_CLK_HSE)
    {
        pRCCHandle->pRCC->CR |= 1<<RCC_CR_HSEON;
        while(!(pRCCHandle->pRCC->CR & 1<<RCC_CR_HSERDY));
        pRCCHandle->RCC_State.System_Clock = 8000000;
    }
    else if(pRCCHandle->RCC_State.Clock_Source == RCC_CLK_PLL)
    {
        pRCCHandle->pRCC->CFGR &= ~(0xF<<RCC_CFGR_PLLMUL); // PLL Input clock multiplied by 2
        pRCCHandle->pRCC->CFGR &= ~(1<<RCC_CFGR_PLLSRC);   // PLL Source - HSI divided by 2
        pRCCHandle->pRCC->CR |= 1<<RCC_CR_PLLON;
        while(!(pRCCHandle->pRCC->CR & 1<<RCC_CR_PLLRDY));
        pRCCHandle->RCC_State.System_Clock = 8000000;
    }
}



void RCC_init(RCC_Handle* pRCCHandle, uint8_t clk_src)
{
    pRCCHandle->pRCC = RCC;
    pRCCHandle->RCC_State.Clock_Source = clk_src;

    //1. Configure the clock source
    RCC_Select_Clock_Source(pRCCHandle);

    //2. Update the system clock
    RCC_Update_Clock_Source(pRCCHandle);

    //3. Update the HCLK
    RCC_Update_HCLK(pRCCHandle);

    //4. Update PCLK1
    RCC_Update_PCLK1(pRCCHandle);

    //5. RCC Update PCLK2
    RCC_Update_PCLK2(pRCCHandle);
}



void RCC_MCO_Config(RCC_Handle* pRCC_Handle, uint8_t mco_clk_src)
{
    pRCC_Handle->pRCC->CFGR |= mco_clk_src<<RCC_CFGR_MCO;
    pRCC_Handle->RCC_State.MCO_Clock_Source = mco_clk_src;

    //GPIO MCO Alternate Function Config - PA8
    GPIO_Handle G_MCO;
    GPIO_Config(&G_MCO, GPIOA, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_PP, GPIO_PIN8, GPIO_OP_SPEED_2);
    GPIO_Init(&G_MCO);
}


/************************************************ RCC API's Definitions End *************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/