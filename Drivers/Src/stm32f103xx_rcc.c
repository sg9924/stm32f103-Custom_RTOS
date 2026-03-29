#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_rcc.h"
#include"stm32f103xx_flash.h"


uint32_t sysclock = RCC_SYSCLK_FREQ_DEFAULT;
RCC_Handle RCCHandle;


/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ RCC API's Definitions Start ***********************************************/
//Helper
static void RCC_Update_Clock_Source();
static void RCC_Update_HCLK();
static void RCC_Update_PCLK1();
static void RCC_Update_PCLK2();
static void RCC_Update_SYSCLK();
static uint16_t RCC_Get_AHB_Prescalar();
static uint8_t RCC_Get_APB1_Prescalar();
static uint8_t RCC_Get_APB2_Prescalar();


static void RCC_Update_Clock_Source()
{
    if(RCC->CR & 1<< RCC_CR_HSEON)
        RCCHandle.Clock_Source = RCC_CLK_HSE;
    else if(RCC->CR & 1<< RCC_CR_HSION)
        RCCHandle.Clock_Source = RCC_CLK_HSI;
    return;
}

static void RCC_Update_HCLK()
{
    uint8_t temp = (RCC->CFGR & 0xF<<RCC_CFGR_HPRE)>>RCC_CFGR_HPRE;

    if(temp<8 && temp>=0)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock;
    else if(temp < 8)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock;
    else if (temp == 8)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/2;
    else if (temp == 9)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/4;
    else if (temp == 10)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/8;
    else if (temp == 11)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/16;
    else if (temp == 12)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/64;
    else if (temp == 13)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/128;
    else if (temp == 14)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/256;
    else if (temp == 15)
        RCCHandle.RCC_Clocks.High_Clock = RCCHandle.RCC_Clocks.System_Clock/512;
    return;
}

static void RCC_Update_PCLK1()
{
    uint8_t temp = (RCC->CFGR & 0x7<<RCC_CFGR_PPRE1)>>RCC_CFGR_PPRE1;
    if(temp < 4)
        RCCHandle.RCC_Clocks.P_Clock_1 = RCCHandle.RCC_Clocks.System_Clock;
    else if (temp == 4)
        RCCHandle.RCC_Clocks.P_Clock_1 = RCCHandle.RCC_Clocks.High_Clock/2;
    else if (temp == 5)
        RCCHandle.RCC_Clocks.P_Clock_1 = RCCHandle.RCC_Clocks.High_Clock/4;
    else if (temp == 6)
        RCCHandle.RCC_Clocks.P_Clock_1 = RCCHandle.RCC_Clocks.High_Clock/8;
    else if (temp == 7)
        RCCHandle.RCC_Clocks.P_Clock_1 = RCCHandle.RCC_Clocks.High_Clock/16;

    //APB1 max clock should be 36MHz
    if(RCCHandle.RCC_Clocks.P_Clock_1 > 36000000) RCCHandle.RCC_Clocks.P_Clock_1 = 36000000;
    return;
}

static void RCC_Update_PCLK2()
{
    uint8_t temp = (RCC->CFGR & 0x7<<RCC_CFGR_PPRE2)>>RCC_CFGR_PPRE2;
    if(temp < 4)
        RCCHandle.RCC_Clocks.P_Clock_2 = RCCHandle.RCC_Clocks.System_Clock;
    else if (temp == 4)
        RCCHandle.RCC_Clocks.P_Clock_2 = RCCHandle.RCC_Clocks.High_Clock/2;
    else if (temp == 5)
        RCCHandle.RCC_Clocks.P_Clock_2 = RCCHandle.RCC_Clocks.High_Clock/4;
    else if (temp == 6)
        RCCHandle.RCC_Clocks.P_Clock_2 = RCCHandle.RCC_Clocks.High_Clock/8;
    else if (temp == 7)
        RCCHandle.RCC_Clocks.P_Clock_2 = RCCHandle.RCC_Clocks.High_Clock/16;
    return;
}

static void RCC_Update_SYSCLK()
{
    uint8_t temp = RCC_GET_SWS();
    if(temp == 0)
    {
        RCCHandle.SYSCLK_Source = SYSCLK_HSI;
        RCCHandle.RCC_Clocks.System_Clock = RCC_SYSCLK_FREQ_DEFAULT;
        sysclock = RCCHandle.RCC_Clocks.System_Clock;
    }
    else if(temp == 1)
    {
        RCCHandle.SYSCLK_Source = SYSCLK_HSE;
        RCCHandle.RCC_Clocks.System_Clock = RCC_SYSCLK_FREQ_DEFAULT;
        sysclock = RCCHandle.RCC_Clocks.System_Clock;
    }
    else if(temp == 2)
    {
        RCCHandle.SYSCLK_Source = SYSCLK_PLL;
        RCCHandle.RCC_Clocks.System_Clock = RCCHandle.RCC_PLL_Config.PLL_clock * RCCHandle.RCC_PLL_Config.PLL_multiplier;
        sysclock = RCCHandle.RCC_Clocks.System_Clock;
    }
    return;
}


static uint16_t RCC_Get_AHB_Prescalar()
{
    uint16_t temp = RCC_GET_HPRE();

    if(temp<8) temp = 1;
    else if(temp==8) temp = 2;
    else if(temp==9) temp = 4;
    else if(temp==10) temp = 8;
    else if(temp==11) temp = 16;
    else if(temp==12) temp = 64;
    else if(temp==13) temp = 128;
    else if(temp==14) temp = 256;
    else if(temp==15) temp = 512;

    return temp;
}


static uint8_t RCC_Get_APB1_Prescalar()
{
    uint8_t temp = RCC_GET_PPRE1();

    if(temp<4) temp = 1;
    else if (temp==4) temp = 2;
    else if (temp==5) temp = 4;
    else if (temp==6) temp = 8;
    else if (temp==7) temp = 16;

    return temp;
}


static uint8_t RCC_Get_APB2_Prescalar()
{
    uint8_t temp = RCC_GET_PPRE2();

    if(temp<4) temp = 1;
    else if (temp==4) temp = 2;
    else if (temp==5) temp = 4;
    else if (temp==6) temp = 8;
    else if (temp==7) temp = 16;

    return temp;
}









void RCC_Update_Clocks()
{
    RCC_Update_Clock_Source();
    RCC_Update_HCLK();
    RCC_Update_PCLK1();
    RCC_Update_PCLK2();
    RCC_Update_SYSCLK();
    //sysclock = RCCHandle.RCC_Clocks.System_Clock;

    return;
}





uint8_t RCC_Get_Clock_Source()
{
    return RCCHandle.Clock_Source;
}

uint8_t RCC_Get_SYSCLK_Source()
{
    return RCCHandle.SYSCLK_Source;
}

uint32_t RCC_Get_HCLK()
{
    return RCCHandle.RCC_Clocks.High_Clock;
}

uint32_t RCC_Get_PCLK1()
{
    return RCCHandle.RCC_Clocks.P_Clock_1;
}

uint32_t RCC_Get_PCLK2()
{
    return RCCHandle.RCC_Clocks.P_Clock_2;
}


uint32_t RCC_Get_SYSCLK()
{
    return RCCHandle.RCC_Clocks.System_Clock;
}






void RCC_Select_Clock_Source(uint8_t clk_src)
{
    if(clk_src == RCC_CLK_HSI)
    {
        RCC_HSI_ENABLE();
        RCC_HSIRDY_WAIT();
    }
    else if(clk_src == RCC_CLK_HSE)
    {
        RCC_HSE_ENABLE();
        RCC_HSERDY_WAIT();
    }
    RCC_Update_Clock_Source();
    return;
}


void RCC_Config_HCLK(uint8_t ahb_prescalar)
{
    RCC->CFGR &= ~(1<<RCC_CFGR_HPRE);

    //Enable Flash Prefetch Buffer for precalar greater than 1
    if(ahb_prescalar>AHB_PRESCALAR_1)
        FLASH->ACR |= 1<<FLASH_ACR_PRFTBE;

    if(ahb_prescalar == AHB_PRESCALAR_2) RCC->CFGR |= 8<<RCC_CFGR_HPRE;
    else if(ahb_prescalar == AHB_PRESCALAR_4) RCC->CFGR |= 9<<RCC_CFGR_HPRE;
    else if(ahb_prescalar == AHB_PRESCALAR_8) RCC->CFGR |= 10<<RCC_CFGR_HPRE;
    else if(ahb_prescalar == AHB_PRESCALAR_16) RCC->CFGR |= 11<<RCC_CFGR_HPRE;
    else if(ahb_prescalar == AHB_PRESCALAR_64) RCC->CFGR |= 12<<RCC_CFGR_HPRE;
    else if(ahb_prescalar == AHB_PRESCALAR_128) RCC->CFGR |= 13<<RCC_CFGR_HPRE;
    else if(ahb_prescalar == AHB_PRESCALAR_256) RCC->CFGR |= 14<<RCC_CFGR_HPRE;
    else if(ahb_prescalar == AHB_PRESCALAR_512) RCC->CFGR |= 15<<RCC_CFGR_HPRE;

    return;
}


//bits should be set in such a way that frequency output of this prescalar should not exceed 36 MHz
void RCC_Config_PCLK1(uint8_t apb1_prescalar)
{
    RCC->CFGR &= ~(1<<RCC_CFGR_PPRE1);
    if(apb1_prescalar == APB1_PRESCALAR_2) RCC->CFGR |= 4<<RCC_CFGR_PPRE1;
    else if(apb1_prescalar == APB1_PRESCALAR_4) RCC->CFGR |= 5<<RCC_CFGR_PPRE1;
    else if(apb1_prescalar == APB1_PRESCALAR_8) RCC->CFGR |= 6<<RCC_CFGR_PPRE1;
    else if(apb1_prescalar == APB1_PRESCALAR_16) RCC->CFGR |= 7<<RCC_CFGR_PPRE1;

    return;
}


void RCC_Config_PCLK2(uint8_t apb2_prescalar)
{
    RCC->CFGR &= ~(1<<RCC_CFGR_PPRE2);
    if(apb2_prescalar == APB2_PRESCALAR_2) RCC->CFGR |= 4<<RCC_CFGR_PPRE2;
    else if(apb2_prescalar == APB2_PRESCALAR_4) RCC->CFGR |= 5<<RCC_CFGR_PPRE2;
    else if(apb2_prescalar == APB2_PRESCALAR_8) RCC->CFGR |= 6<<RCC_CFGR_PPRE2;
    else if(apb2_prescalar == APB2_PRESCALAR_16) RCC->CFGR |= 7<<RCC_CFGR_PPRE2;

    return;
}


void RCC_Config_PLL(uint8_t pll_clk_src, uint8_t pll_mul, uint8_t pll_hse_div)
{
    RCCHandle.RCC_PLL_Config.PLL_HSE_divider = pll_hse_div;
    RCCHandle.RCC_PLL_Config.PLL_multiplier  = pll_mul;
    RCCHandle.RCC_PLL_Config.PLL_source      = pll_clk_src;


    RCC_PLL_DISABLE();
    if(pll_clk_src == PLL_CLK_SRC_HSI_DIV_2)
    {
        RCC->CFGR |= ~(1<<RCC_CFGR_PLLSRC);
        RCCHandle.RCC_PLL_Config.PLL_clock = RCCHandle.RCC_Clocks.High_Clock/2;
    }
    else if(pll_clk_src == PLL_CLK_SRC_HSE)
    {
        RCC->CFGR |= (1<<RCC_CFGR_PLLSRC);
        if(pll_hse_div == PLL_HSE_DIV_2)
        {
            RCC->CFGR |= 1<<RCC_CFGR_PLLXTPRE;
            RCCHandle.RCC_PLL_Config.PLL_clock = RCC_SYSCLK_FREQ_DEFAULT/2;
        }
        else
        {
            RCC->CFGR &= ~(1<<RCC_CFGR_PLLXTPRE);
            RCCHandle.RCC_PLL_Config.PLL_clock = RCC_SYSCLK_FREQ_DEFAULT;
        }

        if((pll_mul-2)>=3 && (pll_mul-2)<16)
        {
            if((RCCHandle.RCC_PLL_Config.PLL_clock == RCC_SYSCLK_FREQ_DEFAULT && (pll_mul-2)<=9) || RCCHandle.RCC_PLL_Config.PLL_clock == 4000000)
                RCC->CFGR |= ((pll_mul-2)) << RCC_CFGR_PLLMUL;
            else if(RCCHandle.RCC_PLL_Config.PLL_clock == RCC_SYSCLK_FREQ_DEFAULT && (pll_mul-2)>9) RCC->CFGR |= 9<<RCC_CFGR_PLLMUL;
            else if((pll_mul-2)==16) RCC->CFGR |= 0xE << RCC_CFGR_PLLMUL;
            else if((pll_mul-2)==2) RCC->CFGR &= ~(0xF << RCC_CFGR_PLLMUL);
        }
    }
    return;
}


/*
<Params>: System Clock Source
<Desc>: Configure the SYSCLK for: HSI, HSE, PLL
<!Note!>:
- Clock Source and Prescalars should be configured first
- If System Clock is chosen as PLL, PLL must be configured first
*/
void RCC_Select_SYSCLK(uint8_t sysclock)
{
    if(sysclock == SYSCLK_HSI)
    {
        RCC_SYSCLK_SWITCH_CLEAR();
        RCC_SYSCLK_SWITCH_HSI();
        while ((RCC->CFGR & 0x3<<RCC_CFGR_SWS)>>RCC_CFGR_SWS != 0x0);
        RCCHandle.RCC_Clocks.System_Clock = RCC_SYSCLK_FREQ_DEFAULT;
    }
    else if(sysclock == SYSCLK_HSE)
    {
        RCC_SYSCLK_SWITCH_CLEAR();
        RCC_SYSCLK_SWITCH_HSE();
        while ((RCC->CFGR & 0x3<<RCC_CFGR_SWS)>>RCC_CFGR_SWS != 0x1);
        RCCHandle.RCC_Clocks.System_Clock = RCC_SYSCLK_FREQ_DEFAULT;
    }
    else if(sysclock == SYSCLK_PLL)
    {
        RCCHandle.RCC_Clocks.System_Clock = RCCHandle.RCC_PLL_Config.PLL_clock * (RCCHandle.RCC_PLL_Config.PLL_multiplier);
        sysclock = RCCHandle.RCC_Clocks.System_Clock;

        //Configure Flash Latency based on system clock
        if(RCCHandle.RCC_Clocks.System_Clock>=48000000)
            FLASH_ConfigLatency(FLASH_WAIT_STATE_2);
        else if(RCCHandle.RCC_Clocks.System_Clock<48000000 && RCCHandle.RCC_Clocks.System_Clock>=24000000)
            FLASH_ConfigLatency(FLASH_WAIT_STATE_1);
        else if(RCCHandle.RCC_Clocks.System_Clock<24000000 && RCCHandle.RCC_Clocks.System_Clock>=8000000)
            FLASH_ConfigLatency(FLASH_WAIT_STATE_0);

        //Enable PLL
        RCC_PLL_ENABLE();
        RCC_PLLRDY_WAIT();
        RCC_SYSCLK_SWITCH_CLEAR();
        RCC_SYSCLK_SWITCH_PLL();
        while ((RCC->CFGR & 0x3<<RCC_CFGR_SWS)>>RCC_CFGR_SWS != 0x2);
        //if(RCC_SYSCLK_GET_SWSTATUS() == SWITCH_STATUS_PLL) return;
    }
    return;
}



//PLL must be configured first if the SYSCLK source is PLL
void RCC_init(uint8_t clk_src, uint8_t sysclk_src, uint8_t AHB_prescalar, uint8_t APB1_prescalar, uint8_t APB2_prescalar)
{
    RCCHandle.pRCC           = RCC;
    RCCHandle.Clock_Source   = clk_src;
    RCCHandle.SYSCLK_Source  = sysclk_src;
    RCCHandle.AHB_Prescalar  = AHB_prescalar;
    RCCHandle.APB1_Prescalar = APB1_prescalar;
    RCCHandle.APB2_Prescalar = APB2_prescalar;

    //Configure clock
    RCC_Select_Clock_Source(RCCHandle.Clock_Source);
    RCC_Config_HCLK(RCCHandle.AHB_Prescalar);
    RCC_Config_PCLK1(RCCHandle.APB1_Prescalar);
    RCC_Config_PCLK2(RCCHandle.APB2_Prescalar);
    RCC_Select_SYSCLK(RCCHandle.SYSCLK_Source);
    RCC_Update_Clocks();
    return;
}



void RCC_MCO_Config(RCC_Handle* pRCC_Handle, uint8_t mco_clk_src)
{
    pRCC_Handle->pRCC->CFGR |= mco_clk_src<<RCC_CFGR_MCO;
    pRCC_Handle->MCO_Clock_Source = mco_clk_src;

    //GPIO MCO Alternate Function Config - PA8
    GPIO_Handle G_MCO;
    GPIO_Config(&G_MCO, GPIOA, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_PP, GPIO_PIN8, GPIO_OP_SPEED_2);
    GPIO_Init(&G_MCO);
    return;
}

//todo
//update system clock api


/************************************************ RCC API's Definitions End *************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/