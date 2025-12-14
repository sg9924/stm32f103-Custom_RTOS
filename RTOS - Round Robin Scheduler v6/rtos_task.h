#ifndef __RTOSTASK_H__
#define __RTOSTASK_H__
#include<stdint.h>

#include "rtos_config.h"


#define TASK_STATE_READY            ('A')
#define TASK_STATE_RUNNING          ('R')
#define TASK_STATE_BLOCKED          ('B')

//Task Notify Actions
#define TASK_NOTIFY_ACTION_SET      0
#define TASK_NOTIFY_ACTION_OR       1
#define TASK_NOTIFY_ACTION_INC      2

#define TASK_NOTIFY_STATE_NONE      0
#define TASK_NOTIFY_STATE_PENDING   1
#define TASK_NOTIFY_STATE_RECEIVED  2


typedef void(*ptask_t)(void);       //task function pointer

typedef struct
{
    uint32_t     task_noti_value;                    //notifcation value
    uint8_t      task_noti_state;                    //notification state
}task_noti_t;


//Task control Block
//!order of structure members should not be changed!
typedef struct tcb
{
    int32_t*     pstack;                             //pointer to the stack
    struct tcb*  pnext;                              //pointer to the next tcb structure (linked list)
    ptask_t      ptask_func;                         //pointer to the task function
    uint8_t      task_id;                            //task identifier
    uint8_t      task_state;                         //task state
    char*        task_desc;                          //task description
    uint32_t     block_tick;                         //ticks for which the task should be blocked
    uint8_t      task_quota;                         //assigned task weight
    uint8_t      task_weight;                        //task wight for weighted round robin
    uint8_t      task_priority;                      //priority: 0 (highest) to 255 (lowest)
    task_noti_t  task_noti[TASK_NOTI_MAX_SIZE];      //task notification array
}tcb_t;

void __task_count_init(void);

void taskIdle(void);
void taskAdd(ptask_t func_ptr, char* task_desc, tcb_t** ptask_handle);
void taskAdd_Weighted(ptask_t func_ptr, char* task_desc, uint8_t task_weight, tcb_t** ptask_handle);
void taskAdd_Idle();

void taskReset_Quota();

void taskDelay(uint32_t tick);
void taskBlock(tcb_t* task, uint32_t timeout_tick);
void taskUnblock(void);
void taskYield(void);



ptask_t getTaskFunc(uint8_t task_num);
ptask_t* getTaskFunc_List();

tcb_t* getTask_Idle();
tcb_t* getTask_List();
uint8_t getTask_Count();

void taskNotify_Send(tcb_t* task, uint32_t value, uint8_t action);
uint32_t taskNotify_Wait(uint32_t clear_on_entry_mask, uint32_t clear_on_exit_mask, uint32_t* out, uint32_t timeout_ticks);

#endif