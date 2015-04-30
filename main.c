/******************************************************************************************************/
// Project prototyping and development for project quarter PWM driver and grid tie inverter
// Author: Chris Todd
// ID: C0304228
// Creation date: 3/10/2015
// Last updated:


#include <stdio.h>
#include "cmsis_boot/stm32f4xx.h"
#include "cmsis_lib/include/stm32f4xx_gpio.h"
#include "cmsis_lib/include/stm32f4xx_rcc.h"
#include "cmsis_lib/include/stm32f4xx_tim.h"
#include "cmsis_lib/include/stm32f4xx_pwm.h"
#include "cmsis_lib/include/misc.h"
#include "cmsis_lib/include/stm32f4xx_adc.h"
#include "lcd/lcd.h"


/* Private typedef -----------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
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
unsigned int result=0;
char adcString[10]={0};
/* Private function prototypes -----------------------------------------------*/
void Delay(unsigned int nTime);
void TimingDelay_Decrement(void);

/* Private functions ---------------------------------------------------------*/


int duty =0;
int index2 = 0;
int period=0;

char *title = "PQ PWM C.T. T.L. A.C. F.F.";

int lookup[101]={0};



int main(void)
{

	SystemInit();
        
       

	if(SysTick_Config(SystemCoreClock/1000)){

		while(1);
	}
        
      
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        
        /*Frequency Calcs 
         * 
         * 1/((1/60)/(steps))
         * For 50 steps we get 3 kHz
         * TIM_Period = 84000000 / 3000 - 1 = 27999
         */

	period=13999;     

	genLookup(lookup,100,13999);
	
	PWM_TIMER_Init3(period); //500 KHz 166
	
        /*
	PWM_InitOC1(83);
        PWM_InitOC2(4199);
        PWM_InitOC3(4199);
        PWM_InitOC4(83);
	*/
        
        //ENABLE Alternate Pin Functions
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
        
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        
          //Reset ADC configurations
	ADC_DeInit();

	//Initialize common ADC settings
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	//Initialize specific ADC settings
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3,&ADC_InitStructure);

	ADC_RegularChannelConfig(ADC3,ADC_Channel_11,1,ADC_SampleTime_3Cycles);

	ADC_Cmd(ADC3, ENABLE);
	
          
     

   
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
          
          //Configure Nested Vector Interrupt Controller for ADC Interrupt

          ADC_ITConfig(ADC3,ADC_IT_EOC,ENABLE);
	  nvicStructure.NVIC_IRQChannel= ADC_IRQn;
	  nvicStructure.NVIC_IRQChannelCmd= ENABLE;
	  nvicStructure.NVIC_IRQChannelPreemptionPriority= 1;
	  nvicStructure.NVIC_IRQChannelSubPriority= 2;
	  NVIC_Init(&nvicStructure);


	  char *string;

	  string="PQ PWM C.T. T.L. A.C. F.F.";

	  lcdInit();

	  //lcd_puts(string);

          ADC_SoftwareStartConv(ADC3);

    while(1)
    {
        sprintf(adcString, "%d", result);
        lcd_puts(adcString);

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
         GPIO_SetBits(GREEN);
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
    	 Delay(250);      
        
    }//END WHILE





}





void Delay(unsigned int nTime)
{

	 TimingDelay=nTime;
	 while(TimingDelay!=0);


}

/*The systick handler in stm32f4xx_it.c calls this function and
 *decrements the TimingDelay variable every millisecond */

void TimingDelay_Decrement(void){
if(TimingDelay!=0x00){

TimingDelay--;
}

}

void SysTick_Handler(void) {
	TimingDelay_Decrement();
}

void TIM3_IRQHandler(void){



	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	    {
	        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);


	if(index2<=50){

	PWMTIM3_InitOC4(lookup[index2]);

	}

	if(index2>=50){

	PWMTIM3_InitOC3(lookup[index2]);

	}



	index2++;

	if(index2==101){

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

void ADC_IRQHandler(void){
   

    while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC));
   result=ADC_GetConversionValue(ADC3);
 
    
    
    
    
  
    
}


