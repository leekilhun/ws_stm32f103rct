/*
 * eeprom.h
 *
 *  Created on: Aug 27, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_HW_EEPROM_H_
#define COMMON_INC_HW_EEPROM_H_


#ifdef __cplusplus
 extern "C" {
#endif



#include "hw_def.h"

#ifdef _USE_HW_EEPROM


bool     eepromInit();
bool     eepromIsInit(void);
bool     eepromValid(uint32_t addr);
bool     eepromReadByte(uint32_t addr, uint8_t *p_data);
bool     eepromWriteByte(uint32_t addr, uint8_t data_in);
bool     eepromRead(uint32_t addr, uint8_t *p_data, uint32_t length);
bool     eepromWrite(uint32_t addr, uint8_t *p_data, uint32_t length);
uint32_t eepromGetLength(void);
bool     eepromFormat(void);


#endif


#ifdef __cplusplus
}
#endif


#endif /* COMMON_INC_HW_EEPROM_H_ */
