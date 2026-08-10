#ifndef __RTOSCONFIG_H__
#define __RTOSCONFIG_H__


//Definitions
//Schedulers
#define SCHEDULER_ROUND_ROBIN                         (1<<0)
#define SCHEDULER_RR_WEIGHTED                         (1<<1)

//Configurations
//Systick
#define TASK_QUANTA_MS                                100
#define TICK_RATE_HZ                                  (1000/TASK_QUANTA_MS)

//Task Configurations
#define NO_OF_TASKS                                   3
#define TASK_NOTI_MAX_SIZE                            3                      //This doesn't include the default Idle Task
#define TASK_DELAY_ABS_CATCHUP_MODE                   0                      //Catchup mode in taskDelayAbs


//Stack Configurations
#define STACKCOLOR_VALUE                              0xDEADBEEF
#define STACK_MAX_POOLSIZE                            512                    //no of words

//Scheduler Configurations
#define RTOS_SCHEDULER_START_DELAY                    3000
#define SCHEDULER                                     SCHEDULER_ROUND_ROBIN


//0 - disabled
//1 - enabled

#define INFO_PRINT                                    1
#define SYSTICK_LED_TOGGLE                            1


#endif