#include "stm32f103xx_fault_handlers.h"
#include "stm32f103xx_serial.h"


//Decode Fault Status Registers
//MMFSR
void decode_MMFSR(uint8_t mmfsr, uint32_t mmfar, uint32_t cfsr)
{
    if(!mmfsr) {Serialprintln("Empty MMFSR...", DEBUG); return;}
    Serialprintln("MMFSR = %x", DEBUG, mmfsr);
    if(cfsr & (1<<SCB_CFSR_IACC_VIOL))   Serialprintln("  IACC_VIOL - Instruction Access Violation", DEBUG);
    if(cfsr & (1<<SCB_CFSR_DACC_VIOL))   Serialprintln("  DACC_VIOL - Data Access Violation", DEBUG);
    if(cfsr & (1<<SCB_CFSR_MUNSTK_ERR))  Serialprintln("  MUNSTK_ERR - Unstacking from Exception Exit Fault", DEBUG);
    if(cfsr & (1<<SCB_CFSR_MSTK_ERR))    Serialprintln("  MSTK_ERR - Stacking of Exception Entry Fault", DEBUG);
    if(cfsr & (1<<SCB_CFSR_MMAR_VALID))  Serialprintln("  MMAR_VALID - MMFAR holds the Fault Address: %x", DEBUG, mmfar);
}

//BFSR
void decode_BFSR(uint8_t bfsr, uint32_t bfar, uint32_t cfsr)
{
    if(!bfsr) {Serialprintln("Empty BFSR...", DEBUG); return;}
    Serialprintln("BFSR = %x", DEBUG, bfsr);
    if(cfsr & (1<<SCB_CFSR_IBUS_ERR))      Serialprintln("  IBUS_ERR - Instruction Bus Fault (Prefetch Abort)", DEBUG);
    if(cfsr & (1<<SCB_CFSR_PRECIS_ERR))    Serialprintln("  PRECIS_ERR - Precise Data Bus Fault (Address in BFAR if Valid)", DEBUG);
    if(cfsr & (1<<SCB_CFSR_IMPRECIS_ERR))  Serialprintln("  IMPRECIS_ERR - Imprecise Data Bus Fault (Asynchronous)", DEBUG);
    if(cfsr & (1<<SCB_CFSR_UNSTK_ERR))     Serialprintln("  UNSTK_ERR - Unstacking On Exception Exit Fault", DEBUG);
    if(cfsr & (1<<SCB_CFSR_STK_ERR))       Serialprintln("  STK_ERR - Stacking on Exception Entry Fault", DEBUG);
    if(cfsr & (1<<SCB_CFSR_BFAR_VALID))    Serialprintln("  BFAR_VALID - BFAR holds the Fault Address: %x", DEBUG, bfar);
}

//UFSR
void decode_UFSR(uint8_t ufsr, uint32_t cfsr)
{
    if(!ufsr) {Serialprintln("Empty UFSR...", DEBUG); return;}
    Serialprintln("UFSR = %x", DEBUG, ufsr);
    if(cfsr & (1<<SCB_CFSR_UNDEFINSTR))   Serialprintln("  UNDEFIN_STR - Undefined Instruction", DEBUG);
    if(cfsr & (1<<SCB_CFSR_INV_STATE))    Serialprintln("  INV_STATE - Invalid State", DEBUG);
    if(cfsr & (1<<SCB_CFSR_INVPC))        Serialprintln("  INVPC - Invalid PC Load (Illegal EXC_RETURN value, Invalid PSR)", DEBUG);
    if(cfsr & (1<<SCB_CFSR_NOCP))         Serialprintln("  NOCP - No Coprocessor", DEBUG);
    if(cfsr & (1<<SCB_CFSR_UNALIGNED))    Serialprintln("  UNALIGNED - Unaligned Access (If Trap has been enabled)", DEBUG);
    if(cfsr & (1<<SCB_CFSR_DIVBYZERO))    Serialprintln("  DIVBYZERO - Divide by Zero (If Trap has been enabled)", DEBUG);
}


//HFSR
void decode_HFSR(uint32_t hfsr)
{
    Serialprintln("HFSR = %x", DEBUG, hfsr);
    if(hfsr & (1<<SCB_HFSR_VECT_TBL)) Serialprintln("  VECT_TBL - Fault on Vector Table Read", DEBUG);
    if(hfsr & (1<<SCB_HFSR_FORCED))   Serialprintln("  FORCED   - Escalated Fault - Check CFSR for the actual Fault", DEBUG);
    if(hfsr & (1<<SCB_HFSR_DEBUG_VT)) Serialprintln("  DEBUG_VT - Ddebug Event (Breakpoint, Watchpoint)", DEBUG);
}

//CFSR
void decode_CFSR(uint32_t cfsr, uint32_t mmfar, uint32_t bfar)
{
    if(!cfsr) {Serialprintln("CFSR = No Configurable Faults found", DEBUG); return;}

    Serialprintln("CFSR = %x", DEBUG, cfsr);

    decode_MMFSR(MMFSR_READ(), cfsr, mmfar);
    decode_BFSR(BFSR_READ(), cfsr, bfar);
    decode_UFSR(UFSR_READ(), cfsr);
}



//HardFault Handler
__attribute__((naked)) static void HardFault_Handler_C(uint32_t* pstack)
{
    Serialprintln("--------------------------------------Hard Fault Handler--------------------------------------", NONE);

    //uint32_t R0  = pstack[0];
    //uint32_t R1  = pstack[1];
    //uint32_t R2  = pstack[2];
    //uint32_t R3  = pstack[3];
    //uint32_t R12 = pstack[4];
    //uint32_t LR  = pstack[5];
    //uint32_t PC  = pstack[6];
    //uint32_t PSR = pstack[7];

    //uint32_t cfsr  = SCB->CFSR;
    //uint32_t hfsr  = SCB->HFSR;
    //uint32_t mmfar = SCB->MMFAR;
    //uint32_t bfar  = SCB->BFAR;

    Serialprintln("R0 = %x, R1 = %x, R2 = %x, R3 = %x", DEBUG, pstack[0],pstack[1],pstack[2],pstack[3]);
    Serialprintln("R12 = %x, LR = %x, PC = %x, PSR = %x", DEBUG, pstack[4],pstack[5],pstack[6],pstack[7]);
    Serialprintln("HFSR = %x", DEBUG, SCB->HFSR);
    Serialprintln("CFSR = %x", DEBUG, SCB->CFSR);
    Serialprintln("MMFAR = %x", DEBUG, SCB->MMFAR);
    Serialprintln("BFAR = %x\r\n", DEBUG, SCB->BFAR);

    decode_HFSR(SCB->HFSR);
    decode_CFSR(SCB->CFSR,SCB->MMFAR,SCB->BFAR);

    Serialprintln("Fault is at PC = %x (use addr2line with your elf to find the line at Source)", DEBUG, pstack[6]);
    Serialprintln("----------------------------------------------------------------------------------------------",  NONE);

    //Breakpoint
    __asm volatile ("BKPT #0");
}


__attribute__((naked)) void HardFault_Handler(void)
{
    __asm volatile("TST LR, #4"); //Test bit 2 for MSP (0)/PSP (1)
    __asm volatile("ITE EQ");
    __asm volatile("MRSEQ R0, MSP");
    __asm volatile("MRSNE R0, PSP");
    __asm volatile("B HardFault_Handler_C");
}