/*
 * canctrl.c
 *
 *  Created on: Jul 10, 2021
 *      Author: gns2l
 */

#include "canctrl.h"
#include "qbuffer.h"
#include "cli.h"

#include "mcp2515.h"

#ifdef _USE_EXHW_CANCTRL

typedef struct
{
  uint32_t prescaler;
  uint32_t sjw;
  uint32_t tseg1;
  uint32_t tseg2;
} canctrl_baud_cfg_t;

const canctrl_baud_cfg_t canctrl_baud_cfg_80m_normal[] =
    {
        {50, 8, 13, 2}, // 100K, 87.5%
        {40, 8, 13, 2}, // 125K, 87.5%
        {20, 8, 13, 2}, // 250K, 87.5%
        {10, 8, 13, 2}, // 500K, 87.5%
        {5, 8, 13, 2},  // 1M,   87.5%
};

const canctrl_baud_cfg_t canctrl_baud_cfg_80m_data[] =
    {
        {40, 8, 11, 8}, // 100K, 60%
        {32, 8, 11, 8}, // 125K, 60%
        {16, 8, 11, 8}, // 250K, 60%
        {8, 8, 11, 8},  // 500K, 60%
        {4, 8, 11, 8},  // 1M,   60%
        {2, 8, 11, 8},  // 2M    60%
        {1, 8, 11, 8},  // 4M    60%
        {1, 8, 9, 6},   // 5M    62.5%
};

const canctrl_baud_cfg_t *p_baud_normal = canctrl_baud_cfg_80m_normal;
const canctrl_baud_cfg_t *p_baud_data = canctrl_baud_cfg_80m_data;

const uint32_t dlc_len_tbl[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

typedef struct
{
  bool is_init;
  bool is_open;

  uint32_t err_code;
  uint8_t state;
  uint32_t recovery_cnt;

  uint32_t q_rx_full_cnt;
  uint32_t q_tx_full_cnt;
  uint32_t fifo_full_cnt;
  uint32_t fifo_lost_cnt;

  uint32_t fifo_idx;
  uint32_t enable_int;
  canctrl_mode_t mode;
  canctrl_frame_t frame;
  canctrl_baud_t baud;
  canctrl_baud_t baud_data;

  uint32_t rx_cnt;
  uint32_t tx_cnt;

  uint8_t hfdcan;
  bool (*handler)(canctrl_msg_t *arg);

  qbuffer_t q_msg;
  canctrl_msg_t can_msg[CANCTRL_MSG_RX_BUF_MAX];
} canctrl_tbl_t;

static canctrl_tbl_t can_tbl[CANCTRL_MAX_CH];

static volatile uint32_t err_int_cnt = 0;
static canctrl_filter_t canctrl_filter_type = CANCTRL_ID_MASK;

#ifdef _USE_HW_CLI
static void cliCanCtrl(cli_args_t *args);
#endif

static void canctrlErrUpdate(uint8_t ch);
static void canctrlRxFifoUpdate(uint8_t ch);

bool canctrlInit(void)
{
  bool ret = true;

  uint8_t i;

  for (i = 0; i < CANCTRL_MAX_CH; i++)
  {
    can_tbl[i].is_init = true;
    can_tbl[i].is_open = false;
    can_tbl[i].err_code = CANCTRL_ERR_NONE;
    can_tbl[i].state = 0;
    can_tbl[i].recovery_cnt = 0;

    can_tbl[i].q_rx_full_cnt = 0;
    can_tbl[i].q_tx_full_cnt = 0;
    can_tbl[i].fifo_full_cnt = 0;
    can_tbl[i].fifo_lost_cnt = 0;

    qbufferCreateBySize(&can_tbl[i].q_msg, (uint8_t *)&can_tbl[i].can_msg[0], sizeof(canctrl_msg_t), CANCTRL_MSG_RX_BUF_MAX);
  }

  mcp2515Init();
  logPrintf("[OK] canctrlInit()\n");
#ifdef _USE_HW_CLI
  cliAdd("canctrl", cliCanCtrl);
#endif
  return ret;
}
bool canctrlLock(void)
{
  bool ret = true;
  return ret;
}

bool canctrlUnLock(void)
{
  return true;
}

bool canctrlOpen(uint8_t ch, canctrl_mode_t mode, canctrl_frame_t frame, canctrl_baud_t baud, canctrl_baud_t baud_data)
{
  bool ret = true;

  if (ch >= CANCTRL_MAX_CH)
    return false;

  switch (ch)
  {
  case _DEF_CAN1:
    can_tbl[ch].hfdcan = 0;
    can_tbl[ch].mode = mode;
    can_tbl[ch].frame = frame;
    can_tbl[ch].baud = baud;
    can_tbl[ch].baud_data = baud_data;
    can_tbl[ch].fifo_idx = 0;
    can_tbl[ch].enable_int = 0;

    mcp2515SetBaud(can_tbl[ch].hfdcan, (McpBaud)baud);
    ret = true;
    break;
  }

  if (ret != true)
  {
    return false;
  }

  canctrlConfigFilter(ch, 0, CANCTRL_STD, 0x0000, 0x0000);
  canctrlConfigFilter(ch, 0, CANCTRL_EXT, 0x0000, 0x0000);

  can_tbl[ch].is_open = true;

  return ret;
}

void canctrlClose(uint8_t ch)
{
}

bool canctrlGetInfo(uint8_t ch, canctrl_info_t *p_info)
{
  if (ch >= CANCTRL_MAX_CH)
    return false;

  p_info->baud = can_tbl[ch].baud;
  p_info->baud_data = can_tbl[ch].baud_data;
  p_info->frame = can_tbl[ch].frame;
  p_info->mode = can_tbl[ch].mode;

  return true;
}

canctrl_dlc_t canctrlGetDlc(uint8_t length)
{
  if (length >= 64)
    return CANCTRL_DLC_64;
  else if (length >= 48)
    return CANCTRL_DLC_48;
  else if (length >= 32)
    return CANCTRL_DLC_32;
  else if (length >= 24)
    return CANCTRL_DLC_24;
  else if (length >= 20)
    return CANCTRL_DLC_20;
  else if (length >= 16)
    return CANCTRL_DLC_16;
  else if (length >= 12)
    return CANCTRL_DLC_12;
  else if (length >= 8)
    return CANCTRL_DLC_8;
  else
    return (canctrl_dlc_t)length;
}

uint8_t canctrlGetLen(canctrl_dlc_t dlc)
{
  return dlc_len_tbl[dlc];
}

bool canctrlConfigFilter(uint8_t ch, uint8_t index, canctrl_id_type_t id_type, uint32_t id, uint32_t id_mask)
{
  bool ret = false;

  if (ch >= CANCTRL_MAX_CH)
    return false;

  if (id_type == CANCTRL_STD)
  {
  }
  else
  {
  }

  return ret;
}

bool canctrlSetFilterType(canctrl_filter_t filter_type)
{
  canctrl_filter_type = filter_type;
  return true;
}
bool canctrlGetFilterType(canctrl_filter_t *p_filter_type)
{
  *p_filter_type = canctrl_filter_type;
  return true;
}

uint32_t canctrlMsgAvailable(uint8_t ch)
{
  if (ch > CANCTRL_MAX_CH)
    return 0;

  return qbufferAvailable(&can_tbl[ch].q_msg);
}

bool canctrlMsgInit(canctrl_msg_t *p_msg, canctrl_frame_t frame, canctrl_id_type_t id_type, canctrl_dlc_t dlc)
{
  p_msg->frame = frame;
  p_msg->id_type = id_type;
  p_msg->dlc = dlc;
  p_msg->length = dlc_len_tbl[dlc];
  return true;
}

bool canctrlMsgWrite(uint8_t ch, canctrl_msg_t *p_msg, uint32_t timeout)
{
  mcp_msg_t msg;
  bool ret = true;

  if (ch > CANCTRL_MAX_CH)
    return false;

  if (can_tbl[ch].err_code & CANCTRL_ERR_BUS_OFF)
    return false;

  switch (p_msg->id_type)
  {
  case CANCTRL_STD:
    msg.ext = false;
    break;

  case CANCTRL_EXT:
    msg.ext = true;
    break;
  }

  switch (p_msg->frame)
  {
  case CANCTRL_CLASSIC:
    break;

  case CANCTRL_FD_NO_BRS:
    break;

  case CANCTRL_FD_BRS:
    break;
  }

  msg.id = p_msg->id;
  msg.ext = true;
  msg.dlc = dlc_len_tbl[p_msg->dlc];

  memcpy(msg.data, p_msg->data, dlc_len_tbl[p_msg->dlc]);

  if (mcp2515SendMsg(can_tbl[ch].hfdcan, &msg) == true)
  {
    ret = true;
  }
  else
  {
    ret = false;
  }

  return ret;
}

bool canctrlMsgRead(uint8_t ch, canctrl_msg_t *p_msg)
{
  bool ret = true;

  if (ch > CANCTRL_MAX_CH)
    return 0;

  ret = qbufferRead(&can_tbl[ch].q_msg, (uint8_t *)p_msg, 1);

  return ret;
}

uint16_t canctrlGetRxErrCount(uint8_t ch)
{
  uint16_t ret = 0;

  if (ch > CANCTRL_MAX_CH)
    return 0;

  ret = mcp2515GetRxErrCount(ch);
  return ret;
}

uint16_t canctrlGetTxErrCount(uint8_t ch)
{
  uint16_t ret = 0;

  if (ch > CANCTRL_MAX_CH)
    return 0;

  ret = mcp2515GetTxErrCount(ch);
  return ret;
}
uint32_t canctrlGetRxCount(uint8_t ch)
{
  if (ch >= CANCTRL_MAX_CH)
    return 0;

  return can_tbl[ch].rx_cnt;
}
uint32_t canctrlGetTxCount(uint8_t ch)
{
  if (ch >= CANCTRL_MAX_CH)
    return 0;

  return can_tbl[ch].tx_cnt;
}
uint32_t canctrlGetError(uint8_t ch)
{
  if (ch > CANCTRL_MAX_CH)
    return 0;

  return mcp2515ReadErrorFlags(can_tbl[ch].hfdcan);
}

uint32_t canctrlGetState(uint8_t ch)
{
  if (ch > CANCTRL_MAX_CH)
    return 0;

  return 0;
}

void canctrlAttachRxInterrupt(uint8_t ch, bool (*handler)(canctrl_msg_t *arg))
{
  if (ch > CANCTRL_MAX_CH)
    return;

  can_tbl[ch].handler = handler;
}

void canctrlDetachRxInterrupt(uint8_t ch)
{
  if (ch > CANCTRL_MAX_CH)
    return;

  can_tbl[ch].handler = NULL;
}

void canctrlRecovery(uint8_t ch)
{
  if (ch > CANCTRL_MAX_CH)
    return;

  can_tbl[ch].recovery_cnt++;
}

bool canctrlUpdate(void)
{
  bool ret = false;
  canctrl_tbl_t *p_can;

  for (int i = 0; i < CANCTRL_MAX_CH; i++)
  {
    p_can = &can_tbl[i];

    canctrlErrUpdate(i);
    canctrlRxFifoUpdate(i);

    switch (p_can->state)
    {
    case 0:
      if (p_can->err_code & CANCTRL_ERR_BUS_OFF)
      {
        canctrlRecovery(i);
        p_can->state = 1;
        ret = true;
      }
      break;

    case 1:
      if ((p_can->err_code & CANCTRL_ERR_BUS_OFF) == 0)
      {
        p_can->state = 0;
      }
      break;
    }
  }

  return ret;
}

void canctrlRxFifoUpdate(uint8_t ch)
{
  canctrl_msg_t *rx_buf;
  mcp_msg_t rx_msg;

  if (mcp2515ReadMsg(can_tbl[ch].hfdcan, &rx_msg) != true)
  {
    return;
  }

  rx_buf = (canctrl_msg_t *)qbufferPeekWrite(&can_tbl[ch].q_msg);

  rx_buf->id = rx_msg.id;

  if (rx_msg.ext != true)
    rx_buf->id_type = CANCTRL_STD;
  else
    rx_buf->id_type = CANCTRL_EXT;

  rx_buf->length = dlc_len_tbl[rx_msg.dlc];
  rx_buf->dlc = canctrlGetDlc(rx_buf->length);
  rx_buf->frame = CANCTRL_CLASSIC;

  can_tbl[ch].rx_cnt++;

  if (qbufferWrite(&can_tbl[ch].q_msg, NULL, 1) != true)
  {
    can_tbl[ch].q_rx_full_cnt++;
  }

  if (can_tbl[ch].handler != NULL)
  {
    if ((*can_tbl[ch].handler)((void *)rx_buf) == true)
    {
      qbufferRead(&can_tbl[ch].q_msg, NULL, 1);
    }
  }
}

void canctrlErrClear(uint8_t ch)
{
  if (ch > CANCTRL_MAX_CH)
    return;

  can_tbl[ch].err_code = CANCTRL_ERR_NONE;
}

void canctrlErrPrint(uint8_t ch)
{
  uint32_t err_code;

  if (ch > CANCTRL_MAX_CH)
    return;

  err_code = can_tbl[ch].err_code;

  if (err_code & CANCTRL_ERR_PASSIVE)
    logPrintf("  ERR : CANCTRL_ERR_PASSIVE\n");
  if (err_code & CANCTRL_ERR_WARNING)
    logPrintf("  ERR : CANCTRL_ERR_WARNING\n");
  if (err_code & CANCTRL_ERR_BUS_OFF)
    logPrintf("  ERR : CANCTRL_ERR_BUS_OFF\n");
}

void canctrlErrUpdate(uint8_t ch)
{
  uint8_t protocol_status;

  protocol_status = mcp2515ReadErrorFlags(can_tbl[ch].hfdcan);

  if (protocol_status & (1 << 3) || protocol_status & (1 << 4))
  {
    can_tbl[ch].err_code |= CANCTRL_ERR_PASSIVE;
  }
  else
  {
    can_tbl[ch].err_code &= ~CANCTRL_ERR_PASSIVE;
  }

  if (protocol_status & (1 << 0))
  {
    can_tbl[ch].err_code |= CANCTRL_ERR_WARNING;
  }
  else
  {
    can_tbl[ch].err_code &= ~CANCTRL_ERR_WARNING;
  }

  if (protocol_status & (1 << 5))
  {
    can_tbl[ch].err_code |= CANCTRL_ERR_BUS_OFF;
  }
  else
  {
    can_tbl[ch].err_code &= ~CANCTRL_ERR_BUS_OFF;
  }

  if (protocol_status & (1 << 6))
  {
    can_tbl[ch].fifo_full_cnt++;
  }
}




void canctrlInfoPrint(uint8_t ch)
{
  canctrl_tbl_t *p_can = &can_tbl[ch];

  #ifdef _USE_HW_CLI
  #define canctrlPrintf   cliPrintf
  #else
  #define canctrlPrintf   logPrintf
  #endif

  canctrlPrintf("ch            : ");
  switch(ch)
  {
    case _DEF_CAN1:
      canctrlPrintf("_DEF_CAN1\n");
      break;
    case _DEF_CAN2:
      canctrlPrintf("_DEF_CAN2\n");
      break;
  }

  canctrlPrintf("is_open       : ");
  if (p_can->is_open)
    canctrlPrintf("true\n");
  else
    canctrlPrintf("false\n");

  canctrlPrintf("baud          : ");
  switch(p_can->baud)
  {
    case CANCTRL_100K:
      canctrlPrintf("100K\n");
      break;
    case CANCTRL_125K:
      canctrlPrintf("125K\n");
      break;
    case CANCTRL_250K:
      canctrlPrintf("250\n");
      break;
    case CANCTRL_500K:
      canctrlPrintf("250\n");
      break;
    case CANCTRL_1M:
      canctrlPrintf("1M\n");
      break;
    default:
      break;
  }

  canctrlPrintf("baud data     : ");
  switch(p_can->baud_data)
  {
    case CANCTRL_100K:
      canctrlPrintf("100K\n");
      break;
    case CANCTRL_125K:
      canctrlPrintf("125K\n");
      break;
    case CANCTRL_250K:
      canctrlPrintf("250\n");
      break;
    case CANCTRL_500K:
      canctrlPrintf("250\n");
      break;
    case CANCTRL_1M:
      canctrlPrintf("1M\n");
      break;

    case CANCTRL_2M:
      canctrlPrintf("2M\n");
      break;
    case CANCTRL_4M:
      canctrlPrintf("4M\n");
      break;
    case CANCTRL_5M:
      canctrlPrintf("5M\n");
      break;
  }

  canctrlPrintf("mode          : ");
  switch(p_can->mode)
  {
    case CANCTRL_NORMAL:
      canctrlPrintf("NORMAL\n");
      break;
    case CANCTRL_MONITOR:
      canctrlPrintf("MONITOR\n");
      break;
    case CANCTRL_LOOPBACK:
      canctrlPrintf("LOOPBACK\n");
      break;
  }

  canctrlPrintf("frame         : ");
  switch(p_can->frame)
  {
    case CANCTRL_CLASSIC:
      canctrlPrintf("CAN_CLASSIC\n");
      break;
    case CANCTRL_FD_NO_BRS:
      canctrlPrintf("CAN_FD_NO_BRS\n");
      break;
    case CANCTRL_FD_BRS:
      canctrlPrintf("CAN_FD_BRS\n");
      break;
  }
}


#ifdef _USE_HW_CLI
void cliCanCtrl(cli_args_t *args)
{
  bool ret = false;

   canctrlLock();

  if (args->argc == 1 && args->isStr(0, "info"))
  {
    for (int i = 0; i < CANCTRL_MAX_CH; i++)
    {
      if (can_tbl[i].is_open == true)
      {
        canctrlInfoPrint(i);
        cliPrintf("is_open       : %d\n", can_tbl[i].is_open);

        cliPrintf("q_rx_full_cnt : %d\n", can_tbl[i].q_rx_full_cnt);
        cliPrintf("q_tx_full_cnt : %d\n", can_tbl[i].q_tx_full_cnt);
        cliPrintf("fifo_full_cnt : %d\n", can_tbl[i].fifo_full_cnt);
        cliPrintf("fifo_lost_cnt : %d\n", can_tbl[i].fifo_lost_cnt);
        cliPrintf("rx error cnt  : %d\n", canctrlGetRxErrCount(i));
        cliPrintf("tx error cnt  : %d\n", canctrlGetTxErrCount(i));
        canctrlErrPrint(i);
        cliPrintf("\n");
      }
      else
      {
        cliPrintf("%d not open\n", i);
        cliPrintf("\n");
      }
    }
    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "open"))
  {
    cliPrintf("ch    : 0~%d\n\n", CANCTRL_MAX_CH - 1);
    cliPrintf("mode  : CAN_NORMAL\n");
    cliPrintf("        CAN_MONITOR\n");
    cliPrintf("        CAN_LOOPBACK\n\n");
    cliPrintf("frame : CAN_CLASSIC\n");
    cliPrintf("        CAN_FD_NO_BRS\n");
    cliPrintf("        CAN_FD_BRS\n\n");
    cliPrintf("baud  : CAN_100K\n");
    cliPrintf("        CAN_125K\n");
    cliPrintf("        CAN_250K\n");
    cliPrintf("        CAN_500K\n");
    cliPrintf("        CAN_1M\n");
    cliPrintf("        CAN_2M\n");
    cliPrintf("        CAN_4M\n");
    cliPrintf("        CAN_5M\n");
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "open") && args->isStr(1, "test"))
  {
    bool can_ret;

    can_ret = canctrlOpen(_DEF_CAN1, CANCTRL_NORMAL, CANCTRL_CLASSIC, CANCTRL_1M, CANCTRL_1M);
    cliPrintf("canOpen() : %s\n", can_ret ? "True" : "False");
    canctrlInfoPrint(_DEF_CAN1);
    ret = true;
  }
  
  if (args->argc == 6 && args->isStr(0, "open"))
  {
    uint8_t ch;
    canctrl_mode_t mode = CANCTRL_NORMAL;
    canctrl_frame_t frame = CANCTRL_CLASSIC;
    canctrl_baud_t baud = CANCTRL_1M;
    canctrl_baud_t baud_data = CANCTRL_1M;
    const char *mode_str[]  = {"CLAN_NORMAL", "CAN_MONITOR", "CAN_LOOPBACK"};
    const char *frame_str[] = {"CAN_CLASSIC"};
    const char *baud_str[]  = {"CAN_100K", "CAN_125K", "CAN_250K", "CAN_500K", "CAN_1M", "CAN_2M", "CAN_4M", "CAN_5M"};

    ch = constrain(args->getData(1), 0, CANCTRL_MAX_CH - 1);

    for (int i=0; i<3; i++)
    {
      if (args->isStr(2, mode_str[i]))
      {
        mode = i;
        break;
      }
    }
    for (int i=0; i<3; i++)
    {
      if (args->isStr(3, frame_str[i]))
      {
        frame = i;
        break;
      }
    }
    for (int i=0; i<8; i++)
    {
      if (args->isStr(4, baud_str[i]))
      {
        baud = i;
        break;
      }
    }
    for (int i=0; i<8; i++)
    {
      if (args->isStr(5, baud_str[i]))
      {
        baud_data = i;
        break;
      }
    }

    bool can_ret;

    can_ret = canctrlOpen(ch, mode, frame, baud, baud_data);
    cliPrintf("canctrlOpen() : %s\n", can_ret ? "True":"False");
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "read_test"))
  {
    uint32_t index = 0;
    uint8_t ch;

    ch = constrain(args->getData(1), 0, CANCTRL_MAX_CH - 1);

    while (cliKeepLoop())
    {
      canctrlUpdate();

      if (canctrlMsgAvailable(ch))
      {
        canctrl_msg_t msg;

        canctrlMsgRead(ch, &msg);

        index %= 1000;
        cliPrintf("%03d(R) <- id ", index++);
        if (msg.id_type == CANCTRL_STD)
        {
          cliPrintf("std ");
        }
        else
        {
          cliPrintf("ext ");
        }
        cliPrintf(": 0x%08X, L:%02d, ", msg.id, msg.length);
        for (int i = 0; i < msg.length; i++)
        {
          cliPrintf("0x%02X ", msg.data[i]);
        }
        cliPrintf("\n");
      }
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "send_test"))
  {
    uint32_t pre_time = 0;
    uint32_t index = 0;
    uint32_t err_code;
    uint8_t ch;
    canctrl_frame_t frame = CANCTRL_CLASSIC;

    ch = constrain(args->getData(1), 0, CANCTRL_MAX_CH - 1);

    err_code = can_tbl[_DEF_CAN1].err_code;

    while (cliKeepLoop())
    {
      canctrl_msg_t msg;

      if (millis() - pre_time >= 1000)
      {
        pre_time = millis();

        msg.frame = frame;
        msg.id_type = CANCTRL_EXT;
        msg.dlc = CANCTRL_DLC_2;
        msg.id = 0x314;
        msg.length = 2;
        msg.data[0] = 1;
        msg.data[1] = 2;
        if (canctrlMsgWrite(ch, &msg, 10) > 0)
        {
          index %= 1000;
          cliPrintf("%03d(T) -> id ", index++);
          if (msg.id_type == CANCTRL_STD)
          {
            cliPrintf("std ");
          }
          else
          {
            cliPrintf("ext ");
          }
          cliPrintf(": 0x%08X, L:%02d, ", msg.id, msg.length);
          for (int i = 0; i < msg.length; i++)
          {
            cliPrintf("0x%02X ", msg.data[i]);
          }
          cliPrintf("\n");
        }
        else
        {
          cliPrintf("err %d \n", mcp2515ReadErrorFlags(can_tbl[ch].hfdcan));
        }

        if (canctrlGetRxErrCount(ch) > 0 || canctrlGetTxErrCount(ch) > 0)
        {
          cliPrintf("ErrCnt : %d, %d\n", canctrlGetRxErrCount(ch), canctrlGetTxErrCount(ch));
        }

        if (err_int_cnt > 0)
        {
          cliPrintf("Cnt : %d\n", err_int_cnt);
          err_int_cnt = 0;
        }
      }

      if (can_tbl[ch].err_code != err_code)
      {
        cliPrintf("ErrCode : 0x%X\n", can_tbl[ch].err_code);
        canctrlErrPrint(ch);
        err_code = can_tbl[ch].err_code;
      }

      if (canctrlUpdate())
      {
        cliPrintf("BusOff Recovery\n");
      }

      if (canctrlMsgAvailable(ch))
      {
        canctrlMsgRead(ch, &msg);

        index %= 1000;
        cliPrintf("%03d(R) <- id ", index++);
        if (msg.id_type == CANCTRL_STD)
        {
          cliPrintf("std ");
        }
        else
        {
          cliPrintf("ext ");
        }
        cliPrintf(": 0x%08X, L:%02d, ", msg.id, msg.length);
        for (int i = 0; i < msg.length; i++)
        {
          cliPrintf("0x%02X ", msg.data[i]);
        }
        cliPrintf("\n");
      }
    }
    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("canctrl info\n");
    cliPrintf("canctrl open\n");
    cliPrintf("canctrl open ch[0~%d] mode frame baud fd_baud\n", CANCTRL_MAX_CH - 1);
    cliPrintf("canctrl open test\n");
    cliPrintf("canctrl read_test ch[0~%d]\n", CANCTRL_MAX_CH - 1);
    cliPrintf("canctrl send_test ch[0~%d] can:fd\n", CANCTRL_MAX_CH - 1);
  }
}
#endif

#endif
