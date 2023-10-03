/*
 * hw_def.h
 *
 *  Created on: Aug 21, 2022
 *      Author: gns2l
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#include "bsp.h"


/****************************************************
   boot/ firmware memory information
 ****************************************************/
#define _DEF_BOOT_VERSION           "B230926R1"
#define _DEF_BOARD_NAME             "SKRE3V12 Bootloader"

#define FLASH_ADDR_TAG              0x0800C800
#define FLASH_ADDR_FW               0x0800CC00
#define FLASH_ADDR_FW_VER           0x0800D000

#define FLASH_ADDR_START            0x0800C800
#define FLASH_ADDR_END              (FLASH_ADDR_START + (256-50)*1024)

/* ------------------------------------------------- */


#define _USE_HW_FLASH

#define _USE_HW_RTC
#define _USE_HW_RESET


#define _USE_HW_UART
#define      HW_UART_MAX_CH         3
#define      HW_LOG_CH              _DEF_UART3
#define _USE_HW_UART_1_RX_DMA
#define _USE_HW_UART_2_RX_DMA

#define _USE_HW_USB
#define _USE_HW_CDC
#define      HW_USE_CDC             1
#define      HW_USE_MSC             0


#define _USE_HW_CLI
#define      HW_CLI_CH              _DEF_UART2
#define      HW_CLI_CMD_LIST_MAX    16
#define      HW_CLI_CMD_NAME_MAX    16
#define      HW_CLI_LINE_HIS_MAX    4
#define      HW_CLI_LINE_BUF_MAX    64



#define _USE_HW_CMD
#define      HW_CMD_MAX_DATA_LENGTH 1024
#define      HW_CMD_CH              _DEF_UART1





#endif /* SRC_HW_HW_DEF_H_ */
