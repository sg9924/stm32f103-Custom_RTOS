#ifndef INC_STM32F103XX_CORE_H
#define INC_STM32F103XX_CORE_H

#include<stdint.h>


//ARM Cortex M3 Core Peripherals Base Addresses
#define ROM_BASE_ADDR          0xE00FF000U
#define EXPPB_BASE_ADDR        0xE0042000U
#define ETM_BASE_ADDR          0xE0041000U
#define TPIU_BASE_ADDR         0xE0040000U
#define RESERVED1              0xE000F000U
#define SCS_BASE_ADDR          0xE000E000U
#define SCB_BASE_ADDR          (SCS_BASE_ADDR + 0xD00U)
#define RESERVED2              0xE0003000U
#define FPB_BASE_ADDR          0xE0002000U
#define DWT_BASE_ADDR          0xE0001000U
#define ITM_BASE_ADDR          0xE0000000U


//System Control Space
#define SCS_ACTLR_ADDR         (SCS_BASE_ADDR + 0x008U)
#define SCS_STCSR_ADDR         (SCS_BASE_ADDR + 0x010U)
#define SCS_STRVR_ADDR         (SCS_BASE_ADDR + 0x014U)
#define SCS_STCVR_ADDR         (SCS_BASE_ADDR + 0x018U)
#define SCS_STCR_ADDR          (SCS_BASE_ADDR + 0x01CU)

//System Control Block - from 0xE000ED00
typedef struct
{
    volatile uint32_t CPUID;          /*<Address Offset: 0x00> <>*/
    volatile uint32_t ICSR;           /*<Address Offset: 0x> <>*/
    volatile uint32_t VTOR;           /*<Address Offset: 0x> <>*/
    volatile uint32_t AIRCR;          /*<Address Offset: 0x> <>*/
    volatile uint32_t SCR;            /*<Address Offset: 0x> <>*/
    volatile uint32_t CCR;            /*<Address Offset: 0x> <>*/
    volatile uint32_t SHPR1;          /*<Address Offset: 0x> <>*/
    volatile uint32_t SHPR2;          /*<Address Offset: 0x> <>*/
    volatile uint32_t SHPR3;          /*<Address Offset: 0x> <>*/
    volatile uint32_t SHCSR;          /*<Address Offset: 0x> <>*/
    volatile uint32_t CFSR;           /*<Address Offset: 0x> <>*/
    volatile uint32_t HFSR;           /*<Address Offset: 0x> <>*/
    volatile uint32_t DFSR;           /*<Address Offset: 0x> <>*/
    volatile uint32_t MMFAR;          /*<Address Offset: 0x> <>*/
    volatile uint32_t BFAR;           /*<Address Offset: 0x> <>*/
    volatile uint32_t AFSR;           /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_PFR0;        /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_PFR1;        /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_DFR0;        /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_AFR0;        /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_MMFR0;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_MMFR1;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_MMFR2;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_MMFR3;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_ISAR0;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_ISAR1;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_ISAR2;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_ISAR3;       /*<Address Offset: 0x> <>*/
    volatile uint32_t ID_ISAR4;       /*<Address Offset: 0x70> <>*/
} SCB_RegDef;



//Peripheral Definitions
#define SCB          ((SCB_RegDef*) SCB_BASE_ADDR)



/*----------------------------------------------------Bit Field Definitions----------------------------------------------------*/
//System Control Block
//CPUID Bit Field Definitions
#define SCB_CPUID_Revision                  0                /*<>*/
#define SCB_CPUID_PartNo                    4                /*<>*/
#define SCB_CPUID_Constant                  16               /*<>*/
#define SCB_CPUID_Variant                   20               /*<>*/
#define SCB_CPUID_Implementer               24               /*<>*/

//ICSR Bit Field Definitions
#define SCB_ICSR_VECTACTIVE                 0                /*<Active Vector - Contains Active Vector Number - Subtract 16 from this to get IRQ No>*/
#define SCB_ICSR_RETOBASE                   11               /*<Return to Base Level>*/
#define SCB_ICSR_VECTPENDING                12               /*<Pending Vector - Shows Exception no of highest priority Pending Enabled Exception><[9:0]>*/
#define SCB_ICSR_ISRPENDING                 22               /*<Interrupt Pending Flag (excluding NMI & Faults)>*/
#define SCB_ICSR_PENDSTCLR                  25               /*<SysTick Clear Pending Bit>*/
#define SCB_ICSR_PENDSTSET                  26               /*<SysTick Set Pending Bit>*/
#define SCB_ICSR_PENDSVCLR                  27               /*<Pend SV Clear Pending Bit>*/ 
#define SCB_ICSR_PENDSVSET                  28               /*<Pend SV Set Pending Bit>*/
#define SCB_ICSR_NMIPENDSET                 31               /*<NMI Set Pending Bit>*/

//VTOR Bit Field Definitions
#define SCB_VTOR_TBLOFF                     9                /*<Vector Table Base Offset Field><[29:9]>*/
#define SCB_VTOR_TBLBASE                    29               /*<Indicates whether the Vector Table is in Code/SRAM Region>*/

//AIRCR Bit Field Definitions
#define SCB_AIRCR_VECT_RESET                0                /*<!Reserved for Debug Use!>*/
#define SCB_AIRCR_VECT_CLR_ACTIVE           1                /*<!Reserved for Debug Use!>*/
#define SCB_AIRCR_SYS_RESET_REQ             2                /*<System Reset Request - Force Reset all major components except Debug>*/
#define SCB_AIRCR_PRIGROUP                  8                /*<Interrupt Priority Grouping - Determines split of Group Priority from Split Priority>*/
#define SCB_AIRCR_ENDIANESS                 15               /*<Data Endianess>*/
#define SCB_AIRCR_VECTKEY                   16               /*<Register Key - Write (0x5FA)><31:16>*/
#define SCB_AIRCR_VECTKEYSTAT               16               /*<Register Key - Read (0xFA05)><31:16>*/

//SCR Bit Field Definitions
#define SCB_SCR_SLEEP_ON_EXIT               1                /*<Configure Sleep on Exit from Handler (ISR) to Thread Mode>*/
#define SCB_SCR_SLEEP_DEEP                  2                /*<Sleep Type in Low Power Mode>*/
#define SCB_SCR_SEVON_PEND                  4                /*<Set Event on Pending Bit>*/

//CCR Bit Field Definitions
#define SCB_CCR_NONBASETHRDENA              0                /*<Configure how processor enters Thread mode>*/
#define SCB_CCR_USERSETMPEND                1                /*<Enabled Unprivileged Access to NVIC_STIR>*/
#define SCB_CCR_UNALIGN_TRP                 3                /*<Enable Unaligned Access Trap>*/
#define SCB_CCR_DIV_0_TRP                   4                /*<Enable Fault for Divide by zero>*/
#define SCB_CCR_BFHFNMMIGN                  8                /*<ENable Handlers with priority -1/-2 to Ignore Bus Faults caused by load and store instructions>*/
#define SCB_CCR_STKALIGN                    9                /*<Configure Stack Alignment on Exception Entry>*/

//SHPR1 Bit Field Definitions
#define SCB_SHPR1_PRI_4R                    0                /*<Priority of System Handler 4 - Read>*/
#define SCB_SHPR1_PRI_4RW                   4                /*<Priority of System Handler 4 - Read Write>*/
#define SCB_SHPR1_PRI_5R                    8                /*<Priority of System Handler 5 - Read>*/
#define SCB_SHPR1_PRI_5RW                   12               /*<Priority of System Handler 5 - Read Write>*/
#define SCB_SHPR1_PRI_6R                    16               /*<Priority of System Handler 6 - Read>*/
#define SCB_SHPR1_PRI_6RW                   20               /*<Priority of System Handler 6 - Read Write>*/

#define SCB_SHPR1_MEM_FAULT                 0
#define SCB_SHPR1_BUS_FAULT                 8
#define SCB_SHPR1_USG_FAULT                 16

//SHCSR Bit Field Definitions
#define SCB_SHCSR_MEMFAULTACT               0                /*<Memory Fault Active>*/
#define SCB_SHCSR_BUSFAULTACT               1                /*<Bus Fault Active>*/
#define SCB_SHCSR_USGFAULTACT               3                /*<Usage Fault Active>*/
#define SCB_SHCSR_SVCALLACT                 7                /*<SVC Call Active>*/
#define SCB_SHCSR_MONITORACT                8                /*<Debug Monitor Active>*/
#define SCB_SHCSR_PENDSVACT                 10               /*<Pend SV Active>*/
#define SCB_SHCSR_SYSTICKACT                11               /*<Systick Active>*/
#define SCB_SHCSR_USGFAULTPENDED            12               /*<Usage Fault Pended>*/
#define SCB_SHCSR_MEMFAULTPENDED            13               /*<Memory Fault Pended>*/
#define SCB_SHCSR_BUSFAULTPENDED            14               /*<Bus Fault Pended>*/
#define SCB_SHCSR_SVCALLPENDED              15               /*<SVC Call Pended>*/
#define SCB_SHCSR_MEMFAULTENA               16               /*<Memory Fault Enable>*/
#define SCB_SHCSR_BUSFAULTENA               17               /*<Bus Fault Enable>*/
#define SCB_SHCSR_USGFAULTENA               18               /*<Usage Fault Enable>*/

//CFSR Bit Field Definitions
//MMFSR
#define SCB_CFSR_IACC_VIOL                  0
#define SCB_CFSR_DACC_VIOL                  1
#define SCB_CFSR_MUNSTK_ERR                 3
#define SCB_CFSR_MSTK_ERR                   4
#define SCB_CFSR_MMAR_VALID                 7
//BFSR
#define SCB_CFSR_IBUS_ERR                   8
#define SCB_CFSR_PRECIS_ERR                 9
#define SCB_CFSR_IMPRECIS_ERR               10               /*<>*/
#define SCB_CFSR_UNSTK_ERR                  11               /*<>*/
#define SCB_CFSR_STK_ERR                    12               /*<>*/
#define SCB_CFSR_BFAR_VALID                 15               /*<>*/
//UFSR
#define SCB_CFSR_UNDEFINSTR                 16               /*<>*/
#define SCB_CFSR_INV_STATE                  17               /*<>*/
#define SCB_CFSR_INVPC                      18               /*<>*/
#define SCB_CFSR_NOCP                       19               /*<>*/
#define SCB_CFSR_UNALIGNED                  24               /*<>*/
#define SCB_CFSR_DIVBYZERO                  25               /*<>*/

//HFSR Bit Field Definitions
#define SCB_HFSR_VECT_TBL                   1                /*<Vector Table Hard Fault>*/
#define SCB_HFSR_FORCED                     30               /*<Forced Hard Fault>*/
#define SCB_HFSR_DEBUG_VT                   31               /*<Reserved for Debug Use - Indicates if Debug Event Occurred>*/





//Macros
//CPUID
#define GET_IMPLEMENTER()       ((SCB->CPUID & (0xFF<<24))>>24)   
#define GET_VARIANT()           ((SCB->CPUID & (0xF<<20))>>20)   
#define GET_PARTNO()            ((SCB->CPUID & (0xFFF<<4))>>4)   
#define GET_REVISION()          (SCB->CPUID & (0xF<<0))   





#endif