/*
 * hw.c
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "hw.h"

__attribute__((section(".version"))) firm_version_t boot_ver =
    {
        _DEF_BOOT_VERSION,
        _DEF_BOARD_NAME
    };


bool hwInit(void)
{
	bool ret = true;

	ret &= bspInit();

#ifdef _USE_HW_CLI
	ret &= cliInit();
#endif
	ret &= uartInit();

	ret &=rtcInit();

	ret &=resetInit();

	ret &= flashInit();


	uartOpen(HW_LOG_CH, 115200);

	logPrintf("[ boot Begin... ]\r\n");
	logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
	logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_BOOT_VERSION);
	logPrintf("Booting..Clock\t\t: %d Mhz\r\n", (int)HAL_RCC_GetSysClockFreq()/1000000);

  usbInit();
  usbBegin(USB_CDC_MODE);
  cdcInit();




	return ret;
}



