/******************************************************************************************************/
// Project prototyping and development for project quarter PWM driver and grid tie inverter
// Author: Chris Todd
// ID: C0304228
// Creation date: 3/10/2015
// Last updated:

#include "cmsis_boot/stm32f4xx.h"
#include "cmsis_lib/include/stm32f4xx_gpio.h"
#include "cmsis_lib/include/stm32f4xx_rcc.h"
#include "cmsis_lib/include/stm32f4xx_tim.h"
#include "cmsis_lib/include/stm32f4xx_pwm.h"
#include "cmsis_lib/include/misc.h"
//#include "lcd/lcd.h"

#include "inc/GUI.h"
#include "inc/DIALOG.h"
#include "inc/GUITDRV_ADS7846.h"

#include "user/LCD_STM32F4.h"
#include "user/stm32f4xx_it.h"
#include "user/pwm.h"
#include "user/ts.h"

#include "rtc.h"

#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef nvicStructure;

/* Private define ------------------------------------------------------------*/
#define GREEN  GPIOD, GPIO_Pin_12
#define ORANGE GPIOD, GPIO_Pin_13
#define RED GPIOD, GPIO_Pin_14
#define BLUE GPIOD, GPIO_Pin_15
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned int TimingDelay=0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 300;
uint16_t CCR2_Val = 100;

/* Private function prototypes -----------------------------------------------*/
void Delay(unsigned int nTime);
void TimingDelay_Decrement(void);

/* Private functions ---------------------------------------------------------*/


WM_HWIN CreateWindow(void);
WM_HWIN hDlg;

extern GUI_PID_STATE pstate;

int duty =0;
int index2 = 0;
int period=0;

//char *title = "PQ PWM C.T. T.L. A.C. F.F.";

int lookup[1601]={0};



int main(void)
{
    /*Initialization*/
    PROGBAR_Handle hProgbar;
    
    SystemInit();
        
    if(SysTick_Config(SystemCoreClock/1000)){
        while(1);
    }
              
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);                

    period=874;

	genLookup(lookup,1600,874);

	 /* Alternating functions for pins */
	//PWM_TIMER_Init(220);  //40Khz 2083
	PWM_TIMER_Init3(period); //500 KHz 166
	//PWM_TIMER_Init8(8399);
    /*
	PWM_InitOC1(83);
    PWM_InitOC2(4199);
    PWM_InitOC3(4199);
    PWM_InitOC4(83);
	*/
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
   // GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
   // GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
    //GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
    /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */


      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
   	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	 /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  //Configure LCD Peripherals
	  GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);


	  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	  nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
	  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  nvicStructure.NVIC_IRQChannelSubPriority = 1;
	  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&nvicStructure);

    Init_GPIO();
    Init_FSMC();
    
    /* Enable the CRC Module */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

    GUI_Init();

    Init_LCD();

    touch_init();

    pwm_init();
    //turn on lcd backlight
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);

    rtcConfig();

    GUI_SetFont(&GUI_Font8x16);
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();
    GUI_DispString("Elex268 Lab6 - RTC");
    //  GUI_DispString(GUI_GetVersionString());
    GUI_DispString("\n\n\n");
    GUI_DrawGradientH(5, 150, 315, 235, 0x0000FF, 0x00FFFF);

    PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX); // Sets the default skin for new widgets

    /* Create progress bar at location x = 10, y = 10, length = 219, height = 30 */
    hProgbar = PROGBAR_CreateEx(50, 180, 219, 30, 0, WM_CF_SHOW, 0, GUI_ID_PROGBAR0);
    /* Set progress bar font */
    PROGBAR_SetFont(hProgbar, &GUI_Font8x16);

    PROGBAR_SetMinMax(hProgbar, 0, 59);

    /* Set progress bar text */
    PROGBAR_SetText(hProgbar, "...");
    /*char *string;

    string="PQ PWM C.T. T.L. A.C. F.F.";

    lcdInit();

    lcd_puts(string);*/

    while(1)
    {
        int tempSeconds;
	char tempSecString[5];

        printTime();

        GUI_PID_GetCurrentState(&pstate);

        GUI_SetFont(&GUI_Font8x16);

        if (pstate.Pressed) {
            GUI_DispDecAt( pstate.x, 280,20,4);
            GUI_DispDecAt( pstate.y, 280,40,4);
            GUI_DispStringAt("-P-", 280,60);
        }
        else {
            GUI_DispDecAt( 0, 280,20,4);
            GUI_DispDecAt( 0, 280,40,4);
            GUI_DispStringAt("- -", 280,60);
        }

        //print progress bar displaying seconds
        tempSeconds=myclockTimeStruct.RTC_Seconds;	//RTC_Seconds in BCD
        tempSeconds=tempSeconds/16*10+tempSeconds%16;	//convert to decimal
        sprintf(tempSecString, "%02d", tempSeconds);	//convert dec value to string

        PROGBAR_SetValue(hProgbar, tempSeconds);		//use dec value for bar position
        PROGBAR_SetText(hProgbar, tempSecString);		//use string for value on progress bar

        GUI_Delay(50);
/*
    	while(duty<=8399){
    		PWM_InitOC4(duty);
    		duty++;
    		Delay(1);

    	}

    	while(duty>=0){
    		PWM_InitOC4(duty);
    		duty--;
    		Delay(1);
    	}
 */
/*

    	while(duty<=8399){

        if(state==0){

    	PWM_InitOC1(duty);

        }
        else if(state==1){

        	PWM_InitOC2(duty);
        }
        else if(state==2){

        	PWM_InitOC3(duty);
        }
        else if(state==3){

        	PWM_InitOC4(duty);
        }
        else{

        	state=0;
        }

    	duty++;
    	Delay(120);

    	}

    	while(duty>=0){

        if(state==0){

        	PWM_InitOC1(duty);

        }
        else if(state==1){

        	PWM_InitOC2(duty);

        }
        else if(state==2){

        	PWM_InitOC3(duty);

        }
        else if(state==3){

        	PWM_InitOC4(duty);

        }

        else{

        	state=0;

        }

    	duty--;
    	Delay(120);

    	}

    	state++;

    	if(state==4){

    		state=0;
    	}

    	Delay(250000);
*/
   /*GPIO_SetBits(GREEN);
    	 Delay(250);
    	 GPIO_ResetBits(GREEN);
    	 Delay(250);

    	 GPIO_SetBits(ORANGE);
    	 Delay(250);
    	 GPIO_ResetBits(ORANGE);
    	 Delay(250);

    	 GPIO_SetBits(RED);
    	 Delay(250);
    	 GPIO_ResetBits(RED);
    	 Delay(250);

    	 GPIO_SetBits(BLUE);
    	 Delay(250);
    	 GPIO_ResetBits(BLUE);
    	 Delay(250); */     
        
    }//END WHILE





}





void Delay(unsigned int nTime)
{

	 TimingDelay=nTime;
	 while(TimingDelay!=0);


}

/*The systick handler in stm32f4xx_it.c calls this function and
 *decrements the TimingDelay variable every millisecond */

// Conflict with display functions, confirmed unused - Taylor
/*
void TimingDelay_Decrement(void){
if(TimingDelay!=0x00){

TimingDelay--;
}

}

void SysTick_Handler(void) {
	TimingDelay_Decrement();
}
*/

void TIM3_IRQHandler(void){



	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	    {
	        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);


	if(index2<=800){

	PWMTIM3_InitOC4(lookup[index2]);

	}

	if(index2>=800){

	PWMTIM3_InitOC3(lookup[index2]);

	}



	index2++;

	if(index2==1601){

		index2=0;
	}

/*
	PWM_InitOC4(duty);

	if((duty<=period)&&(state==0)){

		if(duty!=period){
		duty++;
		}
		else{
		state=1;
		}
	}


	if((duty<=period)&&(state==1)){

		if(duty!=0){
		duty--;
		}
		else{
		state=0;
		}
	}


*/

  }
}



