#ifndef INC_STM32F103XX_FLASH_H
#define INC_STM32F103XX_FLASH_H

#include"stm32f103xx.h"
#include"stm32f103xx_memory_map.h"

/*************************************************** FLASH Definitions Start **************************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/
#define FLASH                                           ((FLASH_RegDef*)FLASH_INTERFACE_BASE_ADDR)


//Register Bit Field Definitions
//Flash Access Control Register
#define FLASH_ACR_LATENCY                               0 /*<Latency: Ratio of SYSCLK to FLASH Access Time>*/
#define FLASH_ACR_LATENCY1                              1 /*<>*/
#define FLASH_ACR_LATENCY2                              2 /*<>*/
#define FLASH_ACR_HLFCYA                                4 /*<Flash Half Cycle Enable>*/
#define FLASH_ACR_PRFTBE                                5 /*<Prefetch Buffer Enable>*/
#define FLASH_ACR_PRFTBS                                6 /*<Prefetch Buffer Status>*/

//Flash Status Register
#define FLASH_SR_BSY                                    0 /*<Busy>*/
#define FLASH_SR_PGERR                                  2 /*<Programming Error>*/
#define FLASH_SR_WRPRTERR                               4 /*<Write Protection Error>*/
#define FLASH_SR_EOP                                    5 /*<End of Operation>*/

//Flash Control Register
//Reset: 0x00000080
#define FLASH_CR_PG                                     0 /*<Programming>*/
#define FLASH_CR_PER                                    1 /*<Page Erase>*/
#define FLASH_CR_MER                                    2 /*<Mass Erase>*/
#define FLASH_CR_OPTPG                                  3 /*<Option Byte Programming>*/
#define FLASH_CR_OPTER                                  4 /*<Option Byte Erase>*/
#define FLASH_CR_STRT                                   6 /*<Start>*/
#define FLASH_CR_LOCK                                   7 /*<Lock: Should always be 1>*/
#define FLASH_CR_OPTWRE                                 9 /*<Option Bytes Write Enable>*/
#define FLASH_CR_ERRIE                                  10 /*<Error Interrupt Enable>*/
#define FLASH_CR_EOPIE                                  12 /*<End of Operation Interrupt Enable>*/

//Flash Option Byte Register
#define FLASH_OBR_OPTERR                                0 /*<Option Byte Error><READ ONLY>*/
#define FLASH_OBR_RDPRT                                 1 /*<Read Protection><READ ONLY>*/
#define FLASH_OBR_WDG_SW                                2 /*<>*/
#define FLASH_OBR_nRST_STOP                             3 /*<>*/
#define FLASH_OBR_nRST_STDBY                            4 /*<>*/
/**************************************************** FLASH Definitions End ***************************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/
/************************************************* FLASH Macros Definitions Start *********************************************/
/*<>*/
#define FLASH_PREFETCH_ENABLE()                         (FLASH->ACR |= 1<<FLASH_ACR_PRFTBE)
#define FLASH_PREFETCH_DISABLE()                        (FLASH->ACR &= ~(1<<FLASH_ACR_PRFTBE))

#define FLASH_LATENCY_CLEAR()                           (FLASH->ACR &= ~(0x8<<FLASH_ACR_LATENCY))
#define FLASH_LATENCY_SET_24MHZ()                       FLASH_LATENCY_CLEAR()
#define FLASH_LATENCY_SET_48MHZ()                       (FLASH->ACR |= 1<<FLASH_ACR_LATENCY1)
#define FLASH_LATENCY_SET_72MHZ()                       (FLASH->ACR |= 1<<FLASH_ACR_LATENCY2)

#define FLASH_LATENCY_SET_0_WAIT_STATES()               FLASH_LATENCY_CLEAR()
#define FLASH_LATENCY_SET_1_WAIT_STATES()               FLASH_LATENCY_SET_48MHZ()
#define FLASH_LATENCY_SET_2_WAIT_STATES()               FLASH_LATENCY_SET_72MHZ()
/************************************************* FLASH Macros Definitions End ***********************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/
/************************************************* FLASH Config Definitions Start *********************************************/
#define FLASH_WAIT_STATE_0                              0
#define FLASH_WAIT_STATE_1                              1
#define FLASH_WAIT_STATE_2                              2

#define FLASH_PREFETCH_BUFFER_DISABLED                  0
#define FLASH_PREFETCH_BUFFER_ENABLED                   1

#define FLASH_HALFCYCLE_ACCESS_DISABLED                 0
#define FLASH_HALFCYCLE_ACCESS_ENABLED                  1


/************************************************* FLASH Config Definitions End ***********************************************/
/*----------------------------------------------------------------------------------------------------------------------------*/

//Flash Handle Structure
typedef struct
{
    FLASH_RegDef* pFLASH;
    uint8_t  wait_state;
    uint8_t  prefetch_buff_state;
    uint8_t  halfcycle_access_state;
    uint32_t FPEC_key;
    uint32_t option_byte_key;
    uint32_t flash_address;
}FLASH_Handle;



void FLASH_ConfigLatency(uint8_t no_of_wait_state);


#endif