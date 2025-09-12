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
    int32_t*     pstack;                             //pointer to the stack
    struct tcb*  pnext;                              //pointer to the next tcb structure (linked list)
    ptask_t      ptask_func;                         //pointer to the task function
    uint8_t      task_id;
    uint8_t      task_state;                         //task state
    char*        task_desc;                          //task description
    uint32_t     block_tick;                         //ticks for which the task should be blocked
    uint32_t     period_tick;                        //period asigned to the task
    uint32_t     next_release_tick;                  //tick value for next release
}tcb_t;

void __task_count_init(void);
void taskAdd(ptask_t func_ptr, char* task_desc);
void taskAdd_Periodic(ptask_t func_ptr, uint32_t period, char* task_desc);
void taskDelay(uint32_t tick);
void taskIdle(void);
void taskUnblock(void);
void taskYield(void);

void taskAdd_Idle();

ptask_t getTaskFunc(uint8_t task_num);
tcb_t* getIdleTask_TCB();
ptask_t* getTaskList();
uint8_t getTaskCount();

#endif