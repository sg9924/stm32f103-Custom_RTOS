#ifndef STM32F103XX_I2C_H
#define STM32F103XX_I2C_H

#include"stm32f103xx.h"
#include"stm32f103xx_gpio.h"
#include"stm32f103xx_rcc.h"

typedef struct
{
    uint8_t  ack_control;
    uint8_t  mode_type;
    uint8_t  mode;
    uint32_t speed;
    uint8_t  fm_duty_cycle;
    uint8_t  address_type;
    uint8_t  device_address;
}I2C_Config;

typedef struct
{
    I2C_RegDef*  pI2Cx;
    I2C_Config   I2CConfig;
    GPIO_Handle* pGPIOHandle;
    uint8_t*     ptxbuffer;
    uint8_t*     prxbuffer;
    uint32_t     tx_len;
    uint32_t     rx_len;
    uint8_t      tx_state;
    uint8_t      rx_state;
}I2C_Handle;


//I2C CR1 Bit Field Definitions
#define I2C_CR1_PE               0
#define I2C_CR1_SMBUS            1
#define I2C_CR1_SMBTYPE          3
#define I2C_CR1_ENARP            4
#define I2C_CR1_ENPEC            5
#define I2C_CR1_ENGC             6
#define I2C_CR1_NOSTRETCH        7
#define I2C_CR1_START            8
#define I2C_CR1_STOP             9
#define I2C_CR1_ACK              10
#define I2C_CR1_POS              11
#define I2C_CR1_PEC              12
#define I2C_CR1_ALERT            13
#define I2C_CR1_SWRST            15

//I2C CR2 Bit Field Definitions
#define I2C_CR2_FREQ             0
#define I2C_CR2_ITERREN          8
#define I2C_CR2_ITEVTEN          9
#define I2C_CR2_ITBUFEN          10
#define I2C_CR2_DMAEN            11
#define I2C_CR2_LAST             12

//I2C OAR1 Bit Field Definitions
#define I2C_OAR1_ADD0            0
#define I2C_OAR1_ADD1            1
#define I2C_OAR1_ADD8            8
#define I2C_OAR1_ADDMODE         15

//I2C OAR2 Bit Field Definitions
#define I2C_OAR2_ENDUAL          0
#define I2C_OAR2_ADD2_1          1

//I2C SR Bit Field Definitions
#define I2C_SR1_SB                0
#define I2C_SR1_ADDR              1
#define I2C_SR1_BTF               2
#define I2C_SR1_ADD10             3
#define I2C_SR1_STOPF             4
#define I2C_SR1_RXNE              6
#define I2C_SR1_TXE               7
#define I2C_SR1_BERR              8
#define I2C_SR1_ARLO              9
#define I2C_SR1_AF                10
#define I2C_SR1_OVR               11
#define I2C_SR1_PECERR            12
#define I2C_SR1_TIMEOUT           14
#define I2C_SR1_SMBALERT          15

//I2C SR2 Bit Field Definitions
#define I2C_SR2_MSL              0
#define I2C_SR2_BUSY             1
#define I2C_SR2_TRA              2
#define I2C_SR2_GENCALL          4
#define I2C_SR2_SMBDEFAULT       5
#define I2C_SR2_SMBHOST          6
#define I2C_SR2_DUALF            7
#define I2C_SR2_PEC              8

//I2C CCR Bit Field Definitions
#define I2C_CCR_CCR              0
#define I2C_CCR_DUTY             14
#define I2C_CCR_FS               15

//I2C TRISE Bit Field Definitions
#define I2C_TRISE0               0
#define I2C_TRISE1               1
#define I2C_TRISE2               2
#define I2C_TRISE3               3
#define I2C_TRISE4               4
#define I2C_TRISE5               5


//I2C ACK Control
#define I2C_ACK_DISABLE          0
#define I2C_ACK_ENABLE           1
#define I2C_ACK_DEFAULT          I2C_ACK_ENABLE

//I2C Address Type
#define I2C_ADDR_7BIT            0
#define I2C_ADDR_10BIT           1

//I2C Mode Type
#define I2C_MODE_TYPE_STD        0
#define I2C_MODE_TYPE_FAST       1

//I2C Mode
#define I2C_MODE_MASTER          0
#define I2C_MODE_SLAVE           1

//I2C SCL Speeds
#define I2C_SCL_SPEED_STD        100000
#define I2C_SCL_SPEED_F2K        200000
#define I2C_SCL_SPEED_F4K        400000

//I2C Fast Mode Duty Cycle
#define I2C_FM_DUTY_2            0
#define I2C_FM_DUTY_16_9         1
#define I2C_FM_DUTY_NONE         2

//I2C Repeated Start Setting
#define I2C_REPEAT_START_DISABLE 0
#define I2C_REPEAT_START_ENABLE  1

//I2C States
#define I2C_STATE_READY          0
#define I2C_STATE_TX_BUSY        1
#define I2C_STATE_RX_BUSY        2

//I2C Interrupts
//I2C Events
#define I2C_EV_SB                1
#define I2C_EV_ADDR              2
#define I2C_EV_ADD10             3
#define I2C_EV_STOPF             4
#define I2C_EV_BTF               5
#define I2C_EV_RXNE              6
#define I2C_EV_TXE               7

//I2C Errors
#define I2C_ERR_BERR             8
#define I2C_ERR_ARLO             9
#define I2C_ERR_AF               10
#define I2C_ERR_OVR              11
#define I2C_ERR_PECERR           12
#define I2C_ERR_TIMEOUT          13
#define I2C_ERR_SMBALERT         14


// I2C Peripherals Definition
#define I2C1                     ((I2C_RegDef*)I2C1_BASE_ADDR)
#define I2C2                     ((I2C_RegDef*)I2C2_BASE_ADDR)


//I2C Peripheral Clock Enable
#define I2C1_PCLK_ENABLE()       (RCC->APB1ENR |= 1<<RCC_APB1ENR_I2C1EN)
#define I2C2_PCLK_ENABLE()       (RCC->APB1ENR |= 1<<RCC_APB1ENR_I2C2EN)

//I2C Peripheral Clock Disable
#define I2C1_PCLK_DISABLE()      (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_I2C1EN))
#define I2C2_PCLK_DISABLE()      (RCC->APB1ENR &= ~(1<<RCC_APB1ENR_I2C2EN))

//I2C Peripheral Enable
#define I2C1_ENABLE()            (I2C1->CR1 |= 1<<I2C_CR1_PE)
#define I2C2_ENABLE()            (I2C2->CR1 |= 1<<I2C_CR1_PE)

//I2C Peripheral Disable
#define I2C1_DISABLE()           (I2C1->CR1 &= ~(1<<I2C_CR1_PE))
#define I2C2_DISABLE()           (I2C2->CR1 &= ~(1<<I2C_CR1_PE))



//Function Declarations
void I2C_PClk_init(I2C_Handle* pI2CHandle, uint8_t mode);
void I2C_P_init(I2C_Handle* pI2CHandle, uint8_t mode);
void I2C_Configure(I2C_Handle* pI2CHandle, uint8_t ack_control, uint8_t mode, uint8_t mode_type, uint32_t speed, uint8_t fm_duty_cycle, uint8_t address_type, uint8_t device_address);
void I2C_init(I2C_Handle* pI2CHandle, GPIO_Handle* pGPIOHandle, I2C_RegDef* pI2Cx);

void I2C_Master_Send(I2C_Handle* pI2CHandle, uint8_t* ptxbuffer, uint32_t len, uint8_t slave_addr, uint8_t repeat_start);
void I2C_Master_Receive(I2C_Handle* pI2CHandle, uint8_t *prxbuffer, uint32_t len, uint8_t slave_addr, uint8_t repeat_start);

void I2C_Slave_Receive(I2C_Handle* pI2CHandle, uint8_t* prxbuffer, uint8_t len, uint8_t repeat_start);
void I2C_Slave_Send(I2C_Handle* pI2CHandle, uint8_t* ptxbuffer, uint32_t len, uint8_t repeat_start);

//Helper Functions
void I2C_Clear_ADDR_Flag(I2C_Handle* pI2CHandle);
void I2C_ACK_Enable(I2C_Handle* pI2CHandle);
void I2C_ACK_Disable(I2C_Handle* pI2CHandle);
void I2C_STOP_Set(I2C_Handle* pI2CHandle);
void I2C_STOP_Reset(I2C_Handle* pI2CHandle);
void I2C_Address_Phase_Read(I2C_Handle* pI2CHandle, uint8_t slave_addr);
void I2C_Address_Phase_Write(I2C_Handle* pI2CHandle, uint8_t slave_addr);



#endif