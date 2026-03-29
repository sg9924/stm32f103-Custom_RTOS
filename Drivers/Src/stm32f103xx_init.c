#include"stm32f103xx_init.h"
#include"stm32f103xx.h"
#include"stm32f103xx_core.h"
#include"stm32f103xx_timer.h"
#include"stm32f103xx_systick.h"
#include"stm32f103xx_rcc.h"
#include"stm32f103xx_serial.h"
#include"stm32f103xx_config.h"

//USART2 for Serial Debugging
//TIM2 for delays

//RCC_Handle R;
TIM_Handle TIM2_Delay;
GPIO_Handle LED;
extern uint32_t sysclock;

void board_init()
{
    clock_init();

    #if BOARD_INIT_SERIAL == 1
    Serial_init(); //Initialize USART2 for Serial Print (Debugging)
    #endif

    #if BOARD_INIT_LED == 1
    led_init();    //Initialize built in LED at PC13
    led_off();     //switch off led
    #endif

    #if BOARD_INIT_TIM2 == 1
    tim2_init();
    #endif

    #if BOARD_INIT_SYSTICK == 1
    Systick_Configure(SYSTICK_CLK_SRC_AHB, SYSTICK_EXCEPTION_ENABLE);
    SYSTICK_ENABLE();
    #endif

    #if BOARD_INIT_DISPLAY_CPU_INFO == 1
    display_cpu_info();
    #endif

    #if BOARD_INIT_DISPLAY_CLK_INFO == 1
    display_clk_info();
    #endif
    
}


void led_init()
{
    GPIO_Config(&LED, GPIOC, GPIO_MODE_OP, GPIO_CONFIG_GP_OP_PP, GPIO_PIN13, GPIO_OP_SPEED_2);
    GPIO_Init(&LED);
}

void led_off()
{
    GPIO_Bit_Set(&LED, GPIO_PIN13);
    Serialprintln("User LED has been initialized", INFO);
}

void led_on()
{
    GPIO_Bit_Reset(&LED, GPIO_PIN13);
}

void led_toggle()
{
    GPIO_OpToggle(GPIOC, GPIO_PIN13);
}

void led_flash(uint8_t freq, uint16_t interval_ms)
{
    for(uint8_t i=0; i<freq ; i++)
    {
        led_toggle();
        Systick_delay(interval_ms);
    }
}


void clock_init()
{
    //Note: Changing APB1 prescalar will affect some USART Peripherals baud rate

    //72Mhz PLL HSE clock
    RCC_Config_PLL(PLL_CLK_SRC_HSE, PLL_MUL_9, PLL_HSE_DIV_NONE);
    RCC_init(RCC_CLK_HSE, SYSCLK_PLL, AHB_PRESCALAR_1, APB1_PRESCALAR_2, APB2_PRESCALAR_1);

    //8Mhz HSE clock
    //RCC_init(RCC_CLK_HSE, SYSCLK_HSE, AHB_PRESCALAR_1, APB1_PRESCALAR_1, APB2_PRESCALAR_1);
}


void display_cpu_info()
{
    SERIAL_NL();
    Serialprintln("Processor Info:", NONE);
    Serialprintln("Implementer: %x", INFO, GET_IMPLEMENTER());
    Serialprintln("Variant: %x", INFO, GET_VARIANT());
    Serialprintln("Part No: %x", INFO, GET_PARTNO());
    Serialprintln("Revision: %x", INFO, GET_REVISION());
}


void display_clk_info()
{
    SERIAL_NL();
    Serialprintln("Clock Info:", NONE);
    if(RCC_Get_Clock_Source() == RCC_CLK_HSE)       Serialprintln("Oscillator is HSE", INFO);
    else if(RCC_Get_Clock_Source()  == RCC_CLK_HSI) Serialprintln("Oscillator is HSI", INFO);

    if(RCC_Get_SYSCLK_Source() == SYSCLK_HSE)       Serialprintln("System Clock is HSE", INFO);
    else if(RCC_Get_SYSCLK_Source()  == SYSCLK_HSI) Serialprintln("System Clock is HSI", INFO);
    else if(RCC_Get_SYSCLK_Source()  == SYSCLK_PLL) Serialprintln("System Clock is PLL", INFO);

    Serialprintln("HCLK is %d MHz", INFO, RCC_Get_HCLK()/1000000);
    Serialprintln("PCLK1 is %d MHz", INFO, RCC_Get_PCLK1()/1000000);
    Serialprintln("PCLK2 is %d MHz", INFO, RCC_Get_PCLK2()/1000000);
}


void tim2_init()
{
    //Configure Timer 2
    //PSC: 1, ARR: max -> 1MHZ: 1us delay for 1 in ARR -> for 72MHz clock
    TIM_Base_Configure(&TIM2_Delay, TIM2, TIM_COUNT_DIR_UP, (sysclock/1000000) - 1, (0xFFFF), TIM_AR_PRELOAD_ENABLE);
    TIM_Base_init(&TIM2_Delay); //initialize
    TIM_Base_Start(&TIM2_Delay); //enable timer
}

void tim_delay_us(uint16_t delay)
{
    TIM2->CNT = 0;
    while(TIM2->CNT < delay);
}


void tim_delay_ms(uint16_t delay)
{
    for(uint16_t i=0; i<delay; i++)
    {
        tim_delay_us(1000);
    }
}