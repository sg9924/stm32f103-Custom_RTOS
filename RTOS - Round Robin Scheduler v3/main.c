#include "rtos_kernel.h"
#include "rtos_task.h"
#include "rtos_port.h"
#include "Semaphore.h"

//Set quanta in milliseconds for the round robin scheduler
#define TASK_QUANTA_MS          500

int8_t s1, s2;


extern tcb_t TCBS[NO_OF_TASKS+1];  //an array of TCB's


/*
--normal semaphore
task1 sema has 1
task2 sema has 0

in task1
take for task 1
task code
give for task2

in task2
take for task 2 (wait)
task code
give for task 1


-- rendevous semaphore
task1 sema has 1
task2 sema has 0

in task1
give for task2
take for task 1 (wait)
task code


in task2
give for task 1
take for task 2 (wait)
task code

*/

void task1(void)
{
    while(1)
    {
        Serialprintln("[%x] This is Task 1 running...", INFO, Systick_get_tick());
        //taskDelay(100);
    }
}

void task2(void)
{
    while(1)
    {
        Serialprintln("[%x] This is Task 2 running...", INFO, Systick_get_tick()); 
    }
}


void task3(void)
{
    while(1)
    {
        Serialprintln("[%x] This is Task 3 running...", INFO, Systick_get_tick()); 
    }
}




int main(void)
{
    board_init();

    char a[50] = "Hello";
    char b[50] = " There";

    __task_count_init();

    //Add the tasks
    taskAdd(&task1, "Task 1");
    taskAdd(&task2, "Task 2");
    taskAdd(&task3, "Task 3");

    //Semaphore_Init(&s1, 1);
    //Semaphore_Init(&s2, 0);

    //Launch Kernel
    rtosKernel_Launch(TASK_QUANTA_MS);

    while(1)
    {

    }
}