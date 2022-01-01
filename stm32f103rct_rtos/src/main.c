/*
 * main.c
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */


#include "main.h"
#include "tickTimer.h"




static void mainThread(void const *argument);

uint8_t failTimer;

int main(void)
{
  bspInit();
  ledOff(_DEF_LED2);
  osThreadDef(mainThread, mainThread, _HW_DEF_RTOS_THREAD_PRI_MAIN, 0, _HW_DEF_RTOS_THREAD_MEM_MAIN);
  if (osThreadCreate(osThread(mainThread), NULL) == NULL)
  {
    /* fail create thread*/
    ledInit();
    failTimer = tickTimer_Start();
    while(1)
    {
      if (tickTimer_MoreThan(failTimer, 100))
      {
        ledToggle(_DEF_LED2);
        failTimer = tickTimer_Start();
      }
    }
  }

  osKernelStart();

  return 0;
}


void mainThread(void const *argument)
{
  UNUSED(argument);

  hwInit();
  moduleInit();
  apInit();
  apMain();

}
