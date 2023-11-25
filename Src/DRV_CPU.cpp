//==============================================================================
#include "DRV_CPU.h"
#include <math.h>
#include "Priorities.h"

//------------------------------------------------------------------------------
void CPU_StartHSE(void){

    NVIC_EnableIRQ(RCC_IRQn);
    NVIC_SetPriority(RCC_IRQn, 0);

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    
    #ifdef STM32F10X_CL
        FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;    // 1 wait state
    #else
        if (HSE_Value <= 24000000){
          FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;  // 1 wait state
        } else {
          FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_1;  // 2 wait states
        }
    #endif
    

    RCC->CR |= (RCC_CR_CSSON | RCC_CR_HSEON);

    while(!(RCC->CR & RCC_CR_HSERDY));

    RCC->CFGR = ((RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_HSE);

    while((RCC->CFGR & RCC_CFGR_SWS)!=RCC_CFGR_SWS_HSE);

    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    
    SystemCoreClockUpdate();
}

//------------------------------------------------------------------------------
void CPU_StartHSI(){

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    RCC->CR |= RCC_CR_HSION;
    while((RCC->CR & RCC_CR_HSIRDY) == 0);
    
    if((RCC->CFGR & RCC_CFGR_SW) != RCC_CFGR_SW_HSI){
        RCC->CFGR &= ~RCC_CFGR_SW;
    }

    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    
    SystemCoreClockUpdate();
}

//------------------------------------------------------------------------------
void CPU_StartPLL(PllSources PllSource, PllFrequencies CpuSpeed){
    uint32_t PllDiv = PllFactors[CpuSpeed][0];
    uint32_t PllMul = PllFactors[CpuSpeed][1];

    CPU_StartHSI();
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
    
    if(PllSource == Pll_Hse){
    	CPU_StartHSE();
    	RCC->CFGR |= RCC_CFGR_PLLSRC;
    }

    RCC->CR &= (uint32_t)(~RCC_CR_PLLON);
    while((RCC->CR & RCC_CR_PLLRDY) != 0){}

    RCC->CFGR = ((RCC->CFGR & (~RCC_CFGR_PLLMULL)) | PllMul);

    
    #ifdef STM32F10X_CL
        RCC->CFGR2 = ((RCC->CFGR2 & (~RCC_CFGR2_PREDIV1)) | PllDiv);
    #else
        RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;
        if(PllDiv == RCC_PREDIV1_DIV2){ RCC->CFGR |= RCC_CFGR_PLLXTPRE;}
    #endif
    
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;

    RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk;
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    RCC->CFGR |= RCC_CFGR_ADCPRE_Msk;

    RCC->CFGR &= ~RCC_CFGR_PPRE1_Msk;

    if((CpuSpeed == Pll72MHz)||(CpuSpeed == Pll64MHz)){
		RCC->CFGR |= (HCLK_DIV2 << RCC_CFGR_PPRE1_Pos);
    }

    FLASH->ACR &= ~(FLASH_ACR_PRFTBE | FLASH_ACR_PRFTBE);

    if((CpuSpeed == Pll72MHz)||(CpuSpeed == Pll64MHz)){
        FLASH->ACR = ((FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_2);	// 2 wait state
	} else if((CpuSpeed == Pll32MHz)||(CpuSpeed == Pll36MHz)){
		FLASH->ACR = ((FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_1);	// 1 wait state
	}

    RCC->CR |= RCC_CR_PLLON;
    
    while ((RCC->CR & RCC_CR_PLLRDY) == 0){}
    
    RCC->CFGR = (RCC->CFGR & (uint32_t) (~RCC_CFGR_SW)) | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){}

    SystemCoreClockUpdate();
}


//------------------------------------------------------------------------------
uint32_t CPU_GetFrequency(){
    
    SystemCoreClockUpdate();
    return(SystemCoreClock);
}

//------------------------------------------------------------------------------
void CPU_SetFrequencyAPB1(BusFrequencies BusSpeed){
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR |= (uint32_t)BusSpeed << 8;
}

//------------------------------------------------------------------------------
uint32_t CPU_GetFrequencyAPB1(){
    uint32_t result = 1;
    uint32_t Div = (RCC->CFGR & RCC_CFGR_PPRE1)>>8;
    if(Div < 4){ Div = 1;}
    else { Div = 2 + (Div - 4);}
    result = SystemCoreClock / Div;
    return(result);
}

//------------------------------------------------------------------------------
uint32_t CPU_GetTimerFrequencyAPB1(){
    uint32_t result = 1;
    uint32_t Mul = 1;
    uint32_t Div = (RCC->CFGR & RCC_CFGR_PPRE1)>>8;
    if(Div < 4){ Div = 1;}
    else { Div = (2 << (Div - 4)); Mul = 2;}
    result = (SystemCoreClock / Div) * Mul;
    return(result);
}

//------------------------------------------------------------------------------
void CPU_SetFrequencyAPB2(BusFrequencies BusSpeed){
    RCC->CFGR &= ~RCC_CFGR_PPRE2;
    RCC->CFGR |= (uint32_t)BusSpeed << 11;
}

//------------------------------------------------------------------------------
uint32_t CPU_GetFrequencyAPB2(){
    uint32_t result = 1;
    uint32_t Mul = 1;
    uint32_t Div = (RCC->CFGR & RCC_CFGR_PPRE2)>>11;
    if(Div < 4){ Div = 1;}
    else { Div = (2 << (Div - 4)); Mul = 2;}
    result = (SystemCoreClock / Div) * Mul;
    return(result);
}

//------------------------------------------------------------------------------
uint32_t CPU_GetTimerFrequencyAPB2(){
    uint32_t result = 1;
    uint32_t Mul = 1;
    uint32_t Div = (RCC->CFGR & RCC_CFGR_PPRE2)>>11;
    if(Div < 4){ Div = 1;}
    else { Div = 2 + (Div - 4); Mul = 2;}
    result = (SystemCoreClock / Div) * Mul;
    return(result);
}

//------------------------------------------------------------------------------
bool CPU_PeripheralClockEnable(void* P){
    bool result = false;

	//----------------------------------------------------------------------
	if((void*)P == TIM2){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_TIM2EN);}
	else if((void*)P == TIM3){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_TIM3EN);}

	#ifdef RCC_APB1ENR_TIM4EN
	else if((void*)P == TIM4){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_TIM4EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM5EN
	else if((void*)P == TIM5){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_TIM5EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM6EN
		else if((void*)P == TIM6){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_TIM6EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM7EN
		else if((void*)P == TIM7){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_TIM7EN);}
	#endif

	else if((void*)P == WWDG){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_WWDGEN);}

	#ifdef RCC_APB1ENR_SPI2EN
		else if((void*)P == SPI2){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_SPI2EN);}
	#endif

	#ifdef RCC_APB1ENR_SPI3EN
		else if((void*)P == SPI3){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_SPI3EN);}
	#endif

	else if((void*)P == USART2){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_USART2EN);}
	else if((void*)P == USART3){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_USART3EN);}

	#ifdef RCC_APB1ENR_UART4EN
		else if((void*)P == UART4){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_UART4EN);}
	#endif

	#ifdef RCC_APB1ENR_UART5EN
		else if((void*)P == UART5){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_UART5EN);}
	#endif

	else if((void*)P == I2C1){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_I2C1EN);}

	#ifdef RCC_APB1ENR_I2C2EN
		else if((void*)P == I2C2){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_I2C2EN);}
	#endif

	#ifdef RCC_APB1ENR_CAN1EN
		else if((void*)P == CAN1){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_CAN1EN);}
	#endif

	#ifdef RCC_APB1ENR_CAN2EN
		else if((void*)P == CAN2){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_CAN2EN);}
	#endif

	#ifdef RCC_APB1ENR_BKPEN
		else if((void*)P == BKP){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_BKPEN);}
	#endif

	else if((void*)P == PWR){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_PWREN);}

	#ifdef RCC_APB1ENR_DACEN
		else if((void*)P == DAC){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_DACEN);}
	#endif

	#ifdef RCC_APB1ENR_USBEN
		else if((void*)P == USB){ result = (bool)(RCC->APB1ENR |= RCC_APB1ENR_USBEN);}
	#endif

	//----------------------------------------------------------------------
	else if((void*)P == AFIO){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_AFIOEN);}

	else if((void*)P == GPIOA){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_IOPAEN);}
	else if((void*)P == GPIOB){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_IOPBEN);}
	else if((void*)P == GPIOC){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_IOPCEN);}

	#ifdef RCC_APB2ENR_IOPDEN
		else if((void*)P == GPIOD){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_IOPDEN);}
	#endif
	#ifdef RCC_APB2ENR_IOPEEN
		else if((void*)P == GPIOE){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_IOPEEN);}
	#endif

	else if((void*)P == ADC1){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_ADC1EN);}

	#ifdef RCC_APB2ENR_ADC2EN
		else if((void*)P == ADC2){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_ADC2EN);}
	#endif

	else if((void*)P == TIM1){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_TIM1EN);}

	else if((void*)P == SPI1){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_SPI1EN);}

	else if((void*)P == USART1){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_USART1EN);}

	#ifdef RCC_APB2ENR_TIM8EN
		else if((void*)P == TIM8){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_TIM8EN);}
	#endif

	#ifdef RCC_APB2ENR_ADC3EN
		else if((void*)P == ADC3){ result = (bool)(RCC->APB2ENR |= RCC_APB2ENR_ADC3EN);}
	#endif

	//----------------------------------------------------------------------
	else if((void*)P == DMA1){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_DMA1EN);}

	#ifdef RCC_AHBENR_DMA2EN
		else if((void*)P == DMA2){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_DMA2EN);}
	#endif

	#ifdef SRAM
		else if((void*)P == SRAM){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_SRAMEN);}
	#endif

	#ifdef FLITFEN
		else if((void*)P == FLITF){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_FLITFEN);}
	#endif

	#ifdef RCC_AHBENR_CRCEN
		else if((void*)P == CRC){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_CRCEN);}
	#endif

	#ifdef OTG_FS
		else if((void*)P == OTG_FS){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_OTGFSEN);}
	#endif

	#ifdef ETH
		else if((void*)P == ETH){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_ETHMACEN);}
	#endif

	#ifdef ETH_TX
		else if((void*)P == ETH_TX){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_ETHMACTXEN);}
	#endif

	#ifdef ETH_RX
		else if((void*)P == ETH_RX){ result = (bool)(RCC->AHBENR |= RCC_AHBENR_ETHMACRXEN);}
	#endif

	return(result);
}

//------------------------------------------------------------------------------
bool CPU_PeripheralClockDisable(void* P){
    bool result = false;

	//----------------------------------------------------------------------
	if((void*)P == TIM2){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN);}
	else if((void*)P == TIM3){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN);}

	#ifdef RCC_APB1ENR_TIM4EN
	else if((void*)P == TIM4){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM5EN
	else if((void*)P == TIM5){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM6EN
		else if((void*)P == TIM6){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM7EN
		else if((void*)P == TIM7){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_TIM7EN);}
	#endif

	else if((void*)P == WWDG){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_WWDGEN);}

	#ifdef RCC_APB1ENR_SPI2EN
		else if((void*)P == SPI2){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN);}
	#endif

	#ifdef RCC_APB1ENR_SPI3EN
		else if((void*)P == SPI3){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN);}
	#endif

	else if((void*)P == USART2){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN);}
	else if((void*)P == USART3){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_USART3EN);}

	#ifdef RCC_APB1ENR_UART4EN
		else if((void*)P == UART4){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN);}
	#endif

	#ifdef RCC_APB1ENR_UART5EN
		else if((void*)P == UART5){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN);}
	#endif

	else if((void*)P == I2C1){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN);}

	#ifdef RCC_APB1ENR_I2C2EN
		else if((void*)P == I2C2){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN);}
	#endif

	#ifdef RCC_APB1ENR_CAN1EN
		else if((void*)P == CAN1){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_CAN1EN);}
	#endif

	#ifdef RCC_APB1ENR_CAN2EN
		else if((void*)P == CAN2){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_CAN2EN);}
	#endif

	#ifdef RCC_APB1ENR_BKPEN
		else if((void*)P == BKP){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_BKPEN);}
	#endif

	else if((void*)P == PWR){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_PWREN);}

	#ifdef RCC_APB1ENR_DACEN
		else if((void*)P == DAC){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_DACEN);}
	#endif

	#ifdef RCC_APB1ENR_USBEN
		else if((void*)P == USB){ result = (bool)(RCC->APB1ENR &= ~RCC_APB1ENR_USBEN);}
	#endif

	//----------------------------------------------------------------------
	else if((void*)P == AFIO){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN);}

	else if((void*)P == GPIOA){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_IOPAEN);}
	else if((void*)P == GPIOB){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_IOPBEN);}
	else if((void*)P == GPIOC){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_IOPCEN);}

	#ifdef RCC_APB2ENR_IOPDEN
		else if((void*)P == GPIOD){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_IOPDEN);}
	#endif
	#ifdef RCC_APB2ENR_IOPEEN
		else if((void*)P == GPIOE){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_IOPEEN);}
	#endif

	else if((void*)P == ADC1){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN);}

	#ifdef RCC_APB2ENR_ADC2EN
		else if((void*)P == ADC2){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_ADC2EN);}
	#endif

	else if((void*)P == TIM1){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN);}

	else if((void*)P == SPI1){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN);}

	else if((void*)P == USART1){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN);}

	#ifdef RCC_APB2ENR_TIM8EN
		else if((void*)P == TIM8){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_TIM8EN);}
	#endif

	#ifdef RCC_APB2ENR_ADC3EN
		else if((void*)P == ADC3){ result = (bool)(RCC->APB2ENR &= ~RCC_APB2ENR_ADC3EN);}
	#endif

	//----------------------------------------------------------------------
	else if((void*)P == DMA1){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_DMA1EN);}

	#ifdef RCC_AHBENR_DMA2EN
		else if((void*)P == DMA2){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_DMA2EN);}
	#endif

	#ifdef SRAM
		else if((void*)P == SRAM){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_SRAMEN);}
	#endif

	#ifdef FLITFEN
		else if((void*)P == FLITF){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_FLITFEN);}
	#endif

	#ifdef RCC_AHBENR_CRCEN
		else if((void*)P == CRC){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_CRCEN);}
	#endif

	#ifdef OTG_FS
		else if((void*)P == OTG_FS){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_OTGFSEN);}
	#endif

	#ifdef ETH
		else if((void*)P == ETH){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_ETHMACEN);}
	#endif

	#ifdef ETH_TX
		else if((void*)P == ETH_TX){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_ETHMACTXEN);}
	#endif

	#ifdef ETH_RX
		else if((void*)P == ETH_RX){ result = (bool)(RCC->AHBENR &= ~RCC_AHBENR_ETHMACRXEN);}
	#endif

	return(result);
}

//------------------------------------------------------------------------------
bool CPU_PeripheralClockStatus(void* P){
    bool result = false;

	//----------------------------------------------------------------------
	if((void*)P == TIM2){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_TIM2EN);}
	else if((void*)P == TIM3){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_TIM3EN);}

	#ifdef RCC_APB1ENR_TIM4EN
	else if((void*)P == TIM4){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_TIM4EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM5EN
	else if((void*)P == TIM5){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_TIM5EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM6EN
		else if((void*)P == TIM6){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_TIM6EN);}
	#endif

	#ifdef RCC_APB1ENR_TIM7EN
		else if((void*)P == TIM7){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_TIM7EN);}
	#endif

	else if((void*)P == WWDG){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_WWDGEN);}

	#ifdef RCC_APB1ENR_SPI2EN
		else if((void*)P == SPI2){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_SPI2EN);}
	#endif

	#ifdef RCC_APB1ENR_SPI3EN
		else if((void*)P == SPI3){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_SPI3EN);}
	#endif

	else if((void*)P == USART2){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_USART2EN);}
	else if((void*)P == USART3){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_USART3EN);}

	#ifdef RCC_APB1ENR_UART4EN
		else if((void*)P == UART4){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_UART4EN);}
	#endif

	#ifdef RCC_APB1ENR_UART5EN
		else if((void*)P == UART5){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_UART5EN);}
	#endif

	else if((void*)P == I2C1){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_I2C1EN);}

	#ifdef RCC_APB1ENR_I2C2EN
		else if((void*)P == I2C2){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_I2C2EN);}
	#endif

	#ifdef RCC_APB1ENR_CAN1EN
		else if((void*)P == CAN1){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_CAN1EN);}
	#endif

	#ifdef RCC_APB1ENR_CAN2EN
		else if((void*)P == CAN2){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_CAN2EN);}
	#endif

	#ifdef RCC_APB1ENR_BKPEN
		else if((void*)P == BKP){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_BKPEN);}
	#endif

	else if((void*)P == PWR){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_PWREN);}

	#ifdef RCC_APB1ENR_DACEN
		else if((void*)P == DAC){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_DACEN);}
	#endif

	#ifdef RCC_APB1ENR_USBEN
		else if((void*)P == USB){ result = (bool)(RCC->APB1ENR & RCC_APB1ENR_USBEN);}
	#endif

	//----------------------------------------------------------------------
	else if((void*)P == AFIO){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_AFIOEN);}

	else if((void*)P == GPIOA){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_IOPAEN);}
	else if((void*)P == GPIOB){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_IOPBEN);}
	else if((void*)P == GPIOC){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_IOPCEN);}

	#ifdef RCC_APB2ENR_IOPDEN
		else if((void*)P == GPIOD){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_IOPDEN);}
	#endif
	#ifdef RCC_APB2ENR_IOPEEN
		else if((void*)P == GPIOE){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_IOPEEN);}
	#endif

	else if((void*)P == ADC1){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_ADC1EN);}

	#ifdef RCC_APB2ENR_ADC2EN
		else if((void*)P == ADC2){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_ADC2EN);}
	#endif

	else if((void*)P == TIM1){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_TIM1EN);}

	else if((void*)P == SPI1){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_SPI1EN);}

	else if((void*)P == USART1){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_USART1EN);}

	#ifdef RCC_APB2ENR_TIM8EN
		else if((void*)P == TIM8){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_TIM8EN);}
	#endif

	#ifdef RCC_APB2ENR_ADC3EN
		else if((void*)P == ADC3){ result = (bool)(RCC->APB2ENR & RCC_APB2ENR_ADC3EN);}
	#endif

	//----------------------------------------------------------------------
	else if((void*)P == DMA1){ result = (bool)(RCC->AHBENR & RCC_AHBENR_DMA1EN);}

	#ifdef RCC_AHBENR_DMA2EN
		else if((void*)P == DMA2){ result = (bool)(RCC->AHBENR & RCC_AHBENR_DMA2EN);}
	#endif

	#ifdef SRAM
		else if((void*)P == SRAM){ result = (bool)(RCC->AHBENR & RCC_AHBENR_SRAMEN);}
	#endif

	#ifdef FLITFEN
		else if((void*)P == FLITF){ result = (bool)(RCC->AHBENR & RCC_AHBENR_FLITFEN);}
	#endif

	#ifdef RCC_AHBENR_CRCEN
		else if((void*)P == CRC){ result = (bool)(RCC->AHBENR & RCC_AHBENR_CRCEN);}
	#endif

	#ifdef OTG_FS
		else if((void*)P == OTG_FS){ result = (bool)(RCC->AHBENR & RCC_AHBENR_OTGFSEN);}
	#endif

	#ifdef ETH
		else if((void*)P == ETH){ result = (bool)(RCC->AHBENR & RCC_AHBENR_ETHMACEN);}
	#endif

	#ifdef ETH_TX
		else if((void*)P == ETH_TX){ result = (bool)(RCC->AHBENR & RCC_AHBENR_ETHMACTXEN);}
	#endif

	#ifdef ETH_RX
		else if((void*)P == ETH_RX){ result = (bool)(RCC->AHBENR & RCC_AHBENR_ETHMACRXEN);}
	#endif

	return(result);
}

//------------------------------------------------------------------------------
void CPU_InitializeWatchdog(uint16_t wdt){
  
    IWDG->KR = IWDG_ENABLE;
    IWDG->KR = IWDG_WRITE_ACCESS;
    IWDG->PR = 3;
    IWDG->RLR = wdt;
    while(IWDG->SR > 0);
    IWDG->KR = IWDG_REFRESH;
    return;
}

//------------------------------------------------------------------------------
bool CPU_CheckWatchdog(){
    bool result = false;
    if(RCC->CSR & __WATCHDOG_FLAG_SET){
        result = true;
        RCC->CSR |= __WATCHDOG_FLAG_REMOVER;
    }
    return(result);
}

//------------------------------------------------------------------------------
void CPU_KickWatchdog(){
  IWDG->KR = IWDG_REFRESH;
}

//------------------------------------------------------------------------------
void CPU_SetPriorityIRQn(IRQn_Type IRQ, uint32_t priority){
    NVIC_SetPriority(IRQ, NVIC_EncodePriority (NVIC_PriorityGroup_4, priority, 0));
}

//------------------------------------------------------------------------------
void CPU_Reset(){
	__DSB();
	__disable_irq();
	SCB->AIRCR = 0x05FA0004;
	while(1);
}

//------------------------------------------------------------------------------
uint32_t CPU_Crc32(uint8_t* pt, uint16_t n){
	CPU_PeripheralClockEnable(CRC);
	CRC->CR = 0x00000000;
	CRC->CR |= 0x00000001;
	for(uint16_t c=0; c<n; c++){ CRC->DR = (uint32_t)pt[c];}
	uint32_t result = CRC->DR;
	CPU_PeripheralClockDisable(CRC);
	return(result);
}

//==============================================================================
