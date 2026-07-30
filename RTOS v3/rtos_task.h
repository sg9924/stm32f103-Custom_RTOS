#ifndef __RTOSTASK_H__
#define __RTOSTASK_H__
#include<stdint.h>

#include "rtos_config.h"


#define TASK_STATE_READY            ('A')
#define TASK_STATE_RUNNING          ('R')
#define TASK_STATE_BLOCKED          ('B')


typedef void(*ptask_t)(void);       //task function pointer

//Task control Block
//!order of structure members should not be changed!
typedef struct tcb
{
    uint32_t*     pstack;                             //pointer to the stack
    uint8_t      stack_size_word;                    //size of task stack in words
    struct tcb*  pnext;                              //pointer to the next tcb structure (linked list)
    ptask_t      ptask_func;                         //pointer to the task function
    uint8_t      task_id;                            //task identifier
    uint8_t      task_state;                         //task state
    char*        task_desc;                          //task description
    uint32_t     block_tick;                         //ticks for which the task should be blocked
    #if SCHEDULER == SCHEDULER_RR_WEIGHTED
    uint8_t      task_quota;                         //assigned task weight
    uint8_t      task_weight;                        //task weight for weighted round robin
    #endif
    #if SCHEDULER == SCHEDULER_PRIORITY
    uint8_t      task_priority;                      //priority: 0 (highest) to 255 (lowest)
    #endif
}tcb_t;

void __task_count_init(void);

#if SCHEDULER == SCHEDULER_RR_WEIGHTED
tcb_t* taskAdd(ptask_t func_ptr, char* task_desc, uint8_t stack_size_word);
#elif SCHEDULER == SCHEDULER_RR_WEIGHTED
tcb_t* taskAdd_Weighted(ptask_t func_ptr, char* task_desc, uint8_t task_weight, uint8_t stack_size_word);
#elif SCHEDULER == SCHEDULER_PRIORITY
tcb_t* taskAdd_Priority(ptask_t func_ptr, char* task_desc, uint8_t task_priority, uint8_t stack_size_word);
#endif
void taskAdd_Idle();


void taskDelay(uint32_t tick);
void taskBlock(tcb_t* task, uint32_t timeout_tick);
void taskUnblock(void);
void taskYield(void);

#if SCHEDULER == SCHEDULER_RR_WEIGHTED
void taskReset_Quota(tcb_t* task);
void taskReset_QuotaAll();
#endif

ptask_t getTaskFunc(uint8_t task_num);
ptask_t* getTaskFunc_List();

tcb_t* getTask_Idle();
tcb_t* getTask_List();
uint8_t getTask_Count();
#if SCHEDULER == SCHEDULER_PRIORITY
tcb_t* getTask_Priority(uint8_t priority);
#endif

#endif