#ifndef __STM32F4xx_pwm_H
#define __STM32F4xx_pwm_H

#ifdef __cplusplus
 extern "C" {
#endif


 int PWM_TIMER_Init(int timerValue);
 int PWM_TIMER_Init3(int timerValue);
 int PWM_TIMER_Init8(int timerValue);
 void PWMTIM4_InitOC1(int duty);
 void PWMTIM4_InitOC2(int duty);
 void PWMTIM4_InitOC3(int duty);
 void PWMTIM4_InitOC4(int duty);

 void PWMTIM3_InitOC3(int duty);
 void PWMTIM3_InitOC4(int duty);

int genLookup(int buff[], float steps, int period);


#ifdef __cplusplus
}
#endif

#endif
