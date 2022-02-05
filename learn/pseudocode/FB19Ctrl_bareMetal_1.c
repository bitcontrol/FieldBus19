/**
 * Filename:        FB19Ctrl_bareMetal_1.c
 * Platform(s):     None; for education purposes only.
 * Project:         FB19Ctrl_bareMetal_1
 * Created:         Jan 22, 2022
 *
 * Description:     This file shows the essential steps how to start and run a FieldBus19
 *                  controller. There exist 3 FB19Ctrl_bareMetal_<n>.c files:
 *                  - FB19Ctrl_bareMetal_1.c: shows no details
 *                  - FB19Ctrl_bareMetal_2.c: shows some details
 *                  - FB19Ctrl_bareMetal_3.c: shows all details
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

int main(void)
{
    // Initialization
    drvSysTick_start(20); // [ms]
    FB19Ctrl_start();
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
    return 0;
}
