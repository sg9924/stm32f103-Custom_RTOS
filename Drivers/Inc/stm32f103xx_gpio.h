#ifndef INC_stm32F103xx_GPIO_H
#define INC_stm32F103xx_GPIO_H

#include"stm32f103xx.h"
#include"stm32f103xx_rcc.h"

#include <stddef.h>

/************************************************** GPIO Definitions Start **************************************************/

// GPIO Ports
#define GPIOA                           ((GPIO_RegDef*)GPIOA_BASE_ADDR)
#define GPIOB                           ((GPIO_RegDef*)GPIOB_BASE_ADDR)
#define GPIOC                           ((GPIO_RegDef*)GPIOC_BASE_ADDR)
#define GPIOD                           ((GPIO_RegDef*)GPIOD_BASE_ADDR)
#define GPIOE                           ((GPIO_RegDef*)GPIOE_BASE_ADDR)
#define GPIOF                           ((GPIO_RegDef*)GPIOF_BASE_ADDR)
#define GPIOG                           ((GPIO_RegDef*)GPIOG_BASE_ADDR)

// GPIO Pin Numbers
#define GPIO_PIN0                       0
#define GPIO_PIN1                       1
#define GPIO_PIN2                       2
#define GPIO_PIN3                       3
#define GPIO_PIN4                       4
#define GPIO_PIN5                       5
#define GPIO_PIN6                       6
#define GPIO_PIN7                       7
#define GPIO_PIN8                       8
#define GPIO_PIN9                       9
#define GPIO_PIN10                      10
#define GPIO_PIN11                      11
#define GPIO_PIN12                      12
#define GPIO_PIN13                      13
#define GPIO_PIN14                      14
#define GPIO_PIN15                      15

// GPIO Modes
#define GPIO_MODE_IP                    0           /*<Input Mode - Reset State>*/
#define GPIO_MODE_OP                    1           /*<Output Mode>*/
#define GPIO_MODE_AF                    2           /*<Alternate Function Mode>*/
#define GPIO_MODE_INTRPT_FE_TRIG        3           /*<Interrupt Falling Edge Trigger Mode>*/
#define GPIO_MODE_INTRPT_RE_TRIG        4           /*<Interrupt Rising Edge Trigger Mode>*/
#define GPIO_MODE_INTRPT_FERE_TRIG      5           /*<Interrupt Falling/Rising Edge Trigger Mode>*/

// GPIO OP Mode Speeds
#define GPIO_OP_SPEED_0                 0
#define GPIO_OP_SPEED_10                1           /*<Output Mode - 10 MHz>*/
#define GPIO_OP_SPEED_2                 2           /*<Output Mode - 2 MHz>*/
#define GPIO_OP_SPEED_50                3           /*<Output Mode - 50 MHz>*/

// GPIO OP Mode Config Types
#define GPIO_CONFIG_GP_OP_PP                   0           /*<General Purpose Output Push Pull>>*/
#define GPIO_CONFIG_GP_OP_OD                   1           /*<General Purpose Output Open Drain>*/
#define GPIO_CONFIG_AF_OP_PP                   2           /*<Alternate Function Output Push Pull>*/
#define GPIO_CONFIG_AF_OP_OD                   3           /*<Alternate Function Output Open Drain>*/

// GPIO Other Mode Config Types
#define GPIO_CONFIG_ANALOG                     0           /*<Analog>*/
#define GPIO_CONFIG_FLOAT                      1           /*<Float>*/
#define GPIO_CONFIG_PU                         2           /*<Pull Up>*/
#define GPIO_CONFIG_PD                         3           /*<Pull Down>*/
#define GPIO_CONFIG_NO_PUPD                    GPIO_CONFIG_FLOAT

/*************************************************** GPIO Definitions End ***************************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/************************************************ GPIO Macros Definitions Start *********************************************/

// GPIO Clock Enable
#define GPIOA_PCLK_EN()         (RCC->APB2ENR |= 1<<2)
#define GPIOB_PCLK_EN()         (RCC->APB2ENR |= 1<<3)
#define GPIOC_PCLK_EN()         (RCC->APB2ENR |= 1<<4)
#define GPIOD_PCLK_EN()         (RCC->APB2ENR |= 1<<5)
#define GPIOE_PCLK_EN()         (RCC->APB2ENR |= 1<<6)
#define GPIOF_PCLK_EN()         (RCC->APB2ENR |= 1<<7)
#define GPIOG_PCLK_EN()         (RCC->APB2ENR |= 1<<8)

// GPIO Register Reset
#define GPIOA_REG_RESET()       do{ RCC->APB2RSTR |= (1<<2); RCC->APB2RSTR &= ~(1<<0); }while(0)
#define GPIOB_REG_RESET()       do{ RCC->APB2RSTR |= (1<<3); RCC->APB2RSTR &= ~(1<<1); }while(0)
#define GPIOC_REG_RESET()       do{ RCC->APB2RSTR |= (1<<4); RCC->APB2RSTR &= ~(1<<2); }while(0)
#define GPIOD_REG_RESET()       do{ RCC->APB2RSTR |= (1<<5); RCC->APB2RSTR &= ~(1<<3); }while(0)
#define GPIOE_REG_RESET()       do{ RCC->APB2RSTR |= (1<<6); RCC->APB2RSTR &= ~(1<<4); }while(0)
#define GPIOF_REG_RESET()       do{ RCC->APB2RSTR |= (1<<7); RCC->APB2RSTR &= ~(1<<5); }while(0)
#define GPIOG_REG_RESET()       do{ RCC->APB2RSTR |= (1<<8); RCC->APB2RSTR &= ~(1<<6); }while(0)

// GPIO Port to Code
#define GPIO_BASEADDR_TO_CODE(x)       ((x == GPIOA)?0:\
                                        (x == GPIOB)?1:\
                                        (x == GPIOC)?2:\
                                        (x == GPIOD)?3:\
                                        (x == GPIOE)?4:\
                                        (x == GPIOF)?5:\
                                        (x == GPIOG)?6:0)

//GPIO Config Lock Unlock
#define GPIO_CONFIG_LOCK
#define GPIO_CONFIG_UNLOCK

/************************************************** GPIO Macros Definitions End *********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/********************************************* GPIO Structure Definitions Start *********************************************/

//GPIO Pin Configuration Structure
typedef struct
{
    uint8_t PinNo;                          /*<Pin number>*/
    uint8_t PinMode;                        /*<Pin mode><input, output, alternate function>*/
    uint8_t PinOutputSpeed;                 /*<Pin output speed><2MHz, 10 MHz & 50 MHz>*/
    uint8_t PinConfigType;                  /*<Pin config type><push pull and opendrain for utput and alternate function mode*/
    uint8_t PinAltFuncMode;                 /*<!yet to defined, Not used currently!>*/
}GPIO_PinConfig;


// GPIO Handler Structure
typedef struct
{
    GPIO_RegDef    *pGPIOx;                 /*<GPIO Port Register Definition>*/
    GPIO_PinConfig  GPIOx_PinConfig;        /*<GPIO Port Pin Config Settings>*/
    uint16_t        GPIO_Pins_Used;
}GPIO_Handle;

/*to explore
// GPIO Pins Used
typedef struct
{
    uint8_t GPIOA_pins[16];
    uint8_t GPIOB_pins[16];
    uint8_t GPIOC_pins[16];
    uint8_t GPIOD_pins[16];
    uint8_t GPIOE_pins[16];
    uint8_t GPIOF_pins[16];
    uint8_t GPIOG_pins[16];
}GPIO_Pins;
*/

/********************************************** GPIO Structure Definitions End **********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** GPIO API's Declarations Start **********************************************/

/*to explore
void GPIO_Used_init();
void GPIO_Used_Update(uint32_t GPIOx, uint8_t* pins, size_t size);
*/

// GPIO Peripheral Clock Initialization
void GPIO_PClk_init(GPIO_RegDef* pGPIOx, uint8_t setup_mode);                       /*<>*/

//GPIO COnfiguration
void GPIO_Config(GPIO_Handle* pGPIOHandle, GPIO_RegDef* pGPIOx, uint8_t mode, uint8_t config_type, uint8_t pin_no, uint8_t op_speed);

// GPIO Initialisation & De-Initialisation
void GPIO_Init(GPIO_Handle* pGPIOHandle);                                           /*<>*/
void GPIO_DeInit(GPIO_RegDef* pGPIOx);                                              /*<>*/


// GPIO Read & Write
// GPIO Read & Write from & to Pin
void GPIO_WriteOpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no, uint8_t value);           /*<>*/
uint8_t GPIO_ReadIpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no);                        /*<>*/
void GPIO_WriteOpPort(GPIO_RegDef* pGPIOx, uint16_t value);                         /*<>*/
uint16_t GPIO_ReadIpPort(GPIO_RegDef* pGPIOx);                                      /*<>*/
uint16_t GPIO_ReadOpPort(GPIO_RegDef* pGPIOx);                                      /*<>*/
void GPIO_Bit_Set(GPIO_Handle* pGPIOHandle, uint8_t pin_no);
void GPIO_Bit_Reset(GPIO_Handle* pGPIOHandle, uint8_t pin_no);


// GPIO Toggle Output
void GPIO_OpToggle(GPIO_RegDef* pGPIOx, uint8_t pin_no);                            /*<>*/


//GPIO IRQ Configure
void GPIO_IRQ_Config(GPIO_Handle* pGPIOHandle, uint8_t mode);
// GPIO IRQ Handler
void GPIO_IRQHandling(uint8_t);                                                     /*<>*/

/************************************************ GPIO API's Declarations End ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/
#endif
