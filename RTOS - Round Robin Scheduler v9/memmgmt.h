#ifndef _MEMMGMT_H_
#define _MEMMGMT_H_

#include <stdint.h>
#include <stddef.h>

void* xmemset(void* ptr, int value, size_t no_of_bytes);
void* xmemcopy(void* dest, void* src, size_t size);
void* xmemmove(void* dest, void* src, size_t size);

#endif