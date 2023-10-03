/*
 * wheel.c
 *
 *  Created on: Apr 23, 2023
 *      Author: gns2l
 *
 *
 *  https://m.blog.naver.com/rlackd93/222449667379
 */


#include "wheel.h"
#include "uart.h"

#if 1

/* TIM4 정보 */
#define TIM4_PULSE_PER_REVOLUTION  100   // 1회전 당 펄스 발생 수
#define TIM4_CLK_FREQ              72000000 // TIM4 클럭 주파수
#define SPEED_CALC_INTERVAL        1000 // 속도 계산 주기 (us)
#define SPEED_UNIT_CONVERSION      60 // RPM으로 변환하기 위한 상수

extern TIM_HandleTypeDef htim4;


/* TIM4 캡처 정보 */
volatile uint32_t TIM4_capture_count = 0;     // TIM4 입력 캡처 카운트
volatile uint32_t TIM4_capture_value = 0;     // TIM4 입력 캡처 값
volatile uint32_t TIM4_prev_capture_value = 0;// 이전 TIM4 입력 캡처 값
volatile uint32_t TIM4_pulse_count = 0;       // TIM4 펄스 발생 수
volatile uint32_t TIM4_direction = 0;         // TIM4 방향
volatile uint32_t TIM4_speed = 0;             // TIM4 속도
volatile uint32_t wheel_cnt = 0;


volatile uint32_t timer_cnt = 0;

int16_t encoder_velocity;
int32_t encoder_position;
uint16_t timer_counter;


typedef struct {
  int16_t velocity;
  int64_t position;
  uint32_t last_counter_value;
}encoder_instance;

encoder_instance enc_instance = {};

static void update_encoder(encoder_instance* encoder_value, TIM_HandleTypeDef *htim);
static void reset_encoder(encoder_instance* encoder_value);

bool wheelInit()
{

  reset_encoder(&enc_instance);

  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1 |TIM_CHANNEL_2);
  HAL_TIM_Base_Start_IT(&htim4);
  //HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

  //HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1); // TIM4 채널 1 캡처 인터럽트 시작
  //HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2); // TIM4 채널 2 캡처 인터럽트 시작

  return true;
}

void update_encoder(encoder_instance* encoder_value, TIM_HandleTypeDef *htim)
{
  uint32_t temp_counter = __HAL_TIM_GET_COUNTER(&htim4);
  static uint8_t first_time = 0;
  if (!first_time)
  {
    encoder_value ->velocity = 0;
    first_time = 1;
  }
  else
  {
    if (temp_counter == encoder_value ->last_counter_value)
    {

    }
    else if (temp_counter > encoder_value ->last_counter_value)
    {
      if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
      {
        encoder_value->velocity = -encoder_value->last_counter_value - (__HAL_TIM_GET_AUTORELOAD(htim)-temp_counter);

      }
      else
      {
        encoder_value->velocity  = temp_counter - encoder_value->last_counter_value;
      }

    }
    else
    {
      if(__HAL_TIM_IS_TIM_COUNTING_DOWN(htim))
      {
        encoder_value->velocity = temp_counter -encoder_value->last_counter_value;
      }
      else
      {
        encoder_value->velocity = temp_counter + (__HAL_TIM_GET_AUTORELOAD(htim) - encoder_value->last_counter_value);
      }

    }

  }
  encoder_value->position += encoder_value->velocity;
  encoder_value->last_counter_value = temp_counter;

}
void reset_encoder(encoder_instance* encoder_value)
{
  encoder_value->position = 0;
  encoder_value->velocity = 0;
  encoder_value->last_counter_value = 0;
}



void wheelCntView()
{
  uartPrintf(_DEF_CH1,  "Wheel TIM4 CH1,2 - timer_cnt %d,pos %d, vel %d \n", timer_counter, encoder_position, encoder_velocity);

//  uartPrintf(_DEF_CH1, "Wheel TIM4 CH1,2 - Pulse cnt %lu, Dir %d, Speed %lurpm, Total wheel_cnt %d \n"
//             ,  TIM4_pulse_count, TIM4_direction, TIM4_speed, wheel_cnt);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  //timer_cnt =  __HAL_TIM_GET_COUNTER(&htim4);
  timer_counter = __HAL_TIM_GET_COUNTER(&htim4);
  update_encoder(&enc_instance,&htim4);
  encoder_position = enc_instance.position;
  encoder_velocity = enc_instance.velocity;
  //wheelCntView();

  HAL_TIM_Base_Start_IT(&htim4);
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
      if (TIM4_capture_value > TIM4_prev_capture_value) // TIM4 방향이 정방향인 경우
      {
        TIM4_direction = 1;
        wheel_cnt++;
      }
      else // TIM4 방향이 역방향인 경우
      {
        TIM4_direction = -1;
        if ( wheel_cnt > 1)
          wheel_cnt--;
      }

      TIM4_pulse_count++; // TIM4 펄스 발생 수 증가
      TIM4_prev_capture_value = TIM4_capture_value; // 이전 TIM4 입력 캡처 값 저장

      // print out
      //uartPrintf(_DEF_CH1, "TIM4 Channel 1 Captured Value: %d, Direction: %d\n",  TIM4_capture_value, TIM4_direction);
    }
    else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) // TIM4 채널 2에서 인터럽트가 발생한 경우
    {
      TIM4_capture_value = HAL_TIM_ReadCapturedValue(&htim4, TIM_CHANNEL_2); // TIM4 채널 2의 캡처 값 읽기

      uint32_t time_diff = TIM4_capture_value - TIM4_prev_capture_value; // TIM4 펄스 간 시간 차이 계산
      TIM4_speed = (uint32_t)((float)TIM4_PULSE_PER_REVOLUTION / (float)time_diff * (float)TIM4_CLK_FREQ / 60.0f); // TIM4 속도 계산 (rpm)

      TIM4_prev_capture_value = TIM4_capture_value; // 이전 TIM4 입력 캡처 값 저장

      // print out
      //uartPrintf(_DEF_CH1, "TIM4 Channel 2 Captured Value: %d, Direction: %d\n",  TIM4_capture_value, TIM4_speed);
    }
  }
}

#endif
