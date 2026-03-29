#include"stm32f103xx_flash.h"



void FLASH_ConfigLatency(uint8_t no_of_wait_state)
{
    //Clear latency fields (default is zero wait state)
    FLASH->ACR &= ~(3<<FLASH_ACR_LATENCY);

    //Enable prefetch buffer for SYSCLK>24MHz
    if(no_of_wait_state != FLASH_WAIT_STATE_0)
        FLASH->ACR |= 1<<FLASH_ACR_PRFTBE;
    
    //one wait state - 24MHz<SYSCLK<48MHz
    if(no_of_wait_state == FLASH_WAIT_STATE_1)
        FLASH->ACR |= 1<<FLASH_ACR_LATENCY;
    //two wait states - 48MHz<SYSCLK<72MHz
    else if(no_of_wait_state == FLASH_WAIT_STATE_2)
        FLASH->ACR |= 1<<FLASH_ACR_LATENCY1;
    return;
}