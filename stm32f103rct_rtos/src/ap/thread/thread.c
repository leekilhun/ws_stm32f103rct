/*
 * thread.c
 *
 *  Created on: 2021. 12. 28.
 *      Author: gns2.lee
 */



#include "thread.h"
#include "common/info.h"
#include "common/trd_cli.h"
#include "display/trd_lcd.h"
#include "manage/can_bus.h"
#include "manage/cmd_bus.h"


#if 1

static const char *thread_name = "Dummy       ";

static thread_t thread_list[THREAD_ID_MAX];
static osMessageQId event_q;

static void threadEvent(void const *argument);



bool threadInit(void)
{
  bool ret = true;


  for (int i=0; i<THREAD_ID_MAX; i++)
  {
    thread_list[i].list = &thread_list[0];
    thread_list[i].name = thread_name;

    thread_list[i].is_init = false;

    thread_list[i].freq = 0;
    thread_list[i].hearbeat = 0;

    thread_list[i].init    = NULL;
    thread_list[i].notify  = threadNotify;
    thread_list[i].onEvent = NULL;
  }


  osMessageQDef(event_q, 16, uint32_t);
  event_q = osMessageCreate(osMessageQ(event_q), NULL);

  osThreadDef(threadEvent, threadEvent, _HW_DEF_RTOS_THREAD_PRI_EVENT, 0, _HW_DEF_RTOS_THREAD_MEM_EVENT);
  if (osThreadCreate(osThread(threadEvent), NULL) != NULL)
  {
    ret = true;
    logPrintf("threadEvent \t\t: OK\r\n");
  }
  else
  {
    logPrintf("threadEvent \t\t: Fail\r\n");
  }

  thread_list[THREAD_ID_INFO].init    = infoThreadInit;
  thread_list[THREAD_ID_CLI].init     = trd_cliThreadInit;
  #ifdef _USE_HW_LCD_DISPLAY
  thread_list[THREAD_ID_LCD].init     = lcdThreadInit;
  #endif
  //thread_list[THREAD_ID_LED].init = canBusThreadInit;
  //thread_list[THREAD_ID_CMD_BUS].init = cmdBusThreadInit;


  for (int i=0; i<THREAD_ID_MAX; i++)
  {
    if (thread_list[i].init != NULL)
    {
      ret &= thread_list[i].init(&thread_list[i]);
    }
  }

  return ret;
}

bool threadNotify(Event_t event)
{
  bool ret = true;
  osStatus status;

  status = osMessagePut(event_q, event, 10);
  if (status != osOK)
  {
    ret = false;
  }

  return ret;
}

void threadEvent(void const *argument)
{
  (void)argument;
  osEvent evt;

  while(1)
  {
    evt = osMessageGet(event_q, 100);
    if (evt.status == osEventMessage)
    {
      for (int i=0; i<THREAD_ID_MAX; i++)
      {
        if (thread_list[i].onEvent != NULL)
        {
          thread_list[i].onEvent((Event_t)evt.value.v);
        }
      }
    }
  }
}

#endif
