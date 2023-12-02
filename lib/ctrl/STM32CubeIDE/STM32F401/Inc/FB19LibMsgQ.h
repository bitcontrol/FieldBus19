/**
 *  - Filename:     FB19LibMsgQ.h
 *  - Platform(s):  All
 *  - Project:      FieldBus19
 *  - Created:      May 19, 2022
 *  - Description:  This file publishes the interface of a FieldBus19 library
 *                  that encapsulates a specific type of message queue.
 *
 *                  It is used by:
 *                  - Bus Controllers as 'low frequency readers queue' in the
 *                    FB19DprCtrl module
 *                  - Bus Subscribers as transmit queue in the FB19DrvSubs
 *                    module
 *                  It is basically a ring buffer that on write overwrites
 *                  the oldest element when full and returns an error on read
 *                  when empty.<br>
 *                  During a read call, the \c msg parameter is both an input
 *                  and output parameter. It is initially an input parameter
 *                  since the <dstId> and <msgId> fields in the object pointed
 *                  to by \c msg* specify what message type shall be returned.
 *                  A read returns an error if it specifies a message whose
 *                  <dstId> AND <msgId> aren't found in the queue.<br>
 *                  Additionally, when the queue stores multiple messages with
 *                  identical <dstId> and <msgId> fields, the oldest of those
 *                  messages is returned on a read (and not simply the oldest
 *                  message in the queue).<br>
 *  - Notes:        This module has been designed for multi-instance usage.<br>
 *                  That scenario comes into play when a Controller and one or
 *                  more Subscribers are implemented in one single application.
 *                  Examples are test applications or connected FB19 bus systems
 *                  that exchange messages via a Bus Controller / Bus Subscriber
 *                  application.<br>
 *                  For achieving multi-instance usage this module doesn't
 *                  contain the message queue. It has to be passed at every
 *                  function call instead. That way, the module is stateless
 *                  and can be used by several clients simultaneously.
 *  - Author:       Andreas Isenegger
 *  - Copyright:    2022-2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 *  @file
 *  @brief          This file publishes the interface of a FieldBus19 library
 *                  that encapsulates a specific type of message queue.
 */

#ifndef FB19LIBMSGQ_H_
#define FB19LIBMSGQ_H_

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
 * The message queue stores this type of data.
 */
typedef struct FB19LibMsgQ_element_struct
{
    /**
     * The age of the message in the queue.
     * The higher this number, the older the message. A '0' means 'this element
     * is empty', or in other words 'this message has been read'.
     */
    short age;

    /** A FieldBus19 message in the queue. */
    FB19Msg_t msg;
} FB19LibMsgQ_element_t;

/**
 * This data type defines a message queue.
 */
typedef struct FB19LibMsgQ_queue_struct
{
    /** A pointer to the array that stores the elements of the queue. */
    FB19LibMsgQ_element_t* array;

    /** The capacity of the queue (length of the array). */
    const short capacity;
} FB19LibMsgQ_queue_t;

//------------------------------------------------------------------------------
// Constants and Variables

//------------------------------------------------------------------------------
// Functions
/**
 * Initializes the elements of the passed queue.
 *
 * Call it at least once before using a queue object. It clears the memory of
 * the internal elements to \c 0.<br>
 * Don't modify the passed \c queue object outside the \c _read() or \c _write()
 * functions of this module!
 *
 * @param[in] queue is the message queue that shall be initialized.
 * @return \c R_SUCCESS if the message queue could be initialized, \c R_ERROR
 * otherwise.
 */
int FB19LibMsgQ_init(FB19LibMsgQ_queue_t queue);

/**
 * Copies the oldest unread message that matches the destination address and
 * message identifier of the message pointed to by the \c msg parameter from
 * the queue into the passed message, thereby removing the message from the
 * queue.
 *
 * @param[in] queue is the message queue that shall be read from.
 * @param[in/out] msg points to the message that will hold the result when this
 * function has returned successfully. If no matching message could be found the
 * message \c msg points to won't be changed.
 * @return \c R_SUCCESS if the parameter checks pass and an unread message that
 * matches the identifiers in \c msg->dscr could be found, \c R_ERROR otherwise.
 */
int FB19LibMsgQ_read(FB19LibMsgQ_queue_t queue, FB19Msg_t* msg);

/**
 * Copies the message pointed to by the \c msg parameter to the queue. If the
 * queue is already full, the oldest message is overwritten.
 *
 * @param[in] queue is the message queue that shall be written to.
 * @param[in] msg points to the message to be queued.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19LibMsgQ_write(FB19LibMsgQ_queue_t queue, const FB19Msg_t* msg);

#ifdef __cplusplus
}
#endif

#endif // FB19LIBMSGQ_H_
