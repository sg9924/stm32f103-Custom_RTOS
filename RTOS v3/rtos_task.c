#include "rtos_task.h"
#include "rtos_kernel.h"
#include "rtos_port.h"


//external variables
extern uint32_t current_tick;
extern tcb_t* ready_queue[TASK_MAX_NO_OF_PRIORITY];
extern tcb_t* blocked_queue[TASK_MAX_NO_OF_PRIORITY];

tcb_t TCBS[NO_OF_TASKS+1];  //declare an array of TCB's
tcb_t *pcurrent;            //current pointer to a tcb

static ptask_t ptask_list[NO_OF_TASKS + 1];
static uint8_t task_count;

static void taskIdle(void);
static void taskAdd_Check(uint8_t task_count);




void __task_count_init(void)
{
    task_count=1;
}


static void taskAdd_Check(uint8_t task_count)
{
    if(task_count>NO_OF_TASKS)
    {
        Serialprintln("Incorrect Task Add!! | Configured Tasks: %d | Current Task No.: %d", FATAL, NO_OF_TASKS, task_count);
        SERIAL_NL();
        __asm("BKPT #0");
    }
}



#if SCHEDULER == SCHEDULER_ROUND_ROBIN
tcb_t* taskAdd(ptask_t func_ptr, char* task_desc, uint8_t stack_size_word)
{
    taskAdd_Check(task_count);

    ptask_list[task_count]             = func_ptr;
    
    TCBS[task_count].ptask_func        = func_ptr;
    TCBS[task_count].task_id           = task_count;
    TCBS[task_count].task_state        = TASK_STATE_READY;
    TCBS[task_count].task_desc         = task_desc;
    TCBS[task_count].block_tick        = 0;

    TCBS[task_count].stack_size_word   = stack_size_word;
    TCBS[task_count].pstack            = Stack_Allocate(stack_size_word);

    assert((TCBS[task_count].pstack!=NULL), "Stack Allocation Failure");
    assert((stack_size_word != 0), "Invalid Stack Size during Task Add");

    ready_queue_add(&TCBS[task_count]);

    Serialprintln("'%s' task has been added", INFO, TCBS[task_count].task_desc);
    return &TCBS[task_count++];
}
#endif



#if SCHEDULER == SCHEDULER_RR_WEIGHTED
tcb_t* taskAdd_Weighted(ptask_t func_ptr, char* task_desc, uint8_t task_weight, uint8_t stack_size_word)
{
    taskAdd_Check(task_count);

    ptask_list[task_count]             = func_ptr;
    
    TCBS[task_count].ptask_func        = func_ptr;
    TCBS[task_count].task_id           = task_count;
    TCBS[task_count].task_state        = TASK_STATE_READY;
    TCBS[task_count].task_desc         = task_desc;
    TCBS[task_count].task_weight       = task_weight;
    TCBS[task_count].block_tick        = 0;

    TCBS[task_count].stack_size_word   = stack_size_word;
    TCBS[task_count].pstack            = Stack_Allocate(stack_size_word);
    
    assert((TCBS[task_count].pstack!=NULL), "Stack Allocation Failure");
    assert((stack_size_word != 0), "Invalid Stack Size during Task Add");

    ready_queue_add(&TCBS[task_count]);

    Serialprintln("'%s' task has been added", INFO, TCBS[task_count].task_desc);
    return &TCBS[task_count++];
}
#endif


#if SCHEDULER == SCHEDULER_PRIORITY
tcb_t* taskAdd_Priority(ptask_t func_ptr, char* task_desc, uint8_t task_priority, uint8_t stack_size_word)
{
    taskAdd_Check(task_count);
    
    ptask_list[task_count]               = func_ptr;
    
    TCBS[task_count].ptask_func          = func_ptr;
    TCBS[task_count].task_id             = task_count;
    TCBS[task_count].task_state          = TASK_STATE_READY;
    TCBS[task_count].task_desc           = task_desc;
    TCBS[task_count].task_priority       = task_priority;
    TCBS[task_count].block_tick          = 0;

    TCBS[task_count].stack_size_word     = stack_size_word;
    TCBS[task_count].pstack              = Stack_Allocate(stack_size_word);
    
    assert((TCBS[task_count].pstack!=NULL), "Stack Allocation Failure");
    assert((stack_size_word != 0), "Invalid Stack Size during Task Add");

    ready_queue_add(&TCBS[task_count]);

    Serialprintln("'%s' task has been added", INFO, TCBS[task_count].task_desc);
    return &TCBS[task_count++];
}
#endif



#if SCHEDULER == SCHEDULER_RR_WEIGHTED
void taskReset_Quota(tcb_t* task)
{
    if(!task) return;
    if(task->task_state == TASK_STATE_READY || task->task_state == TASK_STATE_RUNNING)
        task->task_quota = task->task_weight;
}



void taskReset_QuotaAll()
{
    tcb_t* tcb = TCBS;
    if(!tcb) return;
    do
    {
        if(tcb->task_state == TASK_STATE_READY || tcb->task_state == TASK_STATE_RUNNING)
            tcb->task_quota = tcb->task_weight;
        tcb = tcb + 1;
    }while(tcb <= (TCBS + NO_OF_TASKS));
}
#endif


void taskAdd_Idle()
{
    TCBS[0].ptask_func         = &taskIdle;
    TCBS[0].task_id            = 0;
    TCBS[0].task_state         = TASK_STATE_READY;
    TCBS[0].task_desc          = "Idle Task";
    #if SCHEDULER == SCHEDULER_RR_WEIGHTED
    TCBS[0].task_weight        = 1;
    TCBS[0].task_quota         = 0;
    #endif
    #if SCHEDULER == SCHEDULER_PRIORITY
    TCBS[0].task_priority      = TASK_MAX_NO_OF_PRIORITY-1;  //lowest priority
    #endif
    TCBS[0].block_tick         = 0;

    TCBS[0].stack_size_word    = 100;
    TCBS[0].pstack             = Stack_Allocate(100);
    assert((TCBS[0].pstack!=NULL), "Stack Allocation Failure");
}



void taskIdle(void)
{
    while(1)
    {
        #if IDLE_TASK_PRINT == 1
            #if SCHEDULER == SCHEDULER_ROUND_ROBIN || SCHEDULER == SCHEDULER_RR_WEIGHTED
            Serialprintln("[Tick: %x] [ID: %d] No Tasks to run...", INFO, current_tick, TCBS[0].task_id);
            #elif SCHEDULER == SCHEDULER_PRIORITY
            Serialprintln("[Tick: %x] [ID: %d] [Priority: %d] No Tasks to run...", INFO, current_tick, TCBS[0].task_id, TCBS[0].task_priority);
            #endif
        #endif
    }
}



void taskDelay(uint32_t delay_tick)
{
    if(delay_tick == 0) return;
    taskBlock(NULL, delay_tick);
}


void taskBlock(tcb_t* task, uint32_t timeout_tick)
{
    if(timeout_tick == 0) return;
    
    //current task is assumed if no task is passed as input
    if(task == NULL)
        task = pcurrent;
    
    //task should not be idle task and it should not be blocked already
    if(task->task_id != 0 && task->task_state != TASK_STATE_BLOCKED)
    {
        //set task as blocked
        task->task_state = TASK_STATE_BLOCKED;

        //set block ticks
        task->block_tick = current_tick + timeout_tick;

        //insert into blocked queue
        //this task will be removed from the ready queue by the scheduler
        blocked_queue_add(task);

        //Pend the PendSV Exception to handle context switch
        INTCTRL = PENDSVSET;
    }
}



void taskUnblock(void)
{
    uint8_t priority, yield = 0;
    #if SCHEDULER == SCHEDULER_PRIORITY
    priority = TASK_MAX_NO_OF_PRIORITY;
    #endif

    //iterate through each priority from highest to lowest
    for(uint8_t i=0; i<priority; i++)
    {
        //get the starting task of the priority
        tcb_t* t = blocked_queue[i];
        tcb_t* tprev = NULL;

        //go through the tasks in the queue
        while(t != NULL)
        {
            //check block tick
            //Signed Comparison to Handle Tick Overflow - Works only for half the range of uint32_t
            //as long as the delay value is under the half range value, tick overflow will be handled
            if(t->task_state == TASK_STATE_BLOCKED && (int32_t)(current_tick - t->block_tick) >= 0)
            {
                tcb_t* tnext = t->pnext;

                //remove from blocked queue
                //remove head
                if(tprev == NULL)
                    blocked_queue[i] = tnext;
                //remove middle or last
                else
                    tprev->pnext = tnext;

                //add to ready queue
                t->block_tick = 0;
                ready_queue_add(t);

                //compare the priorities of the unblocked task and current task
                if(t->task_priority > pcurrent->task_priority)
                    yield = 1;
                
                t = tnext;
            }
            else
            {
                tprev = t;
                t = t->pnext;
            }
        }
    }
    if(yield) taskYield(yield);
    return;
}



//Task Yield
void taskYield(bool higherPriorityTaskWoken)
{
    //Higher Priority task woken condition should be checked for Priority Scheduler
    //if true, we yield
    #if SCHEDULER == SCHEDULER_PRIORITY
        if(higherPriorityTaskWoken)
            //Pend the PendSV Exception to handle context switch
            INTCTRL = PENDSVSET;
    //for round robin including weighted, condition need not be checked
    //we yield regardless of the condition
    #else
        //Pend the PendSV Exception to handle context switch
        INTCTRL = PENDSVSET;
    #endif
}



//Task Yield ISR
//To be used only in ISR
void taskYieldFromISR(bool higherPriorityTaskWoken)
{
    //Higher Priority task woken condition should be checked for Priority Scheduler
    //if true, we yield
    #if SCHEDULER == SCHEDULER_PRIORITY
        if(higherPriorityTaskWoken)
            //Pend the PendSV Exception to handle context switch
            INTCTRL = PENDSVSET;
    //for round robin including weighted, condition need not be checked
    //we yield regardless of the condition
    #else
        //Pend the PendSV Exception to handle context switch
        INTCTRL = PENDSVSET;
    #endif
}





ptask_t getTaskFunc(uint8_t task_num)
{
    return ptask_list[task_num];
}

ptask_t* getTaskFunc_List()
{
    return ptask_list;  
}

tcb_t* getTask_List()
{
    return TCBS;
}

tcb_t* getTask_Idle()
{
    return &TCBS[0];
}


uint8_t getTask_Count()
{
    return task_count-1;
}


tcb_t* getTask_Priority(uint8_t priority)
{
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        if(TCBS[i].task_priority == priority)
            return &TCBS[i];
    }

    return (getTask_Idle());
}