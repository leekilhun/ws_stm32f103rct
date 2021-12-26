/*
 * hw_def.h
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#include "bsp.h"



#define _USE_HW_FLASH


#define _USE_HW_RTC
#define _USE_HW_RESET

#define _USE_HW_LED
#define      HW_LED_MAX_CH          2

#define _USE_HW_BUTTON
#define      HW_BUTTON_MAX_CH       2

#define _USE_HW_GPIO
#define      HW_GPIO_MAX_CH         1
#define        _GPIO_W25Q16_NCS     0


//#define _USE_HW_CDC
//#define _USE_HW_USB
#define      HW_USE_CDC             1
#define      HW_USE_MSC             0

#define _USE_HW_UART
#define      HW_UART_MAX_CH         1

#define _USE_HW_LOG
#define      HW_LOG_CH              _DEF_UART1
#define      HW_LOG_BOOT_BUF_MAX    1024
#define      HW_LOG_LIST_BUF_MAX    2048

#define _USE_HW_CLI
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64

#define _USE_HW_CAN
#define      HW_CAN_MAX_CH          1
#define      HW_CAN_MSG_RX_BUF_MAX  16

#define _USE_HW_SPI
#define      HW_SPI_MAX_CH          2
#define        _SPI_W25Q16          _DEF_SPI1
#define        _SPI_SLAVE           _DEF_SPI2


//#define _USE_HW_I2C
#define      HW_I2C_MAX_CH          2



#endif /* SRC_HW_HW_DEF_H_ */
