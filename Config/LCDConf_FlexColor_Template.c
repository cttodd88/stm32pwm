/*********************************************************************
*          Portions COPYRIGHT 2014 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.26 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include "../inc/GUI.h"
#include "../inc/GUIDRV_FlexColor.h"

#include "../user/LCD_STM32F4.h"

#include "../user/GUITDRV_ADS7846.h"
#include "../user/ts.h"

GUITDRV_ADS7846_CONFIG pConfig;
GUITDRV_ADS7846_LAST_VAL p;
GUI_PID_STATE pstate;

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Physical display size
//
#define XSIZE_PHYS  240 // To be adapted to x-screen size
#define YSIZE_PHYS  320 // To be adapted to y-screen size

#define VXSIZE_PHYS         240
#define VYSIZE_PHYS         320

/* Color conversion */
#define COLOR_CONVERSION    GUICC_M565

/* Display driver */
#define DISPLAY_DRIVER GUIDRV_COMPACT_COLOR_16

//
// Buffers / VScreens
//
#define NUM_BUFFERS   1
#define NUM_VSCREENS  1

//
// Display orientation
//

//#define DISPLAY_ORIENTATION  0
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_SWAP_XY | GUI_MIRROR_X)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X)
#define DISPLAY_ORIENTATION (GUI_SWAP_XY)

//
// Touch screen
//
#define USE_TOUCH   1
//
// Touch screen calibration
//#define TOUCH_X_MIN 0x00E0
//#define TOUCH_X_MAX 0x0F40
//#define TOUCH_Y_MIN 0x00C0
//#define TOUCH_Y_MAX 0x0F60

//#define TOUCH_X_MIN 372
//#define TOUCH_X_MAX 3863
//#define TOUCH_Y_MIN 274
//#define TOUCH_Y_MAX 3836

//mel  calibrated
#define GUI_TOUCH_AD_TOP 1917  //128
#define GUI_TOUCH_AD_BOTTOM 118 //1856
#define GUI_TOUCH_AD_LEFT 82  //64
#define GUI_TOUCH_AD_RIGHT 1964 //1920

#define TOUCH_X_MIN 118
#define TOUCH_X_MAX 1917
#define TOUCH_Y_MIN 82
#define TOUCH_Y_MAX 1964



/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif


#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif

#ifndef   LCD_SWAP_XY
  #define LCD_SWAP_XY 0
#endif
#if LCD_SWAP_XY
  #define LCD_XSIZE YSIZE_PHYS
  #define LCD_YSIZE XSIZE_PHYS
#else
  #define LCD_XSIZE XSIZE_PHYS
  #define LCD_YSIZE YSIZE_PHYS
#endif
#define LCD_VXSIZE LCD_XSIZE
#define LCD_VYSIZE LCD_YSIZE


#ifndef   DISPLAY_ORIENTATION
  #define DISPLAY_ORIENTATION  0
#endif

#if ((DISPLAY_ORIENTATION & GUI_SWAP_XY) != 0)
#define LANDSCAPE   1
#else
#define LANDSCAPE   0
#endif

#if (LANDSCAPE == 1)
#define WIDTH       YSIZE_PHYS  /* Screen Width (in pixels)         */
#define HEIGHT      XSIZE_PHYS  /* Screen Hight (in pixels)         */
#else
#define WIDTH       XSIZE_PHYS  /* Screen Width (in pixels)         */
#define HEIGHT      YSIZE_PHYS  /* Screen Hight (in pixels)         */
#endif

#if ((DISPLAY_ORIENTATION & GUI_SWAP_XY) != 0)
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_X) != 0)
    #define TOUCH_TOP    TOUCH_X_MAX
    #define TOUCH_BOTTOM TOUCH_X_MIN
  #else
    #define TOUCH_TOP    TOUCH_X_MIN
    #define TOUCH_BOTTOM TOUCH_X_MAX
  #endif
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_Y) != 0)
    #define TOUCH_LEFT   TOUCH_Y_MAX
    #define TOUCH_RIGHT  TOUCH_Y_MIN
  #else
    #define TOUCH_LEFT   TOUCH_Y_MIN
    #define TOUCH_RIGHT  TOUCH_Y_MAX
  #endif
#else
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_X) != 0)
    #define TOUCH_LEFT   TOUCH_X_MAX
    #define TOUCH_RIGHT  TOUCH_X_MIN
  #else
    #define TOUCH_LEFT   TOUCH_X_MIN
    #define TOUCH_RIGHT  TOUCH_X_MAX
  #endif
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_Y) != 0)
    #define TOUCH_TOP    TOUCH_Y_MAX
    #define TOUCH_BOTTOM TOUCH_Y_MIN
  #else
    #define TOUCH_TOP    TOUCH_Y_MIN
    #define TOUCH_BOTTOM TOUCH_Y_MAX
  #endif
#endif

/*********************************************************************
*
*       Defines, sfrs
*
**********************************************************************
*/
//
// COG interface register addr.
//
//#define LCD_BASE           ((u32)(0x60000000|0x00000000))
//#define LCD_BASE_DATA     ((u32)(0x60000000|0x00020000))
//#define LCD_REG_ADDRESS   (*(volatile U16*)LCD_BASE)
//#define LCD_DATA_ADDRESS  (*(volatile U16*)(LCD_BASE_DATA))

#define  LCD_BASE_Addr               ((U32)(0x60000000 | 0x00000000))
#define  LCD_BASE_Data               ((U32)(0x60000000|0x00020000))
#define  LCD_CMD                     (*(U16 *)(LCD_BASE_Addr))
#define  LCD_Data                    (*(U16 *)(LCD_BASE_Data))




/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
/********************************************************************
*
*       LcdWriteReg
*
* Function description:
*   Sets display register
*/
static void LcdWriteReg(U16 Data) {
  // ... TBD by user
	LCD->Register = Data;
}

/********************************************************************
*
*       LcdWriteData
*
* Function description:
*   Writes a value to a display register
*/
static void LcdWriteData(U16 Data) {
  // ... TBD by user
	LCD->Data = Data;
}

/********************************************************************
*
*       LcdWriteDataMultiple
*
* Function description:
*   Writes multiple values to a display register.
*/
static void LcdWriteDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
	  LCD_Data = *pData++;
  }
}

/********************************************************************
*
*       LcdReadDataMultiple
*
* Function description:
*   Reads multiple values from a display register.
*/
static void LcdReadDataMultiple(U16 * pData, int NumItems) {
  while (NumItems--) {
    // ... TBD by user
	  *pData++ = LCD_Data;
  }
}

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/


/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void) {

  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};



  //
  // Set display driver and color conversion
  //
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_M565, 0, 0);		//mel GUICC_565 will swap red and blue - might need for bmp display
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	//but will mess up color defines in emwin
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetSizeEx (0, XSIZE_PHYS , YSIZE_PHYS);
  LCD_SetVSizeEx(0, VXSIZE_PHYS, VYSIZE_PHYS);
  //
  // Orientation
  //
  Config.Orientation = GUI_SWAP_XY | GUI_MIRROR_Y;
  Config.NumDummyReads = 2; //maybe
  GUIDRV_FlexColor_Config(pDevice, &Config);
  //
  // Set controller and operation mode
  //

  PortAPI.pfWrite16_A0  = LcdWriteReg;
  PortAPI.pfWrite16_A1  = LcdWriteData;
  PortAPI.pfWriteM16_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM16_A1  = LcdReadDataMultiple;

  //Set up the driver to use one of the following controllers:
  //GUIDRV_FLEXCOLOR_F66702		Solomon SSD1284, SSD1289, SSD1298 *
  //GUIDRV_FLEXCOLOR_F66720		Solomon SSD1961, SSD1963

  //Permitted values for parameter pfMode
  //GUIDRV_FLEXCOLOR_M16C0B16	16bpp, no cache, 16 bit bus *
  //GUIDRV_FLEXCOLOR_M16C1B16	16bpp, cache, 16 bit bus
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66702, GUIDRV_FLEXCOLOR_M16C0B16);

  /*
  PortAPI.pfWrite8_A0  = LcdWriteReg;
  PortAPI.pfWrite8_A1  = LcdWriteData;
  PortAPI.pfWriteM8_A1 = LcdWriteDataMultiple;
  PortAPI.pfReadM8_A1  = LcdReadDataMultiple;
  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI, GUIDRV_FLEXCOLOR_F66702, GUIDRV_FLEXCOLOR_M16C0B16);
  */

  //GUITDRV_ADS7846_Config(pConfig);

   //
   // Display driver configuration
   //
  if (LCD_GetSwapXY()) {
     LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
     LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
   } else {
     LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
     LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
   }

/*#if (USE_TOUCH == 1)

  	 pConfig.pfSendCmd = &pfSendCmd;
  	 pConfig.pfGetResult = &pfGetResult;
  	 pConfig.pfGetBusy = &pfGetBusy;
  	 pConfig.pfSetCS = &pfSetCS;

  	 pConfig.pfGetPENIRQ = &pfGetPENIRQ;

  	 pConfig.Orientation = GUI_SWAP_XY | GUI_MIRROR_X;
  	 // - Orientation: Orientation of the touch screen if not the same as the physical orientation.
  	 //                 A or-combination of the defines GUI_SWAP_XY, GUI_MIRROR_X and GUI_MIRROR_Y
  	 //                 can be used.
  	 pConfig.xLog0 = 0; 				// - xLog0      : Logical pixel value of horizontal reference point 0. Typically 0.
  	 pConfig.xLog1 = XSIZE_PHYS -1; 	// - xLog1      : Logical pixel value of horizontal reference point 1. Typically horizontal screen resolution -1.
  	 pConfig.xPhys0 = TOUCH_LEFT;		// - xPhys0     : Physical AD value of horizontal reference point 0.
  	 pConfig.xPhys1 = TOUCH_RIGHT;		// - xPhys1     : Physical AD value of horizontal reference point 1.
  	 pConfig.yLog0 = 0;					// - yLog0      : Logical pixel value of vertical reference point 0. Typically 0.
  	 pConfig.yLog1 = YSIZE_PHYS - 1;	// - yLog1      : Logical pixel value of vertical reference point 1. Typically vertical screen resolution -1.
  	 pConfig.yPhys0 = TOUCH_TOP;		// - yPhys0     : Physical AD value of vertical reference point 0.
  	 pConfig.xPhys1 = TOUCH_BOTTOM;		// - yPhys1     : Physical AD value of vertical reference point 1.


  	GUITDRV_ADS7846_Config(&pConfig);*/
    //
    // Set orientation of touch screen
    //
    //GUI_TOUCH_SetOrientation(DISPLAY_ORIENTATION);

    //
    // Calibrate touch screen
    //
    //GUI_TOUCH_Calibrate(GUI_COORD_X, 0, WIDTH  - 1, TOUCH_LEFT, TOUCH_RIGHT);
    //GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, HEIGHT - 1, TOUCH_TOP,  TOUCH_BOTTOM);


  //#endif
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd) {
  case LCD_X_INITCONTROLLER: {
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...
    return 0;
  }
  default:
    r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

