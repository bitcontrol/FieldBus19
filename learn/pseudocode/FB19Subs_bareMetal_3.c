/**
 * Filename:        FB19Subs_bareMetal_3.c
 * Platform(s):     None; for education purposes only.
 * Project:         FB19Subs_bareMetal_3
 * Created:         Jan 22, 2022
 *
 * Description:     This file shows the essential steps how to start and run a FieldBus19
 *                  subscriber. There exist 3 FB19Subs_bareMetal_<n>.c files:
 *                  - FB19Subs_bareMetal_1.c: shows no details
 *                  - FB19Subs_bareMetal_2.c: shows some details
 *                  - FB19Subs_bareMetal_3.c: shows all details
 *
 * Notes:           The initialization code for the micro-controller has been excluded for clarity.
 *                  This is a bare-metal example, but the FieldBus19 also runs on top of a RTOS.
 *                  Just call the _start() functions once and the _handlers() function periodically.
 *
 * Author:          Andreas Isenegger
 * Copyright:       2022, Bitcontrol GmbH, Switzerland.
 *                  Distributed under Mozilla Public License 2.0.
 */

#include "drvSysTick.h" // System Tick driver
#include "FB19Subs.h"   // Pulls together the necessary FieldBus19 modules
#include "myModules.h"  // Your modules

int main(void)
{
    // Initialization
    if (drvSysTick_start(20) != R_SUCCESS) // [ms]
    {
        return R_ERROR;
    }
    if (FB19Subs_start() != R_SUCCESS)
    {
        return R_ERROR;
    }
    myModC_start();
    myModD_start();

    // The main loop
    while (1)
    {
        myModC_handler();       // Your modules might be time or event driven
        myModD_handler();       // Here they are assumed to be time driven
        FB19Subs_handlers();    // This function MUST be called periodically

        drvSysTick_waitForTick();
    }
    return R_SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Filename:        FB19Subs.c
 * Description:     This file implements the FieldBus19 Subscriber wrapper functions in their
 *                  simplest form.
 */
#include "cfgFB19.h"        // FieldBus19 configuration module
#include "cfgPlatform.h"    // Platform configuration module (e.g. platform equals STM32F401)
#include "cfgProject.h"     // Project configuration module
#include "defGlobal.h"      // Defines R_ERROR and R_SUCCESS among other items
#include "dprFB19Subs.h"    // FieldBus19 Subscriber data preparation module
#include "drvAperiTimer.h"  // Aperiodic Timer driver module
#include "drvFB19Subs.h"    // FieldBus19 Subscriber driver module
#include "drvFB19UART.h"    // FieldBus19 UART driver module
#include "drvGPIO.h"        // GPIO driver module
#include "drvSysTick.h"     // System Tick mdriver module

int FB19Subs_start(void) // Wrapper function that starts the required FieldBus19 modules.
{
    // The function parameters have been excluded for clarity
    if (drvAperiTimer_start(CFG_DRVAPERITIMER_INST_N0,
            &cfgDrvAperiTimer[CFG_DRVAPERITIMER_INST_N0],
            FB19_APERI_TIMER_PERIOD_US(CFG_FB19_BITRATE)) == R_SUCCESS
        &&
        drvGPIO_start(cfgDrvFB19UART_SoC[CFG_DRVFB19UART_INST_N1].instIdGPIO)
        &&
        drvFB19UART_start(CFG_DRVFB19UART_INST_N1,
            &cfgDrvFB19UART_SoC[CFG_DRVFB19UART_INST_N1],
            CFG_FB19_BITRATE) == R_SUCCESS
        &&
        drvFB19Subs_start(&cfgDrvFB19Subs) == R_SUCCESS
        &&
        dprFB19Subs_start() == R_SUCCESS
        &&
        appFB19Subs_start() == R_SUCCESS
    )
    {
        return R_SUCCESS;
    }
    return R_ERROR;
}

void FB19Subs_handlers(void) // Wrapper function that calls the required FieldBus19 handlers.
{
    prpFB19Subs_handler();
    appFB19Subs_handler();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Filename:        myModules.c
 * Description:     This file implements domain specific examples.
 * Notes:           This could also be a C++ file instead of a C file.
 */

#include "defFB19.h"        // FieldBus19 definitions
#include "myModules.h"      // myModules definitions and exports

/**
 * Read desired values from FieldBus19 Controller and apply them to servos.
 * Note: The commented parameters are for clarifying the intended purpose of the function.
 */
void myModC_handler()
{
    FB19_msg_t msg;

    //
    // Do some stuff, for example measure servo currents
    //

    /* Read and apply desired values. */
    msg.dscr.msgId = MY_MSG_ID_SERVO_VALUES;
    FB19Subs_fetch(&msg);
    if (msg.dscr.pldSize > 0) // Did new data arrive?
    {
        myModC_deserialize(&msg /*, instance.servos, NBR_OF_SERVOS*/ );
    }

    /*
     * Optional: return actual values the next time this msg is received.
     * This node could for example return the servo currents.
     */
    myModC_serialize(&msg /*, instance.servos, NBR_OF_SERVOS*/ );
    FB19Subs_submit(&msg);

    //
    // Do some more stuff
    //
}

/**
 * Read values from sensors and send them to FieldBus19 Controller.
 * Note: The commented parameters are for clarifying the intended purpose of the function.
 */
void myModD_handler()
{
    FB19_msg_t msg;

    //
    // Do some stuff
    //

    /*
     * Optional: read and apply new sensor settings.
     * The controller could for example send new sensor configuration data.
     */
    msg.dscr.msgId = MY_MSG_ID_SENSOR_VALUES;
    FB19Subs_fetch(&msg);
    if (msg.dscr.pldSize > 0) // Did new data arrive?
    {
        myModD_deserialize(&msg /*, instance.sensors, NBR_OF_SENSORS*/ );
    }

    /* Store values of sensors in RAM. */
    for (int i = 0; i < NBR_OF_SENSORS; i++)
    {
        instance.sensors[i].value = instance.sensors[i]->readInput();
    }

    /* Return actual values next time a message with this id has been received. */
    myModD_serialize(&msg /*, instance.sensors, NBR_OF_SENSORS*/ );
    FB19Subs_submit(&msg);

    //
    // Do some more stuff
    //
}
