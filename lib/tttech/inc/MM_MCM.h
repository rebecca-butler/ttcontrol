/**************************************************************************
 * Copyright (c) 2010 TTTControl. All rights reserved. Confidential proprietory
 * Schönbrunnerstraße 7, A-1040 Wien, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file MM_MCM.h
 *
 * \brief Minimodule driver for Current Measurement
 *
 *      This driver provides functions for reading the current feedback
 *      channels of a mini module.
 *
 **************************************************************************/
#ifndef _MM_MCM_H
#define _MM_MCM_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "IO_Driver.h"

/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/**
 * \name Channels of Minimodule for Current Measurement
 * \anchor mm_mmc_channels
 *
 * Channel definitions for the minimodule for current measurement
 */
/*@{*/
#define IO_MCM_CHANNEL_00   0
#define IO_MCM_CHANNEL_01   1
#define IO_MCM_CHANNEL_02   2
#define IO_MCM_CHANNEL_03   3

/*@}*/
#define IO_MCM_CHANNEL_MAX  4

/**
 * \name Channel configuration of Minimodule for Current Measurement
 * \anchor mm_mmc_channel_config
 *
 * Channel configurations for the minimodule for current measurement
 */
/*@{*/
#define IO_MCM_NOT_CONFIGURED   0
#define IO_MCM_CONFIG_CURRENT   1
#define IO_MCM_CONFIG_DO        2
/*@}*/


/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**************************************************************************
 *
 * F U N C T I O N S
 *
 **************************************************************************/

/**********************************************************************//**
 *
 * \brief Init function for minimodule for current measurement
 *
 *   This function initializes a current measurement channel
 *   of the minimodule for current measurement.
 *
 *
 * \param mm_channel        Channel to be initialized, one of:
 *                              - IO_MCM_CHANNEL_00
 *                              - IO_MCM_CHANNEL_01
 *                              - IO_MCM_CHANNEL_02
 *                              - IO_MCM_CHANNEL_03
 * \param config            Function of MM channel:
 *                              - IO_MCM_CONFIG_CURRENT: Current measurement
 *                              - IO_MCM_CONFIG_DO:      Digital output
 *
 * \return IO_ErrorType
 * \retval IO_E_OK                  everything fine
 * \retval IO_E_INVALID_PARAMETER   parameter is out of range
 * \retval IO_E_CHANNEL_BUSY        channel has already been set up
 *
 ***************************************************************************
 *
 * \remarks
 *   The configuration "digital output" (IO_MCM_CONFIG_DO) is available
 *   for MCM firmware versions 0.6.0 and higher.
 *   The MCM firmware version can be retrieved with the function
 *   MM_MCM_GetVersionNumber(..)
 *
 * \remarks
 *   If a MCM firmware version lower than 0.6.0 is used and at least
 *   one channel is configured as digital output, the current measurement
 *   functionality will not work on any of the minimodule pins.
 *
 **************************************************************************/
 IO_ErrorType MM_MCM_ChannelInit( ubyte1 mm_channel
                                , ubyte1 config );

 /**********************************************************************//**
  *
  * \brief DeInit function for minimodule for current measurement
  *
  *   This function deinitializes a current measurement channel
  *   of the minimodule for current measurement.
  *
  *
  * \param mm_channel        Channel to be initialized, one of:
  *                              - IO_MCM_CHANNEL_00
  *                              - IO_MCM_CHANNEL_01
  *                              - IO_MCM_CHANNEL_02
  *                              - IO_MCM_CHANNEL_03
  *
  * \return IO_ErrorType
  * \retval IO_E_OK                       everything fine
  * \retval IO_E_INVALID_PARAMETER        parameter is out of range
  * \retval IO_E_CHANNEL_NOT_CONFIGURED   the channel has not been configured
  *
  **************************************************************************/
 IO_ErrorType MM_MCM_ChannelDeInit( ubyte1 mm_channel );

 /**********************************************************************//**
  *
  * \brief Read Current Function
  *
  *   Returns the current value for the given channel
  *
  *
  * \param mm_channel        Channel to be initialized, one of:
  *                              - IO_MCM_CHANNEL_00
  *                              - IO_MCM_CHANNEL_01
  *                              - IO_MCM_CHANNEL_02
  *                              - IO_MCM_CHANNEL_03
  * \param curr_value        pointer to the returned current value
  * \param fresh             indicates whether the returned value is valid
  *
  * \return IO_ErrorType
  * \retval IO_E_OK                       everything fine
  * \retval IO_E_INVALID_PARAMETER        parameter is out of range
  * \retval IO_E_CHANNEL_NOT_CONFIGURED   the channel has not been configured
  * \retval IO_E_NULL_POINTER             a null pointer has been passed
  * \retval IO_E_FET_PROTECTION           FET is disabled, protection is active
  * \retval MM_E_Protocol_NotFinished     Data transfer to minimodule has not yet been finished
  * \retval MM_E_Protocol_Timeout         Timeout in minimodule communication
  * \retval MM_E_Protocol_CRC             Wrong CRC in message from minimodule
  *
  **************************************************************************/
 IO_ErrorType MM_MCM_GetCurr( ubyte1 mm_channel
                            , ubyte2 * const curr_value
                            , bool * const fresh );

 /**********************************************************************//**
  *
  * \brief Set digital output
  *
  *   Sets the state of a minimodule channel which is configured as digital output
  *
  *
  * \param mm_channel        Channel to be initialized, one of:
  *                              - IO_MCM_CHANNEL_00
  *                              - IO_MCM_CHANNEL_01
  *                              - IO_MCM_CHANNEL_02
  *                              - IO_MCM_CHANNEL_03
  * \param state             TRUE to switch on the output, FALSE to switch it off
  *
  * \return IO_ErrorType
  * \retval IO_E_OK                       everything fine
  * \retval IO_E_INVALID_PARAMETER        parameter is out of range
  * \retval IO_E_CHANNEL_NOT_CONFIGURED   the channel has not been configured
  * \retval IO_E_DO_OPEN_LOAD             open load is detected on the output
  * \retval IO_E_DO_SHORT_CIRCUIT         the output current is too high
  * \retval IO_E_FET_PROTECTION           FET is disabled, protection is active
  * \retval MM_E_Protocol_NotFinished     Data transfer to minimodule has not yet been finished
  * \retval MM_E_Protocol_Timeout         Timeout in minimodule communication
  * \retval MM_E_Protocol_CRC             Wrong CRC in message from minimodule
  *
  **************************************************************************/
 IO_ErrorType MM_MCM_SetOutput( ubyte1 mm_channel
                              , bool state );

/**********************************************************************//**
 *
 * \brief Task function for minimodule for current measurement
 *
 *   This task function shall be called every SW cycle.
 *   It handles the communication with the minimodule,
 *   transmits configuration and reads the results
 *
 *
 * \return IO_ErrorType
 * \retval IO_E_OK                  everything fine
 *
 *
 **************************************************************************/
 IO_ErrorType MM_MCM_Task( void );

/**********************************************************************//**
 *
 * \brief Get the SW version number of the minimodule
 *
 *   Retuns the version number of the minimodule.
 *   Version number 0.0.0 means that the version was not yet retrieved
 *   from the minimodule.
 *
 *
 * \param major          Major version
 * \param minor          Minor version
 * \param patchlevel     Patchlevel
 *
 * \return IO_ErrorType
 * \retval IO_E_OK                  everything fine
 *
 *
 **************************************************************************/
 IO_ErrorType MM_MCM_GetVersionNumber( ubyte1 * const major
                                     , ubyte1 * const minor
                                     , ubyte1 * const patchlevel);

#endif /* _MM_MCM_H */
