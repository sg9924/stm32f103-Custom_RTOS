#include<stdint.h>

#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_info.h"
#include "rtos_task.h"
#include "rtos_port.h"



extern tcb_t TCBS[NO_OF_TASKS+1];                 //an array of TCB's
extern tcb_t *pcurrent;                           //current pointer to a tcb
int32_t TCBS_STACK[NO_OF_TASKS+1][STACKSIZE];     //array for stack for each Task

//ready and blocked queues for tasks with priority - one linked list per priority
tcb_t* ready_queue[TASK_MAX_PRIORITY];
tcb_t* blocked_queue[TASK_MAX_PRIORITY];


//Ready Queue APIs
void ready_queue_init()
{
    for(uint8_t i=0; i<TASK_MAX_PRIORITY; i++)
        ready_queue[i] = NULL;
}


void ready_queue_reset()
{
    for(uint8_t i=1; i<NO_OF_TASKS+1; i++)
    {
        //if task is not in blocked queue
        if(check_blocked_queue(&TCBS[i]) != 1)
            add_to_ready_queue(&TCBS[i]);
    }
}


uint8_t check_ready_queue_empty()
{
    for(uint8_t i=0; i<TASK_MAX_PRIORITY; i++)
    {
        if(ready_queue[i] == NULL)
            continue;
        else
            return 0;
    }
    return 1;
}


void add_to_ready_queue(tcb_t* task)
{
    //get priority of task
    uint8_t priority = task->task_priority;

    //set state as ready
    task->task_state = TASK_STATE_READY;
    task->pnext      = NULL;

    if(ready_queue[priority] == NULL)
        ready_queue[priority] = task;
    else
    {
        //get the starting task of the specific priority
        tcb_t* i = ready_queue[priority];

        //iterate till the end of the linked list
        while(i->pnext != NULL) i = i->pnext;

        //add the task to the last node in the queue
        i->pnext = task;
    }
}




//Blocked Queue APIs
void blocked_queue_init()
{
    for(uint8_t i=0; i<TASK_MAX_PRIORITY; i++)
        blocked_queue[i] = NULL;
}

uint8_t check_blocked_queue_empty()
{
    for(uint8_t i=0; i<TASK_MAX_PRIORITY; i++)
    {
        if(blocked_queue[i] == NULL)
            continue;
        else
            return 0;
    }
    return 1;
}



uint8_t check_blocked_queue(tcb_t* task)
{
    tcb_t* t;
    for(uint8_t i=0; i<TASK_MAX_PRIORITY; i++)
    {
        t = blocked_queue[i];
        while(t != NULL)
        {
            if(t == task) return 1;
            //else break;

            t = t->pnext;
        }
    }
    return 0;
}



void add_to_blocked_queue(tcb_t* task)
{
    //get priority of task
    uint8_t priority = task->task_priority;

    //set state as blocked
    task->task_state = TASK_STATE_BLOCKED;
    task->pnext      = NULL;

    if(blocked_queue[priority] == NULL)
        blocked_queue[priority] = task;
    else
    {
        //get the starting task of the specific priority
        tcb_t* i = blocked_queue[priority];

        //iterate till the end of the linked list
        while(i->pnext != NULL) i = i->pnext;

        //add the task to the last node in the queue
        i->pnext = task;
    }
}



uint8_t remove_from_blocked_queue(tcb_t* task, uint8_t state)
{
    #if SCHEDULER == SCHEDULER_PRIORITY
        uint8_t priority = task->task_priority;
    #endif

    if(blocked_queue[priority] != NULL)
    {
        //get the starting task of the specific priority
        tcb_t* i = blocked_queue[priority];

        //starting task of queue matches
        if(i == task)
        {
            i->task_state = state;
            i->block_tick = 0;
            blocked_queue[priority] = i->pnext;
        }
        //first task didn't match
        else
        {   
            //iterate
            while(i->pnext != NULL)
            {
                if(i->pnext == task)
                {
                    (i->pnext)->task_state = state;
                    (i->pnext)->block_tick = 0;
                    i = (i->pnext)->pnext;
                    return 1;
                }
                i = i->pnext;
            }
        }
    }
    return 0;
}




//Assert
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


//Task Stack Initialize
void rtosKernel_TaskStackInit(uint8_t task_num)
{
    //initialize stack pointer
    TCBS[task_num].pstack = &(TCBS_STACK[task_num][STACKSIZE-16]);

    //set T bit (bit 24) in xPSR to indicate Thumb state
    TCBS_STACK[task_num][STACKSIZE-1] = (1<<24);

    //initialize PC and LR
    TCBS_STACK[task_num][STACKSIZE-2] = (int32_t) (TCBS[task_num].ptask_func); //PC - task function address
    #if USE_PSP == 0
    TCBS_STACK[task_num][STACKSIZE-3]  = 0xFFFFFFF9;               //LR - Return to Thread mode and use MSP
    #elif USE_PSP == 1
    TCBS_STACK[task_num][STACKSIZE-3]  = 0xFFFFFFFD;               //LR - Return to Thread mode and use PSP
    #endif

    //initializing stack with dummy contents for other registers
    for(uint8_t i=4; i<=STACKSIZE; i++)
    {
        TCBS_STACK[task_num][STACKSIZE-i]  = 0xDEADBEEF; //Dummy values
    }
}



//Kernel Stack Initialize
void rtosKernel_StackInit(void)
{
    //initialize the stack of each task
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        rtosKernel_TaskStackInit(i);
    }
}


//Task Initialize
void rtosKernel_TaskInit(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    DISABLE_IRQ();

    //Add Idle Task
    taskAdd_Idle();

    //Initialize the TCB Linked List Structure
    //rtosKernel_TCBInit();

    //Initialize the tasks stacks
    rtosKernel_StackInit();

    //Initialize the current pointer of the TCB Linked List
    #if SCHEDULER == SCHEDULER_PRIORITY
    pcurrent = getTask_Priority(0);
    #endif

    //Enable all global Interrupts - clearing PRIMASK bit
    ENABLE_IRQ();
}



void rtosKernel_Init()
{
    __task_count_init();
    #if SCHEDULER == SCHEDULER_PRIORITY
    //initialize the ready and blocked queue
    ready_queue_init();
    blocked_queue_init();
    #endif
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

    //Scheduler Initializations
    //Initialize the first task before Scheduler Launch
    pcurrent->task_state = TASK_STATE_RUNNING;

    #if SCHEDULER == SCHEDULER_PRIORITY
    ready_queue[pcurrent->task_priority] = pcurrent->pnext;
    #endif

    //Enable Systick Timer
    SYSTICK_ENABLE();

    //Launch Scheduler
    rtosScheduler_Launch();
}




void rtosScheduler_Priority()
{
    int8_t state = TASK_STATE_BLOCKED;

    //set task state for finished task (not for idle task)
    if(pcurrent->task_state == TASK_STATE_RUNNING && pcurrent->task_id != 0)
        pcurrent->task_state = TASK_STATE_READY;

    //if the ready queue is empty before the next context switch, reset it
    if(check_ready_queue_empty() == 1)
        ready_queue_reset();

    //loop through the queues starting with the highest priority (0) one
    for(uint8_t i=0; i<TASK_MAX_PRIORITY; i++)
    {
        if(ready_queue[i] != NULL)
        {
            //get the task
            tcb_t* t = ready_queue[i];

            //dequeue
            ready_queue[i] = t->pnext;
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
    }

    //in case of idle task
    pcurrent = getTask_Idle();
    return;
}



//Launch Scheduler
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


//Systick IRQ Handler
__attribute__((naked)) void SysTick_Handler(void)
{
    //Disable all global Interrupts
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
    //call systick increment
    __asm("BL Systick_Tick_Inc");
    //call task unblock
    __asm("BL taskUnblock");
    //call scheduler
    #if SCHEDULER == SCHEDULER_PRIORITY
    __asm("BL rtosScheduler_Priority");
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