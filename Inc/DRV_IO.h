//==============================================================================
/** @file DRV_IO.h
 *  @brief IO Kernel Driver
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
#ifndef DRV_IO_H
    #define DRV_IO_H

#ifdef __cplusplus
extern "C"{
#endif

#include "stm32f1xx.h"
#include "Priorities.h"


//------------------------------------------------------------------------------
/**
 * @if cond_macros
 */
#define IO_MASK_MODE    	((uint32_t) 0x0000000F)
#define IO_MASK_INT     	((uint32_t) 0x00000010)
#define IO_MASK_RISE    	((uint32_t) 0x00000020)
#define IO_MASK_FALL    	((uint32_t) 0x00000040)
#define IO_MASK_PIN     	((uint32_t) 0xF0000000)

#define IO_MASK_EXTI    	((uint32_t)0x0000000F)

#define IO_FIELD_MODE   	((uint32_t)4)
#define IO_FIELD_INT    	((uint32_t)1)
#define IO_FIELD_RISE   	((uint32_t)1)
#define IO_FIELD_FALL   	((uint32_t)1)
#define IO_FIELD_RESERVED 	((uint32_t)17)
#define IO_FIELD_PIN    	((uint32_t)8)
/**
 * @endif
 */

//------------------------------------------------------------------------------
/**
 * @brief IO configuration data structure.
 * - This structure defines how pin configuration info is passed to IO_PinInit()
 */
struct IO_Config{
    uint32_t Mode   	:IO_FIELD_MODE;    	//!< please, refer to enumeration @ref io_modes
    uint32_t Int    	:IO_FIELD_INT;      //!< 1-bit flag. if true, this pin will fire interrupts.
    uint32_t Rise   	:IO_FIELD_RISE;     //!< 1-bit flag. if true, OnRisingEdge events will be fire.
    uint32_t Fall   	:IO_FIELD_FALL;     //!< 1-bit flag. if true, OnFallingEdge events will be fire.
    uint32_t Reserved 	:IO_FIELD_RESERVED;	//!< 17-bit field. NOT used.
    uint32_t Pin    	:IO_FIELD_PIN;      //!< 8-bit field. Pin number (0 to 15)
} ;

//------------------------------------------------------------------------------
/**
 * @if cond_macros2
 */
#define GPIO_SIZE                   0x0400

//------------------------------------------------------------------------------
#define __PINOUT_NORMAL             false
#define __PINOUT_REMAPPED           true

#define __PERIPHERAL_USART          0x00001
#define __PERIPHERAL_SPI            0x00002
#define __PERIPHERAL_I2C            0x00003
//#define __PERIPHERAL_TIMER          0x00004
//#define __PERIPHERAL_ADC            0x00005
//#define __PERIPHERAL_DAC            0x00006
//#define __PERIPHERAL_CAN            0x00007
//#define __PERIPHERAL_USB            0x00008

#define __USART1_STANDARD           0x0010
#define __USART1_REMAPPED           0x8010
#define __USART2_STANDARD           0x0011
#define __USART2_REMAPPED           0x8011
#define __USART3_STANDARD           0x0012
#define __USART3_REMAPPED           0x8012
#define __UART4_STANDARD            0x0013
#define __UART4_REMAPPED            0x0014
#define __UART5_STANDARD            0x0015
#define __UART5_REMAPPED            0x0016

#define __CAN1_STANDARD             0x0020
#define __CAN1_REMAPPED1            0x8020
#define __CAN1_REMAPPED2            0x8021
#define __CAN2_STANDARD             0x0024
#define __CAN2_REMAPPED             0x8025

#define __SPI1_STANDARD             0x0030
#define __SPI1_REMAPPED             0x8030
#define __SPI2_STANDARD             0x0031
#define __SPI2_REMAPPED             0x8031
#define __SPI3_STANDARD             0x0032
#define __SPI3_REMAPPED             0x8032

#define __I2C1_STANDARD             0x0038
#define __I2C1_REMAPPED             0x8038

#define __I2C2_STANDARD             0x0039
#define __I2C2_REMAPPED             0x8039

#define __USB_STANDARD              0x8040
#define __USB_REMAPPED              0x8040

#define __OUTPUT_STANDARD           0x0100
#define __OUTPUT_FAST               0x8100

#define __INPUT_STANDARD            0x0101
#define __INPUT_FAST                0x8101

#define __MASK_REMAP                0x8000
#define __MASK_FULL                 0x4000
#define __MASK_PORT                 0x00FF
#define __MASK_PIN                  0xFF00
#define __MASK_SLAVE                __MASK_FULL

//----------------------------------------
// used in 32bit encoded port_pin (0x0000PPpp)
#define __NOTAV                     0xFFFF
#define __PORTA                     0x0000
#define __PORTB                     0x0100
#define __PORTC                     0x0200
#define __PORTD                     0x0300
#define __PORTE                     0x0400
#define __PORTF                     0x0500
#define __PORTG                     0x0600
#define __PIN0                      0x0000
#define __PIN1                      0x0001
#define __PIN2                      0x0002
#define __PIN3                      0x0003
#define __PIN4                      0x0004
#define __PIN5                      0x0005
#define __PIN6                      0x0006
#define __PIN7                      0x0007
#define __PIN8                      0x0008
#define __PIN9                      0x0009
#define __PIN10                     0x000A
#define __PIN11                     0x000B
#define __PIN12                     0x000C
#define __PIN13                     0x000D
#define __PIN14                     0x000E
#define __PIN15                     0x000F
/**
 * @endif
 */

// @if show_pinout

//------------------------------------------------------------------------------
/**
 * @brief Peripherals pinout configuration structure
 * @warning This structure is now deprecated!
 */
struct IO_Pinout{
    uint16_t peripheral;
    uint16_t config;
    struct {
        uint16_t tx;
        uint16_t rx;
        uint16_t rts;
        uint16_t cts;
    };
    struct{
        uint16_t miso;
        uint16_t mosi;
        uint16_t sck;
        uint16_t nss;
    };
};

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------
/**
 *  @defgroup DRV_IO
 *  @{
 */

//------------------------------------------------------------------------------
/**
 * @enum io_modes
 * @{
 * @brief This enumeration provides options for the "Mode" field
 * of the IO_Config structure.\n
 *
 */
enum io_modes {
    io_In_Analog            = 0x00,	//!< Analog input
    io_In_Floating          = 0x04,	//!< Digital input "floating"
    io_In_PullDown          = 0x08,	//!< Digital input with internal pull-down
    io_In_PullUp            = 0x0C,	//!< Digital input with internal pull-up
    io_Out_PushPull_2MHz    = 0x02,	//!< Digital push-pull output, low-speed data (less EMI)
    io_Out_PushPull_10MHz   = 0x01,	//!< Digital push-pull output, normal-speed data
    io_Out_PushPull_50MHz   = 0x03,	//!< Digital push-pull output, high-speed data
    io_Out_OpenDrain_2MHz   = 0x06,	//!< Digital open-drain output, low-speed data (less EMI)
    io_Out_OpenDrain_10MHz  = 0x05,	//!< Digital open-drain output, normal-speed data
    io_OutOpenDrain_50MHz   = 0x07,	//!< Digital open-drain output, high-speed data
    io_Alt_PushPull_2MHz    = 0x0A,	//!< Peripheral push-pull output, low-speed data (less EMI)
    io_Alt_PushPull_10MHz   = 0x09, //!< Peripheral push-pull output, normal-speed data
    io_Alt_PushPull_50MHz   = 0x0B, //!< Peripheral push-pull output, high-speed data
    io_Alt_OpenDrain_2MHz   = 0x0E,	//!< Peripheral open-drain output, low-speed data (less EMI)
    io_Alt_OpenDrain_10MHz  = 0x0D,	//!< Peripheral open-drain output, normal-speed data
    io_Alt_OpenDrain_50MHz  = 0x0F,	//!< Peripheral open-drain output, high-speed data
};
/**
 * @}
 */

//------------------------------------------------------------------------------
/**
 * @brief IO encoded data.
 * - Data format used to represent Port:Pin in a single 16-bit value.
 */
struct IO_Encoded{
	uint16_t full;			//!< encoded Port:Pin in a single 16-bit value
	union{
		uint8_t pin;		//!< pin number: least significant 8-bits
		uint8_t port;		//!< port number: most significant 8-bits
	};
};

//------------------------------------------------------------------------------
/**
 * @brief GetPortIndex
 * @arg Port is the port "base-address" to get the index (i. e. GPIOA, GPIOB, etc)
 * @return numerical index of the port (0: GPIOA, 1: GPIOB, etc)
 * @note This function is now deprecated, kept for backward compatibility purposes.
 */
uint32_t GetPortIndex(GPIO_TypeDef* Port);

/**
 * @brief IO_PortIndex
 * @arg Port is the port "base-address" to get the index (i. e. GPIOA, GPIOB, etc)
 * @return numerical index of the port (0: GPIOA, 1: GPIOB, etc)
 */
uint16_t IO_PortIndex(GPIO_TypeDef* Port);

/**
 * @brief IO_GetPort
 * @arg Index: numerical index of the port (0: GPIOA, 1: GPIOB, etc)
 * @return the port "base-address" for the given Index (i. e. GPIOA, GPIOB, etc)
 */
GPIO_TypeDef* IO_GetPort(uint32_t Index);


/**
 * @brief IO_PortClockOn
 * @arg Index: numerical index of the port to activate the clock.
 * @return the port "base-address" (i. e. GPIOA, GPIOB, etc)
 */
GPIO_TypeDef* IO_PortClockOn(uint32_t Index);

/**
 * @brief IO_PortClockEnable
 * @arg Port is the port "base-address" (i. e. GPIOA, GPIOB, etc)
 */
void IO_PortClockEnable(GPIO_TypeDef* Port);

/**
 * @brief IO_PinInit
 * @arg Port is the port "base-address" (i. e. GPIOA, GPIOB, etc)
 * @arg PinStruct is the pin configuration parameters structure as seen in @ref IO_Config.
 *
 */
void IO_PinInit(GPIO_TypeDef* Port, IO_Config* PinStruct);

/**
 * @brief IO_GetExtendedIT
 * - Returns the Port:Pin numbers for a given "external interrupt" index
 * @arg Pin is the index of the "interrupt line" which was fired.
 * @return The encoded Port:Pin value according to @ref IO_Encoded.
 *
 */
uint32_t IO_GetExtendedIT(uint32_t PinNumber);

/**
 * @brief IO_SetExtendedIT
 * - Sets a given GPIO pin as "external interrupt" source
 * @arg Port is the port "base-address" (i. e. GPIOA, GPIOB, etc)
 * @arg PinStruct is the pin configuration parameters structure as seen in @ref IO_Config.
 */
void IO_SetExtendedIT(GPIO_TypeDef* Port, IO_Config* PinStruct);

/**
 * @brief IO_ResetExtendedIT
 * - Clears the changes previously made to setup a given GPIO pin as "external interrupt" source
 * @arg Port is the port "base-address" (i. e. GPIOA, GPIOB, etc)
 * @arg PinStruct is the pin configuration parameters structure as seen in @ref IO_Config.
 */
void IO_ResetExtendedIT(GPIO_TypeDef* Port, IO_Config* PinStruct);

/**
 * @brief IO_MaskExtendedIT
 * - Disables a given GPIO pin configured as "external interrupt" source
 * @arg PinStruct is the pin configuration parameters structure as seen in @ref IO_Config.
 */
void IO_MaskExtendedIT(IO_Config* PinStruct);

/**
 * @brief IO_UnmaskExtendedIT
 * - Re-enable a given GPIO pin previously masked.
 * @arg PinStruct is the pin configuration parameters structure as seen in @ref IO_Config.
 */
void IO_UnmaskExtendedIT(IO_Config* PinStruct);

/**
 * @brief IO_ClearPendingExtendedIT
 * - Clears the "pending interrupt" flag for a given GPIO pin.
 * @arg PinStruct is the pin configuration parameters structure as seen in @ref IO_Config.
 */
void IO_ClearPendingExtendedIT(IO_Config* PinStruct);

/**
 * @brief IO_GetIrqNumber
 * - Returns the IRQn for the specified pin.
 * @arg Pin is the pin number (0 to 15).
 */
IRQn_Type IO_GetIrqNumber(uint32_t Pin);


/**
 * @brief IO_PinConfig
 * - Sets the pinout configuration for the specified peripheral.
 * This function is now deprecated and should never be used in the application.
 * @arg ConfigId is the Peripheral:Pinout encoded value.
 */
void IO_PinConfig(uint16_t ConfigId);

/**
 * @}
 */

#endif
//==============================================================================
