#ifndef INC_stm32F103xx_CRC_H
#define INC_stm32F103xx_CRC_H

#include"stm32f103xx.h"
#include"stm32f103xx_memory_map.h"

//CRC Peripheral Definition
#define CRC                   ((CRC_RegDef*)CRC_BASE_ADDR)


//CRC CR register Bit Field Definitions
#define CRC_CR_RESET          0

//CRC IDR register Bit Field Definitions
#define CRC_IDR0              0




uint32_t CRC_Calculate(uint32_t data);
uint32_t CRC_Accumulate(uint32_t* data, uint32_t length);
uint32_t CRC_Get();
void CRC_Set_IDR(uint8_t value);
uint8_t CRC_Get_IDR();


#endif