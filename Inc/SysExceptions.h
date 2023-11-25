//==============================================================================
/**
 * @file SysExceptions.h
 * @brief EDROS - System exceptions definitions
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
#ifndef SYSEXCEPTIONS_H
    #define SYSEXCEPTIONS_H

/**
 * @defgroup SysExceptions
 * @{
 */

    //--------------------------------------------------------------------------
    #define NX_UNKNOWN                          ((uint32_t) 0x00000000)
    #define NX_HARDFAULT                        ((uint32_t) 0x00000001)

    #define NX_PARAMETER_NULL                   ((uint32_t) 0x0000000D)
    #define NX_PARAMETER_INVALID                ((uint32_t) 0x0000000E)
    #define NX_CONSTRUCTOR_PARAMETER_INVALID    ((uint32_t) 0x0000000F)
    
    #define NX_DRVDMA_NO_STREAM_AVAILABLE       ((uint32_t) 0x00000030)

    #define NX_PROPERTY_VALUE_INVALID           ((uint32_t) 0x00000100)
    #define NX_PROPERTY_VALUE_OUT_OF_RANGE      ((uint32_t) 0x00000101)
    #define NX_PROPERTY_VALUE_SET_TO_DEFAULT    ((uint32_t) 0x00000102)
    
    #define NX_NPWM_PRESCALER_INVALID           ((uint32_t) 0x00000401)
    
    #define NX_LPTIMER_PRESCALER_INVALID        ((uint32_t) 0x00000431)
    
    #define NX_NSERIAL_BAUDRATE_UNREACHABLE     ((uint32_t) 0x00000481)
    #define NX_NSERIAL_TXDMA_VECTOR_NULL        ((uint32_t) 0x00000482)
    #define NX_NSERIAL_RXDMA_VECTOR_NULL        ((uint32_t) 0x00000483)
    
    
    #define NX_NADC_NO_CHANNEL_SELECTED         ((uint32_t) 0x00000510)
    #define NX_NADC_NO_TIMER_ALLOCATION         ((uint32_t) 0x00000511)
    #define NX_NADC_UPPERLIMIT_INVALID          ((uint32_t) 0x00000512)
    #define NX_NADC_LOWERLIMIT_INVALID          ((uint32_t) 0x00000513)
    #define NX_NADC_DISABLED_CLOCKCHANGE        ((uint32_t) 0x00000514)

/*
 * @}
 */
#endif 

//============================================================================//
