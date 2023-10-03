/*
 * ap.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */


/**
 * 
 https://www.youtube.com/watch?v=yeVCuAdRnZU&t=252s&ab_channel=SteppeSchool 
 
*/
#include "ap.hpp"

#ifdef _USE_HW_CLI
static void cliApp(cli_args_t *args);
#endif

static void apISR_1ms(void *arg);
static void apISR_10ms(void *arg);

/* TIM4 정보 */
#define TIM4_PULSE_PER_REVOLUTION 100 // 1회전 당 펄스 발생 수
#define TIM4_CLK_FREQ 72000000        // TIM4 클럭 주파수
#define SPEED_CALC_INTERVAL 1000      // 속도 계산 주기 (us)
#define SPEED_UNIT_CONVERSION 60      // RPM으로 변환하기 위한 상수
extern TIM_HandleTypeDef htim4;

/* TIM4 캡처 정보 */
volatile uint32_t TIM4_capture_count = 0;      // TIM4 입력 캡처 카운트
volatile uint32_t TIM4_capture_value = 0;      // TIM4 입력 캡처 값
volatile uint32_t TIM4_prev_capture_value = 0; // 이전 TIM4 입력 캡처 값
volatile uint32_t TIM4_pulse_count = 0;        // TIM4 펄스 발생 수
volatile uint32_t TIM4_direction = 0;          // TIM4 방향
volatile uint32_t TIM4_speed = 0;              // TIM4 속도
volatile uint32_t wheel_cnt = 0;

int16_t encoder_velocity;
int32_t encoder_position;
uint16_t timer_counter;

void apInit(void)
{

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);

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

  // HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
  // HAL_TIM_Base_Start_IT(&htim4);

#ifdef _USE_HW_CLI
  cliAdd("app", cliApp);
#endif

#ifdef _USE_HW_CLI
  cliOpen(HW_CLI_CH, 115200);
#endif
}

void apMain(void)
{
  LOG_PRINT("[OK] App Wheel Encoder Test");
  uint32_t pre_time;
  uint16_t pre_value = 0;
  pre_time = millis();
  while (1)
  {
    if (millis() - pre_time >= 1'000)
    {
      pre_time = millis();
      if (pre_value != timer_counter)
      {
        pre_value = timer_counter;
        LOG_PRINT("timer_counter : %d", timer_counter);
      }
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4) // TIM4에서 인터럽트가 발생한 경우
  {
    timer_counter = __HAL_TIM_GET_COUNTER(htim);
  }
  //HAL_TIM_Base_Start_IT(&htim4);
  //HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 |TIM_CHANNEL_2);
}

/* TIM4 입력 캡처 인터럽트 핸들러 함수 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM4) // TIM4에서 인터럽트가 발생한 경우
  {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // TIM4 채널 1에서 인터럽트가 발생한 경우
    {
      TIM4_capture_value = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_1); // TIM4 채널 1의 캡처 값 읽기
      if (TIM4_capture_value > TIM4_prev_capture_value)                      // TIM4 방향이 정방향인 경우
      {
        TIM4_direction = 1;
        wheel_cnt++;
      }
      else // TIM4 방향이 역방향인 경우
      {
        TIM4_direction = -1;
        if (wheel_cnt > 1)
          wheel_cnt--;
      }

      TIM4_pulse_count++;                           // TIM4 펄스 발생 수 증가
      TIM4_prev_capture_value = TIM4_capture_value; // 이전 TIM4 입력 캡처 값 저장

      // print out
      LOG_PRINT("TIM4 Channel 1 Captured Value: %d, Direction: %d",  TIM4_capture_value, TIM4_direction);
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) // TIM4 채널 2에서 인터럽트가 발생한 경우
    {
      TIM4_capture_value = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2); // TIM4 채널 2의 캡처 값 읽기

      uint32_t time_diff = TIM4_capture_value - TIM4_prev_capture_value;                                           // TIM4 펄스 간 시간 차이 계산
      TIM4_speed = (uint32_t)((float)TIM4_PULSE_PER_REVOLUTION / (float)time_diff * (float)TIM4_CLK_FREQ / 60.0f); // TIM4 속도 계산 (rpm)

      TIM4_prev_capture_value = TIM4_capture_value; // 이전 TIM4 입력 캡처 값 저장

      // print out
       LOG_PRINT("TIM4 Channel 2 Captured Value: %d, Direction: %d",  TIM4_capture_value, TIM4_speed);
    }
  }
}

void apISR_1ms(void *arg)
{
}

void apISR_10ms(void *arg)
{
  updateLED();
  timer_counter = __HAL_TIM_GET_COUNTER(&htim4);
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
