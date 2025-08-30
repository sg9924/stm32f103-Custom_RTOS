#include<stdint.h>
#include"stm32f103xx_exti.h"


/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** EXTI API's Definitions Start ***********************************************/
//!!EXTI doesn't need clock enable!!

//Interrupt trigger configuration function
void EXTI_Intrpt_Config(uint8_t pin_no, uint8_t intrpt_mode, uint8_t mode)
{
    if(mode == ENABLE)
    {
        //Falling Edge trigger
        if(intrpt_mode == GPIO_MODE_INTRPT_FE_TRIG)
        {
            EXTI->FTSR |= (1 << pin_no);
            EXTI->RTSR &= ~(1 << pin_no); //precaution
        }//Rising Edge Trigger
        else if(intrpt_mode == GPIO_MODE_INTRPT_RE_TRIG)
        {
            EXTI->RTSR |= (1 << pin_no);
            EXTI->FTSR &= ~(1 << pin_no); //precaution
        }//Falling & Rising Edge Trigger
        else if(intrpt_mode == GPIO_MODE_INTRPT_FERE_TRIG)
        {
            EXTI->FTSR |= (1 << pin_no);
            EXTI->RTSR |= (1 << pin_no);
        }
    }
    else if(mode == DISABLE)
    {
        //Falling Edge trigger
        if(intrpt_mode == GPIO_MODE_INTRPT_FE_TRIG)
            EXTI->FTSR &= ~(1 << pin_no);
        //Rising Edge Trigger
        else if(intrpt_mode == GPIO_MODE_INTRPT_RE_TRIG)
            EXTI->RTSR &= ~(1 << pin_no);
        //Falling & Rising Edge Trigger
        else if(intrpt_mode == GPIO_MODE_INTRPT_FERE_TRIG)
        {
            EXTI->FTSR &= ~(1 << pin_no);
            EXTI->RTSR &= ~(1 << pin_no);
        }
    }
}


void EXTI_Pend_Clear(uint8_t exti_no)
{
    if(exti_no<=18 && (EXTI->PR)>>exti_no == 1)
        EXTI->PR |= 1<<(exti_no);
}

uint8_t EXTI_Pend_Check(uint8_t exti_no)
{
    return (EXTI->PR & (1<<exti_no));
}

void EXTI_Intrpt_Mask(uint8_t exti_no, uint8_t mode)
{
    if(mode == DISABLE)
        EXTI->IMR &= ~(1<<exti_no);
    else if(mode == ENABLE)
        EXTI->IMR |= 1<<exti_no;
}

void EXTI_Event_Mask(uint8_t exti_no, uint8_t mode)
{
    if(mode == DISABLE)
        EXTI->EMR &= ~(1<<exti_no);
    else if(mode == ENABLE)
        EXTI->EMR |= 1<<exti_no;
}

void EXTI_SWIE(uint8_t exti_no, uint8_t mode)
{
    if(mode == ENABLE)
        EXTI->SWIER |= 1<<exti_no;
    else if(mode == DISABLE)
    {
        if(EXTI->SWIER & (1<<exti_no)) //clear bit only when it is set
            EXTI->SWIER |= 1<<exti_no;
    }
}

/*********************************************** EXTI API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/