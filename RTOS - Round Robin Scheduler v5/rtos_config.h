#ifndef __RTOSCONFIG_H__
#define __RTOSCONFIG_H__


#define NO_OF_TASKS                                   3
#define STACKSIZE                                     200
#define SCHEDULER                                     SCHEDULER_RR_WEIGHTED


//Configurations
//0 - disabled
//1 - enabled

#define INFO_PRINT                                    1



//definitions
#define SCHEDULER_ROUND_ROBIN                         (1<<0)
#define SCHEDULER_RR_WEIGHTED                         (1<<1)


#endif