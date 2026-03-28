
#include<stddef.h>


#include"rtos_port.h"
#include"rtos_kernel.h"
#include"rtos_queue.h"

#include"memmgmt.h"


extern tcb_t *pcurrent;            //current pointer to a tcb


/**********************************************************Helper Functions START*********************************************************/
static bool queueAdd(Queue_t* q, const void* data);
static bool queueRemove(Queue_t* q, void* out);

static void waitlist_Insert(twaitlist_t *wl, tcb_t* t, uint8_t priority, task_node_t* tn);
static tcb_t* waitlist_Pop(twaitlist_t *wl);


static bool queueAdd(Queue_t* q, const void* data)
{
    if(!q || !data) return false;

    //uint8_t* dest = q->buffer + (q->rear * q->element_size);
    uint8_t* dest = (uint8_t*) (_QUEUE_BUFFER_ADDR(q, q->rear));
    xmemcopy(dest, (void*)data, q->element_size);
    q->rear = (q->rear + 1) % q->max_length;
    q->cur_length++;

    return true;
}



static bool queueRemove(Queue_t* q, void* out)
{
    if(!q) return false;
    
    //uint8_t* source = q->buffer + (q->front * q->element_size);
    uint8_t* source = (uint8_t*) (_QUEUE_BUFFER_ADDR(q, q->front));
    xmemcopy(out, source, q->element_size);
    q->front = (q->front + 1) % q->max_length;
    q->cur_length--;
    return true;
}



static void waitlist_Insert(twaitlist_t *wl, tcb_t* t, uint8_t priority, task_node_t* tn)
{
    //task node
    tn->task     = t;
    tn->priority = priority;
    tn->next     = NULL;

    //if wait list is empty or priority of task to add > priority of task at head of list
    if(!wl->head || priority > wl->head->priority)
    {
        tn->next = wl->head;
        wl->head = tn;
        return;
    }

    //temp pointer for traversal
    task_node_t* temp = wl->head;
    //continue traversing as long as next !NULL & priority of next > priority
    while(temp->next && temp->next->priority >= priority) temp = temp->next;
    //insert the Node_t at appropriate place
    tn->next = temp->next;
    temp->next = tn;

    return;
}


//pop the highest priority Node_t
static tcb_t* waitlist_Pop(twaitlist_t *wl)
{
    //checks
    if(!wl) return NULL;
    task_node_t* tn = wl->head;

    //assign new head to the next node after the current head
    wl->head = tn->next;

    tcb_t* task = tn->task;
    return task;
}
/***********************************************************Helper Functions END**********************************************************/



/*************************************************************Queue APIs START************************************************************/
Queue_t* queueCreateStatic(size_t element_size, size_t length, Queue_t* q, uint8_t* buffer)
{

    if(!q || length == 0 || element_size == 0 || buffer == NULL) return NULL;

    q->buffer          = buffer;
    q->front           = 0;
    q->rear            = 0;
    q->element_size    = element_size;
    q->cur_length      = 0;
    q->max_length      = length;
    q->reader_count    = 0;
    q->writer_count    = 0;
    q->readers.head    = NULL;
    q->writers.head    = NULL;

    return q;
}


bool queueSend(Queue_t* q, const void* item, uint16_t wait_tick)
{
    if(!q || !item) return false;

    DISABLE_IRQ();

    //Scenario 1
    //if space available in queue
    if(q->cur_length < q->max_length)
    {
        //add data to the queue
        queueAdd(q, item);

        //check if there is a task waiting to receive data (readers)
        //if so, send the data in the queue to the reader task
        //wake the highest priority receiver task first
        if(q->readers.head)
        {
            //get the waiting task
            tcb_t* task = waitlist_Pop(&q->readers);
            q->reader_count--;

            ENABLE_IRQ();
            //since data has been sent to the buffer, reader/receiver task will copy from queue when it wakes
            blocked_queue_remove(task, TASK_STATE_READY);
        }
        else ENABLE_IRQ();
        return true;
    }

    //Scenario 2:
    //queue is full + wait_tick == 0 -> return
    if(wait_tick == 0)
    {
        ENABLE_IRQ();
        return false;
    }

    //Scenario 3:
    //queue is full + wait_tick != 0
    //task has to wait
    //task is blocked until space frees out or timeout happens
    //wait task_node_t is required, static allocation for now
    task_node_t t;
    //add the task to the writers queue and block it
    waitlist_Insert(&q->writers, pcurrent, pcurrent->task_priority, &t);
    q->writer_count++;

    ENABLE_IRQ();

    //kernel will wait here until timeout or when task is unblocked by queueReceive API when space frees out
    taskBlock(pcurrent, wait_tick);


    //Scenario 3 continued:
    //control will come here when task has been unblocked
    //a. due to timeout
    //b. space is available in queue
    DISABLE_IRQ();

    //attempt to send data to queue again
    //a. check if space is available
    if(q->cur_length < q->max_length)
    {
        //add data to the queue
        queueAdd(q, item);

        //again check for receivers waiting (edge case)
        if(q->readers.head)
        {
            //get the waiting task
            tcb_t* task = waitlist_Pop(&q->readers);
            q->reader_count--;

            ENABLE_IRQ();
            //since data has been sent to the buffer, queue is populated, reader/receiver task will copy from queue when it wakes
            blocked_queue_remove(task, TASK_STATE_READY);
        }
        else ENABLE_IRQ();
        return true;
    }
    //b. space still not available, timeout
    else
    {
        ENABLE_IRQ();
        return false;
    }
}



bool queueReceive(Queue_t* q, const void* item, uint16_t wait_tick)
{
    if(!q || !item) return false;

    DISABLE_IRQ();

    //Scenario 1:
    //if queue has some data
    if(q->cur_length > 0)
    {
        //read from queue
        queueRemove(q, (void*)item);

        //check for any waiting writer tasks
        if(q->writers.head)
        {
            //get the waiting writer task
            tcb_t* t = waitlist_Pop(&q->writers);
            q->writer_count--;

            ENABLE_IRQ();
            //wake the writer task
            blocked_queue_remove(t, TASK_STATE_READY);
        }
        else ENABLE_IRQ();
        return true;
    }

    //Scenario 2:
    //queue is empty + wait_tick == 0
    if(wait_tick == 0)
    {
        ENABLE_IRQ();
        return false;
    }

    //Scenario 3:
    //queue is empty + wait_tick != 0
    //add the writer task to the wait list
    //wait task_node_t is required, static allocation for now
    task_node_t t;
    waitlist_Insert(&q->readers, pcurrent, pcurrent->task_priority, &t);
    q->reader_count++;

    ENABLE_IRQ();

    //kernel will wait here until timeout or when task is unblocked by queueSend API when space frees out
    taskBlock(pcurrent, wait_tick);

    //Scenario 3 continued:
    //control will come here when task has been unblocked
    //a. due to timeout
    //b. queue has some data
    DISABLE_IRQ();

    //attempt to receive data from queue again
    //a. check if queue has data
    if(q->cur_length > 0)
    {
        //copy data from queue
        queueRemove(q, (void*)item);

        //again check for writers waiting (edge case)
        if(q->writers.head)
        {
            //get the waiting task
            tcb_t* task = waitlist_Pop(&q->writers);
            q->writer_count--;

            ENABLE_IRQ();
            //since data has been read from the buffer, space frees up, writer/sender task will write to queue when it wakes
            //wake the writer task
            blocked_queue_remove(task, TASK_STATE_READY);
        }
        else ENABLE_IRQ();
        return true;
    }
    //b. queue is still empty, exit due to timeout
    else
    {
        ENABLE_IRQ();
        return false;
    }
}

bool queuePeek(Queue_t* q, void* out)
{
    if(!q) return false;

    if(out) xmemcopy(out, (void*)(_QUEUE_BUFFER_ADDR(q, q->front)), q->element_size);
    else return false;

    return true;
}



bool queueIsFull(Queue_t *q)
{
    if(!q) return false;
    else if(q->cur_length == q->max_length) return true;
    return false;
}



bool queueIsEmpty(Queue_t *q)
{
    if(!q) return false;
    else if(q->cur_length == 0) return true;
    return false;
}


uint32_t queueBufferAddr(Queue_t* q, uint32_t index)
{
    if(!q) return -1;
    else if(index == 0)
        return (_QUEUE_BUFFER_ADDR(q, q->front));
    else
        return (_QUEUE_BUFFER_ADDR(q, index));
}
/**************************************************************Queue APIs END*************************************************************/