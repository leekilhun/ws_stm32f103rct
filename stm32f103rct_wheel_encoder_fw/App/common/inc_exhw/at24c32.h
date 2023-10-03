/*
 * at24c32.h
 *
 *  Created on: 2023. 6. 10.
 *      Author: gns2l
 */

#ifndef COMMON_INC_EXHW_AT24C32_H_
#define COMMON_INC_EXHW_AT24C32_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "exhw_def.h"


#ifdef _USE_EXHW_AT24C32

  bool at24c32Init(void);
  bool at24c32IsInit(void);
  bool at24c32Valid(uint32_t addr);

  uint8_t at24c32ReadByte(uint32_t addr);
  bool at24c32WriteByte(uint32_t addr, uint8_t data_in);
  uint32_t at24c32GetLength(void);

  bool  at24c32Read(uint32_t addr, uint8_t *p_data, uint32_t length);
  bool  at24c32Write(uint32_t addr, uint8_t *p_data, uint32_t length);

  bool at24c32Format(void);
  bool at24c32pageErase(uint8_t page);
  bool at24c32pageWrite(uint8_t page, uint8_t* p_data);
  bool at24c32pageRead(uint8_t page, uint8_t* p_data);


#endif

#ifdef __cplusplus
}
#endif



#endif /* COMMON_INC_EXHW_AT24C32_H_ */
