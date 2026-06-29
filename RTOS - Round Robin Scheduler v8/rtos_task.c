#include "rtos_task.h"
#include "rtos_kernel.h"
tcb_t TCBS[NO_OF_TASKS + 1];  //declare an array of TCB's
tcb_t *pcurrent;            //current pointer to a tcb

static ptask_t ptask_list[NO_OF_TASKS + 1];
static uint8_t task_count;

extern uint32_t current_tick;
extern tcb_t* ready_queue[1];
extern tcb_t* blocked_queue[1];

static void taskAdd_Check(uint8_t task_count);
static void taskIdle(void);

static void taskBlock_Notify(tcb_t* task, uint32_t tick);
static uint8_t taskUnblock_Notify(tcb_t* task);


void __task_count_init(void)
{
    task_count = 1;
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


tcb_t* taskAdd(ptask_t func_ptr, char* task_desc, uint8_t stack_size_word)
{
    taskAdd_Check(task_count);

    ptask_list[task_count] = func_ptr;
    
    //add task to ready queue if its not idle task
    if(task_count != 0)
    {
        TCBS[task_count].ptask_func        = func_ptr;
        TCBS[task_count].task_id           = task_count;
        TCBS[task_count].task_state        = TASK_STATE_READY;
        TCBS[task_count].task_desc         = task_desc;
        TCBS[task_count].task_weight       = 0;
        TCBS[task_count].task_quota        = 0;
        TCBS[task_count].block_tick        = 0;

        #if STACK_TYPE == STACK_TYPE_INDIVIDUAL
        TCBS[task_count].stack_size_word   = stack_size_word;
        TCBS[task_count].pstack            = Stack_Allocate(stack_size_word);
        assert((TCBS[task_count].pstack!=NULL), "Stack Allocation Failure");
        assert((stack_size_word != 0), "Invalid Stack Size during Task Add");
        #endif

        taskNotify_ResetAll(&TCBS[task_count]);
        ready_queue_add(&TCBS[task_count]);

        Serialprintln("Task %d added", INFO, task_count);

        return &TCBS[task_count++];
    }
    return NULL;
}



tcb_t* taskAdd_Weighted(ptask_t func_ptr, char* task_desc, uint8_t task_weight, uint8_t stack_size_word)
{
    taskAdd_Check(task_count);

    ptask_list[task_count] = func_ptr;
    
    //add task to ready queue if its not idle task
    if(task_count != 0)
    {
        TCBS[task_count].ptask_func        = func_ptr;
        TCBS[task_count].task_id           = task_count;
        TCBS[task_count].task_state        = TASK_STATE_READY;
        TCBS[task_count].task_desc         = task_desc;
        TCBS[task_count].task_weight       = task_weight;
        TCBS[task_count].task_quota        = 0;
        TCBS[task_count].block_tick        = 0;

        #if STACK_TYPE == STACK_TYPE_INDIVIDUAL
        TCBS[task_count].stack_size_word   = stack_size_word;
        TCBS[task_count].pstack            = Stack_Allocate(stack_size_word);
        assert((TCBS[task_count].pstack!=NULL), "Stack Allocation Failure");
        assert((stack_size_word == 0), "Invalid Stack Size during Task Add");
        #endif

        taskNotify_ResetAll(&TCBS[task_count]);
        ready_queue_add(&TCBS[task_count]);

        Serialprintln("Task %d added", INFO, task_count);

        return &TCBS[task_count++];
    }
    return NULL;
}



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



void taskAdd_Idle()
{
    TCBS[0].ptask_func                 = &taskIdle;
    TCBS[0].task_state                 = TASK_STATE_READY;
    TCBS[0].task_desc                  = "Idle Task";
    TCBS[0].task_id                    = 0;
    TCBS[0].task_weight                = 1;
    TCBS[0].task_quota                 = 0;

    #if STACK_TYPE == STACK_TYPE_INDIVIDUAL
        TCBS[0].stack_size_word    = 50;
        TCBS[0].pstack             = Stack_Allocate(50);
        assert((TCBS[0].pstack!=NULL), "Stack Allocation Failure");
    #endif
}


//Idle Task
static void taskIdle(void)
{
    while(1)
    {
        Serialprintln("[Tick: %x] No Tasks to run...", INFO, current_tick);
    }
}



//Relative Task Delay
//Does not include the task execution time in the delay
void taskDelay(uint32_t delay_tick)
{
    taskBlock(NULL, delay_tick);
}



//Absolute Delay
//Includes the task execution time in the delay
//accounts for overflow
void taskDelayAbs(uint32_t* last_wake_tick, uint32_t delay_tick)
{
    DISABLE_IRQ();
    
    //calculate absolute delay tick
    uint32_t abs_delay_tick = *last_wake_tick + delay_tick;
    bool     block          = false;

    //systick overflow
    if(current_tick < *last_wake_tick)
    {
        //abs delay tick overflow wrt last wake tick
        //abs delay tick should be greater than current tick
        if((abs_delay_tick < *last_wake_tick) && abs_delay_tick > current_tick)
            block = true;
    }
    //no systick overflow
    else
    {
        //case 1: abs delay tick overflow wrt last wake tick
        //case 2: no overflow in abs delay tick, it should be greater than the current tick
        if ((abs_delay_tick < *last_wake_tick) || (abs_delay_tick > current_tick))
            block = true;

        //above conditions not satisfied -> missed deadline (RMS)
        //current tick is now greater than abs delay tick
    }

    if(block == true)
    {
        *last_wake_tick = abs_delay_tick;
        taskBlockAbs(NULL, abs_delay_tick);
    }
    else
    {
        //catchup mode
        //don't block, we need to catchup to the current tick
        //similar implementation as in freeRTOS
        #if TASK_DELAY_ABS_CATCHUP_MODE == 1
        //update last wake tick to the absolute delay
        *last_wake_tick = abs_delay_tick;
        //no catchup
        #elif TASK_DELAY_ABS_CATCHUP_MODE == 0
        //update last wake tick to the current tick
        *last_wake_tick = current_tick;
        #endif
    }

    ENABLE_IRQ();
    return;
}


//Task Block Relative
//task will be blocked for the duration provided relative to current tick
void taskBlock(tcb_t* task, uint32_t timeout_tick)
{
    if(task == NULL)
        task = pcurrent;
    
    if(task->task_id != 0 && task->task_state != TASK_STATE_BLOCKED)
    {
        task->task_state = TASK_STATE_BLOCKED;

        //set block ticks
        //relative
        task->block_tick = current_tick + timeout_tick;

        //insert into blocked queue
        //this task will be removed from the ready queue by the scheduler
        blocked_queue_add(task);

        //Pend the PendSV Exception to handle context switch
        INTCTRL = PENDSVSET;
    }
}



//Task Block Absolute
//task will be blocked with duration assumed to be absolute
void taskBlockAbs(tcb_t* task, uint32_t abs_timeout_tick)
{
    if(task == NULL)
        task = pcurrent;
    
    if(task->task_id != 0 && task->task_state != TASK_STATE_BLOCKED)
    {
        task->task_state = TASK_STATE_BLOCKED;

        //set block ticks
        task->block_tick = abs_timeout_tick;

        //insert into blocked queue
        //this task will be removed from the ready queue by the scheduler
        blocked_queue_add(task);

        //Pend the PendSV Exception to handle context switch
        INTCTRL = PENDSVSET;
    }
}



void taskUnblock(void)
{
    //for round robin, blocked_queue array always has one element only.
    tcb_t* t     = blocked_queue[0];
    tcb_t* tprev = NULL;

    //go through the tasks in the queue
    while(t != NULL)
    {
        //check block tick
        if(t->task_state == TASK_STATE_BLOCKED && (int32_t)(current_tick - t->block_tick) >= 0)
        {
            tcb_t* tnext = t->pnext;

            //remove from blocked queue
            //remove head
            if(tprev == NULL)
                blocked_queue[0] = tnext;
            //remove middle or last
            else
                tprev->pnext = tnext;

            //add to ready queue
            t->block_tick = 0;
            ready_queue_add(t);
            
            t = tnext;
        }
        //just traverse to next task
        else
        {
            tprev = t;
            t = t->pnext;
        }
    }
    return;
}



//Task Yield
void taskYield(bool higherPriorityTaskWoken)
{
    if(higherPriorityTaskWoken)
        //Pend the PendSV Exception to handle context switch
        INTCTRL = PENDSVSET;
}


//Task Yield
//To be used only in ISR
void taskYieldFromISR(bool higherPriorityTaskWoken)
{
    if(higherPriorityTaskWoken)
        //Pend the PendSV Exception to handle context switch
        INTCTRL = PENDSVSET;
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

//Helper
static void taskBlock_Notify(tcb_t* task, uint32_t tick)
{
    taskBlock(task, tick);
}


//Helper
static uint8_t taskUnblock_Notify(tcb_t* task)
{
    //Not a Idle Task + Task is blocked + Notification state = Pending
    if(task->task_id != 0 && task->task_state == TASK_STATE_BLOCKED && task->task_noti[0].task_noti_state == TASK_NOTIFY_STATE_PENDING)
    {
        //unblock the waiting task
        blocked_queue_remove(task, TASK_STATE_READY);
        //update the notification state as received
        task->task_noti[0].task_noti_state = TASK_NOTIFY_STATE_RECEIVED;
        //reset the block tick
        task->block_tick = 0;
        //add back to the ready queue
        ready_queue_add(task);
        return 1;
    }

    return 0;
}


void taskNotify_Reset(tcb_t* task, uint8_t index)
{
    task->task_noti[index].task_noti_state = TASK_NOTIFY_STATE_NONE;
    task->task_noti[index].task_noti_value = 0;
}


void taskNotify_Set(tcb_t* task, uint32_t value, uint8_t state, uint8_t index)
{
    task->task_noti[index].task_noti_state = state;
    task->task_noti[index].task_noti_value = value;
}


void taskNotify_ResetAll(tcb_t* task)
{
    for(uint8_t i=0; i<TASK_NOTI_MAX_SIZE; i++)
        taskNotify_Reset(task, i);
}


//Task Notification Send
void taskNotify_Send(tcb_t* task, uint32_t value, uint8_t action)
{
    uint8_t was_blocked = 0;
    DISABLE_IRQ();

    //perform the operations based on the notification actions
    if(action == TASK_NOTIFY_ACTION_SET) task->task_noti[0].task_noti_value = value;
    else if(action == TASK_NOTIFY_ACTION_OR) task->task_noti[0].task_noti_value |= value;
    else if(action == TASK_NOTIFY_ACTION_INC) task->task_noti[0].task_noti_value += 1;

    //if the task was blocked waiting for the notification, it should be unblocked after receiving the notification
    if(taskUnblock_Notify(task))
        was_blocked = 1;

    ENABLE_IRQ();
}


//Task Notification Receive
uint32_t taskNotify_Wait(uint32_t clear_on_entry_mask, uint32_t clear_on_exit_mask, uint32_t* out, uint32_t timeout_ticks)
{
    tcb_t*  current = pcurrent;
    uint32_t result = 0;

    //entry mask
    if(clear_on_entry_mask) current->task_noti[0].task_noti_value &= ~clear_on_entry_mask;

    //if no notification is pending
    if(current->task_noti[0].task_noti_state == TASK_NOTIFY_STATE_NONE)
    {
        //move to pending state
        current->task_noti[0].task_noti_state = TASK_NOTIFY_STATE_PENDING;
        //block the task waiting for the notification
        taskBlock_Notify(pcurrent, timeout_ticks);
    }
    
    //if notification has been received
    //control comes once the task has been unblocked by taskNotifySend
    if(current->task_noti[0].task_noti_state == TASK_NOTIFY_STATE_RECEIVED)
    {
        //get the notification value
        result = current->task_noti[0].task_noti_value;
        //reset the noti state
        current->task_noti[0].task_noti_state = TASK_NOTIFY_STATE_NONE;

        //exit mask
        if(clear_on_exit_mask)
        {
            current->task_noti[0].task_noti_value &= ~clear_on_exit_mask;
            result &= ~clear_on_exit_mask;
        }
    }
    //timed out
    //control comes here when the task was unblocked after the provided ticks had passed
    else
    {
        current->task_noti[0].task_noti_state = TASK_NOTIFY_STATE_NONE;
    }

    //store the result to the pointer if it has been provided
    if(out) *out = result;

    return result;
}



//Task Notification Send for a certain Index
void taskNotify_SendIndex(tcb_t* task, uint32_t value, uint8_t action, uint8_t index)
{
    uint8_t was_blocked = 0;
    DISABLE_IRQ();

    if(action == TASK_NOTIFY_ACTION_SET) task->task_noti[index].task_noti_value = value;
    else if(action == TASK_NOTIFY_ACTION_OR) task->task_noti[index].task_noti_value |= value;
    else if(action == TASK_NOTIFY_ACTION_INC) task->task_noti[index].task_noti_value += 1;

    //if the task was blocked waiting for the notification, it should be unblocked after receiving the notification
    if(taskUnblock_Notify(task))
        was_blocked = 1;

    ENABLE_IRQ();
}



//Task Notification Receive for a certain Index
uint32_t taskNotify_ReceiveIndex(uint32_t clear_on_entry_mask, uint32_t clear_on_exit_mask, uint32_t* out, uint32_t timeout_ticks, uint8_t index)
{
    tcb_t*  current = pcurrent;
    uint32_t result = 0;

    //entry mask
    if(clear_on_entry_mask) current->task_noti[index].task_noti_value &= ~clear_on_entry_mask;

    //if no notification is pending
    if(current->task_noti[index].task_noti_state == TASK_NOTIFY_STATE_NONE)
    {
        current->task_noti[index].task_noti_state = TASK_NOTIFY_STATE_PENDING;
        taskBlock_Notify(pcurrent, timeout_ticks);
    }
    
    //if notification has been received
    if(current->task_noti[index].task_noti_state == TASK_NOTIFY_STATE_RECEIVED)
    {
        result = current->task_noti[index].task_noti_value;
        current->task_noti[index].task_noti_state = TASK_NOTIFY_STATE_NONE;

        //exit mask
        if(clear_on_exit_mask)
        {
            current->task_noti[index].task_noti_value &= ~clear_on_exit_mask;
            result &= ~clear_on_exit_mask;
        }
    }
    //timed out
    //control comes here when the task was unblocked after the provided ticks had passed
    else
    {
        current->task_noti[0].task_noti_state = TASK_NOTIFY_STATE_NONE;
    }

    //store the result to the pointer if it has been provided
    if(out) *out = result;

    return result;
}