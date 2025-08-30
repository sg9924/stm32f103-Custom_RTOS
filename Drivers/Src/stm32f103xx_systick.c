#include "stm32f103xx_systick.h"
#include "stm32f103xx_serial.h"

uint32_t current_tick;


void Systick_Configure(uint8_t clk_src, uint8_t exception)
{
    //Disable Systick
    SYSTICK_DISABLE();
    //set clock source
    if(clk_src == SYSTICK_CLK_SRC_AHB_DIV_8)
        SYSTICK->CSR &= ~(1<<SYST_CSR_CLKSOURCE);
    else if(clk_src == SYSTICK_CLK_SRC_AHB)
        SYSTICK->CSR |= 1<<SYST_CSR_CLKSOURCE;

    //enable exception
    if(exception == SYSTICK_EXCEPTION_DISABLE)
        SYSTICK_DISABLE_INTERRUPT();
    else if(exception == SYSTICK_EXCEPTION_ENABLE)
        SYSTICK_ENABLE_INTERRUPT();

    //Load Reload Value
    SYSTICK_LOAD(SYSTICK_LOAD_VALUE_DEFAULT - 1);

    //Clear current value
    SYSTICK_CLEAR();
}


void Systick_init()
{
    SYSTICK_ENABLE();
}


uint32_t Systick_get_tick()
{
    return current_tick;
}

void Systick_Tick_Inc(void)
{
    current_tick += 1;
}


void Systick_delay(uint32_t delay_ms)
{
    uint32_t tick_start = Systick_get_tick();
    uint32_t wait = delay_ms;

    while((Systick_get_tick() - tick_start) < wait);
}


//Systick Interrupt Handler - gets called if the Systick Exception has been enabled
#if ENABLE_SYSTICK_HANDLER_DEFAULT == 1
void SysTick_Handler(void)
{
    SYSTICK_TICK_INC();
}
#endif