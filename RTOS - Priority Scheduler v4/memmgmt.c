#include"memmgmt.h"


void* xmemset(void* ptr, int value, size_t no_of_bytes)
{
    char* t = (char*)ptr;
    uint8_t i = 0;
    
    while(i++ <= no_of_bytes)
    {
        *(t++) = (char) value;
    }
    return ptr;
}


void* xmemcopy(void* dest, void* src, size_t size)
{
    char* temp = (char*) dest;

    while(size--)
    {
        *((char*)dest++) = *((char*)src++);
    }
    return temp;
}


void* xmemmove(void* dest, void* src, size_t size)
{
    char* temp = (char*) dest;
    char temp_arr[size];

    //source to temp
    for(uint8_t i=0; i<size; i++)
    {
        temp_arr[i] = *((char*)src++);
    }

    //temp to dest
    for(uint8_t i=0; i<size; i++)
    {
        *((char*)dest++) = temp_arr[i];
    }

    return temp;
}