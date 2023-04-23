/*
 * ap.hpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_AP_HPP_
#define INCLUDE_AP_HPP_


#include "utils.h"
#include "uart.h"
#include "cdc.h"
#include "usb.h"
#include "led.h"
#include "pwm.h"
#include "wheel.h"

void  apInit(void);
void  apMain(void);

#endif /* INCLUDE_AP_HPP_ */
