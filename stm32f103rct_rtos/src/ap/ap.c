/*
 * ap.c
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */



#include "ap.h"
#include "thread/thread.h"
#include "tickTimer.h"


uint8_t runTimer;
//static void threadLed(void const *argument);

void apInit(void)
{
  threadInit();
#if 0
  osThreadDef(threadLed, threadLed, _HW_DEF_RTOS_THREAD_PRI_LED, 0, _HW_DEF_RTOS_THREAD_MEM_LED);
   if (osThreadCreate(osThread(threadLed), NULL) != NULL)
   {
     logPrintf("threadLed \t\t: OK\r\n");
   }
   else
   {
     logPrintf("threadLed \t\t: Fail\r\n");
   }
#endif
}

void apMain(void)
{
  ledOff(_DEF_LED1);
  runTimer = tickTimer_Start();

  while(1)
  {
    if (tickTimer_MoreThan(runTimer, 1000))
    {
      ledToggle(_DEF_LED1);
      runTimer = tickTimer_Start();
    }

    //cliMain();
    //count = tickTimer_GetElaspTime(lcdTimer);
   /* if (uartAvailable(_DEF_UART1) > 0 )
    {
      uint8_t rx_data ;
      rx_data = uartRead(_DEF_UART1);
      uartPrintf(_DEF_UART1, "Uart1 Rx %c %x\n",rx_data,rx_data);
    }*/

    delay(2);

  }
}


#if 0
void threadLed(void const *argument)
{
  UNUSED(argument);
  runTimer = tickTimer_Start();
  while(1)
  {
    if (tickTimer_MoreThan(runTimer, 1000))
      {
        ledToggle(_DEF_LED1);
        runTimer = tickTimer_Start();
      }
  }
}

#endif


