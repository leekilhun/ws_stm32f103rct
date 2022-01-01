/*
 * module.c
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */




#include "module.h"


bool moduleInit(void)
{
  bool ret = false;

  ret =  w25qxx_Init();

  logBoot(false);

  return ret;
}
