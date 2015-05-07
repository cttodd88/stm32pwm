#include "../../cmsis_lib/include/stm32f4xx_pwm.h"
#include "../../cmsis_lib/include/stm32f4xx_gpio.h"
#include "../../cmsis_lib/include/stm32f4xx_rcc.h"
#include "../../cmsis_lib/include/stm32f4xx_tim.h"
#include "../../lcd/lcd.h"
#include <math.h>

#define pi 3.14159265

int genLookup(int buff[], float steps, int period){

	int n=0;

	int val=0;

	float angleStep=0;

	angleStep = 360/steps;




	for(n=0;n<=steps;n++){

        val= fabs(period*sin((angleStep*n)*(pi/180)));

		buff[n] = val;

	}


}



 int PWM_TIMER_Init(int timerValue){



	 TIM_TimeBaseInitTypeDef TIM_BaseStruct;

	 /* Enable clock for TIM4 */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	 TIM_BaseStruct.TIM_Prescaler = 0;
	    /* Count up */
	 TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	 TIM_BaseStruct.TIM_Period = timerValue; /* 10kHz PWM */
	 TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	 TIM_BaseStruct.TIM_RepetitionCounter = 0;
	 /* Initialize TIM4 */
	 TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
	 /* Start count on TIM4 */
	 TIM_Cmd(TIM4, ENABLE);
 }

 int PWM_TIMER_Init3(int timerValue){

	// TIM_Period = 84000000 / 20000 - 1 = 4199 1679

	 	 TIM_TimeBaseInitTypeDef TIM_BaseStruct;


	 	 /* Enable clock for TIM4 */
	 	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	 	 TIM_BaseStruct.TIM_Prescaler = 0;
	 	    /* Count up */
	 	 TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	 	 TIM_BaseStruct.TIM_Period = timerValue; /* 10kHz PWM */
	 	 TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	 	 TIM_BaseStruct.TIM_RepetitionCounter = 0;
	 	 /* Initialize TIM4 */
	 	 TIM_TimeBaseInit(TIM3, &TIM_BaseStruct);
	 	 /* Start count on TIM4 */
	 	 TIM_Cmd(TIM3, ENABLE);



 }




 void PWMTIM4_InitOC1(int duty){

	 TIM_OCInitTypeDef TIM_OCStruct;

	     /* Common settings */

	     /* PWM mode 2 = Clear on compare match */
	     /* PWM mode 1 = Set on compare match */
	     TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	     TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	     TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	 /*
	     To get proper duty cycle, you have simple equation

	     pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1

	     where DutyCycle is in percent, between 0 and 100%
		1% = 83
	     25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
	     50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
	     75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
	     100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399

	     Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
	 */
	     TIM_OCStruct.TIM_Pulse = duty; /* 25% duty cycle */
	     TIM_OC1Init(TIM4, &TIM_OCStruct);
	     TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

 }


 void PWMTIM4_InitOC2(int duty){

	 TIM_OCInitTypeDef TIM_OCStruct;


	 TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	 TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	 TIM_OCStruct.TIM_Pulse = duty;
	 TIM_OC2Init(TIM4, &TIM_OCStruct);
	 TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
 }

 void PWMTIM4_InitOC3(int duty){

	 TIM_OCInitTypeDef TIM_OCStruct;


	 TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	 TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;


	 TIM_OCStruct.TIM_Pulse = duty;
	 TIM_OC3Init(TIM4, &TIM_OCStruct);
	 TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
 }

 void PWMTIM4_InitOC4(int duty){

	 TIM_OCInitTypeDef TIM_OCStruct;


	 TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	 TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	 TIM_OCStruct.TIM_Pulse = duty;
	 TIM_OC4Init(TIM4, &TIM_OCStruct);
	 TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

 }

void PWMTIM3_InitOC1(int duty){

	 	 TIM_OCInitTypeDef TIM_OCStruct;


		 TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		 TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		 TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

		 TIM_OCStruct.TIM_Pulse = duty;
		 TIM_OC1Init(TIM3, &TIM_OCStruct);
		 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);


 }

void PWMTIM3_InitOC2(int duty){

	 	 TIM_OCInitTypeDef TIM_OCStruct;


		 TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		 TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		 TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

		 TIM_OCStruct.TIM_Pulse = duty;
		 TIM_OC2Init(TIM3, &TIM_OCStruct);
		 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);


 }


 void PWMTIM3_InitOC3(int duty){

	 	 TIM_OCInitTypeDef TIM_OCStruct;


		 TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		 TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		 TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

		 TIM_OCStruct.TIM_Pulse = duty;
		 TIM_OC3Init(TIM3, &TIM_OCStruct);
		 TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);


 }
 
  void PWMTIM3_InitOC4(int duty){

	 	 TIM_OCInitTypeDef TIM_OCStruct;


		 TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
		 TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
		 TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

		 TIM_OCStruct.TIM_Pulse = duty;
		 TIM_OC4Init(TIM3, &TIM_OCStruct);
		 TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);


 }

 