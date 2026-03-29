#include"stm32f103xx_utilities.h"
#include"stm32f103xx_serial.h"
#include<stdarg.h>

//char temp_buff[33];
char temp_buff[64];
char result_buff[64];

static uint32_t bi;
static char str_buffer[1024];


void wait_ms(uint16_t d)
{
	for(int i=0; i<(d*350); i++);
}

void wait(uint8_t d)
{
	wait_ms(d*1000);
}

uint32_t string_size(char* data)
{
    uint32_t c=0;
    while(*data++ != '\0')
        c++;
    return c;
}

char* string_truncate(char* pbuff, uint8_t length, uint8_t no_of_chars, uint8_t direction)
{
    if(direction == TRUNCATE_RIGHT)
        pbuff[length - 1 - no_of_chars + 1] = '\0';
    
    return pbuff;
}

char* string_concat(char* s1, char* s2)
{
    char* temp = s1;
    //traverse to end of s1
    temp = end_of_string(s1);
    temp--;
    
    while(*s2 != '\0')
    {
        *(temp++) = *(s2++);
    }
    *temp='\0';

    return s1;
}

char* string_trim(char* pbuff, uint8_t direction, char character, uint32_t length)
{
    if(direction == TRIM_RIGHT)
    {
        uint8_t i = length;
        while(pbuff[--i] == character);
        pbuff[++i] = '\0';
    }
    else if(direction == TRIM_LEFT)
    {
        uint8_t i = 0;
        while(pbuff[i++] != character);
        pbuff[i] = '\0';
    }
    return pbuff;
}



char* string_format(char* str, ...)
{
    bi = 0;

	va_list args;              // initializing list pointer 
    va_start(args, str);       // Initialize the argument list

    while (*str != '\0')       // Iterate over each character in the format string
    {
        if (*str == '%')       // Check for the start of a conversion specifier
        {
            str++;             // Move to the next character after '%'
            if (*str == '%')   // Case: '%%' prints a single '%'
            {
                str_buffer[bi++] = '%';
            }
            else if (*str == 'c')          // Case: '%c' prints a character
            {
                int ch = va_arg(args, int);   // Fetch the next argument as int
                str_buffer[bi++] = ch;
            }
            else if (*str == 's')                 // Case: '%s' prints a string
            {
                char *str = va_arg(args, char *);    // Fetch the next argument as char*
                while (*str)                         // Iterate over each character in the string
                {
                    str_buffer[bi++] = *str;
                    str++;
                }
            }
            else if (*str == 'd' || *str == 'i') // Case: '%d' or '%i' prints an integer
            {
                int value = va_arg(args, int);
                _print_int(value, str_buffer, &bi);
            }
            else if(*str == 'x')
            {
                unsigned int value = va_arg(args, unsigned int);
                _print_hex(value, str_buffer, &bi);
            }
            else if(*str == '.' && *(str+2) == 'f')
            {
                double value = va_arg(args, double);
                _print_float(value, str_buffer, &bi, *(++str) - '0');
                ++str;
            }
            else if(*str == 'f')
            {
                double value = va_arg(args, double);
                _print_float(value, str_buffer, &bi, 6);
            }
        }
        else // Case: Regular character, not a conversion specifier
        {
            str_buffer[bi++] = *str;
        }

        str++; // Move to the next character in the format string
    }
    va_end(args); // Clean up the argument list
    
    str_buffer[bi] = '\0';
    return str_buffer;
}





char* array_reverse(char* pbuff, uint8_t length)
{
    uint8_t i=0, j=length-1;
    char temp;

    while(i<=j)
    {
        temp = pbuff[i];
        pbuff[i] = pbuff[j];
        pbuff[j] = temp;
        i++;
        j--;
    }
    return pbuff;
}


char* string_reverse(char* pbuff, uint8_t length)
{
    return (array_reverse(pbuff, length-1)); //send string excluding '/0' character
}

char* end_of_string(char* str)
{
    char* ptr = str;

    while(*(ptr++) != '\0');
    
    return ptr;
}


char* string_copy(char* dest, char* src)
{
    char* ptr = dest;

    while(*src != '\0')
    {
        *ptr = *src;
        ptr++;
        src++;
    }
    *ptr = '\0';

    return dest;
}


/* @digit_extract
Desc: Extracts Digits of uint32_t and stores it in a string
Input:
    uint32_t -> number
    char*    -> buffer address  -> address of buffer where the digits should be stored
    int8_t   -> buffer index    -> starting index from where the digits should be stored
Output:
    int8_t   -> buffer index    -> current index of the buffer
Note:
    NULL maybe passed to use the result_buff defined here at the top, else a specific buffer address can also be passed
*/
int8_t digit_extract(uint32_t num, char* pbuff, int8_t buff_index)
{
    int8_t i = 0, j = buff_index;

    if(pbuff == NULL)
        pbuff = result_buff;

    //extract digits in reverse
    while(num!=0)
    {
        temp_buff[i] = '0' + (num%10);
        num=num/10;
        i++;
    }

    //rearrange the digits in order
    i--;
    while(i>=0)
    {
        pbuff[j] = temp_buff[i];
        i--;
        j++;
    }
    pbuff[j]='\0';

    return j;
}


char* parse_int(int32_t num, char* pbuff)
{
    int8_t index;

    if(pbuff == NULL)
        pbuff = result_buff;

    if(num<0)
    {
        pbuff[0] = '-';
        index = 1;
    }
    else
        index = 0;
    
    digit_extract((uint32_t)num, pbuff, index);

    return pbuff;
}


char* parse_float(float num, char* pbuff, int8_t precision)
{
    int8_t index;

    if(pbuff == NULL)
        pbuff = result_buff;

    if((int32_t)num<0)
    {
        pbuff[0] = '-';
        index = 1;
    }
    else
        index = 0;

    //separate integral and float part
    int32_t integral = (int)num;
    float fractional;

    //integral part parse
    index = digit_extract((uint32_t)integral, pbuff, index);
    pbuff[index] = '.';
    index++;

    //parse decimal part
    if(precision == 0)
    {
        pbuff[index] = '0';
    }
    else if(precision>0 && precision<=6)
    {
        fractional = num - (float)integral;
        //fractional = fractional * power(10, precision);
        while(precision-- > 0)
        {
            fractional = fractional * 10;
            if((uint32_t)fractional == 0) //if Integer part is still, add 0 to string -> handle cases like 23.005
            {
                pbuff[index]='0';
                index++;
            }
        }
    }
    digit_extract((uint32_t)fractional, pbuff, index);

    return pbuff;
}



int32_t string_to_int(char* str)
{
    int8_t sign = 1;
    int32_t result = 0;

    if(*str == '-')
    {
        sign = -1;
        str++;
    }

    for(;*str != '\0'; str++)
    {
        if(*str >= '0' && *str <= '9')
            result = (result*10) + (*str - '0');
        else
            return 1;
    }

    return (sign * result);
}



uint32_t power(uint8_t num, uint8_t power)
{
    uint32_t result=1;
    while(power!=0)
    {
        result = result*num;
        power--;
    }

    return result;
}

/*
char* digit_extract(uint32_t num)
{
    uint32_t temp = num;
    char buff[16];
    static char digits[16];
    int8_t i = 0, j = 0;

    //extract digits in reverse
    while(temp!=0)
    {
        buff[i] = '0' + (temp%10);
        temp=temp/10;
        i++;
    }
    //rearrange the digits in order
    i--;
    while(i>=0)
    {
        digits[j] = buff[i];
        i--;
        j++;
    }

    return digits;
}
*/


uint8_t no_of_digits(uint32_t num)
{
    uint8_t i=0;

    while(num!=0)
    {
        temp_buff[i] = '0' + (num%10);
        num=num/10;
        i++;
    }

    return i;
}