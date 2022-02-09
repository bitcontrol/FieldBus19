/**
 * Filename:        FB19Ctrl_bareMetal_3.c
 * Platform(s):     None; for education purposes only.
 * Project:         FB19Ctrl_bareMetal_3
 * Created:         Jan 22, 2022
 *
 * Description:     This file shows the essential steps how to start and run a FieldBus19
 *                  controller. There exist 3 FB19Ctrl_bareMetal_<n>.c files:
 *                  - FB19Ctrl_bareMetal_1.c: shows no details
 *                  - FB19Ctrl_bareMetal_2.c: shows some details
 *                  - FB19Ctrl_bareMetal_3.c: shows most details
 *
 * Notes:           The initialization code for the micro-controller has been excluded for clarity.
 *
 *                  This is a bare-metal example, but the FieldBus19 also runs on top of a RTOS.
 *                  Just call the _start() functions once and the _handlers() function periodically.
 *
 *                  For keeping this example as simple as possible, the console menu and the UART
 *                  based console I/O modules aren't included in this example. Add them to the
 *                  _start() and _handler() functions if UART based console I/O is required.
 *
 * Author:          Andreas Isenegger
 * Copyright:       2022, Bitcontrol GmbH, Switzerland.
 *                  Distributed under Mozilla Public License 2.0.
 */

#include "drvSysTick.h" // System Tick driver
#include "FB19Ctrl.h"   // Pulls together the necessary FieldBus19 modules
#include "myModules.h"  // Your modules

int main(void)
{
    // Initialization
    if (drvSysTick_start(20000) != R_SUCCESS) // [us]
    {
        return R_ERROR;
    }
    if (FB19Ctrl_start() != R_SUCCESS)
    {
        return R_ERROR;
    }
    myModA_start();
    myModB_start();

    // The main loop
    while (1)
    {
        myModA_handler();       // Your modules might be time or event driven
        myModB_handler();       // Here they are assumed to be time driven
        FB19Ctrl_handlers();    // This function MUST be called periodically

        drvSysTick_waitForTick();
    }
    return R_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Filename:        FB19Ctrl.c
 * Description:     This file implements the FieldBus19 Controller wrapper functions in their
 *                  simplest form.
 */
#include "appFB19Ctrl.h"    // FieldBus19 Controller application module
#include "cfgFB19.h"        // FieldBus19 configuration module
#include "cfgPlatform.h"    // Platform configuration module (e.g. platform equals STM32F401)
#include "cfgProject.h"     // Project configuration module
#include "defGlobal.h"      // Defines R_ERROR and R_SUCCESS among other items
#include "dprFB19Ctrl.h"    // FieldBus19 Controller data preparation module
#include "drvAperiTimer.h"  // Aperiodic Timer driver module
#include "drvFB19Ctrl.h"    // FieldBus19 Controller driver module
#include "drvFB19UART.h"    // FieldBus19 UART driver module
#include "drvGPIO.h"        // GPIO driver module
#include "drvSysTick.h"     // System Tick mdriver module

int FB19Ctrl_start(void) // Wrapper function that starts the required FieldBus19 modules.
{
    /*
     * Start the modules in the order they depend on each other:
     * 1. Driver modules
     * 2. Data Preparation modules
     * 3. Application modules
     */
    if (drvAperiTimer_start(CFG_DRVAPERITIMER_INST_N0,
            &cfgDrvAperiTimer[CFG_DRVAPERITIMER_INST_N0],
            FB19_APERI_TIMER_PERIOD_US(CFG_FB19_BITRATE)) == R_SUCCESS
        &&
        drvGPIO_start(cfgDrvFB19UART_SoC[CFG_DRVFB19UART_INST_N0].instIdGPIO)
        &&
        drvFB19UART_start(CFG_DRVFB19UART_INST_N0,
            &cfgDrvFB19UART_SoC[CFG_DRVFB19UART_INST_N0],
            CFG_FB19_BITRATE) == R_SUCCESS
        &&
        drvFB19Ctrl_start(&cfgDrvFB19Ctrl) == R_SUCCESS
        &&
        dprFB19Ctrl_start() == R_SUCCESS
        &&
        appFB19Ctrl_start() == R_SUCCESS
    )
    {
        return R_SUCCESS;
    }
    return R_ERROR;
}

void FB19Ctrl_handlers(void) // Wrapper function that calls the required FieldBus19 handlers.
{
    prpFB19Ctrl_handler();
    appFB19Ctrl_handler();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Filename:        myModules.c
 * Description:     This file implements domain specific examples.
 * Notes:           This could also be a C++ file instead of a C file.
 */

#include "appFB19.h"        // FieldBus19 application level module
#include "defFB19.h"        // FieldBus19 definitions
#include "myModules.h"      // myModules definitions and exports

/**
 * Write calculated servo values to FieldBus19 Subscriber, which is a 'servo node' in this example.
 * Note: The commented parameters are for clarifying the intended purpose of the function.
 */
void myModA_handler()
{
    FB19_msg_t msg;

    //
    // Do some stuff, for example calculate set values for servos
    //

    /* Send set values to servo node. */
    msg.dscr.dstAddr = appFB19_getBusAddr(MY_SERVO_NODE);
    msg.dscr.msgId = MY_MSG_ID_SERVO_VALUES;
    myModA_serialize(&msg /*, instance.servos, NBR_OF_SERVOS*/ );
    FB19Ctrl_submit(&msg);

    /*
     * Optional: read response from servo node to previous message.
     * The servo node could for example return the servo currents.
     */
    if (FB19Ctrl_fetch(&msg) == R_SUCCESS)
    {
        if (msg.dscr.pldSize > 0) // Did new data arrive?
        {
            myModA_deserialize(&msg /*, instance.servos, NBR_OF_SERVOS*/ );
        }
    }
    else
    {
        // No response message received; handle fault!
    }

    //
    // Do some more stuff
    //
}

/**
 * Read values from a FieldBus19 Subscriber, which is a sensor node in this example.
 * Note: The commented parameters are for clarifying the intended purpose of the function.
 */
void myModB_handler()
{
    FB19_msg_t msg;

    //
    // Do some stuff, for example calculate new sensor gain and offset parameters
    //

    /* Request sensor values from sensor node. */
    msg.dscr.dstAddr = appFB19_getBusAddr(MY_SENSOR_NODE);
    msg.dscr.msgId = MY_MSG_ID_SENSOR_VALUES;
    // Optional: send new sensor gain and offset parameters to sensor node.
    myModB_serialize(&msg /*, instance.sensors, NBR_OF_SENSORS*/ );
    FB19Ctrl_submit(&msg);

    /* Fetch sensor values from previous request. */
    if (FB19Ctrl_fetch(&msg) == R_SUCCESS)
    {
        if (msg.dscr.pldSize > 0) // Did new data arrive?
        {
            myModB_deserialize(&msg /*, instance.sensors, NBR_OF_SENSORS*/ );
        }
        else
        {
            // Handle case where no new sensor data arrived
        }
    }
    else
    {
        // No response message received; handle fault!
    }

    //
    // Do some more stuff
    //
}
