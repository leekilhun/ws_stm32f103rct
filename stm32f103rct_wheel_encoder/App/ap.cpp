/*
 * ap.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "ap.hpp"








void  apInit(void)
{
  ledInit();
  pwmInit();
  uartInit();
  wheelInit();
}
void  apMain(void)
{
  uint32_t pre_time;

  ledSetDuty(_DEF_CH1, 100);
  pre_time = millis();

  //uint8_t text[] = "Hello World ! \n\r";
  while (1)
  {

    if (millis()-pre_time >= 500)
    {
      pre_time = millis();
      //uartWrite(_DEF_CH1, text, sizeof(text));
      ledToggle(_DEF_CH1);
      wheelCntView();
    }


    if (uartAvailable(_DEF_CH1) > 0)
    {
      uint8_t rx_data;

      rx_data = uartRead(_DEF_CH1);
      uartPrintf(_DEF_CH1, "rx data : 0x%02X (%c)\n", rx_data, rx_data);
    }


   }

}


