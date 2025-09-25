#include "rtos_info.h"
#include "rtos_kernel.h"
#include "rtos_config.h"
#include "rtos_task.h"
#include "rtos_port.h"

extern tcb_t *pcurrent;              //current pointer to a tcb



char* rtosInfo_Scheduler(void)
{
    #if SCHEDULER == SCHEDULER_ROUND_ROBIN
        return "ROUND ROBIN";
    #elif SCHEDULER == SCHEDULER_RR_WEIGHTED
        return "ROUND ROBIN WEIGHTED";
    #endif
}


void rtosInfo_Tasks(void)
{
    tcb_t* temp = pcurrent;
    Serialprintln("--------------------------------RTOS Tasks Info--------------------------------", INFO);
    for(uint8_t i=0; i<NO_OF_TASKS+1; i++)
    {
        #if SCHEDULER == SCHEDULER_ROUND_ROBIN
        Serialprintln("Task ID: %d | Task Address: %x | Task Description: %s", INFO, temp->task_id, temp, temp->task_desc);
        #elif SCHEDULER == SCHEDULER_RR_WEIGHTED
        Serialprintln("Task ID: %d | Task Address: %x | Task Description: %s | Task Weight: %d", INFO, temp->task_id, temp, temp->task_desc, temp->task_weight);
        #endif
        temp = temp->pnext;
    }
    Serialprintln("-------------------------------------------------------------------------------", INFO);

    Serialprintln("Starting the %s Scheduler now...\r\n", INFO, rtosInfo_Scheduler());
    tim_delay_ms(5000);
}