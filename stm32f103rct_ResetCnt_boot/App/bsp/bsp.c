/*
 * bsp.c
 *
 *  Created on: Aug 26, 2023
 *      Author: gns2.lee
 */






#include "bsp.h"
#include "uart.h"
#include "utils.h"
#include "usb.h"



bool bspInit(void)
{

  return true;
}


void bspDeInit(void)
{
	//usb port initial

  usbDeInit();
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_Delay(200);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}

  //usbDeInit();
  HAL_RCC_DeInit();

  // Disable Interrupts
  //
  for (int i=0; i<8; i++)
  {
    NVIC->ICER[i] = 0xFFFFFFFF;
    __DSB();
    __ISB();
  }
  SysTick->CTRL = 0;
}



void delay(uint32_t time_ms)
{
  HAL_Delay(time_ms);
}


uint32_t millis(void)
{
  return HAL_GetTick();
}



#ifndef _USE_HW_LOG
void logPrintf(const char *fmt, ...)
{
#ifdef DEBUG
  va_list args;
  int len;
  char buf[256];

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  uartWrite(HW_LOG_CH, (uint8_t *)buf, len);

  va_end(args);

#endif

}
#endif




void logView(const char* file, const char* func, const int line, const char* fmt, ...)
{

#ifdef DEBUG
  const int ARG_TBL_CNT_MAX = 10;
  const uint8_t FILE_STR_MAX = 255;

  char tmp_str[FILE_STR_MAX];
  memset(tmp_str,0, FILE_STR_MAX);
  strcpy(tmp_str, file);
  char* arg_tbl[ARG_TBL_CNT_MAX];
  memset(arg_tbl,0, ARG_TBL_CNT_MAX);
  uint8_t arg_cnt = utilParseArgs(tmp_str, arg_tbl, "/\\", ARG_TBL_CNT_MAX);

  va_list args;
  char buf[256];
  va_start(args, fmt);
  vsnprintf(buf, 256, fmt, args);
  uartPrintf(HW_LOG_CH, "[%s][%s(%d)][%s] \n", arg_tbl[arg_cnt - 1], func, line, (const char*)buf);
  va_end(args);

#endif


}


