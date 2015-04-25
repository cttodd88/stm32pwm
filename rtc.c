//rtc.c
//real time clock functions
//Mel Dundas
//April 13, 2015

#include "rtc.h"
#include "inc/GUI.h"

//#define RTC_CLOCK_SOURCE_LSI
#define RTC_CLOCK_SOURCE_LSE

#define BUTTON_USER GPIOA, GPIO_Pin_0

// #define RTCSet

GPIO_InitTypeDef  GPIO_InitStructure;



volatile uint32_t AsynchPrediv = 0, SynchPrediv = 0;


void rtcConfig(void)
{

	/* GPIOA Periph clock enable */
		  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

		  /* Configure PA0 in input mode */
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Reset RTC Domain - used to clear registers like power off
	//will work without but can fix a confused RTC
	RCC_BackupResetCmd(ENABLE);
	RCC_BackupResetCmd(DISABLE);

	//Enable the Power Controller (PWR) APB1 interface clock using the RCC_APB1PeriphClockCmd() function
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);


	//Enable the access to the RTC domain using the PWR_BackupAccessCmd()
	PWR_BackupAccessCmd(ENABLE);

	// Reset RTC Domain - used to clear registers like power off
	//RCC_BackupResetCmd(ENABLE);
	//RCC_BackupResetCmd(DISABLE);



#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
	//	   Enable the LSI OSC - using for clk source
	RCC_LSICmd(ENABLE);

	//	   Wait till LSI is ready
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);


	//Select the RTC clock source using the RCC_RTCCLKConfig() function
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

		  SynchPrediv = 258;	//from RTCCalibrate program other board 290
		  AsynchPrediv = 127;




#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */


	//Enable RTC Clock using the RCC_RTCCLKCmd() function
	RCC_RTCCLKCmd(ENABLE);

//	   Wait for RTC APB registers synchronization
		  RTC_WaitForSynchro();
//mel

#ifdef RTCSet

	if(GPIO_ReadInputDataBit(BUTTON_USER))	//button down
	{
		RTC_StructInit(&myclockInitTypeStruct); 		//init to default values - 24hr, asynch 0x7f, synch 0xff
		myclockInitTypeStruct.RTC_HourFormat = RTC_HourFormat_12;
		myclockInitTypeStruct.RTC_SynchPrediv = SynchPrediv;	//done defaults in StructInit
		myclockInitTypeStruct.RTC_AsynchPrediv = AsynchPrediv;
		RTC_Init(&myclockInitTypeStruct);				//change the parameters you need to

		RTC_TimeStructInit(&myclockTimeStruct);			//init time struct to AM, 0hrs, 0 mins, 0 secs
		myclockTimeStruct.RTC_H12 = RTC_H12_PM;
		myclockTimeStruct.RTC_Hours = 0x05;
		myclockTimeStruct.RTC_Minutes = 0x35;
		myclockTimeStruct.RTC_Seconds = 0x00;
		RTC_SetTime(RTC_Format_BCD, &myclockTimeStruct);	//change to the time needed

		RTC_DateStructInit(&myclockDateStruct);		//init date to Mon, Jan, 01, 00
		myclockDateStruct.RTC_Date = 0x15;
		myclockDateStruct.RTC_Month = RTC_Month_April;
		myclockDateStruct.RTC_WeekDay = RTC_Weekday_Wednesday;
		myclockDateStruct.RTC_Year = 0x15;
		RTC_SetDate(RTC_Format_BCD, &myclockDateStruct);	//change to date needed

		//RTC_DayLightSavingConfig(RTC_DayLightSaving_SUB1H, RTC_StoreOperation_Reset);

	}
#endif //RTCSet
}


void printTime()
{

	char timebuf[20];
	char datebuf[20];

RTC_GetDate(RTC_Format_BCD, &myclockDateStruct);

	GUI_SetFont(&GUI_Font32B_ASCII);

	GUI_DispStringAt("", 40, 50);

	switch(myclockDateStruct.RTC_WeekDay)
	{
	case 1: GUI_DispString("Mon, "); break;
	case 2: GUI_DispString("Tue, "); break;
	case 3: GUI_DispString("Wed, "); break;
	case 4: GUI_DispString("Thu, "); break;
	case 5: GUI_DispString("Fri, "); break;
	case 6: GUI_DispString("Sat, "); break;
	case 7: GUI_DispString("Sun, "); break;
	}


	switch(myclockDateStruct.RTC_Month)
	{
	case 1: GUI_DispString("Jan "); break;
	case 2: GUI_DispString("Feb "); break;
	case 3: GUI_DispString("Mar "); break;
	case 4: GUI_DispString("Apr "); break;
	case 5: GUI_DispString("May "); break;
	case 6: GUI_DispString("Jun "); break;
	case 7: GUI_DispString("Jul "); break;
	case 8: GUI_DispString("Aug "); break;
	case 9: GUI_DispString("Sep "); break;
	case 0x10: GUI_DispString("Oct "); break;
	case 0x11: GUI_DispString("Nov "); break;
	case 0x12: GUI_DispString("Dec "); break;
	}

	sprintf(datebuf, "%02x 20%02x \n\n", myclockDateStruct.RTC_Date, myclockDateStruct.RTC_Year);

	GUI_DispString(datebuf);

	GUI_DispString("\r\n");

	RTC_GetTime(RTC_Format_BCD, &myclockTimeStruct);

	sprintf(timebuf, "%02x:%02x:%02x", myclockTimeStruct.RTC_Hours, myclockTimeStruct.RTC_Minutes, myclockTimeStruct.RTC_Seconds);

	GUI_DispStringHCenterAt(timebuf, 130, 90);

    if(myclockTimeStruct.RTC_H12 == RTC_H12_AM)
    	GUI_DispString(" AM");
    else {
    	GUI_DispString(" PM");
	}
    GUI_DispString("\r\n");

}
