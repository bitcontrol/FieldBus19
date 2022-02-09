/**
 * Filename:        FB19Ctrl_bareMetal_2.c
 * Platform(s):     None; for education purposes only.
 * Project:         FB19Ctrl_bareMetal_2
 * Created:         Jan 22, 2022
 *
 * Description:     This file shows the essential steps how to start and run a FieldBus19
 *                  controller. There exist 3 FB19Ctrl_bareMetal_<n>.c files:
 *                  - FB19Ctrl_bareMetal_1.c: shows no details
 *                  - FB19Ctrl_bareMetal_2.c: shows some details
 *                  - FB19Ctrl_bareMetal_3.c: shows most details
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
#include "FB19Ctrl.h"   // Pulls together the necessary FieldBus19 modules
#include "myModA.h"     // Your module A
#include "myModB.h"     // Your module B

/** Return value of a function in case of an error. */
#define R_ERROR                         -1

/** Return value of a function in case of success (no error). */
#define R_SUCCESS                       1

int main(void)
{
    // Initialization
    drvSysTick_start(20000); // [us]
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
int FB19Ctrl_start(void) // Wrapper function that starts the required FieldBus19 modules.
{
    /*
     * The function parameters have been excluded for clarity.
     * Start the modules in the order they depend on each other:
     * 1. Driver modules
     * 2. Data Preparation modules
     * 3. Application modules
     */
    if (drvAperiTimer_start() == R_SUCCESS
        &&
        drvGPIO_start() == R_SUCCESS
        &&
        drvFB19UART_start() == R_SUCCESS
        &&
        drvFB19Ctrl_start() == R_SUCCESS
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
