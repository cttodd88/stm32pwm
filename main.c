/******************************************************************************************************/
// Project prototyping and development for project quarter PWM driver and grid tie inverter
// Author: Chris Todd
// ID: C0304228
// Creation date: 3/10/2015
// Last updated: 4/27/2015 - Taylor Long
// Description: Integrated Display code with PWM code

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
#include "sentinelLogo.h"

#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef nvicStructure;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
unsigned int TimingDelay=0;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t CCR1_Val = 300;
uint16_t CCR2_Val = 100;
BUTTON_Handle hButtOutput;
int currentScreen = 0;

/* Private function prototypes -----------------------------------------------*/
void Delay(unsigned int nTime);
void TimingDelay_Decrement(void);
void cbMenu(WM_MESSAGE * pMsg);
void drawHomeScreen(void);
void drawBattScreen(void);
void drawSysScreen(void);
void drawAboutScreen(void);
void updateDispValues(void);

/* Private functions ---------------------------------------------------------*/


//WM_HWIN CreateWindow(void);
//WM_HWIN hDlg;

extern GUI_PID_STATE pstate;

int duty =0;
int index2 = 0;
int lookup[101]={0};
WM_HWIN hScreen; // main screen window handle

//measurement global variables for GUI
float inVoltage = 30.2;
float inCurrent = 1.34;
float inPower = 40.468;
float outVoltage = 119.98;
float outCurrent = 2.8;
float outPower = 335.944;
float outFreq = 59.97;

int main(void)
{
    /*Variables*/
    int period = 13999;
    WM_HWIN hMenu; // menu window handle    
    
    // Button Handles
    BUTTON_Handle hButtonHome;
    BUTTON_Handle hButtonBatt;
    BUTTON_Handle hButtonSys;
    BUTTON_Handle hButtonAbout;
    
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
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    //GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);
    //GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    nvicStructure.NVIC_IRQChannel = TIM3_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

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
    GUI_SetFont(&GUI_Font8x16);
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_DispString("Sentinel Power");
    
    // create window for menu
    hMenu = WM_CreateWindow(0, 190, 320, 50, WM_CF_SHOW, cbMenu, 0);
    
    // create main window
    GUI_SetBkColor(GUI_DARKBLUE);
    hScreen = WM_CreateWindow(0, 16, 320, 174, WM_CF_SHOW, WM_DefaultProc, 0);
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
    hButtOutput = BUTTON_CreateEx(160, 50, 80, 50, hScreen, WM_CF_HIDE | WM_CF_DISABLED, 0, GUI_ID_BUTTON4);
    BUTTON_SetFont(hButtOutput, &GUI_Font8x16);
    BUTTON_SetText(hButtOutput, "Output");
    BUTTON_SetFocussable(hButtOutput, 0);
    
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
        /*switch(currentScreen){
            case 0:
                drawHomeScreen();
                break;
                
            case 1:
                drawBattScreen();
                break;
                
            case 2:
                drawSysScreen();
                break;
                
            case 3:
                drawAboutScreen();
                break;
                
            default:
                drawHomeScreen();
        }*/
        updateDispValues();

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

/*Menu Window Callback Override*/
void cbMenu(WM_MESSAGE * pMsg) {
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
                            drawHomeScreen();
                            currentScreen = 0;
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
                            drawBattScreen();
                            currentScreen = 1;
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
                            drawSysScreen();
                            currentScreen = 2;
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
                            drawAboutScreen();
                            currentScreen = 3;
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
    GUI_Clear();
    
    GUI_SetColor(GUI_CYAN);
    GUI_SetFont(&GUI_Font8x16);
    GUI_GotoXY(0,16);
    
    GUI_DispString("DC Input: Detected \n\n");
    GUI_DispString("Batteries: Detected\n\n");
    GUI_DispString("Temperature: Good\n\n");
    GUI_DispString("Output: Detected");
        
}

/*
 * drawSysScreen()
 * desc: displays system sensor information
 */
void drawSysScreen(void) {
    WM_SelectWindow(hScreen);
    WM_HideWindow(hButtOutput);
    WM_DisableWindow(hButtOutput);
    GUI_Clear();
    
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
    GUI_DispFloat(inVoltage, 6);
    GUI_DispString(" V");
    
    // Output Voltage
    GUI_GotoX(162);
    GUI_DispString("Voltage: ");
    GUI_DispFloat(outVoltage, 6);
    GUI_DispString(" V\n");
    
    // Input Current
    GUI_DispString("Current: ");
    GUI_DispFloat(inCurrent, 6);
    GUI_DispString(" A");
    
    // Output Current
    GUI_GotoX(162);
    GUI_DispString("Current: ");
    GUI_DispFloat(outCurrent, 6);
    GUI_DispString(" A\n");
    
    // Input Power
    GUI_DispString("Power: ");
    GUI_DispFloat(inPower, 6);
    GUI_DispString(" W");
    
    // Output Power
    GUI_GotoX(162);
    GUI_DispString("Power: ");
    GUI_DispFloat(outPower, 6);
    GUI_DispString(" W\n");
    
    // Output Frequency
    GUI_GotoX(162);
    GUI_DispString("Frequency: ");
    GUI_DispFloat(outFreq, 6);
    GUI_DispString(" Hz\n\n");
    
    GUI_DispString("Temperature1: 50 C\n");
    GUI_DispString("Temperature2: 50 C");
    
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
    GUI_Clear();
    
    GUI_SetColor(GUI_CYAN);
    GUI_SetFont(&GUI_Font8x16);
    GUI_GotoXY(0,16);
    
    GUI_DispString("State: Charging\n");
    GUI_DispString("Voltage: 24 V\n");
    GUI_DispString("Current: 0.5 A\n\n");
    GUI_DispString("Capacity Setting: 18 Ah");
}

/*
 * drawAboutScreen()
 * desc: display team logo and version info
 */
void drawAboutScreen(void) {
    WM_SelectWindow(hScreen);
    WM_HideWindow(hButtOutput);
    WM_DisableWindow(hButtOutput);
    GUI_Clear();
    
    GUI_DrawBitmap(&bmsentinelLogo, 12, 12);
    
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
 * updateDispValues()
 */
void updateDispValues(void){
    switch(currentScreen){
        case 0:
            break;
            
        case 1:
            break;
            
        case 2:
            //system screen
            //inputs
            GUI_GotoXY(72,48);
            GUI_DispFloat(inVoltage, 6);
            GUI_GotoXY(72,64);
            GUI_DispFloat(inCurrent, 6);
            GUI_GotoXY(56,80);
            GUI_DispFloat(inPower, 6);
            //outputs
            GUI_GotoXY(234,48);
            GUI_DispFloat(outVoltage, 6);
            GUI_GotoXY(234,64);
            GUI_DispFloat(outCurrent, 6);
            GUI_GotoXY(218,80);
            GUI_DispFloat(outPower, 6);
            GUI_GotoXY(250,96);
            GUI_DispFloat(outFreq, 6);
            break;
        
        case 3:
            
            break;
            
        default:
            break;
    }
}
