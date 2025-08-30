#ifndef INC_STM32F103XX_UTILITIES_H
#define INC_STM32F103XX_UTILITIES_H

#include<stdint.h>
#include"math_lib.h"

#define NULL ((void *)0)

#define TRIM_LEFT               0
#define TRIM_RIGHT              1

#define TRUNCATE_LEFT           0
#define TRUNCATE_RIGHT          1


void wait_ms(uint16_t d);
void wait(uint8_t d);

uint32_t string_size(char* data);
char* string_truncate(char* pbuff, uint8_t length, uint8_t no_of_chars, uint8_t direction);
char* string_concat(char* s1, char* s2);
char* string_trim(char* pbuff, uint8_t direction, char character, uint32_t length);
char* array_reverse(char* pbuff, uint8_t length);
char* string_reverse(char* pbuff, uint8_t length);

int8_t digit_extract(uint32_t num, char* pbuff, int8_t buff_index);
char* parse_int(int32_t num, char* pbuff);
char* parse_float(float num, char* pbuff, int8_t precision);

int32_t string_to_int(char* str);
//char* digit_extract(uint32_t num);
uint8_t no_of_digits(uint32_t num);

uint32_t power(uint8_t num, uint8_t power);

#endif /*INC_STM32F103XX_UTILITIES_H*/