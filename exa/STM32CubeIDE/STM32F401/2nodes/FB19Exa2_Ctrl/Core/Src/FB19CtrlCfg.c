/*
 *  Filename:       FB19CtrlCfg.c
 *  Platform(s):    STM32F401x
 *  Project:        FB19Exa2_Ctrl
 *  Created:        November 29, 2023
 *  Description:    This file implements the interface of the FieldBus19
 *                  Controller configuration module.
 *  Notes:
 *  Author:         Andreas Isenegger
 *  Copyright:      2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */

#include <stm32f4xx.h> // For GPIOA, USART2, ...

#include "FB19Ctrl.h"
#include "FB19CtrlCfg.h"

//------------------------------------------------------------------------------
// Symbols and Macros

//------------------------------------------------------------------------------
// Types

//------------------------------------------------------------------------------
// Constants and Variables

/*
 * FB19 Controller
 * ===============
 */
/* FB19 Controller Data Preparation low frequency reader message queue. */
static FB19LibMsgQ_element_t sDprCtrlLfrArray[4];
static FB19LibMsgQ_queue_t sDprCtrlLfrMsgQ =
    { array: sDprCtrlLfrArray, capacity: sizeof(sDprCtrlLfrArray) / sizeof(FB19LibMsgQ_element_t) };

/* FB19 Controller Data Preparation receive message queue. */
static FB19Msg_t sDprCtrlRxArray[8];
static FB19MsgQ_t sDprCtrlRxMsgQ =
    { array: sDprCtrlRxArray, capacity: sizeof(sDprCtrlRxArray) / sizeof(FB19Msg_t) };

/* FB19 Controller Data Preparation transmit message queue. */
static FB19Msg_t sDprCtrlTxArray[8];
static FB19MsgQ_t sDprCtrlTxMsgQ =
    { array: sDprCtrlTxArray, capacity: sizeof(sDprCtrlTxArray) / sizeof(FB19Msg_t) };

/* FB19 Controller Driver TxRx message queue. */
static FB19Msg_t sDrvCtrlTxRxArray[8];
static FB19MsgQ_t sDrvCtrlTxRxMsgQ =
    { array: sDrvCtrlTxRxArray, capacity: sizeof(sDrvCtrlTxRxArray) / sizeof(FB19Msg_t) };

FB19Ctrl_cfg_t cfgFB19Ctrl =        // FB19 Controller configuration
{
    /* FB19 Controller Data Preparation Module. *******************************/
    {
        &sDprCtrlLfrMsgQ,           // Low frequency reader message queue
        &sDprCtrlRxMsgQ,            // Receive message queue
        &sDprCtrlTxMsgQ,            // Transmit message queue
    },

    /* FB19 Controller Driver Module. *****************************************/
    {
        GPIOA,                      // CMSIS GPIO base address
        7,                          // GPIO_portBitDOE
        &sDrvCtrlTxRxMsgQ,          // Transmit/receive message queue
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

FB19Ctrl_inst_t instFB19Ctrl;       // FB19 Controller memory and instance

//------------------------------------------------------------------------------
// Local Functions

//------------------------------------------------------------------------------
// Global Functions
