#include "stm32f103xx_init.h"
#include "stm32f103xx.h"
#include "stm32f103xx_core.h"
#include "stm32f103xx_timer.h"
#include "stm32f103xx_rcc.h"
#include "stm32f103xx_serial.h"

//USART2 for Serial Debugging
//TIM2 for delays

RCC_Handle R;
TIM_Handle Delay;

void board_init()
{
    Serial_init();              // Initialize USART2 for Serial Print (Debugging)

    display_cpu_info();
    RCC_init(&R, RCC_CLK_HSI);  // Initialize RCC with HSI Clock
    display_clk_src();
    display_clk_freqs();
    
}


void display_cpu_info()
{
    Serialprint("\r\nProcessor Info:\r\n");
    Serialprint("Implementer: %x\r\n", GET_IMPLEMENTER());
    Serialprint("Variant: %x\r\n", GET_VARIANT());
    Serialprint("Part No: %x\r\n", GET_PARTNO());
    Serialprint("Revision: %x\r\n", GET_REVISION());
}


void display_clk_src()
{
    if(RCC_Get_Clock_Source(&R) == RCC_CLK_HSE)
        Serialprint("\r\n[INFO]: Clock Source is HSE\r\n");
    else if(RCC_Get_Clock_Source(&R)  == RCC_CLK_HSI)
        Serialprint("\r\n[INFO]: Clock Source is HSI\r\n");
    else if(RCC_Get_Clock_Source(&R)  == RCC_CLK_PLL)
        Serialprint("\r\n[INFO]: Clock Source is PLL\r\n");
}


void display_clk_freqs()
{
    Serialprint("[INFO]: HCLK is %d MHz\r\n", R.RCC_State.High_Clock/1000000);
    Serialprint("[INFO]: PCLK1 is %d MHz\r\n", R.RCC_State.P_Clock_1/1000000);
    Serialprint("[INFO]: PCLK2 is %d MHz\r\n", R.RCC_State.P_Clock_2/1000000);
}


void tim_delay_ms(uint16_t delay)
{
    //Configure Timer 2
    TIM_Base_Configure(&Delay, TIM2, TIM_COUNT_DIR_UP, (8000-1), delay, TIM_AR_PRELOAD_DISABLE);
    TIM_Base_init(&Delay); //initialize
    TIM_Base_Start(&Delay); //enable timer
    TIM_Update_Event_Check(&Delay); //wait for update event flag
    TIM_Base_Stop(&Delay); //disable timer
}


void tim_delay_us(uint16_t delay)
{
    //Configure Timer 2
    TIM_Base_Configure(&Delay, TIM2, TIM_COUNT_DIR_UP, (8-1), delay, TIM_AR_PRELOAD_DISABLE);
    TIM_Base_init(&Delay); //initialize
    TIM_Base_Start(&Delay); //enable timer
    TIM_Update_Event_Check(&Delay); //wait for update event flag
    TIM_Base_Stop(&Delay); //disable timer
}