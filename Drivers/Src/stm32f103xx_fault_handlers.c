#include "stm32f103xx_fault_handlers.h"
#include "stm32f103xx_serial.h"


//Decode Fault Status Registers
//MMFSR
void decode_MMFSR(uint8_t mmfsr, uint32_t mmfar, uint32_t cfsr)
{
    if(!mmfsr) {Serialprint("\r\n[DEBUG]: Empty MMFSR..."); return;}
    Serialprint("\r\n[DEBUG]: MMFSR = %x", mmfsr);
    if(cfsr & (1<<SCB_CFSR_IACC_VIOL))   Serialprint("\r\n[DEBUG]:   IACC_VIOL  - Instruction Access Violation");
    if(cfsr & (1<<SCB_CFSR_DACC_VIOL))   Serialprint("\r\n[DEBUG]:   DACC_VIOL  - Data Access Violation");
    if(cfsr & (1<<SCB_CFSR_MUNSTK_ERR))  Serialprint("\r\n[DEBUG]:   MUNSTK_ERR - Unstacking from Exception Exit Fault");
    if(cfsr & (1<<SCB_CFSR_MSTK_ERR))    Serialprint("\r\n[DEBUG]:   MSTK_ERR   - Stacking of Exception Entry Fault");
    if(cfsr & (1<<SCB_CFSR_MMAR_VALID))  Serialprint("\r\n[DEBUG]:   MMAR_VALID - MMFAR holds the Fault Address: %x", mmfar);
}

//BFSR
void decode_BFSR(uint8_t bfsr, uint32_t bfar, uint32_t cfsr)
{
    if(!bfsr) {Serialprint("\r\n[DEBUG]: Empty BFSR..."); return;}
    Serialprint("\r\n[DEBUG]: BFSR = %x", bfsr);
    if(cfsr & (1<<SCB_CFSR_IBUS_ERR))      Serialprint("\r\n[DEBUG]:   IBUS_ERR     - Instruction Bus Fault (Prefetch Abort)");
    if(cfsr & (1<<SCB_CFSR_PRECIS_ERR))    Serialprint("\r\n[DEBUG]:   PRECIS_ERR   - Precise Data Bus Fault (Address in BFAR if Valid)");
    if(cfsr & (1<<SCB_CFSR_IMPRECIS_ERR))  Serialprint("\r\n[DEBUG]:   IMPRECIS_ERR - Imprecise Data Bus Fault (Asynchronous)");
    if(cfsr & (1<<SCB_CFSR_UNSTK_ERR))     Serialprint("\r\n[DEBUG]:   UNSTK_ERR    - Unstacking On Exception Exit Fault");
    if(cfsr & (1<<SCB_CFSR_STK_ERR))       Serialprint("\r\n[DEBUG]:   STK_ERR      - Stacking on Exception Entry Fault");
    if(cfsr & (1<<SCB_CFSR_BFAR_VALID))    Serialprint("\r\n[DEBUG]:   BFAR_VALID   - BFAR holds the Fault Address: %x", bfar);
}

//UFSR
void decode_UFSR(uint8_t ufsr, uint32_t cfsr)
{
    if(!ufsr) {Serialprint("\r\n[DEBUG]: Empty UFSR..."); return;}
    Serialprint("\r\n[DEBUG]: UFSR = %x", ufsr);
    if(cfsr & (1<<SCB_CFSR_UNDEFINSTR))   Serialprint("\r\n[DEBUG]:   UNDEFIN_STR - Undefined Instruction");
    if(cfsr & (1<<SCB_CFSR_INV_STATE))    Serialprint("\r\n[DEBUG]:   INV_STATE   - Invalid State");
    if(cfsr & (1<<SCB_CFSR_INVPC))        Serialprint("\r\n[DEBUG]:   INVPC       - Invalid PC Load (Illegal EXC_RETURN value, Invalid PSR)");
    if(cfsr & (1<<SCB_CFSR_NOCP))         Serialprint("\r\n[DEBUG]:   NOCP        - No Coprocessor");
    if(cfsr & (1<<SCB_CFSR_UNALIGNED))    Serialprint("\r\n[DEBUG]:   UNALIGNED   - Unaligned Access (If Trap has been enabled)");
    if(cfsr & (1<<SCB_CFSR_DIVBYZERO))    Serialprint("\r\n[DEBUG]:   DIVBYZERO   - Divide by Zero (If Trap has been enabled)");
}


//HFSR
void decode_HFSR(uint32_t hfsr)
{
    Serialprint("\r\n[DEBUG]: HFSR = %x", hfsr);
    if(hfsr & (1<<SCB_HFSR_VECT_TBL)) Serialprint("\r\n[DEBUG]:   VECT_TBL - Fault on Vector Table Read");
    if(hfsr & (1<<SCB_HFSR_FORCED))   Serialprint("\r\n[DEBUG]:   FORCED   - Escalated Fault - Check CFSR for the actual Fault");
    if(hfsr & (1<<SCB_HFSR_DEBUG_VT)) Serialprint("\r\n[DEBUG]:   DEBUG_VT - Ddebug Event (Breakpoint, Watchpoint)");
}

//CFSR
void decode_CFSR(uint32_t cfsr, uint32_t mmfar, uint32_t bfar)
{
    if(!cfsr) {Serialprint("\r\n[DEBUG]: No Configurable Faults found"); return;}

    Serialprint("\r\n[DEBUG]: CFSR = %x", cfsr);

    decode_MMFSR(MMFSR_READ(), cfsr, mmfar);
    decode_BFSR(BFSR_READ(), cfsr, bfar);
    decode_UFSR(UFSR_READ(), cfsr);
}



//HardFault Handler
__attribute__((naked)) static void HardFault_Handler_C(uint32_t* pstack)
{
    Serialprint("\r\n[DEBUG]: --------------------------------------Hard Fault--------------------------------------");

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

    Serialprint("\r\n[DEBUG]: R0 = %x, R1 = %x, R2 = %x, R3 = %x", pstack[0],pstack[1],pstack[2],pstack[3]);
    Serialprint("\r\n[DEBUG]: R12 = %x, LR = %x, PC = %x, PSR = %x", pstack[4],pstack[5],pstack[6],pstack[7]);
    Serialprint("\r\n[DEBUG]: HFSR = %x", SCB->HFSR);
    Serialprint("\r\n[DEBUG]: CFSR = %x", SCB->CFSR);
    Serialprint("\r\n[DEBUG]: MMFAR = %x", SCB->MMFAR);
    Serialprint("\r\n[DEBUG]: BFAR = %x\r\n", SCB->BFAR);

    decode_HFSR(SCB->HFSR);
    decode_CFSR(SCB->CFSR,SCB->MMFAR,SCB->BFAR);

    Serialprint("\r\n[DEBUG]: Fault is at PC = %x (use addr2line with your elf to find the line at Source)", pstack[6]);
    Serialprint("\r\n[DEBUG]: --------------------------------------------------------------------------------------");

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