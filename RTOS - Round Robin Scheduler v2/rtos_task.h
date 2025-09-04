#ifndef __RTOSTASK_H__
#define __RTOSTASK_H__
#include<stdint.h>

#include "rtos_config.h"


typedef void(*ptask_t)(void);       //task function pointer

//Task control Block
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
ptask_t* getTaskList();
tcb_t* getIdleTask_TCB();

void taskAdd(ptask_t func_ptr, char* task_desc);
void taskAdd_Periodic(ptask_t func_ptr, uint32_t period, char* task_desc);
void taskAdd_Idle(void);
void taskIdle(void);
void taskYield(void);


#endif