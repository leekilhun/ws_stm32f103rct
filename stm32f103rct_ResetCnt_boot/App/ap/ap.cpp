/*
 * ap.c
 *
 *  Created on: Dec 27, 2021
 *      Author: gns2.lee
 */


/*

  boot sequence
  . use uart_2 port

  1. reset button count

 */

#include "ap.hpp"
#include "boot/boot.h"

#if 0

// usb cli test
void apInit(void)
{

#ifdef _USE_HW_CLI
  cliOpen(HW_CMD_CH, 115200);
#endif




}




void apMain(void)
{
  uint32_t pre_time;
  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 500)
    {
      //logPrintf("ResecCount : %d\n", (int)resetGetCount());
      pre_time = millis();
      HAL_GPIO_TogglePin(status_GPIO_Port, status_Pin);

    }
#ifdef _USE_HW_CLI
    cliMain();
#endif

  }
}


#endif

#if 1
cmd_t cmd;


void apInit(void)
{

  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  cmdInit(&cmd);
  cmdOpen(&cmd, HW_CMD_CH, 115200);

#ifdef _USE_HW_CLI
  cliOpen(HW_CLI_CH, 115200);
#endif

  bool run_boot = false;
  uint8_t reg;

  reg = rtcBackupRegRead(0);

  if (reg & (1<<0))
  {
    run_boot = true;
    reg &= ~(1<<0);
    rtcBackupRegWrite(0, reg);
  }


  if (resetGetCount() == 3)
  {
    run_boot = true;
  }


  if (run_boot == false)
  {
    logPrintf("bootVerifyFw[%d] ,bootVerifyCrc[%d] \n",bootVerifyFw(),bootVerifyCrc());
    if (bootVerifyFw() == true && bootVerifyCrc() == true)
    {
      bootJumpToFw();
    }
  }



#if 0
  uint32_t pre_ms=millis();

  /*
   * press the start button and hold the boot fw entered state for 2 seconds,
   * it enters boot mode and waits for download for 5 seconds.
   */
  bool keep_loop = gpioPinRead(_GPIO_OP_SW_START);
  bool jump_fw= false;

  while(keep_loop)
  {
    if ((millis() - pre_ms) > 1000*2)
    {
      //enter boot mode
      if (gpioPinRead(_GPIO_OP_SW_START)){
        jump_fw = false;
      }
      break;
    }
  }


  if (jump_fw)
  {
    if (bootVerifyFw() == true && bootVerifyCrc() == true)
    {
      bootJumpToFw();
    }
  }

  ledOff(_DEF_LED1);
#endif



}




void apMain(void)
{
  uint32_t pre_time;
  pre_time = millis();
  while(1)
  {
    if (millis()-pre_time >= 150)
    {
      //logPrintf("ResecCount : %d\n", (int)resetGetCount());
      pre_time = millis();
      HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);

    }
#ifdef _USE_HW_CLI
//    cliMain();
#endif

    if (cmdReceivePacket(&cmd) == true)
    {
      bootProcessCmd(&cmd);
    }
  }
}


#endif

