/*
 * ap.c
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */



#include "ap.h"
#include "tickTimer.h"


uint8_t runTimer;


void apInit(void)
{
  runTimer = tickTimer_Start();

  //uartOpen(_DEF_UART1, 115200);
  cliOpen(_DEF_UART1, 115200);
  canOpen(_DEF_CAN1, CAN_NORMAL, CAN_CLASSIC, CAN_500K, CAN_500K);
  ledOff(_DEF_LED1);
}

void apMain(void)
{
  while(1)
  {
    if (tickTimer_MoreThan(runTimer, 1000))
    {
      ledToggle(_DEF_LED1);
      runTimer = tickTimer_Start();
    }

    cliMain();
    //count = tickTimer_GetElaspTime(lcdTimer);
   /* if (uartAvailable(_DEF_UART1) > 0 )
    {
      uint8_t rx_data ;
      rx_data = uartRead(_DEF_UART1);
      uartPrintf(_DEF_UART1, "Uart1 Rx %c %x\n",rx_data,rx_data);
    }*/

  }
}

