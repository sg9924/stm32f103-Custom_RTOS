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
    uint8_t      task_priority;                      //priority: 0 (highest) to 255 (lowest)
}tcb_t;

void __task_count_init(void);
void taskAdd_Idle();
void taskAdd_Priority(ptask_t func_ptr, char* task_desc, uint8_t task_priority);


void taskReset_Quota();
void taskDelay(uint32_t tick);
void taskIdle(void);
void taskUnblock(void);
void taskYield(void);

ptask_t getTaskFunc(uint8_t task_num);
ptask_t* getTaskFunc_List();

tcb_t* getTask_Idle();
tcb_t* getTask_List();
tcb_t* getTask_Priority(uint8_t priority);

uint8_t getTaskCount();

#endif