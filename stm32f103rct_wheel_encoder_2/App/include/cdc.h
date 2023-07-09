/*
 * cdc.h
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_CDC_H_
#define INCLUDE_CDC_H_


#include "def.h"

#ifdef __cplusplus
 extern "C" {
#endif

   bool     cdcInit(void);
   uint32_t cdcAvailable(void);
   uint8_t  cdcRead(void);
   void     cdcDataIn(uint8_t rx_data);
   uint32_t cdcWrite(uint8_t *p_data, uint32_t length);
   uint32_t cdcGetBaud(void);

#ifdef __cplusplus
 }
#endif

#endif /* INCLUDE_CDC_H_ */
