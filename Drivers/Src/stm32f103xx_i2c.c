#include"stm32f103xx_i2c.h"


//I2C Peripheral Clock Init
void I2C_PClk_init(I2C_Handle* pI2CHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_PCLK_ENABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_PCLK_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_PCLK_DISABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_PCLK_DISABLE();
    }
}

//I2C Peripheral Enable / Disable
void I2C_P_init(I2C_Handle* pI2CHandle, uint8_t mode)
{
    if(mode == ENABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_ENABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_ENABLE();
    }
    else if(mode == DISABLE)
    {
        if(pI2CHandle->pI2Cx == I2C1)
            I2C1_DISABLE();
        else if(pI2CHandle->pI2Cx == I2C2)
            I2C2_DISABLE();
    }
}

//I2C Configuration
void I2C_Configure(I2C_Handle* pI2CHandle, uint8_t ack_control, uint8_t mode, uint8_t mode_type, uint32_t speed, uint8_t fm_duty_cycle, uint8_t address_type, uint8_t device_address)
{
    pI2CHandle->I2CConfig.ack_control      = ack_control;
    pI2CHandle->I2CConfig.address_type     = address_type;
    pI2CHandle->I2CConfig.device_address   = device_address;
    pI2CHandle->I2CConfig.fm_duty_cycle    = fm_duty_cycle;
    pI2CHandle->I2CConfig.mode             = mode;
    pI2CHandle->I2CConfig.mode_type        = mode_type;
    pI2CHandle->I2CConfig.speed            = speed;

    pI2CHandle->prxbuffer = 0;
    pI2CHandle->ptxbuffer = 0;
    pI2CHandle->rx_len    = 0;
    pI2CHandle->tx_len    = 0;
    pI2CHandle->rx_state  = I2C_STATE_READY;
    pI2CHandle->tx_state  = I2C_STATE_READY;
}


//I2C GPIO Pins Init
void I2C_GPIO_init(I2C_Handle* pI2CHandle)
{
    //Open Drain Configurations, External Pullups of 4.7K to +3.3V should be used
    pI2CHandle->pGPIOHandle->pGPIOx = GPIOB;

    if(pI2CHandle->pI2Cx == I2C1)
    {
        //PB6 - SCL, PB7 - SDA
        //SCL
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN6, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);

        //SDA
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN7, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);
    }
    else if(pI2CHandle->pI2Cx == I2C2)
    {
        //PB10 - SCL, PB11 - SDA
        //SCL
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN10, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);

        //SDA
        GPIO_Config(pI2CHandle->pGPIOHandle, GPIOB, GPIO_MODE_AF, GPIO_CONFIG_AF_OP_OD, GPIO_PIN11, GPIO_OP_SPEED_2);
        GPIO_Init(pI2CHandle->pGPIOHandle);
    }
}


//I2C Peripheral Init
void I2C_init(I2C_Handle* pI2CHandle, GPIO_Handle* pGPIOHandle, I2C_RegDef* pI2Cx)
{
    pI2CHandle->pGPIOHandle = pGPIOHandle;
    pI2CHandle->pI2Cx       = pI2Cx;
    
    //Enable Peripheral Clock
    I2C_PClk_init(pI2CHandle, ENABLE);

    //Configure GPIO pins for I2C
    I2C_GPIO_init(pI2CHandle);
    

    uint32_t temp = 0;

    //Enable ACK (doesn't work when PE is 0)
    //pI2CHandle->pI2Cx->CR1 |= (pI2CHandle->I2CConfig.ack_control)<<I2C_CR1_ACK;

    //Configure Frequency
    temp = 8000000/1000000;
    pI2CHandle->pI2Cx->CR2 |= (temp & 0x3F)<<I2C_CR2_FREQ;

    //5. Configure Host Device Address for Slave Mode
    if(pI2CHandle->I2CConfig.mode == I2C_MODE_SLAVE)
    {
        if(pI2CHandle->I2CConfig.address_type == I2C_ADDR_7BIT)
        {
            temp=0;
            temp = pI2CHandle->I2CConfig.device_address<<I2C_OAR1_ADD1;
            pI2CHandle->pI2Cx->OAR1 |= temp;
        }
        else if(pI2CHandle->I2CConfig.address_type == I2C_ADDR_10BIT)
        {
            //to do
        }
    }

    //Configure CCR
    temp=0;
    temp |= pI2CHandle->I2CConfig.mode_type<<I2C_CCR_FS;         // Standard or Fast Mode Config

    if(pI2CHandle->I2CConfig.mode_type == I2C_MODE_TYPE_STD)
    {
        temp |= 8000000/(2 * pI2CHandle->I2CConfig.speed);
    }
    else
    {
        temp |= pI2CHandle->I2CConfig.fm_duty_cycle << I2C_CCR_DUTY;

        if(pI2CHandle->I2CConfig.fm_duty_cycle == I2C_FM_DUTY_2)
            temp |= 8000000/(3 * pI2CHandle->I2CConfig.speed);
        else if(pI2CHandle->I2CConfig.fm_duty_cycle == I2C_FM_DUTY_16_9)
            temp |= 8000000/(25 * pI2CHandle->I2CConfig.speed);
    }
    pI2CHandle->pI2Cx->CCR |= temp;

    //Configure TRISE
    if(pI2CHandle->I2CConfig.mode_type == I2C_MODE_TYPE_STD)
        pI2CHandle->pI2Cx->TRISE |= ((8000000/1000000) + 1) << I2C_TRISE0;
    else if(pI2CHandle->I2CConfig.mode_type == I2C_MODE_TYPE_FAST)
        pI2CHandle->pI2Cx->TRISE |= ((8000000/1000000)*300 + 1) << I2C_TRISE0;

    //8. Enable I2C Peripheral
    I2C_P_init(pI2CHandle, ENABLE);
}


void I2C_Master_Send(I2C_Handle* pI2CHandle, uint8_t* ptxbuffer, uint32_t len, uint8_t slave_addr, uint8_t repeat_start)
{
    pI2CHandle->ptxbuffer = ptxbuffer;
    pI2CHandle->tx_len = len;

    //Enable ACK before Reception after PE is set
    //I2C_ACK_Enable(pI2CHandle);

    //wait till bus is free
    while((pI2CHandle->pI2Cx->SR2 & 1<<I2C_SR2_BUSY));

    //Generate Start Condition
    pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_START;

    //wait till start generation is completed
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_SB));

    //Address Phase, send Slave address for write
    I2C_Address_Phase_Write(pI2CHandle, slave_addr);

    //slave_addr = slave_addr<<1;  //first 7 bits are address
    //slave_addr &= ~(1);          //last 8th bit is R/NW bit - 0 for write
    //pI2CHandle->pI2Cx->DR = slave_addr;

    //wait till address phase is completed
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_ADDR));

    //clear the ADDR flag, SCL will be pulled to LOW until ADDR is cleared
    I2C_Clear_ADDR_Flag(pI2CHandle);

    //wait till TXE is set
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_TXE));

    //send the data until length becomes zero
    while(pI2CHandle->tx_len > 0)
    {
        //wait till TXE is set
        while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_TXE));

        //load data in DR
        pI2CHandle->pI2Cx->DR = *ptxbuffer;
        ptxbuffer++;
        (pI2CHandle->tx_len)--;
    }

    //when length becomes 0, wait for TXE=1 and BTF=1
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_TXE));
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_BTF));

    //Generate Stop Condition, this also automatically clears the BTF
    if(repeat_start == I2C_REPEAT_START_DISABLE)
        pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_STOP;
}


void I2C_Master_Receive(I2C_Handle* pI2CHandle, uint8_t* prxbuffer, uint32_t len, uint8_t slave_addr, uint8_t repeat_start)
{
    pI2CHandle->prxbuffer = prxbuffer;
    pI2CHandle->rx_len = len;

    //Enable ACK before Reception after PE is set
    I2C_ACK_Enable(pI2CHandle);

    //Generate the Start Condition
    pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_START;

    //wait till start generation is completed
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_SB));

    //Address Phase, send Slave address for Read
    I2C_Address_Phase_Read(pI2CHandle, slave_addr);

    //slave_addr = slave_addr<<1;  //first 7 bits are address
    //slave_addr |= 1;             //last 8th bit is R/NW bit - 1 for read
    //pI2CHandle->pI2Cx->DR = slave_addr;

    //wait till address phase is completed
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_ADDR));

    //Read Data - single byte
    if(pI2CHandle->rx_len == 1)
    {
        //Disable ACK as only one byte will be received
        I2C_ACK_Disable(pI2CHandle);

        //Clear ADDR Flag
        pI2CHandle->pI2Cx->SR1 &= ~(1<<I2C_SR1_ADDR);

        //Wait till RXNE becomes 1
        while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_RXNE));

        //Generate Stop Condition
        if(repeat_start == I2C_REPEAT_START_DISABLE)
            pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_STOP;

        //Read Data into buffer
        *prxbuffer = pI2CHandle->pI2Cx->DR;
    }
    else if(pI2CHandle->rx_len > 1)
    {
        //Clear ADDR flag
        I2C_Clear_ADDR_Flag(pI2CHandle);

        while(pI2CHandle->rx_len > 0)
        {
            //Wait till RXNE becomes 1
            while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_RXNE));

            if(pI2CHandle->rx_len == 2)
            {
                //Disable ACK
                I2C_ACK_Disable(pI2CHandle);

                //Generate Stop Condition
                if(repeat_start == I2C_REPEAT_START_DISABLE)
                    pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_STOP;
            }

            //Read Data into buffer
            *prxbuffer = pI2CHandle->pI2Cx->DR;

            prxbuffer++;
            (pI2CHandle->rx_len)--;
        }
    }

    //Re-enable ACK for future transactions
    I2C_ACK_Enable(pI2CHandle);
}



void I2C_Slave_Send(I2C_Handle* pI2CHandle, uint8_t* ptxbuffer, uint32_t len, uint8_t repeat_start)
{
    pI2CHandle->ptxbuffer = ptxbuffer;
    pI2CHandle->tx_len = len;

    //Enable ACK before Reception after PE is set
    I2C_ACK_Enable(pI2CHandle);

    //wait till ADDR becomes 1 (Slave Address is matched)
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_ADDR));

    //clear the ADDR flag, SCL will be pulled to LOW until ADDR is cleared
    I2C_Clear_ADDR_Flag(pI2CHandle);

    //send the data until length becomes zero
    while(pI2CHandle->tx_len > 0)
    {
        //wait till TXE is set
        while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_TXE));

        //load data in DR
        pI2CHandle->pI2Cx->DR = *ptxbuffer;
        ptxbuffer++;
        (pI2CHandle->tx_len)--;
    }

    //when length becomes 0, wait for TXE=1 and AF=1
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_TXE));
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_AF));

    //clear AF flag
    pI2CHandle->pI2Cx->SR1 &= ~(1<<I2C_SR1_AF);

    //After NACK, release bus by setting STOP
    if(repeat_start == I2C_REPEAT_START_DISABLE)
    {
        //Release SCL and SDA lines
        pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_STOP;
    }
}



void I2C_Slave_Receive(I2C_Handle* pI2CHandle, uint8_t* prxbuffer, uint8_t len, uint8_t repeat_start)
{
    pI2CHandle->prxbuffer = prxbuffer;
    pI2CHandle->rx_len = len;

    //Enable ACK before Reception after PE is set
    I2C_ACK_Enable(pI2CHandle);

    //clear STOP flag if set
    if(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_STOPF)
        I2C_STOP_Reset(pI2CHandle);

    //wait till ADDR becomes 1 (Slave Address is matched)
    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_ADDR));

    //Clear ADDR Flag
    I2C_Clear_ADDR_Flag(pI2CHandle);

    //Receive Data
    if(pI2CHandle->rx_len == 1)
    {
        //Disable ACK
        I2C_ACK_Disable(pI2CHandle);

        //Wait till RXNE becomes 1
        while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_RXNE));

        //Check for Stop Condition
        if(repeat_start == I2C_REPEAT_START_DISABLE)
        {
            //wait until STOP condition is detected
            while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_STOPF));
            //Release SCL and SDA lines
            pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_STOP;
        }

        //Read Data into buffer
        *prxbuffer = pI2CHandle->pI2Cx->DR;
    }
    else if(pI2CHandle->rx_len > 1)
    {
        while(pI2CHandle->rx_len > 0)
        {
            //Wait till RXNE becomes 1
            while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_RXNE));

            if(pI2CHandle->rx_len == 2)
            {
                //Check for Stop Condition
                if(repeat_start == I2C_REPEAT_START_DISABLE)
                {
                    //wait until STOP condition is detected
                    while(!(pI2CHandle->pI2Cx->SR1 & 1<<I2C_SR1_STOPF));
                    //Release SCL and SDA lines
                    pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_STOP;
                }
            }
            //Read Data into buffer
            *prxbuffer = pI2CHandle->pI2Cx->DR;

            prxbuffer++;
            (pI2CHandle->rx_len)--;
        }
    }

    //Re-enable ACK for future transactions
    I2C_ACK_Enable(pI2CHandle);
}








// Helper Functions

// ADDR Flag Clear
void I2C_Clear_ADDR_Flag(I2C_Handle* pI2CHandle)
{
    uint32_t temp = 0;

    //Device is Master and I2C is busy in Reception
    if((pI2CHandle->pI2Cx->SR2 & 1<<I2C_SR2_MSL) && pI2CHandle->rx_state == I2C_STATE_RX_BUSY)
    {
        if(pI2CHandle->rx_len == 1)
        {
            //Disable ACK
            pI2CHandle->pI2Cx->CR1 &= ~(1<<I2C_CR1_ACK);

            //Clear ADDR - Read SR1 & SR2
            temp = pI2CHandle->pI2Cx->SR1;
            temp = pI2CHandle->pI2Cx->SR2;
            (void)temp;
        }
    }
    else //Device mode is slave or I2C is not busy in Reception
    {
        //Clear ADDR - Read SR1 & SR2
        temp = pI2CHandle->pI2Cx->SR1;
        temp = pI2CHandle->pI2Cx->SR2;
        (void)temp;
    }
}

// ACK Enable
void I2C_ACK_Enable(I2C_Handle* pI2CHandle)
{
    pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_ACK;
}

// ACK Disable
void I2C_ACK_Disable(I2C_Handle* pI2CHandle)
{
    pI2CHandle->pI2Cx->CR1 &= ~(1<<I2C_CR1_ACK);
}

// STOP Set
void I2C_STOP_Set(I2C_Handle* pI2CHandle)
{
    pI2CHandle->pI2Cx->CR1 |= 1<<I2C_CR1_STOP;
}

// STOP Reset
void I2C_STOP_Reset(I2C_Handle* pI2CHandle)
{
    pI2CHandle->pI2Cx->CR1 &= ~(1<<I2C_CR1_STOP);
}

void I2C_Address_Phase_Read(I2C_Handle* pI2CHandle, uint8_t slave_addr)
{
    slave_addr = slave_addr<<1;    //first 7 bits are address
    slave_addr |= 1;               //last 8th bit is R/NW bit - 1 for read
    pI2CHandle->pI2Cx->DR = slave_addr;
}

void I2C_Address_Phase_Write(I2C_Handle* pI2CHandle, uint8_t slave_addr)
{
    slave_addr = slave_addr<<1;     //first 7 bits are address
    slave_addr &= ~(1);             //last 8th bit is R/NW bit - 0 for write
    pI2CHandle->pI2Cx->DR = slave_addr;
}