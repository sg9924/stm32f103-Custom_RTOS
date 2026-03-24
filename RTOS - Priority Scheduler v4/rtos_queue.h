#ifndef __QUEUE_H__
#define __QUEUE_H__

#include<stdint.h>
#include<stddef.h>

#include"rtos_task.h"



typedef struct task_node_t
{
    tcb_t* task;
    struct task_node_t *next;
    uint8_t priority;
}task_node_t;

typedef struct
{
    task_node_t* head;
}twaitlist_t;


typedef struct
{
    uint8_t* buffer;          //pointer to buffer
    size_t element_size;      //size of each element in bytes
    size_t max_length;        //maximum length of the queue
    size_t cur_length;        //current length of the queue

    uint32_t front;             //start of queue - read index
    uint32_t rear;              //end of queue - write index

    uint8_t reader_count;     //no of readers
    uint8_t writer_count;     //no of writers

    twaitlist_t readers;      //list of readers waiting to read
    twaitlist_t writers;      //list of writers waiting to write
}Queue_t;


Queue_t* queueCreateStatic(size_t element_size, size_t length, Queue_t* q, uint8_t* buffer);
bool queueDestroy(Queue_t* q);

bool queueSend(Queue_t* q, const void* item, uint16_t wait_tick);
bool queueReceive(Queue_t* q, const void* item, uint16_t wait_tick);


#endif