#ifndef INC_STM32F103XX_SERIAL_H
#define INC_STM32F103XX_SERIAL_H
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_usart.h"
#include"stm32f103xx.h"
#include"stm32f103xx_utilities.h"

#include<stdarg.h>

#define FLOAT_PRECISION_MAX            6

#define BUFF_SIZE                      1024
#define SERIAL_DELAY                   0

#define SERIAL_NL()                    (Serialprintln("", NONE))

//Serial Input Data Type Formats
#define SERIAL_RX_STRING               0
#define SERIAL_RX_INT                  1
#define SERIAL_RX_CHAR                 2

//Serial Message Type
#define NONE                           ('N')
#define DEBUG                          ('D')
#define INFO                           ('I')
#define ASSERT                         ('A')
#define FATAL                          ('F')
#define WARN                           ('W')

#define SERIAL_DEBUG_STRING            ("[DEBUG]: ")
#define SERIAL_INFO_STRING             ("[INFO]: ")
#define SERIAL_WARN_STRING             ("[WARN]: ")
#define SERIAL_FATAL_STRING            ("[FATAL]: ")
#define SERIAL_ASSERT_STRING           ("[ASSERT]: ")
#define SERIAL_NL_STRING               ("\r\n")

#define SERIAL_DEBUG_STRING_W_NL       ("\r\n[DEBUG]: ")
#define SERIAL_INFO_STRING_W_NL        ("\r\n[INFO]: ")
#define SERIAL_WARN_STRING_W_NL        ("\r\n[WARN]: ")
#define SERIAL_FATAL_STRING_W_NL       ("\r\n[FATAL]: ")
#define SERIAL_ASSERT_STRING_W_NL      ("\r\n[ASSERT]: ")



void _print_buffer(char* buffer, uint32_t* buff_ind);
void _print_int(int32_t value, char* buffer, uint32_t* buff_ind);
void _print_hex(uint32_t value, char* buffer, uint32_t* buff_ind);
void _print_float(double value, char* buffer, uint32_t* buff_ind, uint8_t precision);
void _reset_buffer(uint32_t* buff_ind);

void Serial_UART_init();
void Serial_init();

uint8_t Serialprint(const char *format, uint8_t msg_type, ...);
uint8_t Serialprintln(char *format, uint8_t msg_type, ...);
//void SerialInput(char* msg, uint8_t datatype, void* d);
//void SerialInput(char* msg, char* format, ...);


#endif