/**
 *  Filename:       FB19MsgQ.h
 *  Platform(s):    All
 *  Project:        FieldBus19
 *  Created:        Apr 21, 2023
 *  Description:    This file publishes the interface of the FieldBus19 message
 *                  queue.
 *  Notes:          DON'T CHANGE IT!
 *  Author:         Andreas Isenegger
 *  Copyright:      2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */

#ifndef FB19MSGQ_H_
#define FB19MSGQ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "FB19Def.h"
#include "FB19Msg.h"

//------------------------------------------------------------------------------
// Symbols and Macros

//------------------------------------------------------------------------------
// Types
/**
 * FieldBus19 message queue type.
 */
typedef struct FB19MsgQ_struct
{
    /** A pointer to the array that stores the elements of the queue. */
    FB19Msg_t* array;

    /** The capacity of the queue (number of messages in the array). */
    const short capacity;
} FB19MsgQ_t;

//------------------------------------------------------------------------------
// Constants and Variables

//------------------------------------------------------------------------------
// Functions
/**
 * This function is used to initialize a message queue.
 * All messages in the queue will be be initialized with \c 0, including ALL
 * their internal data structures.
 * It is no error, if the capacity of the queue is 0; in that case no
 * operation will be performed on the queue. If the capacity is a negative
 * number however, an error will be returned.
 *
 * @param[in] msg points to the message queue that shall be initialized.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19MsgQ_init(FB19MsgQ_t* queue);

#ifdef __cplusplus
}
#endif

#endif // FB19MSGQ_H_
