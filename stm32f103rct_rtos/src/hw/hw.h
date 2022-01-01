/*
 * hw.h
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */

#ifndef SRC_HW_HW_H_
#define SRC_HW_HW_H_



#include "hw_def.h"

#include "led.h"
#include "rtc.h"
#include "reset.h"
#include "button.h"
#include "cdc.h"
#include "usb.h"
#include "uart.h"
#include "cli.h"
#include "can.h"
#include "i2c.h"
#include "spi.h"
#include "log.h"
#include "flash.h"
#include "gpio.h"

bool hwInit(void);

#endif /* SRC_HW_HW_H_ */
