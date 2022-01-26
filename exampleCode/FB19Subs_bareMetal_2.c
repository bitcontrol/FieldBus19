/**
 * Filename:        FB19Subs_bareMetal_2.c
 * Platform(s):     None; for education purposes only.
 * Project:         FB19Subs_bareMetal_2
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
#include "myModC.h"     // Your module C
#include "myModD.h"     // Your module D

/** Return value of a function in case of an error. */
#define R_ERROR                         -1

/** Return value of a function in case of success (no error). */
#define R_SUCCESS                       1

int main(void)
{
    // Initialization
    drvSysTick_start(20); // [ms]
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
int FB19Subs_start(void) // Wrapper function that starts the required FieldBus19 modules.
{
    // The function parameters have been excluded for clarity
    if (drvAperiTimer_start() == R_SUCCESS
        &&
        drvGPIO_start() == R_SUCCESS
        &&
        drvFB19UART_start() == R_SUCCESS
        &&
        drvFB19Subs_start() == R_SUCCESS
        &&
        prpFB19Subs_start() == R_SUCCESS
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
