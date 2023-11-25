//==============================================================================
/**
 * @file Priorities.h
 * @brief EDROS Framework
 * Definition of priority level groups for IRQs and priority level "names" for\n
 * standardization throughout the framework.\n
 * Definition of internal IRQ vectors for the framework.\n
 * @note These vectors are not supposed to be used from within the application.\n
 * Its purpose is entirely related to "NComponents" internals.\n
 * @version 1.0.0
 * @author   J. Nilo Rodrigues  -  nilo@pobox.com
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
#ifndef PRIORITIES_H
    #define PRIORITIES_H

    //--------------------------------------
    #include <stdio.h>
    #include "GenericTypeDefs.h"

/*
 * @defgroup Priorities
 * @{
 */
//==============================================================================
#define NVIC_PriorityGroup_0         ((uint32_t)0x700) 	/*!< 0 bits for preemption priority,
                                                        	 4 bits for sub-priority */
#define NVIC_PriorityGroup_1         ((uint32_t)0x600) 	/*!< 1 bits for preemption priority,
                                                             3 bits for sub-priority */
#define NVIC_PriorityGroup_2         ((uint32_t)0x500) 	/*!< 2 bits for preemption priority,
                                                             2 bits for sub-priority */
#define NVIC_PriorityGroup_3         ((uint32_t)0x400) 	/*!< 3 bits for preemption priority,
                                                             1 bits for sub-priority */
#define NVIC_PriorityGroup_4         ((uint32_t)0x300) 	/*!< 4 bits for preemption priority,
                                                             0 bits for sub-priority, (EDROS default)*/
//------------------------------------------------------------------------------
#define SYS_PRIORITY_HIGHEST    ((uint8_t) 0x00)       	//!< highest priority level
#define SYS_PRIORITY_LEVEL_1    ((uint8_t) 0x01)		//!< second highest priority level
#define SYS_PRIORITY_LEVEL_2    ((uint8_t) 0x02)		//!< normal priority level 2
#define SYS_PRIORITY_LEVEL_3    ((uint8_t) 0x03)		//!< normal priority level 3
#define SYS_PRIORITY_LEVEL_4    ((uint8_t) 0x04)		//!< normal priority level 4
#define SYS_PRIORITY_LEVEL_5    ((uint8_t) 0x05)		//!< normal priority level 5
#define SYS_PRIORITY_LEVEL_6    ((uint8_t) 0x06)		//!< normal priority level 6
#define SYS_PRIORITY_LEVEL_7    ((uint8_t) 0x07)		//!< normal priority level 7
#define SYS_PRIORITY_LEVEL_8    ((uint8_t) 0x08)		//!< normal priority level 8
#define SYS_PRIORITY_LEVEL_9    ((uint8_t) 0x09)		//!< normal priority level 9
#define SYS_PRIORITY_LEVEL_10   ((uint8_t) 0x0A)		//!< normal priority level 10
#define SYS_PRIORITY_LEVEL_11   ((uint8_t) 0x0B)		//!< normal priority level 11
#define SYS_PRIORITY_LEVEL_12   ((uint8_t) 0x0C)		//!< normal priority level 12
#define SYS_PRIORITY_NORMAL     ((uint8_t) 0x0D)		//!< normal priority level 13
#define SYS_PRIORITY_LOW        ((uint8_t) 0x0E)		//!< second lowest priority level
#define SYS_PRIORITY_LOWEST     ((uint8_t) 0x0F)		//!< lowest priority level

/**
 * @}
 */

//------------------------------------------------------------------------------
/**
 * @defgroup NV_ID
 * @brief Interrupt Vector Indexes
 * @code
 */
#if defined (STM32F103xB) || (STM32F103x6)
NV_ID NV_EXTINT0    = 0x00;     //  0
NV_ID NV_EXTINT1    = 0x01;     //  1
NV_ID NV_EXTINT2    = 0x02;     //  2
NV_ID NV_EXTINT3    = 0x03;     //  3
NV_ID NV_EXTINT4    = 0x04;     //  4
NV_ID NV_EXTINT5    = 0x05;     //  5
NV_ID NV_EXTINT6    = 0x06;     //  6
NV_ID NV_EXTINT7    = 0x07;     //  7
NV_ID NV_EXTINT8    = 0x08;     //  8
NV_ID NV_EXTINT9    = 0x09;     //  9
NV_ID NV_EXTINT10   = 0x0A;     // 10
NV_ID NV_EXTINT11   = 0x0B;     // 11
NV_ID NV_EXTINT12   = 0x0C;     // 12
NV_ID NV_EXTINT13   = 0x0D;     // 13
NV_ID NV_EXTINT14   = 0x0E;     // 14
NV_ID NV_EXTINT15   = 0x0F;     // 15
NV_ID NV_DMA1_CH1   = 0x10;     // 16
NV_ID NV_DMA1_CH2   = 0x11;     // 17
NV_ID NV_DMA1_CH3   = 0x12;     // 18
NV_ID NV_DMA1_CH4   = 0x13;     // 19
NV_ID NV_DMA1_CH5   = 0x14;     // 20
NV_ID NV_DMA1_CH6   = 0x15;     // 21
NV_ID NV_DMA1_CH7   = 0x16;     // 22
NV_ID NV_TIM1       = 0x1C;     // 28
NV_ID NV_TIM2       = 0x1D;     // 29
NV_ID NV_TIM3       = 0x1E;     // 30
NV_ID NV_TIM4       = 0x1F;     // 31
NV_ID NV_I2C1       = 0x24;     // 36
NV_ID NV_I2C2       = 0x25;     // 37
NV_ID NV_SPI1       = 0x26;     // 38
NV_ID NV_SPI2       = 0x27;     // 39
NV_ID NV_SPI3       = 0x28;     // 40
NV_ID NV_UART1      = 0x29;     // 41
NV_ID NV_UART2      = 0x2A;     // 42
NV_ID NV_UART3      = 0x2B;     // 43
NV_ID NV_CAN1       = 0x2E;     // 46
NV_ID NV_USB        = 0x30;     // 48
NV_ID NV_ADC1       = 0x32;     // 50
NV_ID NV_ADC2       = 0x33;     // 51
NV_ID NV_RTC        = 0x35;     // 53
NV_ID NV_LAST       = NV_RTC;
NV_ID NV_NULL       = 0xFF;
#define __SYS_MAX_VECTORS NV_LAST
#elif defined (STM32F105xC) || defined (STM32F107xC)
NV_ID NV_EXTINT0    = 0x00;     //  0
NV_ID NV_EXTINT1    = 0x01;     //  1
NV_ID NV_EXTINT2    = 0x02;     //  2
NV_ID NV_EXTINT3    = 0x03;     //  3
NV_ID NV_EXTINT4    = 0x04;     //  4
NV_ID NV_EXTINT5    = 0x05;     //  5
NV_ID NV_EXTINT6    = 0x06;     //  6
NV_ID NV_EXTINT7    = 0x07;     //  7
NV_ID NV_EXTINT8    = 0x08;     //  8
NV_ID NV_EXTINT9    = 0x09;     //  9
NV_ID NV_EXTINT10   = 0x0A;     // 10
NV_ID NV_EXTINT11   = 0x0B;     // 11
NV_ID NV_EXTINT12   = 0x0C;     // 12
NV_ID NV_EXTINT13   = 0x0D;     // 13
NV_ID NV_EXTINT14   = 0x0E;     // 14
NV_ID NV_EXTINT15   = 0x0F;     // 15
NV_ID NV_DMA1_CH1   = 0x10;     // 16
NV_ID NV_DMA1_CH2   = 0x11;     // 17
NV_ID NV_DMA1_CH3   = 0x12;     // 18
NV_ID NV_DMA1_CH4   = 0x13;     // 19
NV_ID NV_DMA1_CH5   = 0x14;     // 20
NV_ID NV_DMA1_CH6   = 0x15;     // 21
NV_ID NV_DMA1_CH7   = 0x16;     // 22
NV_ID NV_DMA2_CH1   = 0x17;     // 23
NV_ID NV_DMA2_CH2   = 0x18;     // 24
NV_ID NV_DMA2_CH3   = 0x19;     // 25
NV_ID NV_DMA2_CH4   = 0x1A;     // 26
NV_ID NV_DMA2_CH5   = 0x1B;     // 27
NV_ID NV_TIM1       = 0x1C;     // 28
NV_ID NV_TIM2       = 0x1D;     // 29
NV_ID NV_TIM3       = 0x1E;     // 30
NV_ID NV_TIM4       = 0x1F;     // 31
NV_ID NV_TIM5       = 0x20;     // 32
NV_ID NV_TIM6       = 0x21;     // 33
NV_ID NV_TIM7       = 0x22;     // 34
NV_ID NV_TIM8       = 0x23;     // 35
NV_ID NV_I2C1       = 0x24;     // 36
NV_ID NV_I2C2       = 0x25;     // 37
NV_ID NV_SPI1       = 0x26;     // 38
NV_ID NV_SPI2       = 0x27;     // 39
NV_ID NV_SPI3       = 0x28;     // 40
NV_ID NV_UART1      = 0x29;     // 41
NV_ID NV_UART2      = 0x2A;     // 42
NV_ID NV_UART3      = 0x2B;     // 43
NV_ID NV_UART4      = 0x2C;     // 44
NV_ID NV_UART5      = 0x2D;     // 45
NV_ID NV_CAN1       = 0x2E;     // 46
NV_ID NV_CAN2       = 0x2F;     // 47
NV_ID NV_USB        = 0x30;     // 48
NV_ID NV_ETH        = 0x31;     // 49
NV_ID NV_ADC1       = 0x32;     // 50
NV_ID NV_ADC2       = 0x33;     // 51
NV_ID NV_ADC3       = 0x34;     // 52
NV_ID NV_RTC        = 0x35;     // 53
NV_ID NV_LAST       = NV_RTC;
NV_ID NV_NULL       = 0xFF;
#define __SYS_MAX_VECTORS NV_LAST
#endif
/*
 * @endcode
 * @}
 */

//------------------------------------------------------------------------------
#endif
//==============================================================================
