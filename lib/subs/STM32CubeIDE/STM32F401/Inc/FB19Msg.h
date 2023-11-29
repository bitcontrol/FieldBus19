/**
 *  Filename:       FB19Msg.h
 *  Platform(s):    All
 *  Project:        FieldBus19
 *  Created:        Apr 21, 2023
 *  Description:    This file defines FieldBus19 message specific types.
 *  Notes:          DON'T CHANGE IT!
 *  Author:         Andreas Isenegger
 *  Copyright:      2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */

#ifndef FB19MSG_H_
#define FB19MSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> // For uint8_t et al

#include "FB19Def.h" // For FB19_FRAME_SIZE_RAM

//------------------------------------------------------------------------------
// Symbols and Macros

//------------------------------------------------------------------------------
// Types
/**
 * FieldBus19 frame buffer type.
 * It holds the data that goes to or comes from the bus.
 */
typedef uint8_t FB19FrmBuf_t[FB19_FRAME_SIZE_RAM];

/**
 * Frame descriptor that stores information about the FieldBus19 frame.
 */
typedef struct FB19frmDscr_struct
{
    uint8_t size;   /**< Size of the frame in bytes (number of characters on the bus). */
    uint8_t sts;    /**< Stores the \c frameState_t of this frame. */
} FB19FrmDscr_t;

/**
 * FieldBus19 frame type.
 */
typedef struct FB19frm_struct
{
    FB19FrmBuf_t buf;   /**< Frame buffer. */
    FB19FrmDscr_t dscr; /**< Frame descriptor. */
} FB19Frm_t;

/**
 * Message descriptor that stores information about the FieldBus19 message.
 */
typedef struct FB19msgDscr_struct
{
    uint8_t dstAddr;    /**< Destination address. */
    uint8_t errSts;     /**< Low-level frame error status bits. */
    uint8_t msgId;      /**< Message identifier. */
    uint8_t pldSize;    /**< Size of the payload data in bytes. */
} FB19MsgDscr_t;

/**
 * FieldBus19 message type.
 */
typedef struct FB19msg_struct
{
    FB19Frm_t frm;      /**< Bus frame. */
    FB19MsgDscr_t dscr; /**< Message descriptor. */
} FB19Msg_t;

//------------------------------------------------------------------------------
// Constants and Variables

//------------------------------------------------------------------------------
// Functions

#ifdef __cplusplus
}
#endif

#endif // FB19MSG_H_
