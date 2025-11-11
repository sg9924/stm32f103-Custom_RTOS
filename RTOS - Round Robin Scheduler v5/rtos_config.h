#ifndef __RTOSCONFIG_H__
#define __RTOSCONFIG_H__

#define TASK_QUANTA_MS          1000
#define TICK_RATE_HZ                                  (1000/TASK_QUANTA_MS)

#define NO_OF_TASKS                                   3
#define STACKSIZE                                     200
#define SCHEDULER                                     SCHEDULER_ROUND_ROBIN
#define RTOS_SCHEDULER_START_DELAY                    3000


//Configurations
//0 - disabled
//1 - enabled

#define INFO_PRINT                                    1



//definitions
#define SCHEDULER_ROUND_ROBIN                         (1<<0)
#define SCHEDULER_RR_WEIGHTED                         (1<<1)


#endif