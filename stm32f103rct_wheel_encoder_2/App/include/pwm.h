/*
 * pwm.h
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_PWM_H_
#define INCLUDE_PWM_H_

#include "def.h"

#ifdef __cplusplus
 extern "C" {
#endif


   bool pwmInit(void);
   void pwmWrite(uint8_t ch, uint16_t duty);
   uint16_t pwmRead(uint8_t ch);


#ifdef __cplusplus
 }
#endif


#endif /* INCLUDE_PWM_H_ */
