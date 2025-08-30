#ifndef INC_STM32F103XX_FAULT_HANDLERS_H
#define INC_STM32F103XX_FAULT_HANDLERS_H

#include <stdint.h>
#include "stm32f103xx_faults.h"



void decode_HFSR(uint32_t hfsr);
void decode_CFSR(uint32_t cfsr, uint32_t mmfar, uint32_t bfar);
void decode_MMFSR(uint8_t mmfsr, uint32_t mmfar, uint32_t cfsr);
void decode_BFSR(uint8_t bfsr, uint32_t bfar, uint32_t cfsr);
void decode_UFSR(uint8_t ufsr, uint32_t cfsr);




#endif