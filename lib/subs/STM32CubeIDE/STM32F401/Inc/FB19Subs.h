/**
 *  - Filename:     FB19Subs.h
 *  - Platform(s):  All
 *  - Project:      FB19SubsLib
 *  - Created:      May 23, 2023
 *  - Description:  This file publishes the interface of the FieldBus19
 *                  Subscriber library.
 *
 *                  A note on the parameter \c instance:
 *                  The parameter \c instance can be thought of as a 'handle'
 *                  to the Subscriber instance. It is the start address of
 *                  the memory area, where the instance stores its internal
 *                  values (buffers, counters, pointers, ...).
 *
 *                  It doesn't matter for this library if the memory has been
 *                  allocated statically, dynamically or on the stack. Just make
 *                  sure it's not deallocated before the FB19 Subscriber is
 *                  stopped by a call to FB19Subs_stop(), or the behavior of
 *                  the Subscriber will be undefined.<br>
 *                  That way, this library doesn't have to allocate memory
 *                  internally; all required memory space is passed from the
 *                  outside.
 *
 *                  Limitation of the bit rate:<br>
 *                  This version of the FB19 Subscriber library is limited to
 *                  a maximum bit rate of 250kbit/s, although many of STM's
 *                  microcontroller families could run the bus at considerably
 *                  higher rates.
 *
 *                  Limitation of the number of Subscribers:<br>
 *                  This version of the FB19 Subscriber library is limited to
 *                  a maximum number of 8 Subscribers, although it could by
 *                  design handle up to 30 Subscribers.
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
 *                  Subscriber library.
 */

#ifndef FB19SUBS_H_
#define FB19SUBS_H_

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
#define FB19_SUBS_BIT_RATE_MAX          250000

/** Minimum bit rate in bit/s. */
#define FB19_SUBS_BIT_RATE_MIN          2000

/**
 * Highest Bus Subscriber address.
 */
#define FB19_SUBS_BUS_ADDR_HIGHEST      4

/**
 * Lowest Bus Subscriber address.
 */
#define FB19_SUBS_BUS_ADDR_LOWEST       1

/**
 * The size of the memory in bytes required to run an FB19 Subscriber instance.

 * Note that this number doesn't include the sizes of the five message queues
 * that have to be allocated outside this library. They are passed via pointers
 * in the \c FB19Subs_cfg_t configuration type.
 */
#define FB19_SUBS_MEM_SIZE              268

//------------------------------------------------------------------------------
// Types
/** FB19 Subscriber configuration structure. */
typedef struct FB19Subs_cfg_struct
{
    /* FB19 Subscriber Data Preparation Module. *******************************/
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
    } FB19DprSubs;

    /* FB19 Subscriber Driver Module. *****************************************/
    struct {
        /**
         * Bus address of this Subscriber instance.
         * Its range is \c FB19_BUS_ADDR_SUBS_LOWEST to
         * \c FB19_BUS_ADDR_SUBS_HIGHEST, which is currently 1 to 4 for the
         * no cost version.
         * Note that it can change over the lifetime of the Bus Subcriber
         * instance, since it is the Bus Controller that assignes the bus
         * addresses.
         */
        int16_t busAddress;

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
         * A pointer to the receive message queue used by this instance.<br>
         * Typical capacity: 8 messages
         */
        FB19MsgQ_t* rxMsgQ;

        /**
         * A pointer to the transmit message queue used by this instance.<br>
         * Typical capacity: 8 messages
         */
        FB19LibMsgQ_queue_t* txMsgQ;
    } FB19DrvSubs;

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
} FB19Subs_cfg_t;

/** FB19 Subscriber memory and instance type. */
typedef uint8_t FB19Subs_inst_t[FB19_SUBS_MEM_SIZE];

//------------------------------------------------------------------------------
// Constants and Variables

//------------------------------------------------------------------------------
// Functions
/**
 * Starts an FB19 Subscriber instance.
 *
 * The parameter \c instance can be thought of as a 'handle' to the Subscriber
 * instance. It is the start address of the memory area, where the instance
 * stores its internal values (buffers, counters, pointers, ...).
 *
 * The parameter \c config contains system on chip configuration parameters
 * as well as transmit and receive message buffers. It is left to the user to
 * define how large the buffers shall be. Systems with high message rate peaks
 * and low CPU power require larger buffers than those with steady data
 * exchange.
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
int FB19Subs_start(FB19Subs_inst_t* instance,
    const FB19Subs_cfg_t* config,
    int bitRate);

/**
 * Stops an FB19 Subscriber instance.
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
int FB19Subs_stop(FB19Subs_inst_t* instance);

/**
 * Executes the periodic tasks of this library.
 *
 * It essentially moves unread messages that came in since the previous \c
 * _handler() call aside, fetches the received messages from the driver layer
 * and copies them to the next higher layer, and finally copies the messages
 * to be transmitted to the driver layer.
 *
 * Set the period between the calls equal to the period on the FB19 Controller.
 * Since the \c _handler() calls on the FB19 Controller and its subscribers
 * can't be synchronized, some will call their \c _handler() function a bit more
 * frequent than others.<br>
 * If the FB19 Controller's \c _handler() function is called more frequent than
 * that of one of its subscribers, every now and then a response message will
 * be returned unchanged, which indicates this scenario.<br>
 * If the FB19 Controller's \c _handler() function is called less frequent than
 * that of one of its subscribers, a subscriber will every now and then return
 * \c R_ERROR when its \c _submit() function is called. This indicates that the
 * transmit queue was already full before the call and indicates this scenario.
 *
 * When it is known for sure that a FB19 Subscriber receives messages from its
 * FB19 Controller only every Nth Controller's \c _handler() cycle, the period
 * of the Subscriber's \c _handler() calls can be N times of that of the
 * Controller's period.
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
void FB19Subs_handler(FB19Subs_inst_t* instance);

/**
 * Deep copies the oldest unread message that matches the passed message
 * identifier \c msgId into the message the \c msg parameter points to.
 *
 * In the receive message queue, the message is thereby marked as consumed.
 * It cannot be read a second time.
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[out] msg points to the object that will hold the fetched message
 * after this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass and an unread message that
 * matches the message identifier in the descriptor of the message the \c msg
 * parameter points to could be found, \c R_ERROR otherwise.
 */
int FB19Subs_fetch(FB19Subs_inst_t* instance, FB19Msg_t* msg);

/**
 * Copies the bus address of this subscriber instance to the variable \c addr
 * points to.
 *
 * @param[in] instance points to the memory resource used by this instance.
 * @param[out] addr points to the variable that will hold the bus address after
 * this function has returned successfully. In case of an error, it won't
 * be changed.
 * @return \c R_SUCCESS if the parameter checks pass, \c R_ERROR otherwise.
 */
int FB19Subs_getBusAddress(FB19Subs_inst_t* instance, int16_t* addr);

/**
 * Returns the number of noise faults since the start of this Subscriber
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
int FB19Subs_getNoiseFaultCount(FB19Subs_inst_t* instance,
    uint32_t* count);

/**
 * Returns the number of receive faults since the start of this Subscriber
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
int FB19Subs_getRxFaultCount(FB19Subs_inst_t* instance, uint32_t* count);

/**
 * Returns the number of transmit faults since the start of this Subscriber
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
int FB19Subs_getTxFaultCount(FB19Subs_inst_t* instance, uint32_t* count);

/**
 * Returns the start address of the version string, which is a null-terminated
 * C-string.
 *
 * This function may be called even if the instance hasn't been started.
 *
 * @return The version of this library as null-terminated C-string.
 */
const char* FB19Subs_getVersion();

/**
 * Deep copies the message \c msg points to into the transmit message queue
 * of this library. After the next call to the \c _handler() function, it is
 * ready for transmission.
 *
 * It will however only be sent when the FB19 Controller sends this node a
 * request message that matches the message identifier \c msgId inside its
 *  descriptor \c dscr.
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
int FB19Subs_submit(FB19Subs_inst_t* instance, const FB19Msg_t* msg);

/**
 * Handles the timer interrupts of the associated Timer SoC resource.
 *
 * Insert it into the TIM2_IRQHandler() function in the stm32fxxx_it.c file.
 *
 * Example:<br>
 * \code
 *  void TIM2_IRQHandler(void)
 *  {
 *      FB19Subs_timerIRQHandler((FB19Subs_inst_t*)instFB19Subs);
 *  }
 * \endcode
 *
 * @param[in] instance points to the memory resource used by this instance.
 */
void FB19Subs_timerIRQHandler(FB19Subs_inst_t* instance);

/**
 * Handles the USART interrupts of the associated USART SoC resource.
 *
 * Insert it into the USART2_IRQHandler() function in the stm32fxxx_it.c file.
 *
 * Example:<br>
 * \code
 *  void USART2_IRQHandler(void)
 *  {
 *      FB19Subs_UartIRQHandler((FB19Subs_inst_t*)instFB19Subs);
 *  }
 * \endcode
 *
 * @param[in] instance points to the memory resource used by this instance.
 */
void FB19Subs_UartIRQHandler(FB19Subs_inst_t* instance);

#ifdef __cplusplus
}
#endif

#endif // FB19SUBS_H_
