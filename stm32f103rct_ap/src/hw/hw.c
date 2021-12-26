/*
 * hw.c
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */



#include "hw.h"



bool hwInit(void)
{
  bool ret = true;

  ret &= bspInit();
  ret &= rtcInit();
  ret &= resetInit();

  ret &= cliInit();

  ret &= ledInit();
  ret &= buttonInit();
  ret &= flashInit();
  ret &= gpioInit();

  ret &= logInit();
  //ret &= usbInit();
  ret &= uartInit();
  uartOpen(_DEF_UART1, 115200);

  logOpen(_DEF_UART1, 115200);
  logPrintf("\r\n[ Firmware Begin... ]\r\n");

  ret &= spiInit();
  //ret &= i2cInit();
  ret &= canInit();
  //usbBegin(USB_CDC_MODE);
  return ret;
}
