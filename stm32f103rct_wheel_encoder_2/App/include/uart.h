/*
 * uart.h
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#ifndef INCLUDE_UART_H_
#define INCLUDE_UART_H_


#include "def.h"

#ifdef __cplusplus
extern "C" {
#endif

  bool     uartInit(void);
  uint32_t uartWrite(uint8_t ch, uint8_t *p_data, uint32_t length);
  uint32_t uartAvailable(uint8_t ch);
  uint8_t  uartRead(uint8_t ch);
  uint32_t uartPrintf(uint8_t ch, const char *fmt, ...);


#ifdef __cplusplus
}
#endif


#endif /* INCLUDE_UART_H_ */
