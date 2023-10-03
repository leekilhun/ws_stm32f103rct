/*
 * util.h
 *
 *  Created on: Aug 26, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_CORE_UTILS_H_
#define COMMON_CORE_UTILS_H_



#ifdef __cplusplus
 extern "C" {
#endif


#include "def.h"


uint32_t utilConvert8ToU32 (uint8_t *p_data);
uint16_t utilConvert8ToU16 (uint8_t *p_data);

uint8_t utilParseArgs(char *argStr, char **argv, const char* delim_char, int max);
void utilUpdateCrc(uint16_t *p_crc_cur, uint8_t data_in);

#ifdef __cplusplus
}
#endif



#endif /* COMMON_CORE_UTILS_H_ */
