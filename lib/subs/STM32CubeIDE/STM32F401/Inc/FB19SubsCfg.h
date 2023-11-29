/**
 *  Filename:       FB19SubsCfg.h
 *  Platform(s):    STM32F401x
 *  Projects:       FB19CtrlLib, FB19SubsLib
 *  Created:        June 17, 2023
 *  Description:    This file publishes the interface of the FieldBus19
 *                  Subscriber configuration file.
 *                  Add it to the project directory and adjust it
 *                  to the requirements of your application.
 *  Notes:          DON'T CHANGE EXISTING CODE!
 *  Author:         Andreas Isenegger
 *  Copyright:      2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 */

#ifndef FB19SUBSCFG_H_
#define FB19SUBSCFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "FB19Subs.h"

//------------------------------------------------------------------------------
// Symbols and Macros

//------------------------------------------------------------------------------
// Types

//------------------------------------------------------------------------------
// Constants and Variables
extern FB19Subs_cfg_t  cfgFB19Subs;     // FB19 Subscriber configuration
extern FB19Subs_inst_t instFB19Subs;    // FB19 Subscriber memory and instance

//------------------------------------------------------------------------------
// Functions

#ifdef __cplusplus
}
#endif

#endif // FB19SUBSCFG_H_
