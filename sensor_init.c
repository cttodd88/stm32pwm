#include "cmsis_boot/stm32f4xx.h"
#include "cmsis_lib/include/stm32f4xx_gpio.h"
#include "cmsis_lib/include/stm32f4xx_dma.h"
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
DMA_InitTypeDef DMA_InitStructure;




void GPIO_init(void){
    
    //ENABLE Peripheral Clocks
    
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    
     //ENABLE Alternate Pin Functions
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
        //GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
        GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
     //Configure GPIO
        
        //ADC3 Channel 11
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        //ADC2 Channel 2&3
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        
        //PWM Channels
        GPIO_InitStructure.GPIO_Pin = /*GPIO_Pin_6|*/GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   	GPIO_Init(GPIOC, &GPIO_InitStructure);
        
        //Relay Control
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
/*
        //LED's (to be used by display)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	//Configure LCD Peripherals (will be deprecated when merged with VGA screen)
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
*/

        
    
    
}

 void ADC_init(void){
     
        ADC_DeInit();

	//Initialize common ADC settings
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	//Initialize specific ADC settings
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 2;
	ADC_Init(ADC3,&ADC_InitStructure);
        
        ADC_Init(ADC2,&ADC_InitStructure);
        
        //ADC_RegularChannelConfig(ADC3,ADC_Channel_10,1,ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADC3,ADC_Channel_11,1,ADC_SampleTime_3Cycles);
        ADC_RegularChannelConfig(ADC3,ADC_Channel_12,2,ADC_SampleTime_3Cycles);
        //ADC_RegularChannelConfig(ADC3,ADC_Channel_13,4,ADC_SampleTime_3Cycles);
        
        ADC_RegularChannelConfig(ADC2,ADC_Channel_2,1,ADC_SampleTime_480Cycles);
        ADC_RegularChannelConfig(ADC2,ADC_Channel_3,2,ADC_SampleTime_480Cycles);
        
        ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
        ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);
        
	ADC_Cmd(ADC3, ENABLE);
        
        /* Enable ADC2 DMA */
        ADC_DMACmd(ADC2, ENABLE);

	ADC_Cmd(ADC2, ENABLE);
     
     
     
 }
 
 void IT_init(void){
     
          TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	  nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
	  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  nvicStructure.NVIC_IRQChannelSubPriority = 1;
	  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&nvicStructure);
          
          //Configure Nested Vector Interrupt Controller for ADC Interrupt
/*
          ADC_ITConfig(ADC3,ADC_IT_EOC,ENABLE);
	  nvicStructure.NVIC_IRQChannel= ADC_IRQn;
	  nvicStructure.NVIC_IRQChannelCmd= ENABLE;
	  nvicStructure.NVIC_IRQChannelPreemptionPriority= 1;
	  nvicStructure.NVIC_IRQChannelSubPriority= 2;
	  NVIC_Init(&nvicStructure);
  */   
     
 }
 
 void DMA_init(){
     

	
 /*
	//configuring DMA
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC3Val;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 1;
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
     
     */
     
 }
