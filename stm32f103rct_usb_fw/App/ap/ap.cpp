/*
 * ap.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */



#include "ap.hpp"

#ifdef _USE_HW_CLI
static void cliApp(cli_args_t *args);
#endif


static void apISR_1ms(void *arg);
static void apISR_10ms(void *arg);



void apInit(void)
{

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_SET);

	/**
	 * @brief
	 * register the function to the 1ms interrupt sw-timer.
	 */
	{
		swtimer_handle_t timer_ch;
		timer_ch = swtimerGetHandle();
		if (timer_ch >= 0)
		{
			swtimerSet(timer_ch, 1, LOOP_TIME, apISR_1ms, NULL);
			swtimerStart(timer_ch);
		}
		else
		{
			LOG_PRINT("[NG] 1ms swtimerGetHandle()");
		}

		timer_ch = swtimerGetHandle();
		if (timer_ch >= 0)
		{
			swtimerSet(timer_ch, 10, LOOP_TIME, apISR_10ms, NULL);
			swtimerStart(timer_ch);
		}
		else
		{
			LOG_PRINT("[NG] 10ms swtimerGetHandle()");
		}
	}
	// end of swtimer

#ifdef _USE_HW_CLI
  cliAdd("app", cliApp);
#endif


#ifdef _USE_HW_CLI
	cliOpen(HW_CLI_CH, 115200);
#endif




}




void apMain(void)
{
	uint32_t pre_time;
	pre_time = millis();
	while(1)
	{
		if (millis()-pre_time >= 1'000)
		{
			pre_time = millis();
		}

#ifdef _USE_HW_CLI
		cliMain();
#endif

	}
}



void updateLED()
{
  /**
   * @brief
   * status led
   */
  {
    static uint32_t pre_time_update_led;

    if (millis() - pre_time_update_led >= 500)
    {
      pre_time_update_led = millis();
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    }
  }
  // end of status led

}



void apISR_1ms(void *arg)
{
}

void apISR_10ms(void *arg)
{
  updateLED();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _USE_HW_CLI

void cliApp(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1)
  {
    if (args->isStr(0, "info") == true)
    {
      cliPrintf("info \n");
      ret = true;
    }
    else if (args->isStr(0, "btn_info") == true)
    {
      cliPrintf("btn_info \n");
      // for (int i = 0; i < AP_OBJ::BTN_MAX; i++)
      // {
      //   cliPrintf("%-12s pin %d\n", btns[i].GetName(), btns[i].m_cfg.gpio_pin);
      // }
      ret = true;
    }
    else if (args->isStr(0, "btn_show") == true)
    {
      // while (cliKeepLoop())
      // {
      //   for (int i = 0; i < AP_OBJ::BTN_MAX; i++)
      //   {
      //     cliPrintf("%-12s pin[%d] : %d\n", btns[i].GetName(), btns[i].m_cfg.gpio_pin, btns[i].IsPressed());
      //   }
      //   delay(50);
      //   cliPrintf("\x1B[%dA", AP_OBJ::BTN_MAX);
      //   // cliPrintf("\r");
      // }
      ret = true;
    }
  }
  else if (args->argc == 2)
  {
  }
  else if (args->argc == 3)
  {
    if (args->isStr(0, "led") == true)
    {
      // uint8_t ch;
      // bool on_off;

      // ch = (uint8_t)args->getData(1);
      // on_off = (bool)args->getData(2);

      // if (ch < AP_OBJ::LED_MAX)
      // {
      //   if (on_off)
      //     leds[ch].On();
      //   else
      //     leds[ch].Off();
      //   cliPrintf("led ch[%d] %s \n", ch, on_off ? "On" : "Off");
      //   ret = true;
      // }
    }
  }
  else if (args->argc == 4)
  {
  }

  if (ret == false)
  {
    cliPrintf("app info\n");
    cliPrintf("app btn_info\n");
    cliPrintf("app btn_show\n");
    cliPrintf("app led [ch] [0:1 0 off, 1 on] \n");
  }
}
#endif




