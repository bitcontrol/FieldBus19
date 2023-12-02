/**
 *  - Filename:     FB19Ctrl.h
 *  - Platform(s):  All
 *  - Project:      FB19CtrlLib
 *  - Created:      May 23, 2023
 *  - Description:  This file publishes the interface of the FieldBus19
 *                  Controller library.
 *
 *                  A note on the parameter \c instance:
 *                  The parameter \c instance can be thought of as a 'handle'
 *                  to the Controller instance. It is the start address of
 *                  the memory area, where the instance stores its internal
 *                  values (buffers, counters, pointers, ...).
 *
 *                  It doesn't matter for this library if the memory has been
 *                  allocated statically, dynamically or on the stack. Just make
 *                  sure it's not deallocated before the FB19 Controller is
 *                  stopped by a call to FB19Ctrl_stop(), or the behavior of
 *                  the Controller will be undefined.<br>
 *                  That way, this library doesn't have to allocate memory
 *                  internally; all required memory space is passed from the
 *                  outside.
 *
 *                  Limitation of the bit rate:<br>
 *                  This version of the FB19 Controller library is limited to
 *                  a maximum bit rate of 250kbit/s, although many of STM's
 *                  microcontroller families could run the bus at considerably
 *                  higher rates.
 *
 *                  Code size and stack requirements:<br>
 *                  This library was designed with the economic use of resources
 *                  in mind. It occupies little FLASH memory and uses the stack
 *                  economically. Please consult the output of your linker and
 *                  debugger for detailed and accurate information.
 *
 *  - Notes:        DON'T CHANGE EXISTING CODE!
 *  - Author:       Andreas Isenegger
 *  - Copyright:    2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 *  @file
 *  @brief          This file publishes the interface of the FieldBus19
 *                  Controller library.
 */

#ifndef FB19CTRL_H_
#define FB19CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h> // For uint8_t et al
#include <stm32f4xx.h> // For TIM_TypeDef et al

#include "FB19Def.h"
#include "FB19LibMsgQ.h"
#include "FB19Msg.h"
#include "FB19MsgQ.h"

//------------------------------------------------------------------------------
// Symbols and Macros
/** Maximum bit rate in bit/s. */
#define FB19_CTRL_BIT_RATE_MAX          250000

/** Minimum bit rate in bit/s. */
#define FB19_CTRL_BIT_RATE_MIN          2000

/**
 * The size of the memory in bytes required to run an FB19 Controller instance.

 * Note that this number doesn't include the sizes of the four message queues
 * that have to be allocated outside this library. They are passed via pointers
 * in the \c FB19Ctrl_cfg_t configuration type.
 */
#define FB19_CTRL_MEM_SIZE              224

//------------------------------------------------------------------------------
// Types
/** FB19 Controller configuration structure. */
typedef struct FB19Ctrl_cfg_struct
{
    /* FB19 Controller Data Preparation Module. *******************************/
    struct {
        /**
         * A pointer to the Low Frequency Reader message queue used by this
         * instance.<br>
         * Typical capacity: 4 messages
         */
        FB19LibMsgQ_queue_t* lfrMsgQ;

        /**
         * A pointer to the receive message queue used by this instance.<br>
         * Typical capacity: 8 messages
         */
        FB19MsgQ_t* rxMsgQ;

        /**
         * A pointer to the transmit message queue used by this instance.<br>
         * Typical capacity: 8 messages
         */
        FB19MsgQ_t* txMsgQ;
    } FB19DprCtrl;

    /* FB19 Controller Driver Module. *****************************************/
    struct {
        /**
         * This instance's pointer to its related GPIO driver resource that
         * drives the Driver Output Enable signal to the RS-485 transceiver.
         */
        GPIO_TypeDef* GPIO;

        /**
         * Driver Output Enable signal's bit number on the dedicated GPIO port
         * that controls the driver's output on the RS-485 transceiver.
         * Its range is 0 to 15.
         */
        uint16_t GPIO_portBitDOE;

        /**
         * A pointer to the transmit/receive message queue used by this
         * instance.
         *
         * Note that in the current implementation there is only one queue for
         * transmission and reception, since after the transmission of a frame
         * its buffer is of no use anymore and can therefore be reused for the
         * reception of the response frame.<br>
         * Typical capacity: 8 messages
         */
        FB19MsgQ_t* txRxMsgQ;
    } FB19DrvCtrl;

    /* Timer Driver Module. ***************************************************/
    struct {
        /** Interrupt channel number. */
        IRQn_Type NVIC_IRQ_channel; // 32 bits wide

        /** Interrupt group priority. */
        uint8_t NVIC_IRQ_groupPriority;

        /** Interrupt's sub-priority in group. */
        uint8_t NVIC_IRQ_subPriority;

        /** Bit mask to enable the clock to the timer. */
        uint32_t RCC_clockEnableBitMask;

        /** Dedicated RCC APBx peripheral clock enable register. */
        volatile uint32_t* RCC_clockEnableRegister;

        /** Bit mask to reset the timer. */
        uint32_t RCC_resetBitMask;

        /** Dedicated RCC APBx peripheral reset register. */
        volatile uint32_t* RCC_resetRegister;

        /** Width of the ARR and CCRx registers in bits (16 or 32). */
        int16_t  registerWidth;

        /** Pointer to the CMSIS Timer structure. */
        TIM_TypeDef* TIMx;
    } FB19DrvTimer;

    /* UART Driver Module. ****************************************************/
    struct {
        /**
         * Pointer to the CMSIS GPIO structure that hosts the UART's transmit
         * and receive ports.
         */
        GPIO_TypeDef* GPIO;

        /** Alternate function selection code for the dedicated port pins. */
        uint16_t GPIO_altFunctionSelection;

        /** Receiver's bit number on the dedicated GPIO port. Its range is 0 to 15. */
        uint16_t GPIO_portBitRx;

        /** Transmitter's bit number on the dedicated GPIO port. Its range is 0 to 15. */
        uint16_t GPIO_portBitTx;

        /** Interrupt channel number. */
        IRQn_Type NVIC_IRQ_channel;

        /** Interrupt group priority. */
        uint8_t NVIC_IRQ_groupPriority;

        /** Interrupt's sub-priority in group */
        uint8_t NVIC_IRQ_subPriority;

        /** Bit mask to enable the clock to the USART. */
        uint32_t RCC_clockEnableBitMask;

        /** Dedicated RCC APBx peripheral clock enable register. */
        volatile uint32_t* RCC_clockEnableRegister;

        /** Pointer to the CMSIS USART structure. */
        USART_TypeDef* USARTx;
    } FB19DrvUART;
} FB19Ctrl_cfg_t;

/** FB19 Controller memory and instance type. */
typedef uint8_t FB19Ctrl_inst_t[FB19_CTRL_MEM_SIZE];

//------------------------------------------------------------------------------
// Constants and Variables

//------------------------------------------------------------------------------
// Functions
/**
 * Starts an FB19 Controller instance.
 *
 * The parameter \c config contains system on chip configuration parameters
 * as well as transmit and receive message buffers. It is left to the user to
 * define how large the buffers shall be. Systems with high message rate peaks
 * and low CPU power require larger buffers than those with steady data
 * exchange.
 *
 * Starting an instance twice without stopping it in between returns an error
 * and stops the already running instance.
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[in] config points to the structure that configures this instance.
 * It contains system on chip configuration parameters as well as pointers to
 * the transmit and receive message buffers. It is left to the user to define
 * how large the buffers shall be. For example, systems with high message rate
 * peaks and low CPU power require larger buffers than those with steady data
 * exchange.
 * @param[in] bitRate is the number of bits per second during character
 * reception and transmission.
 * @return \c R_SUCCESS if the parameter checks pass, the module instance hasn't
 * been started already and could be started with the passed configuration,
 * \c R_ERROR otherwise.
 */
int FB19Ctrl_start(FB19Ctrl_inst_t* instance,
    const FB19Ctrl_cfg_t* config,
    int bitRate);

/**
 * Stops an FB19 Controller instance.
 *
 * Stopping an instance twice without starting it in between returns no error.
 *
 * Note that a call to this function will neither stop the clock to the
 * configured GPIO nor CRC SoC modules. These modules could be used by other
 * components of the application.
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19Ctrl_stop(FB19Ctrl_inst_t* instance);

/**
 * Executes the periodic tasks of this library.
 *
 * It essentially moves unread messages that came in since the previous \c
 * _handler() call aside, fetches the received messages from the driver layer
 * and copies them to the next higher layer. It finally copies the messages
 * to be transmitted to the driver layer.
 *
 * Adjust the period between the calls to the number of messages to be sent
 * and received between two calls, their average size and the bit rate the bus
 * is operating on.<br>
 * For example, if up to 8 messages should be sent and received, their average
 * payload size is 16 bytes, and the bit rate is 100kbit/s, then the period
 * should be at minimum:<br>
 * T = 8 msg * 2 * (16 + 4 + 2) byte/msg * 10 bit/byte * (1 / 100'000 bit/s)
 * = 35.2ms.<br>
 * Explanation:<br>
 * - 8 msg: 8 request messages between 2 \c _handler() calls
 * - * 2: 8 response messages to the request messages
 * - (16 + 4 + 2): payload bytes + system bytes + inter frame spacing times
 * - 10 bit/byte: on the bus, a byte is a character with start and stop bits
 * - 1 / 100'1000 bit/s: the time it takes to transmit 1 bit
 *
 * It is recommended to round this number up by 10% for taking potential
 * frame timeouts into account. Thus, in this example a handler period of
 * T = 40ms or 25Hz is recommended.
 *
 * Moving unread messages to a different buffer prevents 'message loss'
 * when frequent message writer and reader objects (classes, modules, tasks)
 * compete with infrequent writer and reader objects on the available space
 * in the receive message queue. The frequent writers/readers would overwrite
 * (delete) messages of the infrequent writers/readers, since the latter may
 * pick up their response messages just every 2nd, 3rd or xth \c _handler()
 * call.
 *
 * It does nothing if \c instance is a NULL pointer or if the library
 * hasn't been started yet.
 *
 * @param[in] instance points to the memory resource used by this instance.
 */
void FB19Ctrl_handler(FB19Ctrl_inst_t* instance);

/**
 * Deep copies the oldest unread message that matches the passed destination
 * address \c dstAddr and message identifier \c msgId into the message the
 * \c msg parameter points to.
 *
 * In the receive message queue, the message is thereby marked as consumed.
 * It cannot be read a second time.
 *
 * When at the time the request message was sent to the subscriber its response
 * has timed out, the error flag \c FB19_DRV_ERR_BUS_RSP_FRM_TMO inside the
 * message will be set. Note that a timeout only occurs if the connection to
 * the addressed subscriber is interrupted or the subscriber malfunctions.
 * If the subscriber functions properly but had no response message ready when
 * it received this request message, it simply returns the request message
 * unchanged.
 *
 * When the function returns \c R_ERROR because either no matching message could
 * be found, or the parameter checks fail, the passed message \c msg points to
 * won't be changed.
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[out] msg points to the object that will hold the fetched message
 * after this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and an unread message that
 * matches the destination address and message identifier in the descriptor of
 * the message the \c msg parameter points to could be found, \c R_ERROR
 * otherwise.
 */
int FB19Ctrl_fetch(FB19Ctrl_inst_t* instance, FB19Msg_t* msg);

/**
 * Returns the number of noise faults since the start of this Controller
 * instance.
 *
 * Every noise detection error increases the fault count by one. If more than
 * one noise spike occurred during the time of a character, it is stored as one
 * fault (not several).
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[out] count points to the variable that will hold the number of
 * noise faults since the start of this module. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19Ctrl_getNoiseFaultCount(FB19Ctrl_inst_t* instance,
    uint32_t* count);

/**
 * Returns the number of receive faults since the start of this Controller
 * instance.
 *
 * Every frame reception error increases the fault count by one. If more than
 * one error occurred during the reception of a frame, it is stored as one
 * fault (not several).
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[out] count points to the variable that will hold the number of
 * receive faults since the start of this module. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19Ctrl_getRxFaultCount(FB19Ctrl_inst_t* instance, uint32_t* count);

/**
 * Returns the number of transmit faults since the start of this Controller
 * instance.
 *
 * Every frame transmission error increases the fault count by one. If more than
 * one error occurred during the transmission of a frame, it is stored as one
 * fault (not several).
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[out] count points to the variable that will hold the number of
 * transmit faults since the start of this module. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19Ctrl_getTxFaultCount(FB19Ctrl_inst_t* instance, uint32_t* count);

/**
 * Returns the start address of the version string, which is a null-terminated
 * C-string.
 *
 * This function may be called even if the instance hasn't been started.
 *
 * @return The version of this library as null-terminated C-string.
 */
const char* FB19Ctrl_getVersion();

/**
 * Deep copies the message \c msg points to into the transmit message queue
 * of this library. After the next call to the \c _handler() function, it is
 * scheduled for transmission.
 *
 * The messages are transmitted in the order they have been added through
 * the \c _submit() calls.
 *
 * When the transmit message buffer is full before this function is called,
 * it returns \c R_ERROR. It also returns this value if the descriptor \c desc
 * of the message \c msg points to contains implausible data. Its fields
 * \c dstAddr, \c msgId and \c pldSize are checked for their range when this
 * function is called.
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[in] msg points to the message that will be added to the transmit
 * queue if this function returns successfully.
 * @return \c R_SUCCESS if the parameter checks pass, the message \c msg
 * points to contains plausible data in the descriptor, and the transmit queue
 * had the capacity to store this message, \c R_ERROR otherwise.
 */
int FB19Ctrl_submit(FB19Ctrl_inst_t* instance, const FB19Msg_t* msg);

/**
 * Handles the timer interrupts of the associated Timer SoC resource.
 *
 * Insert it into the TIM2_IRQHandler() function in the stm32fxxx_it.c file.
 *
 * Example:<br>
 * \code
 *  void TIM2_IRQHandler(void)
 *  {
 *      FB19Ctrl_timerIRQHandler((FB19Ctrl_inst_t*)instFB19Ctrl);
 *  }
 * \endcode
 *
 * @param[in] instance points to the memory resource used by this instance.
 */
void FB19Ctrl_timerIRQHandler(FB19Ctrl_inst_t* instance);

/**
 * Handles the USART interrupts of the associated USART SoC resource.
 *
 * Insert it into the USART2_IRQHandler() function in the stm32fxxx_it.c file.
 *
 * Example:<br>
 * \code
 *  void USART2_IRQHandler(void)
 *  {
 *      FB19Ctrl_UartIRQHandler((FB19Ctrl_inst_t*)instFB19Ctrl);
 *  }
 * \endcode
 *
 * @param[in] instance points to the memory resource used by this instance.
 */
void FB19Ctrl_UartIRQHandler(FB19Ctrl_inst_t* instance);

#ifdef __cplusplus
}
#endif

#endif // FB19CTRL_H_
