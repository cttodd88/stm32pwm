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
#include "cmsis_lib/include/stm32f4xx_dma.h"
#include "cmsis_lib/include/stm32f4xx_tim.h"
#include "cmsis_lib/include/stm32f4xx_pwm.h"
#include "cmsis_lib/include/misc.h"
#include "cmsis_lib/include/stm32f4xx_adc.h"
#include "lcd/lcd.h"
#include "sensor_init.h"


typedef struct ADCValues{
int16_t channel_1 ;
int16_t channel_2 ;
int16_t channel_3 ;
int16_t channel_11 ;
int16_t channel_12 ;
} ADCValues_t;


/* Private typedef -----------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef nvicStructure;
DMA_InitTypeDef DMA_InitStructure;
//ADCValues_t ADC3ConvertedValue;


/* Private define ------------------------------------------------------------*/
#define GREEN  GPIOD, GPIO_Pin_12
#define ORANGE GPIOD, GPIO_Pin_13
#define RED GPIOD, GPIO_Pin_14
#define BLUE GPIOD, GPIO_Pin_15
#define ADC3_DR_ADDRESS ((uint32_t)0x4001224C)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned int TimingDelay=0;
unsigned int result=0;
char adcString[10]={0};

volatile uint16_t ADC3ConvertedValue[2];
/* Private function prototypes -----------------------------------------------*/
void Delay(unsigned int nTime);
void TimingDelay_Decrement(void);

/* Private functions ---------------------------------------------------------*/


int duty =0;
int index2 = 0;
int period=0;

//__IO uint16_t ADC3ConvertedValue = 0;

char *title = "PQ PWM C.T. T.L. A.C. F.F.";

int lookup[101]={0};



int main(void)
{

	SystemInit();
        
       

	if(SysTick_Config(SystemCoreClock/1000)){

		while(1);
	}
        
       
	period=13999;     

	genLookup(lookup,100,13999);
	
	PWM_TIMER_Init3(period); //500 KHz 166
        
        PWMTIM3_InitOC1(3500);
        
        
        GPIO_init(); //Enable peripheral clocks and configure GPIO
        
        
        //configuring DMA
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3ConvertedValue[0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
        
        
        //DMA_init();
        
        ADC_init(); //Enable ADC Channels for sensors
        
        IT_init(); //Enable PWM interrupts
	
        
	  char *string;

	  string="PQ PWM C.T. T.L. A.C. F.F.";

	  lcdInit();

	  //lcd_puts(string);
          
          ADC_ContinuousModeCmd(ADC3,ENABLE);

          ADC_SoftwareStartConv(ADC3);
          

    while(1)
    {
        sprintf(adcString, "%d %d", ADC3ConvertedValue[0], ADC3ConvertedValue[1]);
        lcd_clear();
        lcd_puts(adcString);
        

/*
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
        */
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


