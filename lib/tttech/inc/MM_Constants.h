/**************************************************************************
 * Copyright (c) 2010 TTTControl. All rights reserved. Confidential proprietory
 * Schönbrunnerstraße 7, A-1040 Wien, Austria. office@ttcontrol.com
 **************************************************************************/
/**********************************************************************//**
 * \file MM_Constants.h
 *
 * \brief Global defines for minimodules
 *
 *      This header file defines the Error Codes for the minimodules
 *
 **************************************************************************/

#ifndef _MM_CONSTANTS_H
#define _MM_CONSTANTS_H

/**************************************************************************
 *
 * I N C L U D E S
 *
 **************************************************************************/

#include "ptypes_xe167.h"

/**************************************************************************
 *
 * D A T A   S T R U C T U R E
 *
 **************************************************************************/

/**************************************************************************
 *
 * D E F I N I T I O N S
 *
 **************************************************************************/

/*
 * M I N I M O D U L E   E R R O R   V A L U E S
 */

/**
 * \name Minimodule error values
 * \anchor mm_err_values_doc
 *
 * Errors codes that can be returned by minimodule functions
 */
/*@{*/
#define MM_E_Protocol_Timeout                   200 /**< a timeout in the minimodule communication has occured  */
#define MM_E_Protocol_NotFinished               201 /**< message has not yet been completely received           */
#define MM_E_Protocol_CRC                       202 /**< The CRC check of the received message failed           */
#define MM_E_Protocol_NoCommunication           203 /**< no data received as no command has been transmitted    */
#define MM_E_Protocol_TransmissionError         205 /**< message could not be transmitted.                      */
#define MM_E_Protocol_InvalidMessageSize        206 /**< message size was not correct.                          */
#define MM_E_Protocol_InvalidCommand            207 /**< message with invalid command was received.             */
/*@}*/



#endif /* _MM_CONSTANTS_H */
