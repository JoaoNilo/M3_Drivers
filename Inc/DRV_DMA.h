//==============================================================================
/** @file DRV_DMA.h
 *  @brief DMA Kernel Driver
 *  @version 1.0.0
 *  @author   J. Nilo Rodrigues  -  nilo@pobox.com
 *
 *------------------------------------------------------------------------------
 *
 * <h2><center>&copy; Copyright (c) 2020 Joao Nilo Rodrigues </center></h2>
 * <h2><center> All rights reserved. </center></h2>
 *
 * This software component is licensed by "Joao Nilo Rodrigues" under BSD 3-Clause
 * license, the "License".
 * You may not use this file except in compliance with the License.
 *               You may obtain a copy of the License at:
 *                 opensource.org/licenses/BSD-3-Clause
 *
 *///---------------------------------------------------------------------------
#ifndef DRV_DMA_H
    #define DRV_DMA_H

	#include <stddef.h>
    #include "stm32f1xx.h"
	#include "Priorities.h"

#ifdef __cplusplus
extern "C"{
#endif

//-------------------------- migration table ----------------------------------
#define DMA1_CLOCK_ON  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
#define DMA2_CLOCK_ON RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

//-----------------------------------------------------------------------------
#define DMA_IFCR_ALL               (DMA_IFCR_CTEIF1 |  DMA_IFCR_CHTIF1 | DMA_IFCR_CTCIF1 | DMA_IFCR_CGIF1)
#define DMA_CCR_DISABLE			   ((uint32_t)0xFFFF0000)
//#define DMA_CCR_ISR_COM            (DMA_CCR_TCIE | DMA_CCR_TEIE)
#define DMA_CCR_ALL            	   (DMA_CCR_TCIE | DMA_CCR_HTIE | DMA_CCR_TEIE)
#define DMA_CCR_BYTES              0
#define DMA_CCR_WORDS              (DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0)
//#define DMA_CCR_DWORDS             (DMA_CCR_PSIZE_1 | DMA_CCR_MSIZE_1)
#define DMA_CCR_SEND_BYTES         (DMA_CCR_ISR_COM | DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_BYTES)
//#define DMA_CCR_BLK_SEND_BYTES     (DMA_CCR1_DIR | DMA_CCR1_MINC | DMA_CCR_BYTES)
#define DMA_CCR_RECEIVE_BYTES      ((DMA_CCR_TCIE | DMA_CCR_TEIE) | DMA_CCR_MINC | DMA_CCR_BYTES)

//#define DMA_CCR_EN                  DMA_CCR_EN
//#define DMA_CCR_BLK_RECEIVE_BYTES   (DMA_CCR_ISR_COM | DMA_CCR1_MINC | DMA_CCR_BYTES)
//#define DMA_CCR_PUSH_BYTES          (DMA_CCR_ISR_COM | DMA_CCR1_DIR | DMA_CCR_BYTES)
//#define DMA_CCR_MOVE_BYTES          (DMA_CCR1_PINC | DMA_CCR1_MINC | DMA_CCR_BYTES | DMA_CCR1_MEM2MEM)
//#define DMA_CCR_RECEIVE_WORDS       (DMA_CCR_ISR_COM | DMA_CCR1_MINC | DMA_CCR_WORDS)
#define DMA_CCR_RECEIVE_ADC        (DMA_CCR_ISR_ALL | DMA_CCR_CIRC | DMA_CCR_MINC | DMA_CCR_WORDS)
//#define CHANNEL_DEFAULT_PROFILE     DMA_CCR_SEND_BYTES


//extern "C"{

//-----------------------------------------------------------------------------
/**
 *  @defgroup DRV_DMA
 *  @{
 */

/**
 * @brief DMA_GetChannelIndex
 * - Gets a numerical index for a given DMA channel.
 * @arg CHn is the DMA channel number
 * @return the numerical index index
 * - (0 to 6 for DMA1 channels)
 * - (7 to 11 for DMA2 channels, when available)
 * @note This function is used by other functions of this driver.
 * Normally it is useless for the application developer.
 */
uint32_t DMA_GetChannelIndex(DMA_Channel_TypeDef* CHn);

/**
 * @brief DMA_GetChannelIRQn
 * - Gets the IRQ number for a given DMA channel.
 * @arg CHn is the DMA channel number
 * @return the channel IRQn (cast to IRQn_Type)
 */
IRQn_Type DMA_GetChannelIRQn(DMA_Channel_TypeDef* CHn);


/**
 * @brief DMA_SetInterruptFlags
 * - This function can set or clear the selected "interrupt enable" flags.
 * @arg CHn is the DMA channel
 * @arg Flags to set or clear. (i. e. DMA_CCR_TCIE, DMA_CCR_TEIE, etc)
 * @arg Status is the new interrupt state (true for enabled).
 * @note The interrupt flags can be "ored" together. ( DMA_CCR_TCIE | DMA_CCR_TEIE)
 */
void DMA_SetInterruptFlags(DMA_Channel_TypeDef* CHn, uint32_t Flags, bool Status);

/**
 * @brief DMA_EnableInterrupts
 * - This function sets the selected "interrupt enable" flags in the CCR.
 * @arg CHn is the DMA channel
 * @arg Flags the interrupt flags to enable. (i. e. DMA_CCR_TCIE, DMA_CCR_TEIE, etc)
 * @note The interrupt flags can be "ored" together. ( DMA_CCR_TCIE | DMA_CCR_TEIE)
 */
void DMA_EnableInterrupts(DMA_Channel_TypeDef* CH, uint32_t Flags);

/**
 * @brief DMA_DisableInterrupts
 * - This function clears the selected "interrupt enable" flags in the CCR.
 * @arg CHn is the DMA channel
 * @arg Flags the interrupt flags to disable. (i. e. DMA_CCR_TCIE, DMA_CCR_TEIE, etc)
 * @note The interrupt flags can be "ored" together. ( DMA_CCR_TCIE | DMA_CCR_TEIE)
 */
void DMA_DisableInterrupts(DMA_Channel_TypeDef* CH, uint32_t Flags);

/**
 * @brief DMA_ClearInterrupt
 * - This function resets the selected interrupt flags (via IFCR).
 * @arg CHn is the DMA channel
 * @arg Flags the interrupt flags to clear. (i. e. DMA_CCR_TCIE, DMA_CCR_TEIE, etc)
 * @note The interrupt flags can be "ored" together. ( DMA_CCR_TCIE | DMA_CCR_TEIE)
 */
void DMA_ClearInterrupts(DMA_Channel_TypeDef* CHn, uint32_t flags);

/**
 * @brief DMA_CheckInterrupt
 * - This function checks the status of selected interrupt flags (via ISR).
 * @arg CHn is the DMA channel
 * @arg Flag the interrupt flags to check. (i. e. DMA_ISR_TCIE, DMA_CCR_TEIE, etc)
 * @return the "interrupt flag" status.
 * @warning The interrupt flag can be "ored" together. ( DMA_CCR_TCIE | DMA_CCR_TEIE)
 */
bool DMA_CheckInterrupts(DMA_Channel_TypeDef* CHn, uint32_t Flag);

/**
 * @brief DMA_WaitFreeChannel
 * - This function locks the execution until the current DMA transfer ends.
 * @arg CHn is the DMA channel
 */
void DMA_WaitFreeChannel(DMA_Channel_TypeDef* CHn);

/**
 * @brief DMA_Move
 * - This function can be used to move big blocks of data from one memory position to another.
 * The final result emulates the "memcpy" function, although, much faster.
 * @arg CHn is the DMA channel
 * @arg SrcAddr the address of the source data buffer.
 * @arg SrcAddr the address of the destination data buffer.
 * @arg N the size of the source data packet.
 * @return the operation result (true if successful);
 */
bool DMA_Move(DMA_Channel_TypeDef* CHn, uint8_t* SrcAddr, uint8_t* DstAddr, uint16_t N);

/**
 * @} // close group DRV_DMA
 */

#ifdef __cplusplus
}
#endif

#endif
//==============================================================================
