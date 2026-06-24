#ifndef __RTOSCONFIG_H__
#define __RTOSCONFIG_H__

//Definitons
//Scheduler Definitions
#define SCHEDULER_ROUND_ROBIN                         (1<<0)
#define SCHEDULER_RR_WEIGHTED                         (1<<1)
#define SCHEDULER_PRIORITY                            (1<<2)

//Configurations
//Systick
#define TASK_QUANTA_MS                                1000
#define TICK_RATE_HZ                                  (1000/TASK_QUANTA_MS)

//Tasks
#define NO_OF_TASKS                                   3
#define STACKSIZE                                     200
#define TASK_DESC_LENGTH                              100
#define TASK_MAX_NO_OF_PRIORITY                       4

//Scheduler
#define RTOS_SCHEDULER_START_DELAY                    3000
#define SCHEDULER                                     SCHEDULER_PRIORITY


//Configurations
//0 - disabled
//1 - enabled

#define INFO_PRINT                                    1
#define IDLE_TASK_PRINT                               1
#define SYSTICK_LED_TOGGLE                            1

#endif