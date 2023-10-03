/*
 * hw.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#include "flash.h"
#include "uart.h"
#include "reset.h"
#include "rtc.h"
#include "usb.h"
#include "cdc.h"
#include "cli.h"

#include "cmd.h"


bool hwInit(void);

#ifdef __cplusplus
}
#endif
