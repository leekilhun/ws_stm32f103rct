/*
 * led.h
 *
 *  Created on: Aug 27, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_EXHW_LED_H_
#define COMMON_INC_EXHW_LED_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"


#define LED_MAX_CH  EXHW_LED_MAX_CH


bool ledInit(void);
void ledOn(uint8_t ch);
void ledOff(uint8_t ch);
void ledToggle(uint8_t ch);

#ifdef __cplusplus
}
#endif


#endif /* COMMON_INC_EXHW_LED_H_ */
