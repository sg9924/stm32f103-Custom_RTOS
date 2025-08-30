#include<stdint.h>

#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "stm32f103xx_serial.h"


tcb_t TCBS[NO_OF_TASKS];  //declare an array of TCB's
tcb_t *pcurrent;          //current pointer to a tcb


int32_t TCBS_STACK[NO_OF_TASKS][STACKSIZE];     //array for stack for each Task


//RTOS Kernel Stack Initialize
//MSP is being used for both Tasks and Kernel
void rtosKernel_StackInit(void)
{
    //initialize the stack pointer
    for(uint8_t i=0; i<NO_OF_TASKS; i++)
    {
        TCBS[i].pstack = &(TCBS_STACK[i][STACKSIZE-16]);

        //set T bit (bit 24) in xPSR to indicate Thumb state
        TCBS_STACK[i][STACKSIZE-1] = (1<<24);

        //initializing stack with dummy contents for other registers
        //skipping PC ...
        TCBS_STACK[i][STACKSIZE-3]  = 0xFFFFFFF9; //LR - Return to Thread mode and use MSP
        TCBS_STACK[i][STACKSIZE-4]  = 0xAAAAAA12; //R12
        TCBS_STACK[i][STACKSIZE-5]  = 0xAAAAAAA3; //R3
        TCBS_STACK[i][STACKSIZE-6]  = 0xAAAAAAA2; //R2
        TCBS_STACK[i][STACKSIZE-7]  = 0xAAAAAAA1; //R1
        TCBS_STACK[i][STACKSIZE-8]  = 0xAAAAAAA0; //R0
        
        TCBS_STACK[i][STACKSIZE-9]  = 0xAAAAAA11; //R11
        TCBS_STACK[i][STACKSIZE-10] = 0xAAAAAA10; //R10
        TCBS_STACK[i][STACKSIZE-11] = 0xAAAAAAA9; //R9
        TCBS_STACK[i][STACKSIZE-12] = 0xAAAAAAA8; //R8
        TCBS_STACK[i][STACKSIZE-13] = 0xAAAAAAA7; //R7
        TCBS_STACK[i][STACKSIZE-14] = 0xAAAAAAA6; //R6
        TCBS_STACK[i][STACKSIZE-15] = 0xAAAAAAA5; //R5
        TCBS_STACK[i][STACKSIZE-16] = 0xAAAAAAA4; //R4
    }
}


//Task Initialize
//Create Linked List of Tasks
//Initialize PC for each task
//Initialize Stack for each Task
//Initialize Current Task Pointer
void rtosKernel_TaskInit(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    __asm("CPSID I");

    //Initialize Linked list of the tasks
    for(uint8_t i=0; i<NO_OF_TASKS-1; i++)
    {
        TCBS[i].pnext = &(TCBS[i+1]);
    }
    TCBS[NO_OF_TASKS-1].pnext = &(TCBS[0]); //circular linking

    //Initialize the tasks stacks
    rtosKernel_StackInit();

    //get the list of tasks
    ptask_t* task_list = getTaskList();

    //Initialize the PC for each task
    for(uint8_t i=0; i<NO_OF_TASKS; i++)
    {
        TCBS_STACK[i][STACKSIZE-2] = (int32_t) (*(task_list + i));
    }

    //Initialize the current pointer of the TCB Linked List
    pcurrent = &(TCBS[0]);

    //Enable all global Interrupts - clearing PRIMASK bit
    __asm("CPSIE I");
}