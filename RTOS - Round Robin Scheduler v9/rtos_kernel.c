#include<stdint.h>

#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_queue.h"
#include "rtos_info.h"
#include "rtos_task.h"
#include "rtos_port.h"



extern tcb_t   TCBS[NO_OF_TASKS+1];                       //an array of TCB's
extern tcb_t   *pcurrent;                                 //current pointer to a tcb

uint32_t       TCBS_STACK_POOL[STACK_MAX_POOLSIZE];       //Stack Pool Array
uint32_t       stack_pool_index = 0;                      //Keeps track of memory used (in terms of words)

tcb_t*         ready_queue[1];
tcb_t*         blocked_queue[1];



static void ready_queue_init();
static void ready_queue_reset();
static uint8_t ready_queue_check_empty();

static void blocked_queue_init();
static uint8_t blocked_queue_check_empty();
static uint8_t blocked_queue_check(tcb_t* task);


static void rtosKernel_TaskStackInit(uint8_t task_num);
static void rtosKernel_StackInit(void);
static void rtosKernel_TaskInit(void);
__attribute__((naked)) static void rtosScheduler_Launch(void);



/****************************************************Ready Queue APIs Start*****************************************************/
static void ready_queue_init()
{
    ready_queue[0] = NULL;
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
    if(ready_queue[0] == NULL)
        return 1;
    else
        return 0; 
}


void ready_queue_add(tcb_t* task)
{
    //set state as ready
    task->task_state = TASK_STATE_READY;
    task->pnext      = NULL;

    if(ready_queue[0] == NULL)
        ready_queue[0] = task;
    else
    {
        //get the starting task
        tcb_t* i = ready_queue[0];

        //iterate till the end of the linked list
        while(i->pnext != NULL) i = i->pnext;

        //add the task to the last node in the queue
        i->pnext = task;
    }
}


uint8_t ready_queue_remove(tcb_t* task, uint8_t state)
{
    if(ready_queue[0] != NULL)
    {
        //get the starting task
        tcb_t* i = ready_queue[0];

        //starting task of queue matches
        if(i == task)
        {
            i->task_state = state;
            ready_queue[0] = i->pnext;
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
    blocked_queue[0] = NULL;
}

static uint8_t blocked_queue_check_empty()
{
    if(blocked_queue[0] == NULL)
        return 1;
    else
        return 0;
}



static uint8_t blocked_queue_check(tcb_t* task)
{
    tcb_t* t = blocked_queue[0];

    while(t != NULL)
    {
        if(t == task) return 1;
        else break;

        t = t->pnext;
    }
    return 0;
}



void blocked_queue_add(tcb_t* task)
{
    //set state as blocked
    task->task_state = TASK_STATE_BLOCKED;
    task->pnext      = NULL;

    if(blocked_queue[0] == NULL)
        blocked_queue[0] = task;
    else
    {
        //get the starting task
        tcb_t* i = blocked_queue[0];

        //iterate till the end of the linked list
        while(i->pnext != NULL) i = i->pnext;

        //add the task to the last node in the queue
        i->pnext = task;
    }
}



uint8_t blocked_queue_remove(tcb_t* task, uint8_t state)
{
    if(blocked_queue[0] != NULL)
    {
        //get the starting task
        tcb_t* i = blocked_queue[0];

        //starting task of queue matches
        if(i == task)
        {
            i->task_state = state;
            i->block_tick = 0;
            blocked_queue[0] = i->pnext;
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
    //assert failed
    if(condition == 0)
    {
        Serialprintln(assert_msg, ASSERT);
        while(1);  //stop execution on assert fail
        return 0;
    }
    //assert succeeded
    return 1;
}
/*******************************************************Kernel APIs Start****************************************************/
uint32_t* Stack_Allocate(uint32_t size_in_words)
{
    //Align to 8 bytes
    stack_pool_index  = WORD_ALIGN_8BYTE(stack_pool_index);
    size_in_words     = WORD_ALIGN_8BYTE(size_in_words);

    //check for overflow
    if(stack_pool_index + size_in_words > STACK_MAX_POOLSIZE)
        return NULL; //Out of Memory

    //get pointer to memory just after the last allocated task stack memory
    //this pointer returns the lower memory address
    uint32_t* new_ptr = &TCBS_STACK_POOL[stack_pool_index];

    //update the pool index
    stack_pool_index += size_in_words;

    //return the higher memory address outside the new block as the stack is descending with decrement first
    return (new_ptr + size_in_words);
}



//Task Stack Initialize
static void rtosKernel_TaskStackInit(uint8_t task_num)
{
    //Get Stack Top
    uint32_t* pstack = TCBS[task_num].pstack;
    pstack--;

    //reassign stack pointer with the registers pushed into the task stack for scheduler launch
    TCBS[task_num].pstack = (uint32_t*)((uint32_t)TCBS[task_num].pstack - (16*4));

    //set T bit (bit 24) in xPSR to indicate Thumb state
    *pstack = (1<<24);                                         //xPSR

    //initialize PC and LR
    pstack--; *pstack = (int32_t)(TCBS[task_num].ptask_func);  //PC - task function address

    //below line for MSP usage only
    //pstack--; *pstack = 0xFFFFFFF9;                            //LR - Return to Thread mode and use MSP (not yet introduced PSP for tasks)

    //below line for PSP uasage for tasks and MSP for rest
    pstack--; *pstack = 0xFFFFFFFD;                            //LR - Return to Thread mode and use PSP

    //initializing stack with dummy contents for other registers
    for(int16_t i=4; i<=16; i++)
    {
        pstack--; *pstack = 0;                                //Dummy values
    }

    //stack coloring for rest of the stack
    for(int16_t i=17; i<=TCBS[task_num].stack_size_word; i++)
    {
        pstack--; *pstack  = STACKCOLOR_VALUE;                 //Stack Color Value
    }
}



//RTOS Kernel Stack Initialize
static void rtosKernel_StackInit(void)
{
    //initialize the stack of each task
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        rtosKernel_TaskStackInit(i);
    }
}


static void rtosKernel_TaskInit(void)
{
    //Disable all global Interrupts - Setting PRIMASK bit
    CRITICAL_SECTION_ENTER(key);

    //Add Idle Task
    taskAdd_Idle();

    //Initialize the tasks stacks
    rtosKernel_StackInit();

    //Initialize the current pointer of the TCB Linked List
    pcurrent = &(TCBS[1]);

    //Enable all global Interrupts - clearing PRIMASK bit
    CRITICAL_SECTION_EXIT(key);
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

    //set systick clock source
    SYSTICK_CLK_SRC_SET(SYSTICK_CLK_SRC_AHB_DIV_8);
    
    //load systick based on its clock source
    if(SYSTICK_GET_CLK_SRC() == SYSTICK_CLK_SRC_AHB)
        SYSTICK_LOAD((quanta * (RCC_Get_SYSCLK()/1000)) - 1);
    //AHB by 8
    else
        SYSTICK_LOAD((quanta * ((RCC_Get_SYSCLK()/8)/1000)) - 1);

    //Clear the current priority configurations for both lanes
    //PendSV sits in bits [23:16], SysTick sits in bits [31:24]
    SCB->SHPR3 &= ~(0xFFFF0000UL);

    //Set both SysTick and PendSV to the lowest priority (0xFF)
    //PendSV:  0xFF shifted by 16 bits -> 0x00FF0000
    //SysTick: 0xFF shifted by 24 bits -> 0xFF000000
    SCB->SHPR3 |= (0xFFUL << 16) | (0xFFUL << 24);

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
    //dequeue the current task to be run
    ready_queue[0] = pcurrent->pnext;
    #elif SCHEDULER == SCHEDULER_RR_WEIGHTED
    taskReset_QuotaAll();
    pcurrent->task_quota--;
    ready_queue[0] = pcurrent->pnext;
    #endif

    //Launch Scheduler
    rtosScheduler_Launch();
}
/*******************************************************Kernel APIs End****************************************************/

/*****************************************************Scheduler APIs Start*************************************************/
#if SCHEDULER == SCHEDULER_ROUND_ROBIN
void rtosScheduler_RoundRobin(void)
{
    DISABLE_IRQ();

    //finished task (not idle task)
    if(pcurrent->task_state == TASK_STATE_RUNNING && pcurrent->task_id != 0)
    {
        //add to ready queue
        ready_queue_add(pcurrent);
    }
    //idle task
    else if (pcurrent->task_id == 0)
    {
        pcurrent->task_state = TASK_STATE_READY;
    }

    //get the next task
    if(ready_queue[0] != NULL)
    {
        //get the next task
        tcb_t* t = ready_queue[0];

        //dequeue the task which is to be run next
        ready_queue[0] = t->pnext;
        t->pnext = NULL;

        //set the new task as current
        pcurrent = t;

        //if the new task is ready and it is not a idle task
        if(pcurrent->task_state == TASK_STATE_READY && pcurrent->task_id != 0)
        {
            pcurrent->task_state = TASK_STATE_RUNNING;
        }
    }

    //Ready queue is empty: all tasks are blocked including the current task or idle task is only running
    if(ready_queue[0] == NULL && (pcurrent->task_state == TASK_STATE_BLOCKED || pcurrent->task_id == 0))
    {
        pcurrent = getTask_Idle();
        pcurrent->task_state = TASK_STATE_RUNNING;
    }

    ENABLE_IRQ();
    return;
}
#endif



#if SCHEDULER == SCHEDULER_RR_WEIGHTED
void rtosScheduler_RoundRobinWeighted(void)
{
    DISABLE_IRQ();

    //get the next task
    if(ready_queue[0] != NULL)
    {   
        //not an idle task
        if(pcurrent->task_id != 0)
        {
            //if the task is running and quota is not 0
            if(pcurrent->task_state == TASK_STATE_RUNNING && pcurrent->task_quota >= 1)
            {
                //decrease quota
                pcurrent->task_quota--;
                return;
            }
            //if the task's quota is 0, then go to next task
            else if(pcurrent->task_state == TASK_STATE_RUNNING && pcurrent->task_quota == 0)
            {
                //reset quota of current task
                taskReset_Quota(pcurrent);
                //add current task to ready queue
                ready_queue_add(pcurrent);

                //get the next task
                tcb_t* t = ready_queue[0];

                //dequeue the task which is to be run next
                ready_queue[0] = t->pnext;
                t->pnext       = NULL;

                //set the new task as current
                pcurrent = t;

                //set new task's state as running
                if(pcurrent->task_state == TASK_STATE_READY && pcurrent->task_id != 0 && pcurrent->task_quota != 0)
                {
                    pcurrent->task_state = TASK_STATE_RUNNING;
                    //decrease quota
                    pcurrent->task_quota--;
                    return;
                }
            }
            //if quota > 0, just return
            else if(pcurrent->task_state == TASK_STATE_RUNNING && pcurrent->task_quota > 0) return;
        }
        //idle task
        else if (pcurrent->task_id == 0)
        {
            pcurrent->task_state = TASK_STATE_READY;
            return;
        }
    }

    //Ready queue is empty: all tasks are blocked including the current task or idle task is only running
    if(ready_queue[0] == NULL && (pcurrent->task_state == TASK_STATE_BLOCKED || pcurrent->task_id == 0))
    {
        pcurrent             = getTask_Idle();
        pcurrent->task_state = TASK_STATE_RUNNING;
    }

    ENABLE_IRQ();
    return;
}
#endif




__attribute__((naked)) static void rtosScheduler_Launch(void)
{
    __asm volatile (
        //Reset MSP to the top of stack (from vector table[0])
        //reclaims any boot-time/kernel-init stack usage before we leave it forever
        "LDR R0, =0xE000ED08    \n"   //VTOR address
        "LDR R0, [R0]           \n"   //R0 = vector table base address
        "LDR R0, [R0]           \n"   //R0 = initial MSP value (vector table entry 0)
        "MSR MSP, R0            \n"   //Reset MSP

        //Enable interrupts (in case they were left disabled from boot/init code)
        "CPSIE I                \n"
        "CPSIE F                \n"
        "DSB                    \n"
        "ISB                    \n"

        //Trigger SVC exception - this actually starts the first task
        "SVC #0                 \n"
        "NOP                    \n"
        "NOP                    \n"
    );
}



//SVC Handler - starts the first task
__attribute__((naked)) void SVC_Handler(void)
{
    __asm volatile (
        //Get address of pcurrent, then the current TCB pointer, then the task's saved SP
        "LDR R3, =pcurrent      \n"   //R3 = &pcurrent
        "LDR R1, [R3]           \n"   //R1 = pcurrent (TCB pointer)
        "LDR R0, [R1]           \n"   //R0 = pcurrent->pstack (task's saved SP, points at R4 slot)

        //Manually restore R4-R11 (software-saved registers, not part of
        //automatic hardware exception frame)
        "LDMIA R0!, {R4-R11}    \n"   //R0 now points at the R0 slot of the fake frame

        //Point PSP at the remainder of the frame (R0-R3,R12,LR,PC,xPSR)
        //These are NOT restored here - hardware will auto-unstack them on exception return
        "MSR PSP, R0            \n"
        "ISB                    \n"

        //Force EXC_RETURN to: return to Thread mode, use PSP
        //(0xFFFFFFFD pattern - set bit 2, and ensure bits 0/1/3+ are already correct
        //from the EXC_RETURN value the SVC entry itself gave us in LR)
        "ORR LR, LR, #0x04      \n"

        //Exception return - hardware automatically pops R0-R3,R12,LR,PC,xPSR
        //from PSP (since bit 2 of LR/EXC_RETURN = 1), landing directly in the task
        "BX LR                  \n"
    );
}
/*****************************************************Scheduler APIs End*************************************************/

//Systick IRQ Handler (Standard C Function now, not naked)
void SysTick_Handler(void)
{
    DISABLE_IRQ();

    //LED Toggle for activity indicator
    #if BOARD_INIT_LED == 1 && SYSTICK_LED_TOGGLE == 1
    led_toggle();
    #endif

    //Increment SysTick
    Systick_Tick_Inc();
    //Unblock valid tasks
    taskCheck_BlockedListTimeout();

    //Pend the PendSV Exception to handle context switch
    INTCTRL = PENDSVSET;

    ENABLE_IRQ();
}


//PendSV IRQ Handler - Handles the actual context switch
__attribute__((naked)) void PendSV_Handler(void)
{
    DISABLE_IRQ();

    //Get MSP of Current Task
    __asm("MRS R0, PSP");
    //Save its remaining software context: R4-R11, on PSP
    __asm("STMDB R0!, {R4-R11}");
    
    //Load address of pcurrent and save the current SP (PSP) into the TCB
    //load address of the current pointer to R1
    __asm("LDR R1, =pcurrent");
    //load address of the stack pointer to R2
    __asm("LDR R2, [R1]");
    //Save the updated PSP of the current task into the TCB
    __asm("STR R0, [R2]");

    //Save the EXC_RETURN value (LR) onto the stack 
    //before making a function call, otherwise 'BL' will overwrite it.
    //save LR
    __asm("PUSH {LR}");

    //Call the scheduler to select the next task.
    //This updates the 'pcurrent' pointer variable to points to the new TCB.
    #if SCHEDULER == SCHEDULER_ROUND_ROBIN
    __asm("BL rtosScheduler_RoundRobin");
    #elif SCHEDULER == SCHEDULER_RR_WEIGHTED
    __asm("BL rtosScheduler_RoundRobinWeighted");
    #endif

    //Restore the EXC_RETURN value back into LR
    //pop LR
    __asm("POP {LR}");

    //Reload the address of pcurrent which now points to the new task
    __asm("LDR R1, =pcurrent");
    //load address of the next TCB in R1 (from address at R1)
    __asm("LDR R2, [R1]");
    //Load the PSP from the new task's TCB
    __asm("LDR R0, [R2]");
    
    //Restore R4-R11 from the new task's stack
    __asm("LDMIA R0!, {R4-R11}");

    //Write the updated SP back into PSP
    __asm("MSR PSP, R0");
    
    ENABLE_IRQ();

    //Return from exception, automatic pop of registers R0-R3, R12, LR, PC, xPSR
    __asm("BX LR");
}