#include<stdint.h>

#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_info.h"
#include "rtos_task.h"
#include "rtos_port.h"


//external variables
extern tcb_t TCBS[NO_OF_TASKS+1];                 //an array of TCB's
extern tcb_t *pcurrent;                           //current pointer to a tcb


int32_t TCBS_STACK[NO_OF_TASKS+1][STACKSIZE];     //array for stack for each Task
tcb_t* ready_queue[MAX_NO_OF_PRIORITY];            //ready queues for each priority
tcb_t* blocked_queue[MAX_NO_OF_PRIORITY];          //blocked queues for each priority



static void ready_queue_init();
static void ready_queue_reset();
static uint8_t ready_queue_check_empty();

static void blocked_queue_init();
static uint8_t blocked_queue_check_empty();
static uint8_t blocked_queue_check(tcb_t* task);


static void rtosKernel_TaskStackInit(uint8_t task_num);
static void rtosKernel_StackInit(void);
static void rtosKernel_TaskInit(void);
__attribute__((naked)) void rtosScheduler_Launch(void);


static void rtosScheduler_Priority();

/****************************************************Ready Queue APIs Start*****************************************************/
static void ready_queue_init()
{
    for(uint8_t i=0; i<MAX_NO_OF_PRIORITY; i++)
        ready_queue[i] = NULL;
}


static void ready_queue_reset()
{
    for(uint8_t i=1; i<NO_OF_TASKS+1; i++)
    {
        //if task is not in blocked queue
        if(blocked_queue_check(&TCBS[i]) != 1)
            ready_queue_add(&TCBS[i]);
    }
}


static uint8_t ready_queue_check_empty()
{
    for(uint8_t i=0; i<MAX_NO_OF_PRIORITY; i++)
    {
        if(ready_queue[i] == NULL)
            continue;
        else
            return 0;
    }
    return 1;
}


void ready_queue_add(tcb_t* task)
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


uint8_t ready_queue_remove(tcb_t* task, uint8_t state)
{
    #if SCHEDULER == SCHEDULER_PRIORITY
        uint8_t priority = task->task_priority;
    #endif

    if(ready_queue[priority] != NULL)
    {
        //get the starting task of the specific priority
        tcb_t* i = ready_queue[priority];

        //starting task of queue matches
        if(i == task)
        {
            i->task_state = state;
            ready_queue[priority] = i->pnext;
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
                    i = (i->pnext)->pnext;
                    return 1;
                }
                i = i->pnext;
            }
        }
    }
    return 0;
}
/****************************************************Ready Queue APIs End*******************************************************/

/***************************************************Blocked Queue APIs Start****************************************************/
static void blocked_queue_init()
{
    for(uint8_t i=0; i<MAX_NO_OF_PRIORITY; i++)
        blocked_queue[i] = NULL;
}

static uint8_t blocked_queue_check_empty()
{
    for(uint8_t i=0; i<MAX_NO_OF_PRIORITY; i++)
    {
        if(blocked_queue[i] == NULL)
            continue;
        else
            return 0;
    }
    return 1;
}



static uint8_t blocked_queue_check(tcb_t* task)
{
    tcb_t* t;
    for(uint8_t i=0; i<MAX_NO_OF_PRIORITY; i++)
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



void blocked_queue_add(tcb_t* task)
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



uint8_t blocked_queue_remove(tcb_t* task, uint8_t state)
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
/**************************************************Blocked Queue APIs End***************************************************/
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
/*******************************************************Kernel APIs Start****************************************************/
//Task Stack Initialize
static void rtosKernel_TaskStackInit(uint8_t task_num)
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



//Kernel Stack Initialize
static void rtosKernel_StackInit(void)
{
    //initialize the stack of each task
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        rtosKernel_TaskStackInit(i);
    }
}


//Task Initialize
static void rtosKernel_TaskInit(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    DISABLE_IRQ();

    //Add Idle Task
    taskAdd_Idle();

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

    //set systick clock source
    SYSTICK_CLK_SRC_SET(SYSTICK_CLK_SRC_AHB_DIV_8);
    
    //load systick based on its clock source
    if(SYSTICK_GET_CLK_SRC() == SYSTICK_CLK_SRC_AHB)
        SYSTICK_LOAD((quanta * (RCC_Get_SYSCLK()/1000)) - 1);
    //AHB by 8
    else
        SYSTICK_LOAD((quanta * ((RCC_Get_SYSCLK()/8)/1000)) - 1);

    SCB->SHPR3 |= 0xFF<<24; //set lowest priority for systick handler

    SYSTICK_ENABLE_INTERRUPT();

    //Initialize the Tasks
    rtosKernel_TaskInit();

    #if INFO_PRINT == 1
    //Print Info
    rtosInfo_Tasks();
    #endif

    //Scheduler Initializations
    #if SCHEDULER == SCHEDULER_PRIORITY
    ready_queue[pcurrent->task_priority] = pcurrent->pnext;
    #endif

    //Initialize the first task before Scheduler Launch
    pcurrent->task_state = TASK_STATE_RUNNING;

    //Enable Systick Timer
    SYSTICK_ENABLE();

    //Launch Scheduler
    rtosScheduler_Launch();
}
/*******************************************************Kernel APIs End****************************************************/

/*****************************************************Scheduler APIs Start*************************************************/
static void rtosScheduler_Priority()
{
    int8_t state = TASK_STATE_BLOCKED;
    uint8_t loop_priority = MAX_NO_OF_PRIORITY-1;

    //current task - still running and its not a idle task
    if(pcurrent->task_state == TASK_STATE_RUNNING && pcurrent->task_id != 0)
    {
        //set loop limit for pirority
        loop_priority = pcurrent->task_priority;
        //add to ready queue
        ready_queue_add(pcurrent);
    }
    //idle task
    else if (pcurrent->task_id == 0)
    {
        pcurrent->task_state = TASK_STATE_READY;
        //ready_queue_remove(pcurrent, TASK_STATE_READY);
    }


    //loop through the queue starting with the highest priority (0) one
    for(uint8_t i=0; i<=loop_priority; i++)
    {
        if(ready_queue[i] != NULL)
        {
            //get the next task in ready queue
            tcb_t* t = ready_queue[i];

            //check priority
            //high or equal priority - task preemption should occur
            //current task is blocked - task preemption should occur
            if(t->task_priority <= pcurrent->task_priority || pcurrent->task_state == TASK_STATE_BLOCKED)
            {
                //dequeue the task
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
    }

    //loop through ready queue completed
    //no task found to preempt & current task is blocked
    //idle task should be selected for preemption
    if(state == TASK_STATE_BLOCKED)
    {
        pcurrent = getTask_Idle();
        pcurrent->task_state = TASK_STATE_RUNNING;
    }
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
/*****************************************************Scheduler APIs End*************************************************/



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

    #if BOARD_INIT_LED == 1 && SYSTICK_LED_TOGGLE == 1
    //led toggle for visuals
    __asm("BL led_toggle");
    #endif

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