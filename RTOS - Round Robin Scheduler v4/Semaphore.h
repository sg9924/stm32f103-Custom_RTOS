#ifndef __SEMAPHORE__
#define __SEMAPHORE__
#include <stdint.h>


void Semaphore_Init(int8_t* semaphore, int8_t value);
void Semaphore_Give(int8_t* semaphore);
void Semaphore_Take(int8_t* semaphore);
void Semaphore_CooperativeTake(int8_t *semaphore);


#endif