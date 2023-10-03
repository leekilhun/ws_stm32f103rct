/*
 * exhw.c
 *
 *  Created on: Aug 26, 2023
 *      Author: gns2.lee
 */

#include "exhw.h"

bool exhwInit(void)
{

#ifdef _USE_EXHW_LED
  ledInit();
#endif

#ifdef _USE_EXHW_BUTTON
  buttonInit();
#endif

#ifdef _USE_EXHW_AT24C32
  if (at24c32Init())
  {
    LOG_PRINT("at24c32Init OK");
  }
  else
  {
    LOG_PRINT("at24c32Init Fail");
  }
#endif

#ifdef _USE_EXHW_CANCTRL
  if (canctrlInit()) // included  mcp2515Init();
  {
    LOG_PRINT("canctrlInit OK");
  }
  else
  {
    LOG_PRINT("canctrlInit Fail");
  }
#endif

#ifdef _USE_EXHW_SD // use _USE_HW_FATFS
  if (sdInit())
  {
    LOG_PRINT("sdInit OK");
    if (fatfsInit())
    {
      LOG_PRINT("fatfsInit OK");
    }
    else
    {
      LOG_PRINT("fatfsInit Fail");
    }
  }
  else
  {
    LOG_PRINT("sdInit Fail");
  }
#endif

  return true;
}
