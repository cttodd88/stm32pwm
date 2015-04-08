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
#include "lcd/lcd.h"


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

/* Private function prototypes -----------------------------------------------*/
void Delay(unsigned int nTime);
void TimingDelay_Decrement(void);

/* Private functions ---------------------------------------------------------*/


int duty =0;
int index2 = 0;
int period=0;

char *title = "PQ PWM C.T. T.L. A.C. F.F.";

int lookup[1601]={0};



int main(void)
{

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


	  char *string;

	  string="PQ PWM C.T. T.L. A.C. F.F.";

	  lcdInit();

	  lcd_puts(string);



    while(1)
    {



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



