#ifndef MATHLIB_H
#define MATHLIB_H

#define CEIL(x) do{\
    if((int32_t)x < x)\
        x = (int32_t)x + 1;\
    }while(0)

#define FLOOR(x) x = (int32_t)x




#endif