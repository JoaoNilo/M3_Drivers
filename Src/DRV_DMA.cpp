//==============================================================================
#include "DRV_DMA.h"

//------------------------------------------------------------------------------
// get the DMA "Channel" number by its Handler
uint32_t DMA_GetChannelIndex(DMA_Channel_TypeDef* Channel){
	uint32_t result = 0xFFFFFFFF;
	switch((uint32_t)Channel){
		case DMA1_Channel1_BASE: result = 0; break;
		case DMA1_Channel2_BASE: result = 1; break;
		case DMA1_Channel3_BASE: result = 2; break;
		case DMA1_Channel4_BASE: result = 3; break;
		case DMA1_Channel5_BASE: result = 4; break;
		case DMA1_Channel6_BASE: result = 5; break;
		case DMA1_Channel7_BASE: result = 6; break;
        
		#ifdef DMA2
		case DMA2_Channel1_BASE: result = 7; break;
		case DMA2_Channel2_BASE: result = 8; break;
		case DMA2_Channel3_BASE: result = 9; break;
		case DMA2_Channel4_BASE: result = 10; break;
		case DMA2_Channel5_BASE: result = 11; break;
		#endif
		default: break;
	}
    return(result);
}

//------------------------------------------------------------------------------
// get the IRQn of the DMA channel by its Handler
IRQn_Type DMA_GetChannelIRQn(DMA_Channel_TypeDef* Channel){
    uint32_t result = -1; 
    uint32_t index = DMA_GetChannelIndex(Channel);
    if(index < 7){ 
        result = index + (uint32_t)DMA1_Channel1_IRQn;
    }
	#ifdef DMA2
		else if(index < 11){
			result = (index - 7) + (uint32_t)DMA2_Channel1_IRQn;
		} else if(index == 11){
			#ifdef STM32F10X_HD
				result = DMA2_Channel4_5_IRQn;
			#endif
			#ifdef STM32F10X_CL
				result = DMA2_Channel5_IRQn;
			#endif
		}
	#endif
    return((IRQn_Type)result);
}

//------------------------------------------------------------------------------
// set or reset "interrupt enable" flags in the CCR of the DMA Channel
void DMA_SetInterruptFlags(DMA_Channel_TypeDef* CH, uint32_t flags, bool status){

    flags &= 0x0E;
    if(status){ CH->CCR |= flags;}
    else { CH->CCR &=  ~flags;}
}

//------------------------------------------------------------------------------
void DMA_EnableInterrupts(DMA_Channel_TypeDef* CH, uint32_t flags){
    DMA_SetInterruptFlags(CH, flags, true);
}

//------------------------------------------------------------------------------
void DMA_DisableInterrupts(DMA_Channel_TypeDef* CH, uint32_t flags){
    DMA_SetInterruptFlags(CH, flags, false);
}

//------------------------------------------------------------------------------
// check the interrupt flags of the DMA channel
bool DMA_CheckInterrupts(DMA_Channel_TypeDef* CH, uint32_t flags){
	bool result = false;
	uint32_t index = DMA_GetChannelIndex(CH);
    #define DMA_CHANNEL_INTS_MASK   0x0F
    #define DMA_CHANNEL_INTS_BITS   0x04

    flags &= DMA_CHANNEL_INTS_MASK;
    
	if(index < 7){
        flags <<= (DMA_CHANNEL_INTS_BITS * index);
		result = (bool)(DMA1->ISR & flags);
	} else {
		#ifdef DMA2
        flags <<= (DMA_CHANNEL_INTS_BITS * (index-7));
		result = (bool)(DMA2->ISR & flags);
		#endif
	}
	return(result);
}

//------------------------------------------------------------------------------
// clear the specified interrupt flags of the DMA channel
void DMA_ClearInterrupts(DMA_Channel_TypeDef* CH, uint32_t flags){

    uint32_t index = DMA_GetChannelIndex(CH);
    #define DMA_CHANNEL_INTS_MASK   0x0F
    #define DMA_CHANNEL_INTS_BITS   0x04

    flags &= DMA_CHANNEL_INTS_MASK;
    
	if(index < 7){
        flags <<= (DMA_CHANNEL_INTS_BITS * index);
		DMA1->IFCR |= flags;
	} else {
		#ifdef DMA2
        flags <<= (DMA_CHANNEL_INTS_BITS * (index-7));
		DMA2->IFCR |= flags;
		#endif
	}
}

//------------------------------------------------------------------------------
// clear ALL the interrupt flags of the DMA channel
void DMA_ClearInterrupts(DMA_Channel_TypeDef* CH){
    DMA_ClearInterrupts(CH, DMA_IFCR_ALL);
}

//------------------------------------------------------------------------------
// wait for DMA channel "available" (no undergoing transfer)
void DMA_WaitFreeChannel(DMA_Channel_TypeDef* CH){
    if(CH->CCR & DMA_CCR_EN){
        while(DMA_CheckInterrupts(CH, DMA_ISR_TCIF1)){}
    }
}

//------------------------------------------------------------------------------
// memory to memory DMA transfer
bool DMA_Move(DMA_Channel_TypeDef* Channel, uint8_t* Paddr, uint8_t* Maddr, uint16_t N){
    bool result = false;

    if((Channel != NULL)&& !(Channel->CCR & DMA_CCR_EN)){
        if((Paddr!= NULL)&&(Maddr!=NULL)){
            Channel->CCR &= ~DMA_CCR_EN;
            Channel->CPAR = (uint32_t)Paddr;
            Channel->CMAR = (uint32_t)Maddr;
            Channel->CNDTR = N;
            Channel->CCR = (DMA_CCR_RECEIVE_BYTES | DMA_CCR_EN);
            result = true;
        }
    }
    return(result);
}

//==============================================================================
