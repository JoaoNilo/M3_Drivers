//==============================================================================
/** @file DRV_SSR.h
 *  @brief System Services Requests Kernel Driver
 *  @version 1.0.0
 *  @author J. Nilo Rodrigues - nilo@pobox.com
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
#ifndef DRV_SSR_H
    #define DRV_SSR_H

//------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"{
#endif

	#include "stm32f1xx.h"
    #include <SysExceptions.h>

	#define LSB16	((uint32_t)0x0000FFFF)
  	#define MSB16	((uint32_t)0xFFFF0000)
  	#define MSBITS	((uint32_t)16)

	//--------------------------------------------------------------------------
    #define SVC_RELOCATE_VECTORS			((uint8_t) 0x00)
	#define SVC_INCLUDE_COMPONENT			((uint8_t) 0x01)
	#define SVC_EXCLUDE_COMPONENT			((uint8_t) 0x02)
	#define SVC_INSTALL_CALLBACK			((uint8_t) 0x03)
	#define SVC_FIND_COMPONENT			    ((uint8_t) 0x04)
	#define SVC_GET_CALLBACK				((uint8_t) 0x05)
	#define SVC_GET_SYSTEM_TIME				((uint8_t) 0x06)
	#define SVC_INSTALL_TIMEOUT				((uint8_t) 0x07)
	#define SVC_GET_KS_CODE					((uint8_t) 0x08)
	#define SVC_GET_CALLBACK_VECTOR			((uint8_t) 0x09)
	#define SVC_THROW_MESSAGE				((uint8_t) 0x10)
	#define SVC_THROW_EXCEPTION				((uint8_t) 0x11)

	#define SVC_MICROSECONDS				((uint8_t) 0x12)
	#define SVC_DELAY						((uint8_t) 0x13)
	#define SVC_MICRODELAY					((uint8_t) 0x14)

	//--------------------------------------------------------------------------
	/**
	 *  @defgroup DRV_SSR
	 *  @{
	 */

	//--------------------------------------------------------------------------
	#if defined(__GNUC__)
		#define __svc							__attribute__((naked)) __attribute__((noinline))

		//void __attribute__((naked)) __attribute__((noinline)) SSR_IncludeComponent(uint32_t hComp);
		void __svc SSR_IncludeComponent(uint32_t hComp);

		//void __attribute__((naked)) __attribute__((noinline)) SSR_ExcludeComponent(uint32_t hComp);
		void __svc SSR_ExcludeComponent(uint32_t hComp);

		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_InstallCallback(uint32_t, uint32_t);
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetCallback(uint32_t);
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetSystemTime(void);
		bool __attribute__((naked)) __attribute__((noinline)) SSR_InstallTimeout(uint32_t, uint32_t);
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetKSCode(void);
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_GetCallbackVector(uint32_t);
		void __attribute__((naked)) __attribute__((noinline)) SSR_ThrowMessage(uint32_t, uint32_t, uint32_t, uint32_t);
		void __attribute__((naked)) __attribute__((noinline)) SSR_ThrowException(uint32_t);
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_Microseconds();
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_Delay(uint32_t);
		uint32_t __attribute__((naked)) __attribute__((noinline)) SSR_MicroDelay(uint32_t);

    #elif defined(__ICCARM__)
		__irq void SSR_IncludeComponent(uint32_t);
		__irq void SSR_ExcludeComponent(uint32_t);
		__irq uint32_t SSR_InstallCallback(uint32_t, uint32_t);
		__irq uint32_t SSR_GetCallback(uint32_t);
		__irq uint32_t SSR_GetSystemTime(void);
		__irq bool SSR_InstallTimeout(uint32_t, uint32_t);
		__irq uint32_t SSR_GetKSCode(void);
		__irq uint32_t SSR_GetCallbackVector(uint32_t);
		__irq void SSR_ThrowMessage(uint32_t, uint32_t, uint32_t, uint32_t);
		__irq void SSR_ThrowException(uint32_t);
    #endif

	//--------------------------------------------------------------------------
	/**
	 * @brief SSR_Relocate
	 * - Relocates the ISR vector table.
	 * @arg nVectors: the size of vector table to relocate
	 * @note  1 - The relocation defaults to 0x20000000 in RAM.
	 * @note  2 - Make sure to allocate enough space in this position before relocating the vector table.
	 */
	void  SSR_Relocate(int nVectors);

	/**
	 * @brief SSR_Allocate
	 * - Allocates a particular ISR function handler in the vector table.
	 * @arg IsrAddress: the ISR function address
	 * @arg VectorIndex is the IRQ number for this ISR function.
	 */
	void SSR_Allocate(uint32_t IsrAddress, uint32_t VectorIndex);

/*
 * @}
 */

#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------

#endif
//==============================================================================
