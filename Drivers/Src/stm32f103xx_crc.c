#include"stm32F103xx_crc.h"




uint32_t CRC_Calculate(uint32_t data)
{
    CRC->DR = data;
    CRC_Get();
}


uint32_t CRC_Accumulate(uint32_t* data, uint32_t length)
{
    for(uint8_t i=0; i<length; i++)
    {
        CRC->DR = data[i];
    }
    CRC_Get();
}


uint32_t CRC_Get()
{
    return(CRC->DR);
}


void CRC_Set_IDR(uint8_t value)
{
    CRC->IDR = value;
}


uint8_t CRC_Get_IDR()
{
    return(CRC->IDR);
}