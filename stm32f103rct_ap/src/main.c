/*
 * main.c
 *
 *  Created on: 2021. 12. 9.
 *      Author: gns2l
 */


#include "main.h"





int main(void)
{
  hwInit();
  moduleInit();
  apInit();
  apMain();

  return 0;
}
