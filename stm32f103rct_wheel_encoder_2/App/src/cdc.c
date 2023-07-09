/*
 * cdc.h
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */



#include "cdc.h"


bool cdcInit(void)
{
  bool ret = true;
  return ret;
}



/*
usbd_cdc_if.c


uint32_t cdcAvailable(void);
uint8_t  cdcRead(void);
void     cdcDataIn(uint8_t rx_data);
uint32_t cdcWrite(uint8_t *p_data, uint32_t length);
uint32_t cdcGetBaud(void);
*/
