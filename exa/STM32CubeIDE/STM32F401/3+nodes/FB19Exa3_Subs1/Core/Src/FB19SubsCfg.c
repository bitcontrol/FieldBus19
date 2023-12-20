/*
 *  Filename:       FB19SubsCfg.c
 *  Platform(s):    STM32F401x
 *  Project:        FB19Exa3_Subs1
 *  Created:        November 29, 2023
 *  Description:    This file implements the interface of the FieldBus19
 *                  Subscriber configuration module.
 *  Notes:
 *  Author:         Andreas Isenegger
 *  Copyright:      2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */

#include <stm32f4xx.h> // For GPIOA, USART2, ...

#include "FB19Subs.h"
#include "FB19SubsCfg.h"

//------------------------------------------------------------------------------
// Symbols and Macros

//------------------------------------------------------------------------------
// Types

//------------------------------------------------------------------------------
// Constants and Variables

/*
 * FB19 Subscriber
 * ===============
 */

/* FB19 Subscriber Data Preparation low frequency reader message queue. */
static FB19LibMsgQ_element_t sDprSubsLfrArray[4];
static FB19LibMsgQ_queue_t sDprSubsLfrMsgQ =
    { array: sDprSubsLfrArray, capacity: sizeof(sDprSubsLfrArray) / sizeof(FB19LibMsgQ_element_t) };

/* FB19 Subscriber Data Preparation receive message queue. */
static FB19Msg_t sDprSubsRxArray[8];
static FB19MsgQ_t sDprSubsRxMsgQ =
    { array: sDprSubsRxArray, capacity: sizeof(sDprSubsRxArray) / sizeof(FB19Msg_t) };

/* FB19 Subscriber Data Preparation transmit message queue. */
static FB19Msg_t sDprSubsTxArray[8];
static FB19MsgQ_t sDprSubsTxMsgQ =
    { array: sDprSubsTxArray, capacity: sizeof(sDprSubsTxArray) / sizeof(FB19Msg_t) };

/* FB19 Subscriber Driver Rx message queue. */
static FB19Msg_t sDrvSubsRxArray[8];
static FB19MsgQ_t sDrvSubsRxMsgQ =
    { array: sDrvSubsRxArray, capacity: sizeof(sDrvSubsRxArray) / sizeof(FB19Msg_t) };

static FB19LibMsgQ_element_t sDrvSubsLfrArray[8];
static FB19LibMsgQ_queue_t sDrvSubsTxMsgQ =
    { array: sDrvSubsLfrArray, capacity: sizeof(sDrvSubsLfrArray) / sizeof(FB19LibMsgQ_element_t) };

FB19Subs_cfg_t cfgFB19Subs =        // FB19 Subscriber configuration
{
    /* FB19 Subscriber Data Preparation Module. *******************************/
    {
        &sDprSubsLfrMsgQ,           // Low frequency reader message queue
        &sDprSubsRxMsgQ,            // Receive message queue
        &sDprSubsTxMsgQ,            // Transmit message queue
    },

    /* FB19 Subscriber Driver Module. *****************************************/
    {
        MY_SUBSCRIBER_BUS_ADDRESS,  // Bus address
        GPIOA,                      // CMSIS GPIO base address
        7,                          // GPIO_portBitDOE
        &sDrvSubsRxMsgQ,            // Receive message queue
        &sDrvSubsTxMsgQ,            // Transmit message queue
    },

    /* Timer Driver Module. ***************************************************/
    {
        TIM3_IRQn,                  // NVIC_IRQ_channel
        2,                          // NVIC_IRQ_groupPriority
        3,                          // NVIC_IRQ_subPriority
        1UL << 1,                   // RCC_clockEnableBitMask
        &(RCC->APB1ENR),            // RCC_clockEnableRegister
        1UL << 1,                   // RCC_resetBitMask
        &(RCC->APB1RSTR),           // RCC_resetRegister
        16,                         // registerWidth
        TIM3,                       // TIMx
    },

    /* UART Driver Module. ****************************************************/
    {
        GPIOA,                      // CMSIS GPIO base address
        8,                          // GPIO_alternateFunctionSelection
        12,                         // GPIO_portBitNumberRx
        11,                         // GPIO_portBitNumberTx
        USART6_IRQn,                // NVIC_IRQ_channel
        2,                          // NVIC_IRQ_groupPriority
        3,                          // NVIC_IRQ_subPriority
        RCC_APB2ENR_USART6EN,       // RCC_clockEnableBitMask
        &(RCC->APB2ENR),            // RCC_clockEnableRegister
        USART6,                     // USARTx
    },
};

FB19Subs_inst_t instFB19Subs;       // FB19 Subscriber memory and instance

//------------------------------------------------------------------------------
// Local Functions

//------------------------------------------------------------------------------
// Global Functions
