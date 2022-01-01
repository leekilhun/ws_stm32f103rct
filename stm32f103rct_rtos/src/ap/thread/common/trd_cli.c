/*
 * trd_cli.c
 *
 *  Created on: Dec 31, 2021
 *      Author: gns2l
 */



#include "trd_cli.h"
#include "tickTimer.h"

#if 1

static const char *thread_name = "cli         ";
static thread_t *thread = NULL;

uint8_t  cli_ch     = _UART_CLI;
uint32_t cli_baud   = 115200;

static void cliThread(void const *argument);


bool trd_cliThreadInit(thread_t *p_thread)
{
  bool ret = false;

  thread = p_thread;

  thread->name = thread_name;


  osThreadDef(cliThread, cliThread, _HW_DEF_RTOS_THREAD_PRI_CLI, 0, _HW_DEF_RTOS_THREAD_MEM_CLI);
  if (osThreadCreate(osThread(cliThread), NULL) != NULL)
  {
    ret = true;
    logPrintf("cliThread  \t\t: OK\r\n");
  }
  else
  {
    logPrintf("cliThread  \t\t: Fail\r\n");
  }

  p_thread->is_init = ret;

  return ret;
}

void cliThread(void const *argument)
{
  (void)argument;


  cliOpen(cli_ch, cli_baud);
  //canOpen(_DEF_CAN1, CAN_NORMAL, CAN_CLASSIC, CAN_1M, CAN_2M);
  //canOpen(_DEF_CAN2, CAN_NORMAL, CAN_CLASSIC, CAN_1M, CAN_2M);
  ledOff(_DEF_LED2);
  uint8_t cliTimer = tickTimer_Start();
  while(1)
  {
    if (tickTimer_MoreThan(cliTimer, 100))
    {
      ledToggle(_DEF_LED2);
      cliTimer = tickTimer_Start();
    }

    cliMain();
    delay(5);
    thread->hearbeat++;
  }
}

#endif
