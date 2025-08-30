#include"stm32f103xx_gpio.h"
#include"stm32f103xx_afio.h"
#include"stm32f103xx_exti.h"
#include"stm32f103xx_nvic.h"
#include"stm32f103xx_utilities.h"

//GPIO_Pins GPIO_Used;

/*--------------------------------------------------------------------------------------------------------------------------*/
/*********************************************** GPIO API's Definitions Start ***********************************************/

/* to test later
void GPIO_Used_init()
{
    uint8_t i=0;
    while(i<16)
    {
        GPIO_Used.GPIOA_pins[i] = 0;
        GPIO_Used.GPIOB_pins[i] = 0;
        GPIO_Used.GPIOC_pins[i] = 0;
        GPIO_Used.GPIOD_pins[i] = 0;
        GPIO_Used.GPIOE_pins[i] = 0;
        GPIO_Used.GPIOF_pins[i] = 0;
        GPIO_Used.GPIOG_pins[i] = 0;
        i++;
    }
}

// GPIO Pins used Update
void GPIO_Used_Update(uint32_t GPIOx, uint8_t* pins, size_t size)
{
    if(GPIOx == GPIOA_BASE_ADDR)
    {
        while((size--)!=0)
        {
            GPIO_Used.GPIOA_pins[*pins++] = 1;
        }
    }
    else if(GPIOx == GPIOB_BASE_ADDR)
    {
        while((size--)!=0)
        {
            //GPIO_Used.GPIOB_pins[*pins++] = 1;
        }
    }
    else if(GPIOx == GPIOC_BASE_ADDR)
    {
        while((size--)!=0)
        {
            //GPIO_Used.GPIOC_pins[*pins++] = 1;
        }
    }
    else if(GPIOx == GPIOD_BASE_ADDR)
    {
        while((size--)!=0)
        {
            //GPIO_Used.GPIOD_pins[*pins++] = 1;
        }
    }
    else if(GPIOx == GPIOF_BASE_ADDR)
    {
        while((size--)!=0)
        {
            //GPIO_Used.GPIOE_pins[*pins++] = 1;
        }
    }
    else if(GPIOx == GPIOG_BASE_ADDR)
    {
        while((size--)!=0)
        {
            //GPIO_Used.GPIOF_pins[*pins++] = 1;
        }
    }
}
*/

// GPIO Peripheral Clock Setup
void GPIO_PClk_init(GPIO_RegDef* pGPIOx, uint8_t setup_mode)
{
    if(setup_mode == ENABLE)
    {
        if(pGPIOx == GPIOA)
            GPIOA_PCLK_EN();
        else if (pGPIOx == GPIOB)
            GPIOB_PCLK_EN();
        else if (pGPIOx == GPIOC)
            GPIOC_PCLK_EN();
        else if (pGPIOx == GPIOD)
            GPIOD_PCLK_EN();
        else if (pGPIOx == GPIOE)
            GPIOE_PCLK_EN();
        else if (pGPIOx == GPIOF)
            GPIOF_PCLK_EN();
        else if (pGPIOx == GPIOG)
            GPIOG_PCLK_EN();
    }
    else
    {
        //TODO
    }
}


//Set Default GPIO Configuration
void GPIO_Config(GPIO_Handle* pGPIOHandle, GPIO_RegDef* pGPIOx, uint8_t mode, uint8_t config_type, uint8_t pin_no, uint8_t op_speed)
{
    pGPIOHandle->pGPIOx = pGPIOx;
    pGPIOHandle->GPIOx_PinConfig.PinMode = mode;
    pGPIOHandle->GPIOx_PinConfig.PinConfigType = config_type;
    pGPIOHandle->GPIOx_PinConfig.PinNo = pin_no;
    pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed = op_speed;
}


// GPIO Initialisation & De-Initialisation
void GPIO_Init(GPIO_Handle* pGPIOHandle)
{
    //uint32_t temp=0; //temp register value

    //1. enable the peripheral clock for GPIO
    GPIO_PClk_init(pGPIOHandle->pGPIOx, ENABLE);
    
    uint8_t reg_no = pGPIOHandle->GPIOx_PinConfig.PinNo/8;              //get register number : 0->GPIOx_CRL, 1->GPIOx_CRH
    uint8_t section_no = pGPIOHandle->GPIOx_PinConfig.PinNo%8;          //get section position
    uint8_t bit_no = section_no*4;                                      //MODE bit position no
    uint32_t mode, config_type;

    //2. frame the mode and config type value of GPIO pin
    if(pGPIOHandle->GPIOx_PinConfig.PinMode <= GPIO_MODE_AF)
    {
        //Non-Interrupt Mode    
        //output/alternate function mode + speed is configured + GP/AF config type
        if(pGPIOHandle->GPIOx_PinConfig.PinMode != GPIO_MODE_IP
        && pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed != 0
        && pGPIOHandle->GPIOx_PinConfig.PinConfigType <= GPIO_CONFIG_AF_OP_OD
        && pGPIOHandle->GPIOx_PinConfig.PinConfigType >= GPIO_CONFIG_GP_OP_PP)
        {
            mode = pGPIOHandle->GPIOx_PinConfig.PinOutputSpeed << (bit_no);
            config_type = pGPIOHandle->GPIOx_PinConfig.PinConfigType << (bit_no+2);
        }//input mode + floating/pull up/pull down/analog config type
        else if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_IP && pGPIOHandle->GPIOx_PinConfig.PinConfigType <= GPIO_CONFIG_PD)
        {
            mode = ~(3 << (bit_no)); //input mode
            
            if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_ANALOG) //Analog
                config_type = ~(~pGPIOHandle->GPIOx_PinConfig.PinConfigType << (bit_no+2)); //CNF bit position = MODE bit position + 2
            else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_PU) //Pull Up
            {
                config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
                pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
                pGPIOHandle->pGPIOx->ODR |= 1 << (pGPIOHandle->GPIOx_PinConfig.PinNo);
            }
            else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_PD) //Pull Down
            {
                config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
                pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
            }
            else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_FLOAT) //No PUPD (ie) Floating
                config_type = 1 << (bit_no+2);     //CNF bit position = MODE bit position + 2
        }
        
        //3. configuring GPIO mode
        if(pGPIOHandle->GPIOx_PinConfig.PinMode == GPIO_MODE_IP)
        {
            pGPIOHandle->pGPIOx->CR[reg_no] &= mode;
        }
        else
        {
            //clear mode bits
            pGPIOHandle->pGPIOx->CR[reg_no] &= ~(3 << bit_no);
            pGPIOHandle->pGPIOx->CR[reg_no] |= mode;
        }
        
        //4. configure GPIO config type
        //clear config bits
        pGPIOHandle->pGPIOx->CR[reg_no] &= ~(3 << (bit_no+2));
        pGPIOHandle->pGPIOx->CR[reg_no] |= config_type;
    }
    else
    {
        //Interrupt mode
        //configure for input
        mode = ~(3 << (bit_no)); //input mode
            
        if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_ANALOG) //Analog
            config_type = ~(~pGPIOHandle->GPIOx_PinConfig.PinConfigType << (bit_no+2)); //CNF bit position = MODE bit position + 2
        else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_PU) //Pull Up
        {
            config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
            pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
            pGPIOHandle->pGPIOx->ODR |= 1 << (pGPIOHandle->GPIOx_PinConfig.PinNo);
        }
        else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_PD) //Pull Down
        {
            config_type = 2 << (bit_no+2);     //CNF bit position = MODE bit position + 2
            pGPIOHandle->pGPIOx->ODR &= ~(1 << (pGPIOHandle->GPIOx_PinConfig.PinNo));
        }
        else if(pGPIOHandle->GPIOx_PinConfig.PinConfigType == GPIO_CONFIG_FLOAT) //No PUPD (ie) Floating
            config_type = 1 << (bit_no+2);     //CNF bit position = MODE bit position + 2
        
        //configuring GPIO mode
        pGPIOHandle->pGPIOx->CR[reg_no] &= mode;
        
        //clear config bits
        pGPIOHandle->pGPIOx->CR[reg_no] &= ~(3 << (bit_no+2));
        //configuring GPIO config type
        pGPIOHandle->pGPIOx->CR[reg_no] |= config_type;
            
        GPIO_IRQ_Config(pGPIOHandle, ENABLE);
    }
}


// GPIo De-Initialization
void GPIO_DeInit(GPIO_RegDef* pGPIOx)
{
    if(pGPIOx == GPIOA)
        GPIOA_REG_RESET();
    else if (pGPIOx == GPIOB)
        GPIOB_REG_RESET();
    else if (pGPIOx == GPIOC)
        GPIOC_REG_RESET();
    else if (pGPIOx == GPIOD)
        GPIOD_REG_RESET();
    else if (pGPIOx == GPIOE)
        GPIOE_REG_RESET();
    else if (pGPIOx == GPIOF)
        GPIOF_REG_RESET();
    else if (pGPIOx == GPIOG)
        GPIOG_REG_RESET();
}



// GPIO Toggle Output
void GPIO_OpToggle(GPIO_RegDef* pGPIOx, uint8_t pin_no)
{
    pGPIOx->ODR ^= (1<<pin_no); // toggle the bit value for the specified GPIO pin
}



// GPIO Read & Write
// GPIO Read from Pin
uint8_t GPIO_ReadIpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no)
{
   uint8_t value;
   value = (uint8_t)((pGPIOx->IDR >> pin_no) & 0x00000001); // the IDR data can be accessed in word mode only
   return value;
}


// GPIO Write to Pin
void GPIO_WriteOpPin(GPIO_RegDef* pGPIOx, uint8_t pin_no, uint8_t value)
{
    if(value == GPIO_PIN_SET)
        pGPIOx->ODR |= (1<<pin_no); // setting output bit to 1
    else
        pGPIOx->ODR &= ~(1<<pin_no); // clearing output bit to 0
}


//GPIO Read from Port
uint16_t GPIO_ReadIpPort(GPIO_RegDef* pGPIOx)
{
   uint16_t value;
   value = (uint16_t)(pGPIOx->IDR & 0xFFFF);
   return value;
}


//GPIO Read from Port
uint16_t GPIO_ReadOpPort(GPIO_RegDef* pGPIOx)
{
    return (pGPIOx->ODR && 0xFFFF);
}


//GPIO Write to Port
void GPIO_WriteOpPort(GPIO_RegDef* pGPIOx, uint16_t value)
{
    pGPIOx->ODR = value;
}


//GPIO IRQ Config
void GPIO_IRQ_Config(GPIO_Handle* pGPIOHandle, uint8_t mode)
{
    //configure edge triggering of interrupts in EXTI (peripheral side)
    EXTI_Intrpt_Config(pGPIOHandle->GPIOx_PinConfig.PinNo, pGPIOHandle->GPIOx_PinConfig.PinMode, mode);
    
    //configure exticr for port selection in AFIO (peripheral side)
    AFIO_EXTI_Config(pGPIOHandle->pGPIOx, pGPIOHandle->GPIOx_PinConfig.PinNo, mode);
    
    //enable exti interrupts (peripheral side)
    EXTI_Intrpt_Mask(pGPIOHandle->GPIOx_PinConfig.PinNo, mode);
    
    //configure enable and mask bits in NVIC side
    uint8_t irq_no = nvic_irq_gpio_port_map(pGPIOHandle);

    if(mode == ENABLE)
        nvic_intrpt_enable(irq_no);
    else if(mode == DISABLE)
        nvic_intrpt_disable(irq_no);
}


// GPIO Interrupt Handling
void GPIO_IRQHandling(uint8_t pin_no)
{
    if(EXTI_Pend_Check(pin_no)) // If the PR register is set for the Pin Number,
    {
        //clear the EXTI PR register for the specified Pin Number
        EXTI_Pend_Clear(pin_no);
    }
}


void GPIO_Bit_Set(GPIO_Handle* pGPIOHandle, uint8_t pin_no)
{
    uint8_t offset = pin_no;
    pGPIOHandle->pGPIOx->BSRR |= 1<<offset;
}


void GPIO_Bit_Reset(GPIO_Handle* pGPIOHandle, uint8_t pin_no)
{
    uint8_t offset = pin_no + 16;
    pGPIOHandle->pGPIOx->BSRR |= 1 << offset;
}

/*********************************************** GPIO API's Definitions Start ***********************************************/
/*--------------------------------------------------------------------------------------------------------------------------*/