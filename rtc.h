#ifndef __rtc_h
#define __rtc_h

#include "cmsis_lib/include/stm32f4xx_gpio.h"
#include "cmsis_lib/include/stm32f4xx_rtc.h"
#include "cmsis_lib/include/stm32f4xx_rcc.h"
#include "cmsis_lib/include/stm32f4xx_pwr.h"

#include "cmsis_lib/include/stm32f4xx_tim.h"
#include <stdio.h>


RTC_InitTypeDef myclockInitTypeStruct;		//3 structures needed to set RTC
RTC_TimeTypeDef myclockTimeStruct;
RTC_DateTypeDef myclockDateStruct;


void rtcConfig(void);
void printTime();


#endif //__rtc_h
