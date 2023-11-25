//==============================================================================
/** @file GenericTypeDefs.h
 *  @brief Global type definitions.
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
#ifndef GENERICTYPEDEFS_H
    #define GENERICTYPEDEFS_H
    #include <stdint.h>
	#include <stdbool.h>

//------------------------------------------------------------------------------

#ifndef NULL
	#define NULL 		((void*)0)
#endif

/** @defgroup GenericTypeDefs
 * @{
 */

/**
 * @def __SYS_MAX_OBJECTS
 * - size of the notification table (total capacity of instantiated NComponents)
 */
#define __SYS_MAX_OBJECTS       32

/**
 * @def __SYS_MAX_SIGNALS
 * - size of the "high priority" notification queue (total capacity of pending events)
 */
#define __SYS_MAX_SIGNALS        3

//------------------------------------------------------------------------------
typedef uint32_t* 				HANDLE;
typedef const uint32_t 	    	NV_ID;
    
//------------------------------------------------------------------------------
/**
 * @struct NMESSAGE
 * This data structure is key for the event-driven concept. It is used throughout the
 * whole framework as the notification unit.
 *
 */
struct NMESSAGE{
    uint32_t message;	//!< the message "ID" (see @ref SysMessages.h)
    uint32_t data1;		//!< general purpose data field, usually NV_ID when calling InterruptCallback
    uint32_t data2;		//!< general purpose data field, context dependent (variable, register value, etc.)
	uint32_t tag;		//!< general purpose data field (sometimes used as index or counter)
};

/**
 * @}
 */
#endif
//------------------------------------------------------------------------------
