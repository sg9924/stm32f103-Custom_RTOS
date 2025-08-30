#ifndef __RTOSTASK_H__
#define __RTOSTASK_H__
#include<stdint.h>

#include "rtos_config.h"


typedef void(*ptask_t)(void);       //task function pointer

//Task control Block
typedef struct tcb
{
    int32_t*     pstack;            //pointer to the stack
    struct tcb*  pnext;             //pointer to the next tcb structure (linked list)
    ptask_t      ptask_func;        //pointer to the task function
    char*        task_desc;         //task description
}tcb_t;

void __task_count_init(void);
ptask_t* getTaskList();

void taskAdd(ptask_t func_ptr, char* desc);
void taskYield(void);

#endif