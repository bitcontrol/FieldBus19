/**
 *  Filename:       FB19LibMsg.h
 *  Platform(s):    All
 *  Project:        FieldBus19
 *  Created:        May 17, 2022
 *  Description:    This file publishes the interface of the FieldBus19
 *                  message library module.
 *
 *                  How to use the FB19LibMsg_append...() and
 *                  FB19LibMsg_remove...() functions effectively:
 *                  - Write data into the messages using the _append() functions
 *                  - Read data in the reverse order it has been written to
 *                    the messages
 *                  - That way, no indexes that keep track of the writes and
 *                    reads are required
 *  Notes:
 *  Author:         Andreas Isenegger
 *  Copyright:      2022-2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */

#ifndef FB19LIBMSG_H_
#define FB19LIBMSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> // For size_t

#include "FB19Msg.h"

//------------------------------------------------------------------------------
// Symbols and Macros

//------------------------------------------------------------------------------
// Types

//------------------------------------------------------------------------------
// Constants and Variables

//------------------------------------------------------------------------------
// Functions
/**
 * Appends an 8 bit character to the payload of the message and increments its
 * payload size by the size of the argument in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.

 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] value is the \c char value to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the value could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendChar(FB19Msg_t* msg, char value);

/**
 * Appends arbitrary data stored in the passed buffer to the payload of the
 * message and increments its payload size by the size of the data in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.

 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] buffer points to the buffer that holds the data that shall be
 * appended to the message.
 * @param[in] size is the size of the data in the buffer in bytes.
 * @return \c R_SUCCESS if the parameter checks pass and the data could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendData(FB19Msg_t* msg, const void* buffer, size_t size);

/**
 * Appends the passed 64 bit wide double precision floating point number to
 * the payload of the message and increments its payload size by the size of
 * the argument in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.

 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] value is the \c double value to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the value could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendDouble(FB19Msg_t* msg, double value);

/**
 * Appends the passed 32 bit wide single precision floating point number to
 * the payload of the message and increments its payload size by the size of
 * the argument in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.

 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] value is the \c float value to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the value could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendFloat(FB19Msg_t* msg, float value);

/**
 * Appends the passed 8 bit signed integer number to the payload of the message
 * and increments its payload size by the size of the argument in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.

 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] value is the \c int8_t value to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the value could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendInt8(FB19Msg_t* msg, int8_t value);

/**
 * Appends the passed 16 bit signed integer number to the payload of the message
 * and increments its payload size by the size of the argument in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.

 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] value is the \c int16_t value to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the value could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendInt16(FB19Msg_t* msg, int16_t value);

/**
 * Appends the passed 32 bit signed integer number to the payload of the message
 * and increments its payload size by the size of the argument in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.

 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] value is the \c int32_t value to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the value could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendInt32(FB19Msg_t* msg, int32_t value);

/**
 * Appends the passed 64 bit signed integer number to the payload of the message
 * and increments its payload size by the size of the argument in bytes.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.
 *
 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] value is the \c int64_t value to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the value could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendInt64(FB19Msg_t* msg, int64_t value);

/**
 * Appends the passed string terminated by a '\0' character to the payload of
 * the message including the '\0' character and increments its payload size
 * by the size of the string including its terminating '\0' character.
 * If the data doesn't fit into the message because its end would exceed the
 * end of the payload section, it won't be added and \c R_ERROR is returned.
 *
 * The string is appended in reverse byte order to the payload buffer ('\0'
 * character is inserted first). That way it can be removed from the payload
 * later (see _removeString()) without having to specify its length.
 *
 * @param[in/out] msg points to the message that will store the passed argument
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @param[in] str is the string to be appended to the message.
 * @return \c R_SUCCESS if the parameter checks pass and the string could be
 * appended, \c R_ERROR otherwise.
 */
int FB19LibMsg_appendString(FB19Msg_t* msg, const char* str);

/**
 * Removes an 8 bit character from the end of the payload and decreases its
 * size by the size of the returned value in bytes.
 * If the data couldn't be retrieved because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the \c char value will be
 * removed.
 * @param[out] value points to the variable that will hold the \c char value
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeChar(FB19Msg_t* msg, char* value);

/**
 * Removes arbitrary data from the end of the payload and decreases its size
 * by the size of the returned data in bytes.
 * If the data couldn't be removed because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 * The caller has to make sure the passed buffer is large enough to store
 * the returned data. This is exactly \c size number of bytes, or 0 bytes
 * in case of an error.
 *
 * @param [in] msg points to the message from which the \c data will be
 * removed.
 * @param[out] buffer points to the variable that will hold the data when
 * this function has returned successfully. In case of an error, it won't be
 * changed.
 * @param[in] size is the number of bytes that shall be removed from the
 * message. If it doesn't contain the requested amount of payload data,
 * the output variable won't be changed and the function returns \c R_ERROR.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeData(FB19Msg_t* msg, void* buffer, size_t size);

/**
 * Removes a 64 bit wide double precision floating point number from the end
 * of the payload and decreases its size by the size of the returned value
 * in bytes.
 * If the data couldn't be retrieved because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the \c double value will be
 * removed.
 * @param[out] value points to the variable that will hold the \c double value
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeDouble(FB19Msg_t* msg, double* value);

/**
 * Removes a 32 bit wide single precision floating point number from the end
 * of the payload and decreases its size by the size of the returned value
 * in bytes.
 * If the data couldn't be retrieved because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the \c float value will be
 * removed.
 * @param[out] value points to the variable that will hold the \c float value
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeFloat(FB19Msg_t* msg, float* value);

/**
 * Removes a signed 8 bit integer number from the end of the payload and
 * decreases its size by the size of the returned value in bytes.
 * If the data couldn't be removed because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the \c int8_t value will
 * be removed.
 * @param[out] value points to the variable that will hold the \c int8_t value
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeInt8(FB19Msg_t* msg, int8_t* value);

/**
 * Removes a signed 16 bit integer number from the end of the payload and
 * decreases its size by the size of the returned value in bytes.
 * If the data couldn't be removed because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the \c int16_t value will be
 * removed.
 * @param[out] value points to the variable that will hold the \c int16_t value
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeInt16(FB19Msg_t* msg, int16_t* value);

/**
 * Removes a signed 32 bit integer number from the end of the payload and
 * decreases its size by the size of the returned value in bytes.
 * If the data couldn't be removed because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the \c int32_t value will be
 * removed.
 * @param[out] value points to the variable that will hold the \c int32_t value
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeInt32(FB19Msg_t* msg, int32_t* value);

/**
 * Removes a signed 64 bit integer number from the end of the payload and
 * decreases its size by the size of the returned value in bytes.
 * If the data couldn't be removed because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the \c int64_t value will be
 * removed.
 * @param[out] value points to the variable that will hold the \c int64_t value
 * when this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeInt64(FB19Msg_t* msg, int64_t* value);

/**
 * Removes a string terminated by a '\0' character from the end of the payload
 * and decreases its size by the size of the returned string (including its '\0'
 * character) in bytes.
 * If the data couldn't be removed because its beginning would exceed the
 * beginning of the payload section, it won't be removed and \c R_ERROR is
 * returned.
 *
 * @param [in] msg points to the message from which the string will be
 * removed.
 * @param[out] str points to the variable that will hold the null-terminated
 * string when this function has returned successfully. In case of an error,
 * it won't be changed.
 * @param[in] bufLen is the length of the buffer in bytes that will hold the
 * null-terminated string. If it is too small, the output variable won't be
 * changed and the function returns \c R_ERROR.
 * @return \c R_SUCCESS if the parameter checks pass and the argument could be
 * removed, \c R_ERROR otherwise.
 */
int FB19LibMsg_removeString(FB19Msg_t* msg, char* str, size_t bufLen);

/**
 * Resets the internal elements of the message to an empty message.
 * Note that the data in the frame (header, payload, trailer) is NOT set to 0.
 *
 * @param[in/out] msg points to the message that will be reset. In case of
 * an error, it won't be changed.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19LibMsg_reset(FB19Msg_t* msg);

#ifdef __cplusplus
}
#endif

#endif // FB19LIBMSG_H_
