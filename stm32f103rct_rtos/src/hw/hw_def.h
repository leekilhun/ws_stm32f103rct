/*
 * hw_def.h
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */

#ifndef SRC_HW_HW_DEF_H_
#define SRC_HW_HW_DEF_H_



#include "bsp.h"


#define _USE_HW_RTOS

#define _HW_DEF_RTOS_MEM_SIZE(x)              ((x)/4)

#define _HW_DEF_RTOS_THREAD_PRI_MAIN          osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_CLI           osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_LED           osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_EVENT         osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_INFO          osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_LCD           osPriorityNormal
//#define _HW_DEF_RTOS_THREAD_PRI_CAN_BUS       osPriorityNormal
#define _HW_DEF_RTOS_THREAD_PRI_CMD_BUS       osPriorityNormal

#define _HW_DEF_RTOS_THREAD_MEM_MAIN          _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#define _HW_DEF_RTOS_THREAD_MEM_CLI           _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#define _HW_DEF_RTOS_THREAD_MEM_LED           _HW_DEF_RTOS_MEM_SIZE(    256)
#define _HW_DEF_RTOS_THREAD_MEM_EVENT         _HW_DEF_RTOS_MEM_SIZE( 1*1024)
#define _HW_DEF_RTOS_THREAD_MEM_INFO          _HW_DEF_RTOS_MEM_SIZE(    512)
#define _HW_DEF_RTOS_THREAD_MEM_LCD           _HW_DEF_RTOS_MEM_SIZE( 1*1024)
//#define _HW_DEF_RTOS_THREAD_MEM_CAN_BUS       _HW_DEF_RTOS_MEM_SIZE( 2*1024)
#define _HW_DEF_RTOS_THREAD_MEM_CMD_BUS       _HW_DEF_RTOS_MEM_SIZE( 1*1024)


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
#define      _UART_CLI              _DEF_UART1

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
