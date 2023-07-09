/*
 * ap.cpp
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 */

#include "ap.hpp"



uint32_t counter_value = 0;
uint32_t direction = 0;
float pulse_frequency = 0.0;



//extern TIM_HandleTypeDef htim4;

void  apInit(void)
{
  ledInit();
  pwmInit();
  uartInit();

  //HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
  wheelInit();
}


#if 0

void viewLog()
{
  uartPrintf(_DEF_CH1, "Direction: %s\n", direction ? "Clockwise" : "Counterclockwise");
  uartPrintf(_DEF_CH1, "Pulse Counter: %lu\n", counter_value);
  //printf("Pulse Frequency: %.2f Hz\n", pulse_frequency);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4)
  {
    counter_value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    direction = htim->Instance->CR1 & TIM_CR1_DIR;

    // Calculate pulse frequency
    float timer_frequency = HAL_RCC_GetPCLK1Freq() / (htim->Instance->PSC + 1);
    pulse_frequency = (float)timer_frequency / counter_value;
  }

  viewLog();
}


#endif


void  apMain(void)
{
  uint32_t pre_time;

  ledSetDuty(_DEF_CH1, 100);
  pre_time = millis();

  //HAL_TIM_Base_Start(&htim4);
  //uint8_t text[] = "Hello World ! \n\r";
  while (1)
  {

    if (millis()-pre_time >= 1000)
    {
      pre_time = millis();
      //uartWrite(_DEF_CH1, text, sizeof(text));
      ledToggle(_DEF_CH1);
      //wheelCntView();
    }



#if 0
    if (uartAvailable(_DEF_CH1) > 0)
    {
      uint8_t rx_data;

      rx_data = uartRead(_DEF_CH1);
      uartPrintf(_DEF_CH1, "rx data : 0x%02X (%c)\n", rx_data, rx_data);
    }

#endif
   }

}


