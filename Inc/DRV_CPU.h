//==============================================================================
/** @file DRV_CPU.h
 *  @brief CPU Kernel Driver
 *  This driver provides useful functions related to the NVIC and RCC,\n
 *  like clock frequency setup, enable/disable peripheral clocks, etc.
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
#ifndef DRV_CPU_H
    #define DRV_CPU_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include "stm32f1xx.h"

#define IWDG_ENABLE                     ((uint32_t)0x0000CCCC)
#define IWDG_WRITE_ACCESS               ((uint32_t)0x00005555)
#define IWDG_REFRESH                    ((uint32_t)0x0000AAAA)
#define IWDG_RELOAD                     ((uint32_t)100)

#define WATCHDOG_FLAG_SET             	0x20000000
#define WATCHDOG_FLAG_REMOVER         	0x00800000
#define SOFTRESET_FLAG_SET            	0x10000000
#define SOFTRESET_FLAG_REMOVER        	0x00800000

#define __WATCHDOG_FLAG_SET             0x20000000
#define __WATCHDOG_FLAG_REMOVER         0x01000000

//==============================================================================
#define HSE_Value               ((uint32_t) 8000000)
#define HSI_Value               ((uint32_t) 8000000)
#define SYSCLK_FREQ_72MHz       72000000
//==============================================================================

#define RCC_PREDIV1_DIV1	0
#define RCC_PREDIV1_DIV2	1
#define RCC_PLLMULL_4		(RCC_CFGR_PLLMULL_1)
#define RCC_PLLMULL_8		(RCC_CFGR_PLLMULL_1 | RCC_CFGR_PLLMULL_2)
#define RCC_PLLMULL_9		(RCC_CFGR_PLLMULL_0 | RCC_CFGR_PLLMULL_1 | RCC_CFGR_PLLMULL_2)


//------------------------------------------------------------------------------
const uint32_t PllFactors[6][2] = {
    {RCC_PREDIV1_DIV2, RCC_PLLMULL_4},   	// [2, 4] = (f / 2) * 4
    {RCC_PREDIV1_DIV2, RCC_PLLMULL_8},   	// [2, 8] = (f / 2) * 8
    {RCC_PREDIV1_DIV2, RCC_PLLMULL_9},   	// [2, 9] = (f / 2) * 9
    {RCC_PREDIV1_DIV1, RCC_PLLMULL_8},   	// [1, 8] = (f / 2) * 8
    {RCC_PREDIV1_DIV1, RCC_PLLMULL_9},   	// [1, 9] = (f / 1) * 9
};

//------------------------------------------------------------------------------

/**
 *  @defgroup DRV_CPU
 *  @{
 */

/**
 * @enum PllSources
 * @{
 * @brief This enumeration defines options for the PLL source oscillator.
 *
 */
enum PllSources { 	Pll_Hsi,				//!< High Speed Internal oscillator as source for the PLL
					Pll_Hse					//!< High Speed External oscillator as source for the PLL
				};
/**
 * @}
 */


/**
 * @enum PllFrequencies
 * @brief This enumeration defines options for the PLL output frequency.
 * @{
 */
enum PllFrequencies { Pll16MHz,				//!< PLL as source clock, running at 16Mhz
					  Pll32MHz,				//!< PLL as source clock, running at 32Mhz
					  Pll36MHz,				//!< PLL as source clock, running at 36Mhz
					  Pll64MHz,				//!< PLL as source clock, running at 64Mhz
					  Pll72MHz				//!< PLL as source clock, running at 72Mhz
					};
/**
 * @}
 */

/**
 * @enum BusFrequencies
 * @brief This enumeration defines options for the APB prescaler (divider).
 * @note The APBx frequency is always related to HCLK frequency, so the resulting frequency
 * is also dependent of the HCLK prescaler. For EDROS applications, most of the time this
 * prescaler is left as "0", so the HCLK is the same as SYSCLK.
 * @{
 */
enum BusFrequencies { 	HCLK_DIV1  = 0,		//!< APBx clock frequency is (HCLK / 1)
						HCLK_DIV2  = 4,		//!< APBx clock frequency is (HCLK / 2)
						HCLK_DIV4  = 5,		//!< APBx clock frequency is (HCLK / 4)
						HCLK_DIV8  = 6,		//!< APBx clock frequency is (HCLK / 8)
						HCLK_DIV16 = 7		//!< APBx clock frequency is (HCLK / 16)
					};
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
/**
 * @brief CPU_StartHSE
 * - Starts or changes the MCU clock source to High Speed External oscillator (HSE).
 * @note This clock source demands the use of an external crystal oscillator.
 * Make sure this resource is available in your hardware.
 */
void CPU_StartHSE(void);

/**
 * @brief CPU_StartHSI
 * - Starts or changes the MCU clock source to High Speed Internal oscillator (HSI).
 * @note This clock source should not be used when accurate timing is needed, specially when
 * wide operating temperature is expected.
 */
void CPU_StartHSI();

/**
 * @brief CPU_StartPLL
 * - Starts or changes the MCU clock source to Phase-Locked Loop (PLL).
 * @arg PllSource
 * - the clock source to the PLL. As seen before, both HSI or HSE have pros and cons.
 * The PllSources enumeration provides the options for this parameter.
 * @arg CpuSpeed
 * - the desired clock speed for the microcontroller unit.
 * The CPUSpeed enumeration provides the options for this parameter.
 */
void CPU_StartPLL(PllSources PllSource, PllFrequencies CpuSpeed);

/**
 * @brief CPU_GetFrequency
 * - This function returns the current system clock frequency (SYSCLK)
 * @return SYSCLK frequency (in MHz)
 */
uint32_t CPU_GetFrequency();

/**
 * @brief CPU_SetFrequencyAPB1
 * - This function can be used to set a new clock frequency to APB1 peripherals bus.
 * @arg newApb1Freq: new APB1 frequency chosen from the @ref BusFrequencies enumeration.
 * @note Note that APB buses frequencies are always defined as fractions of the HCLK,
 * which in turn is a fraction of SYSCLK. In the case of EDROS, SYSCLK and HCLK are
 * always set to the same frequency.
 */
void CPU_SetFrequencyAPB1(BusFrequencies newApb1Freq);


/**
 * @brief CPU_GetFrequencyAPB1
 * - This function returns clock frequency for the APB1 peripherals bus.
 * @return APB1 clock frequency (in MHz)
 */
uint32_t CPU_GetFrequencyAPB1();


/**
 * @brief CPU_SetFrequencyAPB2
 * - This function can be used to set a new clock frequency to APB2 peripherals bus.
 * @arg newApb2Freq: new APB2 frequency chosen from the @ref BusFrequencies enumeration.
 * @note Note that APB buses frequencies are always defined as fractions of the HCLK,
 * which in turn is a fraction of SYSCLK. In the case of EDROS, SYSCLK and HCLK are
 * always set to the same frequency.
 */
void CPU_SetFrequencyAPB2(BusFrequencies  newApb2Freq);

/**
 * @brief CPU_GetFrequencyAPB2
 * - This function returns clock frequency for the APB2 peripherals bus.
 * @return APB2 clock frequency (in MHz)
 */
uint32_t CPU_GetFrequencyAPB2();

/**
 * @brief CPU_GetTimerFrequencyAPB1
 * - This function returns clock frequency for the APB1 timers.
 * @return APB1 "timers" clock frequency (in MHz)
 * @note Depending on the selected prescaler value for the APB1 clock
 * the frequency available for the timer might be different to the APB1
 * bus clock frequency itself. Please refer to the datasheet for detailed
 * description.
 */
uint32_t CPU_GetTimerFrequencyAPB1();


/**
 * @brief CPU_GetTimerFrequencyAPB2
 * - This function returns clock frequency for the APB2 timers.
 * @return APB2 "timers" clock frequency (in MHz)
 */
uint32_t CPU_GetTimerFrequencyAPB2();

/**
 * @brief CPU_PeripheralClockEnable
 * - Turns the peripheral clock on.
 * @arg P_handle is peripheral handle (i. e. USART1, TIM2, etc)
 * @return true if success (peripheral found and successfully enabled)
 */
bool CPU_PeripheralClockEnable(void* P_handle);

/**
 * @brief CPU_PeripheralClockDisable
 * - Turns the peripheral clock off.
 * @arg P_handle is peripheral handle (i. e. USART1, TIM2, etc)
 * @return true if success (peripheral found and successfully disabled)
 */
bool CPU_PeripheralClockDisable(void*);

/**
 * @brief CPU_PeripheralClockStatus
 * - Returns the peripheral clock status.
 * @arg P_handle is peripheral handle (i. e. USART1, TIM2, etc)
 * @return true if clock is "on".
 */
bool CPU_PeripheralClockStatus(void*);

/**
 * @brief CPU_InitializeWatchdog
 * - Starts the Independent Watchdog (IWG)
 * @arg T is the interval in milliseconds.
 * @note Once enabled the watchdog can no longer be disabled from the application.
 * @note Care should be taken if low-power modes are in the application.
 */
void CPU_InitializeWatchdog(uint16_t T);

/**
 * @brief CPU_CheckWatchdog
 * - Checks if the last restart was caused by the watchdog (IWG).
 * @return true if watchdog caused the last restart.
 */
bool CPU_CheckWatchdog();

/**
 * @brief CPU_KickWatchdog
 * - Refreshes the watchdog counter to avoid restart.
 */
void CPU_KickWatchdog();

/**
 * @brief CPU_Reset
 * - Forced software reset.
 */
void CPU_Reset();

/**
 * @brief CPU_SetPriorityIRQn
 * - Sets the priority for a given IQRn in NVIC.
 * @arg IRQn is the number of the IRQ line
 * @arg priority is the desired priority level.
 * @note Important to note that EDROS defines NVIC_PriorityGroup_4 as its default.
 * This means that all four bits are used for priority and none for sub-priority.
 * Priority levels are defined in @ref Priorities
 */
void CPU_SetPriorityIRQn(IRQn_Type IRQn, uint32_t piority);

/**
 * @brief CPU_Crc32
 * - General purpose 32-bit CRC generation based on the Ethernet poly/seed.
 * @arg pt is a pointer to the input data buffer
 * @arg n is the number of bytes in the buffer
 * @return calculated  32-bit CRC
 */
uint32_t CPU_Crc32(uint8_t*, uint16_t);

/**
 * @} // close group DRV_CPU
 */




#endif
//==============================================================================
