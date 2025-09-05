#include "rtos_info.h"
#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_task.h"
#include "rtos_port.h"

extern tcb_t *pcurrent;              //current pointer to a tcb


void rtosInfo_Tasks(void)
{
    tcb_t* temp = pcurrent;
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        Serialprint("\r\nTask ID: %d | Task Address: %x | Task Description: %s", temp->task_id, temp, temp->task_desc);
        temp = temp->pnext;
    }
    Serialprint("\r\n");
}