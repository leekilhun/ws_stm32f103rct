/*
 * log.h
 *
 *  Created on: Aug 26, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_HW_LOG_H_
#define COMMON_INC_HW_LOG_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "hw_def.h"


#ifdef _USE_HW_LOG

#define LOG_CH            HW_LOG_CH
#define LOG_BOOT_BUF_MAX  HW_LOG_BOOT_BUF_MAX
#define LOG_LIST_BUF_MAX  HW_LOG_LIST_BUF_MAX


bool logInit(void);
void logEnable(void);
void logDisable(void);
bool logOpen(uint8_t ch, uint32_t baud);
void logBoot(uint8_t enable);
void logPrintf(const char *fmt, ...);

#endif

#ifdef __cplusplus
}
#endif




#endif /* COMMON_INC_HW_LOG_H_ */
