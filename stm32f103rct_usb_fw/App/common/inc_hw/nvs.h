/*
 * nvs.h
 *
 *  Created on: Aug 27, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_HW_NVS_H_
#define COMMON_INC_HW_NVS_H_



#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_NVS


bool nvsInit(void);
bool nvsIsInit(void);

bool nvsIsExist(const char *p_name);
bool nvsSet(const char *p_name, void *p_data, uint32_t length);
bool nvsGet(const char *p_name, void *p_data, uint32_t length);

#endif


#ifdef __cplusplus
}
#endif


#endif /* COMMON_INC_HW_NVS_H_ */
