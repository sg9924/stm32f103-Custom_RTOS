#include<stdint.h>
#include "stm32F103xx_afio.h"

/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** AFIO API's Definitions Start ***********************************************/

void AFIO_EXTI_Config(GPIO_RegDef* port_code, uint8_t pin_no, uint8_t mode)
{
    uint8_t reg_no = pin_no/4;                          //get EXTICR Register number
    uint8_t bit_no = (pin_no%4)*4;                      //get bit no. of the EXTICR register
    uint8_t port_no = GPIO_BASEADDR_TO_CODE(port_code); //get the corresponding code of the specified GPIO port
    //uint8_t port_no = 2;
    
    if(mode == ENABLE)
        AFIO->EXTICR[reg_no] |= (port_no << bit_no);
    else if (mode == DISABLE)
        AFIO->EXTICR[reg_no] &= ~(port_no << bit_no);
}


void AFIO_EVENT_Config(uint8_t port, uint8_t pin_no, uint8_t mode)
{
    if(mode == ENABLE)
    {
        AFIO->EVCR |= port << AFIO_EVCR_PORT0;
        AFIO->EVCR |= pin_no << AFIO_EVCR_PIN0;
        AFIO->EVCR |= 1<<AFIO_EVCR_EVOE;
    }
    else if(mode == DISABLE)
    {
        AFIO->EVCR &= ~(port << AFIO_EVCR_PORT0);
        AFIO->EVCR &= ~(pin_no << AFIO_EVCR_PIN0);
        AFIO->EVCR &= ~(1<<AFIO_EVCR_EVOE);
    }
}
/*********************************************** AFIO API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/