#include<stdint.h>

#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_queue.h"
#include "rtos_info.h"
#include "rtos_task.h"
#include "rtos_port.h"



extern tcb_t  TCBS[NO_OF_TASKS+1];                 //an array of TCB's
extern tcb_t *pcurrent;                           //current pointer to a tcb
extern tcb_t* ready_queue[1];
extern tcb_t* blocked_queue[1];
int32_t       TCBS_STACK[NO_OF_TASKS+1][STACKSIZE];     //array for stack for each Task




uint8_t assert(uint8_t condition, char* assert_msg)
{
    if(condition == 0) //assert failed
    {
        Serialprintln(assert_msg, ASSERT);
        while(1);  //stop execution on assert fail

        return 0;
    }
    else //assert succeeded
        return 1;
}



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
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        rtosKernel_TaskStackInit(i);
    }
}


void rtosKernel_TaskInit(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    DISABLE_IRQ();

    //Add Idle Task
    taskAdd_Idle();

    //Initialize the tasks stacks
    rtosKernel_StackInit();

    //Initialize the current pointer of the TCB Linked List
    pcurrent = &(TCBS[1]);

    //Enable all global Interrupts - clearing PRIMASK bit
    ENABLE_IRQ();
}



void rtosKernel_Init()
{
    __task_count_init();
    ready_queue_init();
    blocked_queue_init();
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

    #if INFO_PRINT == 1
    //Print Info
    rtosInfo_Tasks();
    #endif

    //Enable Systick Timer
    SYSTICK_ENABLE();

    //Scheduler Initializations
    //Initialize the first task before Scheduler Launch
    pcurrent->task_state = TASK_STATE_RUNNING;

    #if SCHEDULER == SCHEDULER_ROUND_ROBIN
    ready_queue[0] = pcurrent->pnext;
    #elif SCHEDULER == SCHEDULER_RR_WEIGHTED
    taskReset_Quota();
    pcurrent->task_quota--;
    ready_queue[0] = pcurrent->pnext;
    #endif

    //Launch Scheduler
    rtosScheduler_Launch();
}



void rtosScheduler_RoundRobin(void)
{
    uint8_t state = TASK_STATE_BLOCKED;

    //set task state for finished task (not for idle task)
    if(pcurrent->task_state == TASK_STATE_RUNNING && pcurrent->task_id != 0)
        pcurrent->task_state = TASK_STATE_READY;
    
    //if the ready queue is empty before the next context switch, reset it
    if(ready_queue_check_empty() == 1)
        ready_queue_reset();

    //go through the tasks in the queue
    if(ready_queue[0] != NULL)
    {
        //get the task
        tcb_t* t = ready_queue[0];

        //dequeue
        ready_queue[0] = t->pnext;
        t->pnext = NULL;

        //set the new task as current
        pcurrent = t;

        //get its state
        state = pcurrent->task_state;

        //if the new task is ready and it is not a idle task
        if(state == TASK_STATE_READY && pcurrent->task_id != 0)
        {
            pcurrent->task_state = TASK_STATE_RUNNING;
            return;
        }
    }

    //if all the tasks are blocked, run the idle task
    if(state == TASK_STATE_BLOCKED)
        pcurrent = getTask_Idle();
}



void rtosScheduler_RoundRobinWeighted(void)
{
    uint8_t state = TASK_STATE_BLOCKED;

    //if the ready queue is empty and the current task has no quota, reset the queue and the quota
    if(ready_queue_check_empty() == 1 && pcurrent->task_quota == 0)
    {
        ready_queue_reset();
        taskReset_Quota();

        pcurrent = ready_queue[0];
        pcurrent->task_state = TASK_STATE_RUNNING;
        ready_queue[0] = pcurrent->pnext;
    }

    //context switch
    //go through the tasks in the queue
    if(ready_queue[0] != NULL)
    {   
        //get current task state
        state = pcurrent->task_state;

        //not an idle task
        if(pcurrent->task_id != 0)
        {
            //if the task is running and quota is not 0
            if(state == TASK_STATE_RUNNING && pcurrent->task_quota >= 1)
            {
                //decrease quota
                pcurrent->task_quota--;
            }
            //if the task's quota is 0, then go to next task
            else if(pcurrent->task_quota == 0)
            {   
                //get the task
                tcb_t* t = ready_queue[0];

                //dequeue
                ready_queue[0] = t->pnext;
                t->pnext = NULL;

                //set the new task as current
                pcurrent = t;

                //decrease quota
                pcurrent->task_quota--;

                //set new task's state as running
                if(pcurrent->task_state == TASK_STATE_READY && pcurrent->task_id != 0)
                    pcurrent->task_state = TASK_STATE_RUNNING;
                    

                return;
            }
            //if quota > 0, just return
            else if(pcurrent->task_quota > 0) return;
        }
    }

    //if all the tasks are blocked, run the idle task
    if(state == TASK_STATE_BLOCKED)
        pcurrent = getTask_Idle();
}





__attribute__((naked)) void rtosScheduler_Launch(void)
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
    //Disable all global Interrupts
    DISABLE_IRQ()  ;


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
    //call systick increment
    __asm("BL Systick_Tick_Inc");
    //call task unblock
    __asm("BL taskUnblock");
    //call scheduler
    #if SCHEDULER == SCHEDULER_ROUND_ROBIN
    __asm("BL rtosScheduler_RoundRobin");
    #elif SCHEDULER == SCHEDULER_RR_WEIGHTED
    __asm("BL rtosScheduler_RoundRobinWeighted");
    #endif
    //restore R0,LR
    __asm("POP {R0,LR}");

    //load address of the next TCB in R1 (from address at R0)
    __asm("LDR R1, [R0]");
    //Load the SP from the TCB into Cortex-M SP
    __asm("LDR SP, [R1]");
    //Restore R4-R11 into the next task
    __asm("POP {R4-R11}");
    
    
    //Enable all global Interrupts
    ENABLE_IRQ();

    //go to the next task
    __asm("BX LR");
}