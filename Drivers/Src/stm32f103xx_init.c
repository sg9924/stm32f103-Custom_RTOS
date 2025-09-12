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
GPIO_Handle LED;

void board_init()
{
    Serial_init();              // Initialize USART2 for Serial Print (Debugging)

    display_cpu_info();
    RCC_init(&R, RCC_CLK_HSI);  // Initialize RCC with HSI Clock
    display_clk_src();
    display_clk_freqs();
    
}


void led_init()
{
    GPIO_Config(&LED, GPIOC, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, GPIO_PIN13, GPIO_OP_SPEED_2);
    GPIO_Init(&LED);
}


void led_toggle()
{
    GPIO_OpToggle(GPIOC, GPIO_PIN13);
}


void display_cpu_info()
{
    Serialprintln("Processor Info:", INFO);
    Serialprintln("Implementer: %x", INFO, GET_IMPLEMENTER());
    Serialprintln("Variant: %x", INFO, GET_VARIANT());
    Serialprintln("Part No: %x", INFO, GET_PARTNO());
    Serialprintln("Revision: %x", INFO, GET_REVISION());
}


void display_clk_src()
{
    if(RCC_Get_Clock_Source(&R) == RCC_CLK_HSE)       Serialprintln("Clock Source is HSE", INFO);
    else if(RCC_Get_Clock_Source(&R)  == RCC_CLK_HSI) Serialprintln("Clock Source is HSI", INFO);
    else if(RCC_Get_Clock_Source(&R)  == RCC_CLK_PLL) Serialprintln("Clock Source is PLL", INFO);
}


void display_clk_freqs()
{
    Serialprintln("HCLK is %d MHz", INFO, R.RCC_State.High_Clock/1000000);
    Serialprintln("PCLK1 is %d MHz", INFO, R.RCC_State.P_Clock_1/1000000);
    Serialprintln("PCLK2 is %d MHz", INFO, R.RCC_State.P_Clock_2/1000000);
}


void tim_delay_ms(uint16_t delay)
{
    //Configure Timer 2
    TIM_Base_Configure(&Delay, TIM2, TIM_COUNT_DIR_UP, (8000-1), delay, TIM_AR_PRELOAD_ENABLE);
    TIM_Count_Reset(&Delay); //reset count
    TIM_Base_init(&Delay); //initialize
    TIM_Base_Start(&Delay); //enable timer
    TIM_Update_Event_Check(&Delay); //wait for update event flag
    TIM_Base_Stop(&Delay); //disable timer
}


void tim_delay_us(uint16_t delay)
{
    //Configure Timer 2
    TIM_Base_Configure(&Delay, TIM2, TIM_COUNT_DIR_UP, (8-1), delay, TIM_AR_PRELOAD_ENABLE);
    TIM_Base_init(&Delay); //initialize
    TIM_Base_Start(&Delay); //enable timer
    TIM_Update_Event_Check(&Delay); //wait for update event flag
    TIM_Base_Stop(&Delay); //disable timer
}