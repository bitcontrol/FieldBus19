/**
 *  - Filename:     FB19Def.h
 *  - Platform(s):  All
 *  - Project:      FieldBus19
 *  - Created:      Apr 28, 2021
 *  - Description:  This file defines FieldBus19 specific symbols.
 *                  DON'T CHANGE IT!
 *  - Notes:
 *  - Author:       Andreas Isenegger
 *  - Copyright:    2021-2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 *  @file
 *  @brief          This file defines FieldBus19 specific symbols.
 */

#ifndef FB19DEF_H_
#define FB19DEF_H_

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------------------------------------------------------
// Symbols and Macros

/* --- Booleans and Errors------------------------------- */
/** Since the type \c bool doesn't exist in C, it's defined here as \c int. */
#define BOOL                            int

/** Since the value \c false doesn't exist in C, it's defined here as \c 0. */
#define FALSE                           0

/** Since the value \c true doesn't exist in C, it's defined here as \c 1. */
#define TRUE                            1

/** Return value of a function in case of an error. */
#define R_ERROR                         -1

/** Return value of a function in case of success (no error). */
#define R_SUCCESS                       1


/* --- Bus Addresses and Nodes -------------------------- */
/**
 * Bus Controller address.
 */
#define FB19_BUS_ADDR_CTRL                  0

/**
 * Lowest Bus Subscriber address.
 */
#define FB19_BUS_ADDR_SUBS_LOWEST           1

/**
 * Highest Bus Subscriber address.
 */
#define FB19_BUS_ADDR_SUBS_HIGHEST          ((1 << FB19_DST_ADDR_BITS) - 1 - 1)

/**
 * Broadcast bus address.
 */
#define FB19_BUS_ADDR_BROADCAST             ((1 << FB19_DST_ADDR_BITS) - 1)

/**
 * Invalid bus address.
 * Used to indicate that for a given node identifier no bus address exists.
 */
#define FB19_BUS_ADDR_INVALID               (-1)

/**
 * Maximum number of Bus Nodes.
 * Per definition, a bus node can either be a Bus Controller or a Bus
 * Subscriber.<br>
 * The \c DstAddr field in a bus frame is 5 bits long, thereby providing 32
 * different bus addresses. However, \c DstAddr 31 is the broadcast address.
 * This makes 31 addresses available for Bus Nodes.
 */
#define FB19_NBR_NODES_MAX                  ((1 << FB19_DST_ADDR_BITS) - 1)

/**
 * Maximum number of Bus Subscribers.
 * \c DstAddr 0 is reserved for the Bus Controller, this reduces the available
 * addresses for Bus Subscribers by one.
 */
#define FB19_NBR_SUBSCRIBERS_MAX            (FB19_NBR_NODES_MAX - 1)


/* --- Bus Driver's Error Bits -------------------------- */
/**
 * No error.
 */
#define FB19_DRV_ERR_NONE                   0

/**
 * When this bit is set, the software's 'Rx listens Tx' feature sensed a
 * 'character collision'. This can be caused by strong electrical
 * distortion on the bus line, faulty hardware or because two or more bus
 * participants were transmitting characters simultaneously.<br>
 * Value: \c 0x01
 */
#define FB19_DRV_ERR_BUS_FRM_TX             (1 << 0)

/**
 * When this bit is set, the UART flagged a frame error because the Stop bit
 * was missing (UART framing error).<br>
 * Value: \c 0x02
 */
#define FB19_DRV_ERR_UART_RX_FRM            (1 << 1)

/**
 * When this bit is set, the UART flagged a noise error because during bit
 * sampling the bits weren't stable HIGH or LOW (UART noise error).<br>
 * Value: \c 0x04
 */
#define FB19_DRV_ERR_UART_RX_NSE            (1 << 2)

/**
 * When this bit is set, the UART flagged a buffer overrun error because the
 * receive buffer captured a new character before it was read at least once
 * (UART Rx buffer overrun error).<br>
 * Value: \c 0x08
 */
#define FB19_DRV_ERR_UART_RX_OVR            (1 << 3)

/**
 * Unsolicited traffic.
 * When this bit is set, there is unsolicited traffic or noise on the bus
 * line.<br>
 * Value: \c 0x10
 */
#define FB19_DRV_ERR_BUS_UNS_TRF            (1 << 4)

/**
 * When this bit is set, the calculated checksum of the received frame doesn't
 * correspond with the checksum it contains.<br>
 * Value: \c 0x20
 */
#define FB19_DRV_ERR_BUS_FRM_CKS            (1 << 5)

/**
 * Inter-character timeout.
 * When this bit is set, a Bus Subscriber sent less characters than
 * the \c DataSize field of the frame's header specified, or the characters
 * are too long apart in time.<br>
 * Value: \c 0x40
 */
#define FB19_DRV_ERR_BUS_ITR_CHR_TMO        (1 << 6)

/**
 * Response frame timeout.
 * When this bit is set, a Bus Subscriber didn't respond in time to the message
 * from the Bus Controller.<br>
 * Value: \c 0x80
 */
#define FB19_DRV_ERR_BUS_RSP_FRM_TMO        (1 << 7)

/** Highest error number of this driver module. */
#define FB19_DRV_ERR_HIGHEST                FB19_DRV_ERR_BUS_RSP_FRM_TMO

/* Throw a compile time error if the error symbols don't have the expected
 * values. Update the documentation if they change. */
#if (\
    FB19_DRV_ERR_NONE               != 0    || \
    FB19_DRV_ERR_BUS_FRM_TX         != 0x01 || \
    FB19_DRV_ERR_UART_RX_FRM        != 0x02 || \
    FB19_DRV_ERR_UART_RX_NSE        != 0x04 || \
    FB19_DRV_ERR_UART_RX_OVR        != 0x08 || \
    FB19_DRV_ERR_BUS_UNS_TRF        != 0x10 || \
    FB19_DRV_ERR_BUS_FRM_CKS        != 0x20 || \
    FB19_DRV_ERR_BUS_ITR_CHR_TMO    != 0x40 || \
    FB19_DRV_ERR_BUS_RSP_FRM_TMO    != 0x80 \
    )
#error
#endif

/* Throw a compile time error if the error symbols exceed 8 bits. */
#if (FB19_DRV_ERR_HIGHEST > (1 << 7))
#error
#endif


/* --- Data Sizes --------------------------------------- */
/**
 * Number of bits in the \c DataSize field of a bus frame.
 */
#define FB19_DATA_SIZE_BITS                 5

/**
 * Number of bits in the \c DstAddr field of a bus frame.
 */
#define FB19_DST_ADDR_BITS                  5

/**
 * Number of bits in the \c MsgId field of a bus frame.
 */
#define FB19_MSG_ID_BITS                    6


/* --- Frame Components and their Sizes ----------------- */
/**
 * Maximum payload size in bytes.
 */
#define FB19_FRAME_PAYLOAD_SIZE_MAX         ((1 << FB19_DATA_SIZE_BITS) - 1)

/**
 * Size of the frame header in bytes.
 * The header consists of (from highest to lowest bits):
 * - Destination Address: 5 bits
 * - Message Identifier: 6 bits
 * - Data Size: 5 bits
 */
#define FB19_FRAME_SIZE_HEADER              2

/**
 * Size of the frame trailer in bytes.
 * The trailer contains the checksum.
 */
#define FB19_FRAME_SIZE_TRAILER             2

/**
 * Maximum frame size in bytes.
 */
#define FB19_FRAME_SIZE_MAX                 (FB19_FRAME_SIZE_HEADER + \
                                             FB19_FRAME_PAYLOAD_SIZE_MAX + \
                                             FB19_FRAME_SIZE_TRAILER)
/**
 * Minimum frame size in bytes.
 */
#define FB19_FRAME_SIZE_MIN                 (FB19_FRAME_SIZE_HEADER + \
                                             FB19_FRAME_SIZE_TRAILER)
/**
 * Size of the frame in the RAM in bytes.
 * A stuff byte has been added for making its size divisible by 4.
 * Note that the module dprFB19Ctrl and possibly others are using the stuff
 * byte for internal purposes. If it was to be removed, fix the code and run
 * the correcponding tests.
 */
#define FB19_FRAME_SIZE_RAM                 (FB19_FRAME_SIZE_MAX + 1)


/* --- Message Identifiers ------------------------------ */
/**
 * Lowest FB19 system message identifier.
 */
#define FB19_MSG_ID_SYS_LOWEST              0

/**
 * Highest FB19 system message identifier.
 */
#define FB19_MSG_ID_SYS_HIGHEST             ((1 << FB19_MSG_ID_BITS) / 4 - 1)

/**
 * Lowest user defined message identifier.
 */
#define FB19_MSG_ID_USR_LOWEST              (FB19_MSG_ID_SYS_HIGHEST + 1)

/**
 * Highest user defined message identifier.
 */
#define FB19_MSG_ID_USR_HIGHEST             ((1 << FB19_MSG_ID_BITS) - 1)


/* --- Timeout Values ----------------------------------- */
/**
 * Duration of a character on the bus line in [us].
 * The \c bitRate is in [bit/s].<br>
 * The value contains 5% reserve for clock frequency differences between nodes.<br>
 * The \c +0.6f improves accuracy for high bit rates (roughly above 500kbit/s).<br>
 * This definition is accurate enough for bit rates up to 1Mbit/s. For higher
 * bit rates, it requires a redefinition, for example to nanosecond resolution.
 */
#define FB19_CHAR_TIME_US(bitRate)          ((unsigned int)(10500000.f / (bitRate) + 0.6f))

/**
 * Bus recovery timeout for Bus Controllers and Bus Subscribers in [number of
 * characters]. During this time, the bus nodes discard any received character.
 * However, a received character restarts this timeout.<br>
 * When the timeout has elapsed, Bus Controllers transmit the next frame, if
 * there is one in their Tx queue. Bus Subscribers will start processing incoming
 * characters (accept the next frame).<br>
 * Note that the timeout of Bus Controllers has to be slightly longer than
 * the timeout of of Bus Subscribers. Although there is this one timeout value
 * only, this condition is met because the Bus Controllers initiate their frame
 * transmission with an inter-frame timeout.
 */
#define FB19_BUS_RECO_TIMEOUT_CHAR_CNT      3

/**
 * Inter-character timeout in [number of characters].
 * The number is composed of the maximum inter-character idle time between a
 * character and its following character inside a frame plus the character to
 * be received that triggers the next UART's RXNE interrupt.
 */
#define FB19_INTER_CHAR_TIMEOUT_CHAR_CNT    3

/**
 * Inter-frame time between a request frame of a Bus Controller and a response
 * frame of a Bus Subscriber in [number of characters].
 */
#define FB19_INTER_FRAME_TIME_CHAR_CNT      1

/**
 * Response timeout in [number of characters].
 * The number is composed of the maximum inter-frame idle time between a
 * request frame from a Bus Controller and a response frame from a Bus
 * Subscriber plus the character to be received that triggers the UART's
 * RXNE interrupt.
 */
#define FB19_RESPONSE_TIMEOUT_CHAR_CNT      4


//------------------------------------------------------------------------------
// Types

//------------------------------------------------------------------------------
// Constants and Variables

//------------------------------------------------------------------------------
// Functions

#ifdef __cplusplus
}
#endif

#endif // FB19DEF_H_
