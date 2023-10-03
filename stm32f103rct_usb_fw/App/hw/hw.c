/*
 * hw.c
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */


#include "hw.h"



extern uint32_t _flash_tag_addr;
extern uint32_t __isr_vector_addr;


__attribute__((section(".version"))) firm_version_t firm_ver =
{
    _DEF_FIRM_VERSION,
    _DEF_BOARD_NAME
};


__attribute__((section(".tag"))) firm_tag_t firm_tag =
{
    .magic_number = 0xAAAA5555,
    //-- fw info
    //
    .addr_tag = (uint32_t)&_flash_tag_addr,
    .addr_fw  = (uint32_t)&__isr_vector_addr,
    .size_tag = 1024,

    //-- tag info
    //
};



bool hwInit(void)
{
	bool ret = true;

#ifdef _USE_HW_CLI
	ret &= cliInit();
#endif

#ifdef _USE_HW_UART
  ret &= uartInit();
#endif

#ifdef _USE_HW_RTC
  ret &=rtcInit();
#endif

#ifdef _USE_HW_RESET
  ret &= resetInit();
#endif

#ifdef _USE_HW_SWTIMER
  ret &=swtimerInit();
#endif

#ifdef _USE_HW_SPI
  ret &=spiInit();
#endif

#ifdef _USE_HW_FLASH
  ret &= flashInit();
#endif


	uartOpen(HW_LOG_CH, 115200);

	logPrintf("[ boot Begin... ]\r\n");
	logPrintf("Booting..Name \t\t: %s\r\n", _DEF_BOARD_NAME);
	logPrintf("Booting..Ver  \t\t: %s\r\n", _DEF_FIRM_VERSION);
  logPrintf("FW Addr Begin\t\t: 0x%X\r\n", (int)firm_tag.addr_fw);
  logPrintf("FW Addr End  \t\t: 0x%X\r\n", (int)firm_tag.addr_fw + firm_tag.size_tag);
	logPrintf("Booting..Clock\t\t: %d Mhz\r\n", (int)HAL_RCC_GetSysClockFreq()/1000000);

  usbInit();
  usbBegin(USB_CDC_MODE);
  cdcInit();


	return ret;
}



