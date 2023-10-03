/*
 * exhw_def.h
 *
 *  Created on: Aug 26, 2023
 *      Author: gns2.lee
 */

#ifndef EXHW_EXHW_DEF_H_
#define EXHW_EXHW_DEF_H_



#include "hw.h"


//#define	_USE_EXHW_AT24C32

//#define _USE_EXHW_LED
#define      EXHW_LED_MAX_CH          1
#define      EXHW_LED_CH_STATUS       _DEF_LED1


//#define _USE_EXHW_BUTTON
#define      EXHW_BUTTON_MAX_CH       3
#define      EXHW_BUTTON_CH_BOOT      _DEF_BUTTON1
#define      EXHW_BUTTON_CH_S1        _DEF_BUTTON2
#define      EXHW_BUTTON_CH_S2        _DEF_BUTTON3
#endif /* EXHW_EXHW_DEF_H_ */

//#define  _USE_EXHW_MCP2515
#define       EXHW_MCP2515_MAX_CH     1

//#define _USE_EXHW_CANCTRL
#define      EXHW_CANCTRL_MAX_CH     1
#define      EXHW_CANCTRL_MSG_RX_BUF_MAX  16


//#define _USE_EXHW_SD

//#define _USE_EXHW_FATFS

//#define _USE_EXHW_CMD
#define      EXHW_CMD_MAX_DATA_LENGTH 1024
