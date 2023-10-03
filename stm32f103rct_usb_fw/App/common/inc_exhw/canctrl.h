/*
 * canctrl.h
 *
 *  Created on: sep 10, 2023
 *      Author: gns2l
 */

#ifndef SRC_COMMON_INC_EXHW_CANCTRL_H_
#define SRC_COMMON_INC_EXHW_CANCTRL_H_


#ifdef __cplusplus
 extern "C" {
#endif

#include "exhw_def.h"


#ifdef _USE_EXHW_CANCTRL


#define CANCTRL_MAX_CH            EXHW_CANCTRL_MAX_CH
#define CANCTRL_MSG_RX_BUF_MAX    EXHW_CANCTRL_MSG_RX_BUF_MAX


typedef enum
{
  CANCTRL_100K,
  CANCTRL_125K,
  CANCTRL_250K,
  CANCTRL_500K,
  CANCTRL_1M,
  CANCTRL_2M,
  CANCTRL_4M,
  CANCTRL_5M
} canctrl_baud_t;

typedef enum
{
  CANCTRL_NORMAL,
  CANCTRL_MONITOR,
  CANCTRL_LOOPBACK
}canctrl_mode_t;

typedef enum
{
  CANCTRL_CLASSIC,
  CANCTRL_FD_NO_BRS,
  CANCTRL_FD_BRS
} canctrl_frame_t;

typedef enum
{
  CANCTRL_STD,
  CANCTRL_EXT
} canctrl_id_type_t;

typedef enum
{
  CANCTRL_DLC_0,
  CANCTRL_DLC_1,
  CANCTRL_DLC_2,
  CANCTRL_DLC_3,
  CANCTRL_DLC_4,
  CANCTRL_DLC_5,
  CANCTRL_DLC_6,
  CANCTRL_DLC_7,
  CANCTRL_DLC_8,
  CANCTRL_DLC_12,
  CANCTRL_DLC_16,
  CANCTRL_DLC_20,
  CANCTRL_DLC_24,
  CANCTRL_DLC_32,
  CANCTRL_DLC_48,
  CANCTRL_DLC_64
} canctrl_dlc_t;

typedef enum
{
  CANCTRL_ID_MASK,
  CANCTRL_ID_RANGE
} canctrl_filter_t;

typedef enum
{
  CANCTRL_ERR_NONE      = 0x00000000,
  CANCTRL_ERR_PASSIVE   = 0x00000001,
  CANCTRL_ERR_WARNING   = 0x00000002,
  CANCTRL_ERR_BUS_OFF   = 0x00000004,
  CANCTRL_ERR_BUS_FAULT = 0x00000008,
  CANCTRL_ERR_ETC       = 0x00000010,
} canctrl_err_t;


typedef enum
{
  CANCTRL_EVT_NONE,
  CANCTRL_EVT_MSG,
  CANCTRL_EVT_ERR_PASSIVE,
  CANCTRL_EVT_ERR_WARNING,
  CANCTRL_EVT_ERR_BUS_OFF,
  CANCTRL_EVT_ERR_BUS_FAULT,
  CANCTRL_EVT_ERR_ETC,
} canctrlEvent_t;

typedef struct
{
  canctrl_mode_t  mode;
  canctrl_frame_t frame;
  canctrl_baud_t  baud;
  canctrl_baud_t  baud_data;
} canctrl_info_t;

typedef struct
{
  uint32_t id;
  uint16_t length;
  uint8_t  data[64];

  canctrl_dlc_t      dlc;
  canctrl_id_type_t  id_type;
  canctrl_frame_t    frame;
} canctrl_msg_t;





bool     canctrlInit(void);
bool     canctrlOpen(uint8_t ch, canctrl_mode_t mode, canctrl_frame_t frame, canctrl_baud_t baud, canctrl_baud_t baud_data);
void     canctrlClose(uint8_t ch);
bool     canctrlGetInfo(uint8_t ch, canctrl_info_t *p_info);
bool     canctrlSetFilterType(canctrl_filter_t filter_type);
bool     canctrlGetFilterType(canctrl_filter_t *p_filter_type);
bool     canctrlConfigFilter(uint8_t ch, uint8_t index, canctrl_id_type_t id_type, uint32_t id, uint32_t id_mask);
canctrl_dlc_t canctrlGetDlc(uint8_t length);
uint8_t  canctrlGetLen(canctrl_dlc_t dlc);

bool     canctrlMsgInit(canctrl_msg_t *p_msg, canctrl_frame_t frame, canctrl_id_type_t  id_type, canctrl_dlc_t dlc);
uint32_t canctrlMsgAvailable(uint8_t ch);
bool     canctrlMsgWrite(uint8_t ch, canctrl_msg_t *p_msg, uint32_t timeout);
bool     canctrlMsgRead(uint8_t ch, canctrl_msg_t *p_msg);

uint16_t canctrlGetRxErrCount(uint8_t ch);
uint16_t canctrlGetTxErrCount(uint8_t ch);
uint32_t canctrlGetRxCount(uint8_t ch);
uint32_t canctrlGetTxCount(uint8_t ch);
uint32_t canctrlGetError(uint8_t ch);
uint32_t canctrlGetState(uint8_t ch);

void     canctrlErrClear(uint8_t ch);
void     canctrlErrPrint(uint8_t ch);
bool     canctrlUpdate(void);

void     canctrlAttachRxInterrupt(uint8_t ch, bool (*handler)(canctrl_msg_t *arg));
void     canctrlDetachRxInterrupt(uint8_t ch);

bool     canctrlLock(void);
bool     canctrlUnLock(void);


#endif

#ifdef __cplusplus
 }
#endif


#endif /* SRC_COMMON_INC_EXHW_CANCTRL_H_ */
