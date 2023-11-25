//==============================================================================
#include "DRV_IO.h"

IO_Pinout pins;

//------------------------------------------------------------------------------
// return the port index (0x000000PP)
uint32_t GetPortIndex(GPIO_TypeDef* p){
    return((uint32_t)p - (uint32_t)GPIOA_BASE) / (uint32_t)GPIO_SIZE;
}

//------------------------------------------------------------------------------
// return the port index (0x0000PP00), as used in "IO_PortClockOn"
uint16_t IO_PortIndex(GPIO_TypeDef* p){
    uint16_t result = ((uint32_t)p - (uint32_t)GPIOA_BASE) / (uint32_t)GPIO_SIZE;
    return(result<<8);
}

//------------------------------------------------------------------------------
// return the port base-address based on its index (0x0000PP00)
GPIO_TypeDef* IO_GetPort(uint32_t p){
    p = (p & __MASK_PIN) >> 8;
    uint32_t result = ((uint32_t)p*(uint32_t)GPIO_SIZE + (uint32_t)GPIOA_BASE);
    return((GPIO_TypeDef*)result);
}

//------------------------------------------------------------------------------
void IO_PortClockEnable(GPIO_TypeDef* p){
    uint32_t result = ((uint32_t)p - (uint32_t)GPIOA_BASE) / (uint32_t)GPIO_SIZE;
    IO_PortClockOn(result<<8);
}

//------------------------------------------------------------------------------
// starts the clock for port, given its "port index" (0x0000PP00), returns the port handler
GPIO_TypeDef* IO_PortClockOn(uint32_t portindex){
    portindex = (portindex & __MASK_PIN)>>8L;
    switch(portindex){
        case 0: RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; return(GPIOA);
        case 1: RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; return(GPIOB);
        case 2: RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; return(GPIOC);
        #ifdef RCC_APB2ENR_IOPDEN
            case 3: RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; return(GPIOD);
        #endif
        #ifdef RCC_APB2ENR_IOPEEN
            case 4: RCC->APB2ENR |= RCC_APB2ENR_IOPEEN; return(GPIOE);
        #endif
		#ifdef RCC_APB2ENR_IOPFEN
			case 5: RCC->APB2ENR |= RCC_APB2ENR_IOPFEN; return(GPIOF);
		#endif

		#ifdef RCC_APB2ENR_IOPGEN
			case 6: RCC->APB2ENR |= RCC_APB2ENR_IOPGEN; return(GPIOG);
		#endif

        #ifdef RCC_APB2ENR_IOPFEN
            case 7: RCC->APB2ENR |= RCC_APB2ENR_IOPFEN; return(GPIOH);
        #endif
        default: return((GPIO_TypeDef*) 0);
    }
}

//------------------------------------------------------------------------------
// setup and initializes a single I/O port pin
void IO_PinInit(GPIO_TypeDef* Port, IO_Config* PinConfig){
    int32_t offset = 0;
    uint32_t pin = PinConfig->Pin;
    
    // enable clock
	IO_PortClockEnable(Port);

    uint32_t* MODER = (uint32_t*)&Port->CRL;
    
    if(pin>15){ return;}
    else if(pin>7){ MODER = (uint32_t*)&Port->CRH; pin -= 8;}

    offset = (pin * IO_FIELD_MODE);
    
    *MODER &= ~(IO_MASK_MODE << offset);
    
    uint32_t mode = PinConfig->Mode;

    Port->ODR &= ~((uint32_t)1 << PinConfig->Pin);

    if(PinConfig->Mode == io_In_PullUp){
    	mode = io_In_PullDown;
    	Port->ODR |= ((uint32_t)1 << PinConfig->Pin);
    }

    *MODER |= (mode << offset);


    if(((bool)PinConfig->Int)&&
        ((PinConfig->Mode==io_In_Floating)||
            (PinConfig->Mode==io_In_PullDown)||
                (PinConfig->Mode==io_In_PullUp))){
                    IO_SetExtendedIT(Port, PinConfig);
    }
}

//------------------------------------------------------------------------------
// return port and pin which fired an "External Interrupt"
uint32_t IO_GetExtendedIT(uint32_t pin) {
    uint32_t EXTI_Index = pin/4;              // 0.. 3
    uint32_t EXTI_Offset = pin%4;             // 0.. 3
    
    uint32_t result = (AFIO->EXTICR[EXTI_Index]>>(EXTI_Offset*4)) & IO_MASK_EXTI;
    result = (result<<8)|(pin & IO_MASK_EXTI);
    return(result);
}

//------------------------------------------------------------------------------
// setup input pin as "external interrupt"
void IO_SetExtendedIT(GPIO_TypeDef* Porta, IO_Config* Pino) {

    uint32_t port_index = GetPortIndex(Porta);              // 0.. 7
    uint32_t EXTI_Index = Pino->Pin/4;                      // 0.. 3
    uint32_t EXTI_Offset = Pino->Pin%4;                     // 0.. 3
    
    uint32_t mask = ~(IO_MASK_EXTI<<(EXTI_Offset*4));
    uint32_t stamp = (port_index<<(EXTI_Offset*4));
    
    // turn-on the AFIO clock
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                                     // (1) turn-on mux clock
    
    // set the IO port index
    AFIO->EXTICR[EXTI_Index] &= mask;                                       // (1) 
    AFIO->EXTICR[EXTI_Index] |= stamp;                                      // (1) line selection in mux
    
    // set the interrupt mask
    EXTI->IMR |= (uint16_t)(1 << Pino->Pin);                                // (2)
    
    // enable rising-edge trigger as needed
    if(Pino->Rise){ EXTI->RTSR |= (uint16_t)(1 << Pino->Pin);}              // (3)
    else { EXTI->RTSR &= ~(uint16_t)(1 << Pino->Pin);}                      // (3)
    
    // enable falling-edge trigger as needed
    if(Pino->Fall){ EXTI->FTSR |= (uint16_t)(1 << Pino->Pin);}              // (4)
    else { EXTI->FTSR &= ~(uint16_t)(1 << Pino->Pin);}                      // (4)

    // configure NVIC for Extended Interrupt
    IRQn_Type EXTINT_IRQn = IO_GetIrqNumber(Pino->Pin);
    uint32_t p = NVIC_EncodePriority(NVIC_PriorityGroup_4, SYS_PRIORITY_NORMAL, 0);
    NVIC_SetPriority(EXTINT_IRQn, p);                                       // (5)
    NVIC_EnableIRQ(EXTINT_IRQn);                                            // (6)
}

//------------------------------------------------------------------------------
void IO_ResetExtendedIT(GPIO_TypeDef* Porta, IO_Config* Pino) {

    IRQn_Type EXTINT_IRQn = IO_GetIrqNumber(Pino->Pin);
    NVIC_DisableIRQ(EXTINT_IRQn);

    // Clear EXTI line configuration
    EXTI->IMR  &= ~(uint16_t)(1 << Pino->Pin);
    EXTI->EMR  &= ~(uint16_t)(1 << Pino->Pin);
    
    // Clear Rising Falling edge configuration
    EXTI->RTSR &= ~(uint16_t)(1 << Pino->Pin);
    EXTI->FTSR &= ~(uint16_t)(1 << Pino->Pin);
}

//------------------------------------------------------------------------------
void IO_MaskExtendedIT(IO_Config* Pino) {
    EXTI->IMR &= ~(uint16_t)(1 << Pino->Pin);
}

//------------------------------------------------------------------------------
void IO_UnmaskExtendedIT(IO_Config* Pino) {
    EXTI->IMR |= (uint16_t)(1 << Pino->Pin);
}

//------------------------------------------------------------------------------
void IO_ClearPendingExtendedIT(IO_Config* Pino) {
    EXTI->PR |= (uint16_t)(1 << Pino->Pin);
}

//------------------------------------------------------------------------------
IRQn_Type IO_GetIrqNumber(uint32_t pin){
    IRQn_Type result = EXTI0_IRQn;
    switch(pin){
        case 0: result = EXTI0_IRQn; break;
        case 1: result = EXTI1_IRQn; break;
        case 2: result = EXTI2_IRQn; break;
        case 3: result = EXTI3_IRQn; break;
        case 4: result = EXTI4_IRQn; break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:  result = EXTI9_5_IRQn; break;  
        default: result = EXTI15_10_IRQn; break;
    }
    return(result);
}

//==============================================================================
