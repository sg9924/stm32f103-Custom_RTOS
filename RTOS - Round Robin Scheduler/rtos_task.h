#ifndef __RTOSTASK_H__
#define __RTOSTASK_H__
#include<stdint.h>

#include "rtos_config.h"


//Task control Block
typedef struct tcb
{
    int32_t      *pstack;  //pointer to the stack
    struct tcb   *pnext;   //pointer to the next tcb structure (linked list)
}tcb_t;

typedef void(*ptask_t)(void);

void __task_count_init(void);
void addTask(ptask_t func_ptr);
ptask_t* getTaskList();

#endif