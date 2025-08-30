#include "stm32f103xx_faults.h"
#include "stm32f103xx_serial.h"
#include <stdint.h>


void enable_faults(void)
{
    BUSFAULT_ENABLE();
    MEMFAULT_ENABLE();
    USGFAULT_ENABLE();

    //Enable Traps
    DIV_0_TRP_ENABLE();
    UNALIGN_TRP_ENABLE();
}