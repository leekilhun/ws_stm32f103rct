/*
 * led.h
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_LED_H_
#define INCLUDE_LED_H_

#include "def.h"

#ifdef __cplusplus
 extern "C" {
#endif


   bool ledInit(void);
   void ledOn(uint8_t ch);
   void ledOff(uint8_t ch);
   void ledToggle(uint8_t ch);
   void ledSetDuty(uint8_t ch, uint16_t duty_data);


#ifdef __cplusplus
 }
#endif

#endif /* INCLUDE_LED_H_ */
