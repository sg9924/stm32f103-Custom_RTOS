#ifndef INC_stm32F103xx_AFIO_MAPPINGS
#define INC_stm32F103xx_AFIO_MAPPINGS
#include"stm32f103xx_gpio.h"

typedef struct 
{
    uint32_t GPIO;
    uint32_t peripheral;
    uint8_t  pins[6];
    uint8_t  pins_remap[6];
}AF_Mapping;


//Debugging
#define AF_PIN_JTMS
#define AF_PIN_JTCK
#define AF_PIN_JTDI
#define AF_PIN_JTDO
#define AF_PIN_SWDIO
#define AF_PIN_SWCLK
#define AF_PIN_TRACESWO
#define AF_PIN_NJTRST
#define AF_PIN_TRACECK
#define AF_PIN_TRACED0
#define AF_PIN_TRACED1
#define AF_PIN_TRACED2
#define AF_PIN_TRACED3



//USART2
#define USART2_GPIO         GPIOA_BASE_ADDR
#define USART2_CTS          GPIO_PIN0
#define USART2_RTS          GPIO_PIN1
#define USART2_TX           GPIO_PIN2
#define USART2_RX           GPIO_PIN3
#define USART2_CK           GPIO_PIN4


#endif /*INC_stm32F103xx_AFIO_MAPPINGS*/