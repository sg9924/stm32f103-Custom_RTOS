#include "rtos_task.h"
#include "rtos_kernel.h"
#include "rtos_port.h"


//external variables
extern uint32_t current_tick;
extern tcb_t* ready_queue[MAX_NO_OF_PRIORITY];
extern tcb_t* blocked_queue[MAX_NO_OF_PRIORITY];

tcb_t TCBS[NO_OF_TASKS+1];  //declare an array of TCB's
tcb_t *pcurrent;            //current pointer to a tcb

static ptask_t ptask_list[NO_OF_TASKS + 1];
static uint8_t task_count;

static void taskIdle(void);




void __task_count_init(void)
{
    task_count=1;
}



void taskAdd_Priority(ptask_t func_ptr, char* task_desc, uint8_t task_priority)
{
    if(task_count>NO_OF_TASKS)
    {
        Serialprintln("Incorrect Task Add!! | Configured Tasks: %d | Current Task No.: %d", FATAL, NO_OF_TASKS, task_count);
        SERIAL_NL();
        __asm("BKPT #0");
    }
    
    ptask_list[task_count]               = func_ptr;
    
    TCBS[task_count].ptask_func          = func_ptr;
    TCBS[task_count].task_id             = task_count;
    TCBS[task_count].task_state          = TASK_STATE_READY;
    TCBS[task_count].task_desc           = task_desc;
    TCBS[task_count].task_priority       = task_priority;
    TCBS[task_count].block_tick          = 0;
    ready_queue_add(&TCBS[task_count]);
    taskNotify_Reset(&TCBS[task_count]);

    Serialprintln("'%s' task has been added", INFO, TCBS[task_count].task_desc);
    task_count++;
}


//Add Idle Task
void taskAdd_Idle()
{
    TCBS[0].ptask_func    = &taskIdle;
    TCBS[0].task_desc     = "Idle Task";
    TCBS[0].task_id       = 0;
    TCBS[0].task_priority = MAX_NO_OF_PRIORITY-1; //lowest priority
}



//Idle Task Definition
static void taskIdle(void)
{
    while(1)
    {
        #if IDLE_TASK_PRINT == 1
        Serialprint("[Tick: %x] [ID: %d] [Priority: %d] Idle Task\r\n", INFO, current_tick, TCBS[0].task_id, TCBS[0].task_priority);
        #endif
    }
}



void taskDelay(uint32_t timeout_tick)
{   
    taskBlock(NULL, timeout_tick);
}


void taskBlock(tcb_t* task, uint32_t timeout_tick)
{
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

        //add to blocked queue/list
        blocked_queue_add(task);

        //Pend the systick Exception to switch to next task
        SYSTICK_EXCEPTION_PEND();
    }
}



void taskUnblock(void)
{
    uint8_t priority, yield = 0;
    #if SCHEDULER == SCHEDULER_PRIORITY
    priority = MAX_NO_OF_PRIORITY;
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
            if(t->task_state == TASK_STATE_BLOCKED && t->block_tick == current_tick)
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
                t->pnext      = NULL;
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
    if(yield) taskYield();
    return;
}



void taskYield(void)
{
    SYSTICK_CLEAR();
    SYSTICK_EXCEPTION_PEND();
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


/*****************************************************Task Notofications APIs START*****************************************************/
//Reset Task Notification Parameters
void taskNotify_Reset(tcb_t* task)
{
    task->task_noti_state = TASK_NOTIFY_STATE_NONE;
    task->task_noti_value = 0;
}

//Set Task Notification Parameters
void taskNotify_Set(tcb_t* task, uint32_t value, uint8_t state, uint8_t index)
{
    task->task_noti_state = state;
    task->task_noti_value = value;
}


//Send Task Notification
void taskNotify_Send(tcb_t* task, uint32_t value, uint8_t action)
{
    //flag variable
    uint8_t was_blocked = 0;

    /*CRITICAL SECTION START*/
    DISABLE_IRQ();

    if(task->task_noti_state != TASK_NOTIFY_STATE_RECEIVED)
    {
        //set notification value in the task receiving the notification
        if(action == TASK_NOTIFY_ACTION_SET) task->task_noti_value = value;
        else if(action == TASK_NOTIFY_ACTION_OR) task->task_noti_value |= value;
        else if(action == TASK_NOTIFY_ACTION_INC) task->task_noti_value += 1;
    }

    //check if task is blocked and its waiting for notification
    if(task->task_id != 0 && task->task_state == TASK_STATE_BLOCKED && task->task_noti_state == TASK_NOTIFY_STATE_PENDING)
    {
        //move task from blocked to ready queue
        blocked_queue_remove(task, TASK_STATE_READY);
        task->task_noti_state = TASK_NOTIFY_STATE_RECEIVED;
        task->block_tick = 0;
        ready_queue_add(task);

        was_blocked = 1;
    }
    else
        was_blocked = 0;

    ENABLE_IRQ();

    //if the unblocked task has higher priority (strictly), yield now - priority scheduler
    if(was_blocked && task->task_priority > pcurrent->task_priority)
    {
        #if SCHEDULER == SCHEDULER_PRIORITY
        taskYield();
        #endif
    }
}


//Receive Task Notification
uint32_t taskNotify_Wait(uint32_t clear_on_entry_mask, uint32_t clear_on_exit_mask, uint32_t* out, uint32_t wait_tick)
{
    tcb_t* current = pcurrent;
    uint32_t result;

    //entry mask
    if(clear_on_entry_mask) current->task_noti_value &= ~clear_on_entry_mask;

    //if no notification is pending
    if(current->task_noti_state == TASK_NOTIFY_STATE_NONE)
    {
        //wait for notification
        current->task_noti_state = TASK_NOTIFY_STATE_PENDING;
        //block the task
        taskBlock(current, wait_tick);
    }
    
    //control comes here if, either the task has been unblocked by taskNotify_Send or timeout occurred
    //if a notification has been received
    if(current->task_noti_state == TASK_NOTIFY_STATE_RECEIVED)
    {
        //get the noti value
        result = current->task_noti_value;
        //reset noti state for future runs
        current->task_noti_state = TASK_NOTIFY_STATE_NONE;

        //exit mask
        if(clear_on_exit_mask) current->task_noti_value &= ~clear_on_exit_mask;
    }
    //timeout: provided wait_tick has passed
    else
    {
        //reset noti state for future runs
        current->task_noti_state = TASK_NOTIFY_STATE_NONE;
        result = 0;
    }

    //store the result to the pointer if it has been provided
    if(out) *out = result;

    return result;
}
/******************************************************Task Notofications APIs END******************************************************/