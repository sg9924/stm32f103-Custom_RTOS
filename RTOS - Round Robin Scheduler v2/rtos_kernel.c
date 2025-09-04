#include<stdint.h>

#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_task.h"
#include "rtos_port.h"


extern tcb_t TCBS[NO_OF_TASKS];  //declare an array of TCB's
extern tcb_t *pcurrent;          //current pointer to a tcb


int32_t TCBS_STACK[NO_OF_TASKS][STACKSIZE];     //array for stack for each Task



void rtosKernel_TaskStackInit(uint8_t task_num)
{
    //initialize stack pointer
    TCBS[task_num].pstack = &(TCBS_STACK[task_num][STACKSIZE-16]);

    //set T bit (bit 24) in xPSR to indicate Thumb state
    TCBS_STACK[task_num][STACKSIZE-1] = (1<<24);

    //initialize PC and LR
    TCBS_STACK[task_num][STACKSIZE-2] = (int32_t) (TCBS[task_num].ptask_func); //PC - task function address
    TCBS_STACK[task_num][STACKSIZE-3]  = 0xFFFFFFF9;               //LR - Return to Thread mode and use MSP

    //initializing stack with dummy contents for other registers
    for(uint8_t i=4; i<=STACKSIZE; i++)
    {
        TCBS_STACK[task_num][STACKSIZE-i]  = 0xDEADBEEF; //Dummy values
    }
}



//RTOS Kernel Stack Initialize
void rtosKernel_StackInit(void)
{
    //initialize the stack of each task
    for(uint8_t i=0; i<NO_OF_TASKS; i++)
    {
        rtosKernel_TaskStackInit(i);
    }
}


void rtosKernel_TCBInit(void)
{
    //Initialize Linked list of the tasks
    for(uint8_t i=0; i<NO_OF_TASKS; i++)
    {
        //connect to next TCB node
        if(i<NO_OF_TASKS-1)
            TCBS[i].pnext = &(TCBS[i+1]);
    }
    TCBS[NO_OF_TASKS-1].pnext = &(TCBS[0]); //circular linking
}



void rtosKernel_TaskInit(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    DISABLE_IRQ();

    rtosKernel_TCBInit();

    //Initialize the tasks stacks
    rtosKernel_StackInit();

    //Initialize the current pointer of the TCB Linked List
    pcurrent = &(TCBS[0]);

    __task_count_init();

    //Enable all global Interrupts - clearing PRIMASK bit
    ENABLE_IRQ();
}



void rtosKernel_Launch(uint32_t quanta)
{
    //Systick Timer Config
    SYSTICK_DISABLE();
    SYSTICK_CLEAR();
    SYSTICK->CSR |= 1<<SYST_CSR_CLKSOURCE;
    SYSTICK_LOAD((quanta * (SYSCORE_CLK/1000)) - 1);
    SCB->SHPR3 |= 0xFF<<24; //set lowest priority for systick handler
    SYSTICK_ENABLE_INTERRUPT();

    //Initialize the Tasks
    rtosKernel_TaskInit();

    SYSTICK_ENABLE();

    //Launch Scheduler
    rtosScheduler_Launch();
}



static void rtosScheduler_RoundRobin(void)
{
    //go to next task
    pcurrent = pcurrent->pnext;
}



void rtosScheduler_Launch(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    DISABLE_IRQ();


    //Initialize the ARM Cortex M SP
    //load address of the current pointer to R0
    __asm("LDR R0, =pcurrent");
    //load the address of the TCB of the current task into R2
    __asm("LDR R2, [R0]");
    //Load the SP of the current task from the TCB
    __asm("LDR SP, [R2]");
    //Restore R4-R11
    __asm("POP {R4-R11}");
    //Restore R0-R3
    __asm("POP {R0-R3}");
    //Restore R12
    __asm("POP {R12}");
    //Skip LR in the saved stack
    __asm("ADD SP, SP, #4");
    //Create a new starting point by loading the PC from the saved stack of the TCB pointed by pcurrent into the LR
    __asm("POP {LR}");
    //Skip xPSR in the saved stack
    __asm("ADD SP, SP, #4");


    //Enable all global Interrupts - clearing PRIMASK bit
    ENABLE_IRQ();

    //go to the task
    __asm("BX LR");
}



__attribute__((naked)) void SysTick_Handler(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    DISABLE_IRQ();


    //Suspend Current Task - Save its Context
    //Push R4-R11 onto the stack
    __asm("PUSH {R4-R11}");
    //load address of the current pointer to R0
    __asm("LDR R0, =pcurrent");
    //load the address of the TCB of the current task into R1
    __asm("LDR R1, [R0]");
    //Save the SP of the current task into the TCB
    __asm("STR SP, [R1]");


    //Switch to the next task
    //save R0,LR
    __asm("PUSH {R0,LR}");
    //call systick increment function
    __asm("BL Systick_Tick_Inc");
    //call round robin scheduler
    __asm("BL rtosScheduler_RoundRobin");
    //restore R0,LR
    __asm("POP {R0,LR}");

    //load address of the next TCB in R1 (from address at R0)
    __asm("LDR R1, [R0]");
    //Load the SP from the TCB into Cortex-M SP
    __asm("LDR SP, [R1]");
    //Restore R4-R11 into the next task
    __asm("POP {R4-R11}");
    
    
    //Enable all global Interrupts - clearing PRIMASK bit
    ENABLE_IRQ();

    //go to the next task
    __asm("BX LR");
}