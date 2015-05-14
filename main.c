/******************************************************************************************************/
// Project prototyping and development for project quarter PWM driver and grid tie inverter
// Author: Chris Todd
// ID: C0304228
// Creation date: 3/10/2015
// Last updated: 4/27/2015 - Taylor Long
// Description: Integrated Display code with PWM code
// Modified: 5/12/2015 - Taylor Long
// Description: GUI Initial Implementation completed

#include <stdio.h>
#include "cmsis_boot/stm32f4xx.h"
#include "cmsis_lib/include/stm32f4xx_gpio.h"
#include "cmsis_lib/include/stm32f4xx_rcc.h"
#include "cmsis_lib/include/stm32f4xx_tim.h"
#include "cmsis_lib/include/stm32f4xx_pwm.h"
#include "cmsis_lib/include/misc.h"
//#include "lcd/lcd.h"
#include "cmsis_lib/include/stm32f4xx_dma.h"
#include "cmsis_lib/include/stm32f4xx_adc.h"
#include "sensor_init.h"

#include "inc/GUI.h"
#include "inc/DIALOG.h"
#include "inc/GUITDRV_ADS7846.h"

#include "user/LCD_STM32F4.h"
#include "user/stm32f4xx_it.h"
#include "user/pwm.h"
#include "user/ts.h"
#include "delays.h"

#include "rtc.h"
//#include "sentinelLogo.h"
#include "img/sentinelLogo2.h"
#include "img/sentinelLogoBW.h"

#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
ADC_InitTypeDef ADC_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef nvicStructure;
DMA_InitTypeDef DMA_InitStructure;
//ADCValues_t ADC3ConvertedValue;

/* Private define ------------------------------------------------------------*/
typedef struct ADCValues{
int16_t channel_1 ;
int16_t channel_2 ;
int16_t channel_3 ;
int16_t channel_11 ;
int16_t channel_12 ;
} ADCValues_t;

typedef struct{
    // inputs
    float inVoltage;
    float inCurrent;
    float inPower;
    // outputs
    float outVoltage;
    float outCurrent;
    float outPower;
    float outFreq;
    // temperatures
    float temp1;
    float temp2;
} SENSORS;

typedef struct{
    int state;
    int voltage;
    float current;
    int chargeLevel;
} BATTINFO;

#define ADC3_DR_ADDRESS ((uint32_t)0x4001224C)

#define DCFLAG 0x10
#define BATTFLAG 0x08
#define TEMPFLAG 0x04
#define OUTFLAG 0x02
#define ONFLAG 0x01

#define GUI_ID_FRAMEWIN0 GUI_ID_USER + 1
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
//unsigned int TimingDelay=0;
unsigned int result=0;
char adcString[10]={0};

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 300;
uint16_t CCR2_Val = 100;
WM_HWIN hScreen; // main screen window handle
BUTTON_Handle hButtOutput;
BUTTON_Handle hButtCapacity;
PROGBAR_Handle hChargeBar;
FRAMEWIN_Handle hFrame;
SPINBOX_Handle hSpin;
int currentScreen = 0; // tracks current screen
int batteryCap = 18; // battery capacity in Ah
unsigned char homeStatusFlg = DCFLAG | BATTFLAG;

volatile uint16_t ADC3ConvertedValue[2];

/* Private function prototypes -----------------------------------------------*/
void Delay(unsigned int nTime);
float convertTemp(int t);
void TimingDelay_Decrement(void);
SENSORS getMeasurement(void);
BATTINFO getBattMeas(void);
void cbWindow(WM_MESSAGE * pMsg);
void cbFrame(WM_MESSAGE * pMsg);
void drawHomeScreen(void);
void drawBattScreen(void);
void drawSysScreen(void);
void drawAboutScreen(void);
void updateDispValues(void);
void drawBatteryCapacity(void);
void setBatteryCapacity(void);

/* Private functions ---------------------------------------------------------*/


//WM_HWIN CreateWindow(void);
//WM_HWIN hDlg;

extern GUI_PID_STATE pstate;

int duty =0;
int index2 = 0;
int period=0;

//__IO uint16_t ADC3ConvertedValue = 0;

int lookup[101]={0};

int main(void)
{
    /*Variables*/
    int period = 13999;
    WM_HWIN hMenu; // menu window handle
    WM_HWIN hChild; // frame content window handle
        
    // Button Handles
    BUTTON_Handle hButtonHome;
    BUTTON_Handle hButtonBatt;
    BUTTON_Handle hButtonSys;
    BUTTON_Handle hButtonAbout;
    BUTTON_Handle hButtOK;
    
    /*Initialization*/
    
    SystemInit();
        
    if(SysTick_Config(SystemCoreClock/1000)){
        while(1);
    }
    
    Init_GPIO(); // Ports C,D,E Clocks enabled in here
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //period=874;
    genLookup(lookup,100,13999);
       
    

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

    Init_FSMC();
    
    /* Enable the CRC Module */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);

    GUI_Init();

    Init_LCD();

    touch_init();

    pwm_init();
    //turn on lcd backlight
    GPIO_ResetBits(GPIOD, GPIO_Pin_12);

    //rtcConfig();

    /*
     * GUI Setup
     */
    // Splash Screen
    /*GUI_SetBkColor(GUI_WHITE);
    GUI_Clear();
    GUI_DrawBitmap(&bmSentinelLogoBW, 60,20);
    DelayMs(2000);*/
    
    // Setup
    GUI_SetFont(&GUI_Font8x16);
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_DispString("Sentinel Power");
    
    // create window for menu
    hMenu = WM_CreateWindow(0, 190, 320, 50, WM_CF_SHOW, cbWindow, 0);
    
    // create main window
    GUI_SetBkColor(GUI_DARKBLUE);
    hScreen = WM_CreateWindow(0, 16, 320, 174, WM_CF_SHOW, cbWindow, 0);
    WM_SelectWindow(hScreen);
    GUI_Clear();
    
    /*
     * Draw Menu
     */
    WM_SelectWindow(hMenu);
    
    //Home button
    hButtonHome = BUTTON_CreateEx(0, 0, 80, 50, hMenu, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
    BUTTON_SetFont(hButtonHome, &GUI_Font8x16);
    BUTTON_SetText(hButtonHome, "Home");
    BUTTON_SetFocussable(hButtonHome, 0);
    
    //Battery button
    hButtonBatt = BUTTON_CreateEx(80, 0, 80, 50, hMenu, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
    BUTTON_SetFont(hButtonBatt, &GUI_Font8x16);
    BUTTON_SetText(hButtonBatt, "Battery");
    BUTTON_SetFocussable(hButtonBatt, 0);
    
    //System button
    hButtonSys = BUTTON_CreateEx(160, 0, 80, 50, hMenu, WM_CF_SHOW, 0, GUI_ID_BUTTON2);
    BUTTON_SetFont(hButtonSys, &GUI_Font8x16);
    BUTTON_SetText(hButtonSys, "System");
    BUTTON_SetFocussable(hButtonSys, 0);
    
    //About button
    hButtonAbout = BUTTON_CreateEx(240, 0, 80, 50, hMenu, WM_CF_SHOW, 0, GUI_ID_BUTTON3);
    BUTTON_SetFont(hButtonAbout, &GUI_Font8x16);
    BUTTON_SetText(hButtonAbout, "About");
    BUTTON_SetFocussable(hButtonAbout, 0);
    
    BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
    //HOME SCREEN: Output button
    hButtOutput = BUTTON_CreateEx(180, 106, 116, 50, hScreen, WM_CF_HIDE | WM_CF_DISABLED, 0, GUI_ID_BUTTON4);
    BUTTON_SetFont(hButtOutput, GUI_FONT_20B_ASCII);
    BUTTON_SetText(hButtOutput, "OUTPUT");
    BUTTON_SetFocussable(hButtOutput, 0);
    
    //BATTERY SCREEN: Set Capacity
    hButtCapacity = BUTTON_CreateEx(20, 120, 120, 30, hScreen, WM_CF_HIDE | WM_CF_DISABLED, 0, GUI_ID_BUTTON5);
    BUTTON_SetFont(hButtCapacity, &GUI_Font8x16);
    BUTTON_SetText(hButtCapacity, "Set Capacity");
    BUTTON_SetFocussable(hButtCapacity, 0);
    
    hChargeBar = PROGBAR_CreateEx(215, 40, 50, 120, hScreen, WM_CF_HIDE, PROGBAR_CF_VERTICAL, GUI_ID_PROGBAR0);
    PROGBAR_SetBarColor(hChargeBar, 0, GUI_DARKGREEN);
    PROGBAR_SetBarColor(hChargeBar, 1, GUI_BLACK);    
    
    /*
     * Frame Window for user input of battery capacity
     */
    hFrame = FRAMEWIN_CreateEx(10, 10, 120, 125, hScreen, WM_CF_HIDE, 0, GUI_ID_FRAMEWIN0, 
            "Set Battery Capacity", cbWindow);
    
    hChild = WM_GetClientWindow(hFrame);
    WM_SetCallback((hChild), cbFrame);
    
    hSpin = SPINBOX_CreateEx(0,0, WM_GetWindowSizeX(hChild),
            50, hChild, WM_CF_SHOW, GUI_ID_SPINBOX0, 1, 200);
    SPINBOX_SetFont(hSpin, GUI_FONT_32B_ASCII);
    SPINBOX_SetValue(hSpin, batteryCap);
    
    hButtOK = BUTTON_CreateEx(20 - FRAMEWIN_GetBorderSize(hFrame), 60, 80, 40,
            hChild, WM_CF_SHOW, 0, GUI_ID_BUTTON6);
    BUTTON_SetFont(hButtOK, &GUI_Font8x16);
    BUTTON_SetText(hButtOK, "OK");
    BUTTON_SetFocussable(hButtOK, 0);
    
    GUI_Clear();
    
    WM_SelectWindow(hScreen); // set the main drawing window
    WM_SetFocus(hMenu); // give input focus to menu
    drawHomeScreen(); // default menu selection

    /*
     * Touch Calibration from TOUCH_sample.c
     */
    /*GUI_PID_STATE TouchState;
    int           xPhys;
    int           yPhys;

    //GUI_Init();
    GUI_CURSOR_Show();
    GUI_CURSOR_Select(&GUI_CursorCrossL);
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLACK);
    GUI_Clear();
    GUI_DispString("Measurement of\nA/D converter values");*/
    /*
     * END OF CALIBRATION CODE
     */
    ADC_ContinuousModeCmd(ADC3,ENABLE);

    ADC_SoftwareStartConv(ADC3);
    
    while(1)
    {
        /*
         * Touch Calibration from TOUCH_sample.c
         */
        /*GUI_PID_GetCurrentState(&TouchState);  // Get the touch position in pixel
        xPhys = GUI_TOUCH_GetxPhys();     // Get the A/D mesurement result in x
        yPhys = GUI_TOUCH_GetyPhys();     // Get the A/D mesurement result in y
        //
        // Display the measurement result
        //
        GUI_SetColor(GUI_BLUE);
        GUI_DispStringAt("Analog input:\n", 0, 20);
        GUI_GotoY(GUI_GetDispPosY() + 2);
        GUI_DispString("x:");
        GUI_DispDec(xPhys, 4);
        GUI_DispString(", y:");
        GUI_DispDec(yPhys, 4);
        //
        // Display the according position
        //
        GUI_SetColor(GUI_RED);
        GUI_GotoY(GUI_GetDispPosY() + 4);
        GUI_DispString("\nPosition:\n");
        GUI_GotoY(GUI_GetDispPosY() + 2);
        GUI_DispString("x:");
        GUI_DispDec(TouchState.x,4);
        GUI_DispString(", y:");
        GUI_DispDec(TouchState.y,4);
        //
        // Wait a while
        //
        GUI_Delay(50);*/
        /*
         * END OF CALIBRATION CODE
         */
        
        // Debug Code
        /*
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
        }*/
        
        updateDispValues();

        GUI_Delay(50);
    
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

float convertTemp(int t) {
    float temperature = 0;
    
    temperature = (t * 0.0732421); // ADC conversion to F
    temperature = (temperature - 32) / 1.8; // F to C
    
    return temperature;
}

/*
 * getMeasurement()
 * Will get information from sensors and other measurements
 */
SENSORS getMeasurement(void) {
    SENSORS measured;
    measured.inVoltage = 30.2;
    measured.inCurrent = 1.34;
    measured.inPower = 40.468;
    measured.outVoltage = 119.98;
    measured.outCurrent = 2.8;
    measured.outPower = 335.944;
    measured.outFreq = 59.97;
    measured.temp1 = convertTemp(ADC3ConvertedValue[0]); //52.43;
    measured.temp2 = convertTemp(ADC3ConvertedValue[1]); //59.87;
    
    return measured;
}
/*
 * getBattMeas()
 * Contains the battery information from sensors
 */
BATTINFO getBattMeas(void) {
    BATTINFO meas;
    meas.voltage = 24;
    meas.current = 0.5;
    meas.state = 1;
    meas.chargeLevel = 72;
    
    return meas;
}

/*Window Callback Override*/
void cbWindow(WM_MESSAGE * pMsg) {
    int NCode;
    int Id;
    // USER START (Optionally insert additional variables)
    // USER END

    switch (pMsg->MsgId) {
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) {
                case GUI_ID_BUTTON0: // Notifications sent by 'Home Button'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            //GUI_Clear();
                            //GUI_DispStringAt("HOME", 0 , 100);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            if(currentScreen) {
                                currentScreen = 0;
                                drawHomeScreen();
                            }                            
                            break;
                    }
                    break;
                case GUI_ID_BUTTON1: // Notifications sent by 'Battery Button'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            //GUI_Clear();
                            //GUI_DispStringAt("BATTERY", 0, 100);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            if(!(currentScreen == 1)) {
                                currentScreen = 1;
                                drawBattScreen();
                            }
                            break;
                    }
                    break;
                case GUI_ID_BUTTON2: // Notifications sent by 'System Button'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            //GUI_Clear();
                            //GUI_DispStringAt("SYSTEM", 0, 100);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            if(!(currentScreen == 2)) {
                                currentScreen = 2;
                                drawSysScreen();
                            }
                            break;
                    }
                    break;
                case GUI_ID_BUTTON3: // Notifications sent by 'About Button'
                    switch(NCode) {
                        case WM_NOTIFICATION_CLICKED:
                            //GUI_Clear();
                            //GUI_DispStringAt("ABOUT", 0, 100);
                            break;
                        case WM_NOTIFICATION_RELEASED:
                            if(!(currentScreen == 3)){
                                currentScreen = 3;
                                drawAboutScreen();
                            }
                            break;
                    }
                    break;
                case GUI_ID_BUTTON4: // Notifications sent by 'HOME:Output Button'
                    switch(NCode) {
                        case WM_NOTIFICATION_RELEASED:
                            homeStatusFlg ^= ONFLAG;
                            break;
                    }
                    break;
                case GUI_ID_BUTTON5: // Notifications sent by 'BATT: Set Battery Capacity'
                    switch(NCode) {
                        case WM_NOTIFICATION_RELEASED:
                            drawBatteryCapacity();
                            break;
                    }
                    break;
            }
            break;

        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/*Frame Callback Override*/
void cbFrame(WM_MESSAGE * pMsg) {
    int Id;
    int NCode;
    
    switch (pMsg->MsgId) {
        case WM_PAINT:
            GUI_SetBkColor(0xc0c0c0); // default frame color from API document
            GUI_Clear();
            break;
        case WM_NOTIFY_PARENT:
            Id    = WM_GetId(pMsg->hWinSrc);
            NCode = pMsg->Data.v;
            switch(Id) {
                case GUI_ID_BUTTON6:
                        switch(NCode) {
                            case WM_NOTIFICATION_RELEASED:
                                setBatteryCapacity();
                                break;
                        }
                        break;
            }
            break;
        default:
            WM_DefaultProc(pMsg);
  }
}
/*
 * GUI SCREEN DRAWING FUNCTIONS
 * ============================
 */

/*
 * drawHomeScreen()
 */
void drawHomeScreen(void) {   
    WM_SelectWindow(hScreen);
    WM_ShowWindow(hButtOutput);
    WM_EnableWindow(hButtOutput);
    WM_HideWindow(hButtCapacity);
    WM_DisableWindow(hButtCapacity);
    WM_HideWindow(hChargeBar);
    WM_HideWindow(hFrame);
    
    GUI_Clear();
        
    // indicator BG circles
    GUI_SetColor(GUI_BLACK);
    GUI_FillCircle(20,24,16); // DC Input
    GUI_FillCircle(20,66,16); // Battery
    GUI_FillCircle(20,108,16); // Temperature
    GUI_FillCircle(20,150,16); // Existing Out
    GUI_FillCircle(283,40,32); // On
    GUI_FillCircle(192,40,32); // Off
    
    updateDispValues(); // display current status indicators 
    
    GUI_SetColor(GUI_CYAN);
    GUI_SetFont(&GUI_Font8x16);
    GUI_DispStringAt("DC Input", 44, 16);
    GUI_DispStringAt("Battery", 44, 58);
    GUI_DispStringAt("Temperature", 44, 100);
    GUI_DispStringAt("Existing Out", 44, 142);
    GUI_DispStringAt("ON", 276, 76);
    GUI_DispStringAt("OFF", 181, 76);
    
}

/*
 * drawSysScreen()
 * desc: displays system sensor information
 */
void drawSysScreen(void) {
    WM_SelectWindow(hScreen);
    WM_HideWindow(hButtOutput);
    WM_DisableWindow(hButtOutput);
    WM_HideWindow(hButtCapacity);
    WM_DisableWindow(hButtCapacity);
    WM_HideWindow(hChargeBar);
    WM_HideWindow(hFrame);
    GUI_Clear();
    
    SENSORS meas = getMeasurement();
    
    // Input/Output dividing line
    GUI_SetColor(GUI_DARKCYAN);    
    GUI_FillRect(159, 4, 160, 113);
    GUI_FillRect(3, 118, 316, 119);
    
    GUI_SetColor(GUI_CYAN);
    GUI_SetFont(&GUI_Font8x16);
    GUI_GotoXY(60,16);
    GUI_DispString("INPUT");
    GUI_GotoX(216);
    GUI_DispString("OUTPUT\n\n");
    
    // Input Voltage
    GUI_DispString("Voltage: ");
    GUI_DispFloat(meas.inVoltage, 6);
    GUI_DispString(" V");
    
    // Output Voltage
    GUI_GotoX(162);
    GUI_DispString("Voltage: ");
    GUI_DispFloat(meas.outVoltage, 6);
    GUI_DispString(" V\n");
    
    // Input Current
    GUI_DispString("Current: ");
    GUI_DispFloat(meas.inCurrent, 6);
    GUI_DispString(" A");
    
    // Output Current
    GUI_GotoX(162);
    GUI_DispString("Current: ");
    GUI_DispFloat(meas.outCurrent, 6);
    GUI_DispString(" A\n");
    
    // Input Power
    GUI_DispString("Power: ");
    GUI_DispFloat(meas.inPower, 6);
    GUI_DispString(" W");
    
    // Output Power
    GUI_GotoX(162);
    GUI_DispString("Power: ");
    GUI_DispFloat(meas.outPower, 6);
    GUI_DispString(" W\n");
    
    // Output Frequency
    GUI_GotoX(162);
    GUI_DispString("Frequency: ");
    GUI_DispFloat(meas.outFreq, 6);
    GUI_DispString(" Hz\n\n");
    
    // Temperature Readings
    GUI_DispString("Temperature1: ");
    GUI_DispFloat(meas.temp1, 6);
    GUI_DispChar(' ');
    GUI_DispChar(176); // degree sign
    GUI_DispString("C\n");
    GUI_DispString("Temperature2: ");
    GUI_DispFloat(meas.temp2, 6);
    GUI_DispChar(' ');
    GUI_DispChar(176); // degree sign
    GUI_DispChar('C');
    
    // Set Back Color back to default
    GUI_SetBkColor(GUI_DARKBLUE);
}

/*
 * drawBattScreen()
 */
void drawBattScreen(void) {
    WM_SelectWindow(hScreen);
    WM_HideWindow(hButtOutput);
    WM_DisableWindow(hButtOutput);
    WM_ShowWindow(hButtCapacity);
    WM_EnableWindow(hButtCapacity);
    WM_ShowWindow(hChargeBar);
    GUI_Clear();
    
    BATTINFO m = getBattMeas();
       
    PROGBAR_SetValue(hChargeBar, m.chargeLevel);
    //GUI_DispStringAt("72%", 228, 16);    
    
    GUI_SetColor(GUI_CYAN);
    GUI_SetFont(&GUI_Font8x16);
    //GUI_GotoXY(0,16);
    
    GUI_GotoXY(228, 16);
    GUI_DispDec(m.chargeLevel, 2);
    GUI_DispChar('%');
    //GUI_DispStringAt("72%", 228, 16);
    if(m.state){
        GUI_DispStringAt("State: Charging\n", 0, 16);
    }
    else {
        GUI_DispStringAt("State: Discharging\n", 0, 16);
    }
    GUI_DispString("Voltage: ");
    GUI_DispDec(m.voltage, 2);
    GUI_DispString(" V\n");
    GUI_DispString("Current: ");
    GUI_DispFloat(m.current, 3);
    GUI_DispString(" A\n\n");
    GUI_DispString("Capacity: ");
    GUI_DispDec(batteryCap, 2);
    GUI_DispString(" Ah");
}

/*
 * drawAboutScreen()
 * desc: display team logo and version info
 */
void drawAboutScreen(void) {
    WM_SelectWindow(hScreen);
    WM_HideWindow(hButtOutput);
    WM_DisableWindow(hButtOutput);
    WM_HideWindow(hButtCapacity);
    WM_DisableWindow(hButtCapacity);
    WM_HideWindow(hChargeBar);
    WM_HideWindow(hFrame);
    GUI_Clear();
    
    GUI_DrawBitmap(&bmSentinelLogo2, 12, 12);
    
    GUI_SetColor(GUI_CYAN);
    GUI_SetFont(GUI_FONT_32B_ASCII);
    
    GUI_GotoXY(174, 12);
    GUI_DispString("Sentinel\n");
    GUI_GotoX(174);
    GUI_DispString("Power\n");
    GUI_SetFont(GUI_FONT_13B_ASCII);
    GUI_GotoXY(174, 123);
    GUI_DispString("Firmware Version 0.1");
    
    GUI_GotoXY(174, 149);
    GUI_DispString("STemWin v");
    GUI_DispString(GUI_GetVersionString());
}

/*
 * drawBatteryCapacity()
 * Shows the frame for user input of battery capacity
 */
void drawBatteryCapacity(void) {
    WM_DisableWindow(hButtCapacity);
    SPINBOX_SetValue(hSpin, batteryCap);
    WM_SelectWindow(hFrame);
    WM_ShowWindow(hFrame);
    WM_SetFocus(hFrame);
}

/*
 * setBatteryCapacity()
 * Writes the input value to the variable and hides the input frame
 */
void setBatteryCapacity(void) {
    batteryCap = SPINBOX_GetValue(hSpin);
    WM_SelectWindow(hScreen);
    WM_HideWindow(hFrame);
    WM_HideWindow(hButtCapacity);
    
    GUI_Clear();
    drawBattScreen();
}

/*
 * updateDispValues()
 * Refreshes current screen with updated values
 */
void updateDispValues(void){
    SENSORS m = getMeasurement();
    BATTINFO b = getBattMeas(); 
    
    switch(currentScreen){
        case 0:
            // home screen
            if(homeStatusFlg & DCFLAG) {
                GUI_SetColor(GUI_GREEN);
            }
            else {
                GUI_SetColor(GUI_RED);
            }
            GUI_FillCircle(20,24,12); // DC Input

            if(homeStatusFlg & BATTFLAG) {
                GUI_SetColor(GUI_GREEN);
            }
            else {
                GUI_SetColor(GUI_RED);
            }
            GUI_FillCircle(20,66,12); // Battery

            if(homeStatusFlg & TEMPFLAG) {
                GUI_SetColor(GUI_GREEN);
            }
            else {
                GUI_SetColor(GUI_RED);
            }
            GUI_FillCircle(20,108,12); // Temperature

            if(homeStatusFlg & OUTFLAG) {
                GUI_SetColor(GUI_GREEN);
            }
            else {
                GUI_SetColor(GUI_DARKGRAY);
            }
            GUI_FillCircle(20,150,12); // Existing Out

            if(homeStatusFlg & ONFLAG) {
                GUI_SetColor(GUI_GREEN);
                GUI_FillCircle(283,40,27); // On
                GUI_SetColor(GUI_DARKGRAY);
                GUI_FillCircle(192,40,27); // Off
            } else {
                GUI_SetColor(GUI_DARKGRAY);
                GUI_FillCircle(283,40,27); // On
                GUI_SetColor(GUI_RED);
                GUI_FillCircle(192,40,27); // Off
            }
            break;
            
        case 1:
            // battery screen
            WM_SelectWindow(hScreen);
            
            GUI_GotoXY(228,16);
            GUI_DispDec(b.chargeLevel, 2);
            GUI_GotoXY(56,16);
            if(b.state) {
                GUI_DispString("Charging   ");
            }
            else {
                GUI_DispString("Discharging");
            }
            GUI_GotoXY(72,32);
            GUI_DispDec(b.voltage, 2);
            GUI_GotoXY(72,48);
            GUI_DispFloat(b.current, 3);
            GUI_GotoXY(80,80);
            GUI_DispDec(batteryCap, 2);
            break;
            
        case 2:
            // system screen
            // inputs
            GUI_GotoXY(72,48);
            GUI_DispFloat(m.inVoltage, 6);
            GUI_GotoXY(72,64);
            GUI_DispFloat(m.inCurrent, 6);
            GUI_GotoXY(56,80);
            GUI_DispFloat(m.inPower, 6);
            // outputs
            GUI_GotoXY(234,48);
            GUI_DispFloat(m.outVoltage, 6);
            GUI_GotoXY(234,64);
            GUI_DispFloat(m.outCurrent, 6);
            GUI_GotoXY(218,80);
            GUI_DispFloat(m.outPower, 6);
            GUI_GotoXY(250,96);
            GUI_DispFloat(m.outFreq, 6);
            // temperatures
            GUI_GotoXY(112,128);
            GUI_DispFloat(m.temp1, 6);
            GUI_GotoXY(112,144);
            GUI_DispFloat(m.temp2, 6);
            break;
        
        case 3:
            // about screen
            // nothing to update
            break;
            
        default:
            break;
    }
}
