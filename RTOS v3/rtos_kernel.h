#ifndef __RTOSKERNEL_H__
#define __RTOSKERNEL_H__
#include <stdint.h>

#include "rtos_task.h"
#include "rtos_port.h"

#define SCHEDULER_SWITCH_CONTEXT()      INTCTRL = PENDSVSET


void rtosKernel_Init();
void rtosKernel_Launch(uint32_t quanta);

//Stack Allocate
uint32_t* Stack_Allocate(uint32_t size_in_words);

uint8_t assert(uint8_t condition, char* assert_msg);

void ready_queue_add(tcb_t* task);
uint8_t ready_queue_remove(tcb_t* task, uint8_t state);

void blocked_queue_add(tcb_t* task);
uint8_t blocked_queue_remove(tcb_t* task, uint8_t state);



#endif