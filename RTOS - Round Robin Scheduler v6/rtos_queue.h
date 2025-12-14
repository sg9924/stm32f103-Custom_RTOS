#ifndef __RTOSQUEUE_H__
#define __RTOSQUEUE_H__
#include <stdint.h>

#include "rtos_task.h"
#include "rtos_config.h"


void ready_queue_init();
void ready_queue_add(tcb_t* task);
uint8_t ready_queue_remove(tcb_t* task, uint8_t state);
uint8_t ready_queue_check_empty();
void ready_queue_reset();

void blocked_queue_init();
uint8_t blocked_queue_check(tcb_t* task);
uint8_t blocked_queue_check_empty();
void blocked_queue_add(tcb_t* task);
uint8_t blocked_queue_remove(tcb_t* task, uint8_t state);

#endif