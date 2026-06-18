#ifndef __RTOSCONFIG_H__
#define __RTOSCONFIG_H__

//Definitions

//Scheduler
#define SCHEDULER_PRIORITY                            (1<<2)

//Stack
#define STACK_TYPE_COMMON                             0U
#define STACK_TYPE_INDIVIDUAL                         1U

//Configurations
//Systick
#define TASK_QUANTA_MS                                1
#define TICK_RATE_HZ                                  (1000/TASK_QUANTA_MS)

//Task Configurations
#define NO_OF_TASKS                                   3
#define TASK_DESC_LENGTH                              100
#define MAX_NO_OF_PRIORITY                            4

//Stack Configurations
#define STACK_TYPE                                    STACK_TYPE_COMMON
#define STACKCOLOR_VALUE                              0xDEADBEEF

#if STACK_TYPE == STACK_TYPE_COMMON
#define STACKSIZE                                     200
#elif STACK_TYPE == STACK_TYPE_INDIVIDUAL
#define STACK_MAX_POOLSIZE                            512                   //no of words
#endif

//Scheduler Configurations
#define RTOS_SCHEDULER_START_DELAY                    3000
#define SCHEDULER                                     SCHEDULER_PRIORITY



//0 - disabled
//1 - enabled
#define INFO_PRINT                                    1
#define IDLE_TASK_PRINT                               0
#define SYSTICK_LED_TOGGLE                            1


#endif