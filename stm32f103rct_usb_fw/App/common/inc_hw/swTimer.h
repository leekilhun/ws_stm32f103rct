/*
 * swTimer.h
 *
 *  Created on: Aug 27, 2023
 *      Author: gns2.lee
 */

#ifndef COMMON_INC_HW_SWTIMER_H_
#define COMMON_INC_HW_SWTIMER_H_



#ifdef __cplusplus
extern "C" {
#endif


#include "hw_def.h"

#ifdef _USE_HW_SWTIMER

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef NULL
#define NULL 0
#endif




#define ONE_TIME            1
#define LOOP_TIME           2



typedef int16_t             swtimer_handle_t;



bool swtimerInit(void);
void swtimerSet  (swtimer_handle_t TmrNum, uint32_t TmrData, uint8_t TmrMode, void (*Fnct)(void *),void *arg);
void swtimerStart(swtimer_handle_t TmrNum);
void swtimerStop (swtimer_handle_t TmrNum);
void swtimerReset(swtimer_handle_t TmrNum);
void swtimerISR(void);


swtimer_handle_t swtimerGetHandle(void);
uint32_t swtimerGetCounter(void);

#endif

#ifdef __cplusplus
}
#endif





#endif /* COMMON_INC_HW_SWTIMER_H_ */
