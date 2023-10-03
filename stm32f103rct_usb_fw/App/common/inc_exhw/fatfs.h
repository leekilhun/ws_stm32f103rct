/*
 * fatfs.h
 *
 *  Created on: 2023. 09. 24.
 *      Author: gns2
 */

#ifndef SRC_COMMON_EXHW_INCLUDE_FATFS_H_
#define SRC_COMMON_EXHW_INCLUDE_FATFS_H_

#include "exhw_def.h"


#ifdef _USE_EXHW_FATFS

#ifdef __cplusplus
extern "C" {
#endif

bool fatfsInit(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* SRC_COMMON_EXHW_INCLUDE_FATFS_H_ */
