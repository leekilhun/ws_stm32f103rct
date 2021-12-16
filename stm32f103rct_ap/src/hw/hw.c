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
  ret &= ledInit();
  ret &= buttonInit();
  ret &= flashInit();

  //ret &= usbInit();
  ret &= uartInit();

  ret &= cliInit();
  //ret &= gpioInit();

  //ret &= logInit();

  ret &= spiInit();
  //ret &= i2cInit();
  ret &= canInit();
  //usbBegin(USB_CDC_MODE);
  return ret;
}
