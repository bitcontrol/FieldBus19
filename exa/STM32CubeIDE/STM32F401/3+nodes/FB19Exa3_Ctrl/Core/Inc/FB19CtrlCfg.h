/**
 *  - Filename:     FB19CtrlCfg.h
 *  - Platform(s):  STM32F401x
 *  - Projects:     FB19CtrlLib
 *  - Created:      November 29, 2023
 *  - Description:  This file publishes the interface of the FieldBus19
 *                  Controller configuration file.
 *                  Add it to the project directory and adjust it
 *                  to the requirements of your application.
 *  - Notes:        DON'T CHANGE EXISTING CODE!
 *  - Author:       Andreas Isenegger
 *  - Copyright:    2023, Bitcontrol GmbH, Switzerland.
 *                  All rights reserved.
 *  @file
 *  @brief          This file publishes the interface of the FieldBus19
 *                  Controller configuration file.
 */

#ifndef FB19CTRLCFG_H_
#define FB19CTRLCFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "FB19Ctrl.h"

//------------------------------------------------------------------------------
// Symbols and Macros

//------------------------------------------------------------------------------
// Types

//------------------------------------------------------------------------------
// Constants and Variables
extern FB19Ctrl_cfg_t  myCfgFB19Ctrl;   /**< FB19 Controller configuration. */
extern FB19Ctrl_inst_t myInstFB19Ctrl;  /**< FB19 Controller memory and instance. */

//------------------------------------------------------------------------------
// Functions

#ifdef __cplusplus
}
#endif

#endif // FB19CTRLCFG_H_
