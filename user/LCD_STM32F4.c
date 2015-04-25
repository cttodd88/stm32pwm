/*
 * LCD_STM32F4.c
 *
 *  Created on: 11.4.2012
 *      Author: martin
 */

/* Includes ******************************************************************/

#include "LCD_STM32F4.h"
#include "stm32f4xx_conf.h"
#include "../cmsis_lib/include/stm32f4xx_gpio.h"
#include "../cmsis_lib/include/stm32f4xx_rcc.h"
#include "../cmsis_lib/include/stm32f4xx_fsmc.h"
#include "../delays.h"


/* Private Variables *********************************************************/



/* Functions *****************************************************************/


/*
 * Reset and Initialize Display.
 */

void Init_LCD(void)
{
  /* Reset */

  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
  DelayMs(30);
  GPIO_SetBits(GPIOC, GPIO_Pin_13);
  DelayMs(10);

  /* Display ON Sequence (data sheet page 72 Solomon Systech SSD1289 Rev1.3 Apr2007) */

  Write_Command(0x0007, 0x0021);
  Write_Command(0x0000, 0x0001);
  Write_Command(0x0007, 0x0023);
  Write_Command(0x0010, 0x0000);  /* Exit Sleep Mode */
  DelayMs(30);
  Write_Command(0x0007, 0x0033);

  /*
   * Entry Mode R11h = 6018h
   *
   * DFM1 = 1, DFM0 = 1 => 65k Color Mode
   * ID0 = 1, AM = 1    => the way of automatic incrementing
   *                       of address counter in RAM
   */
  Write_Command(0x0011, 0x6018);
  Write_Command(0x0002, 0x0600);  /* LCD driver AC Setting */

  /* Initialize other Registers */

  /*
   * Device Output Control R01h = 2B3Fh
   *
   * REV = 1            => character and graphics are reversed
   * BGR = 1            => BGR color is assigned from S0
   * TB  = 1            => sets gate output sequence (see datasheet page 29)
   * MUX[8, 5:0]        => specify number of lines for the LCD driver
   */
  Write_Command(0x0001, 0x2B3F);
}

/*
 * Write to LCD RAM.
 */

void Write_Command(uint16_t reg, uint16_t data)
{
  LCD->Register = reg;
  LCD->Data = data;
}


void Init_GPIO(void)
{
  GPIO_InitTypeDef init={0};

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |
      RCC_AHB1Periph_GPIOC, ENABLE);

  /* RESET */
  init.GPIO_Pin=GPIO_Pin_13;
  init.GPIO_Mode=GPIO_Mode_OUT;
  init.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOC,&init);

  /* PORTD */
  init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
      GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
      GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
  init.GPIO_Mode = GPIO_Mode_AF;
  init.GPIO_Speed = GPIO_Speed_50MHz;
  init.GPIO_OType = GPIO_OType_PP;
  init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &init);

  /* PORTD */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);     // D2
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);     // D3
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);     // NOE -> RD
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);     // NWE -> WR
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);     // NE1 -> CS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);     // D13
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);     // D14
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);    // D15
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);    // A16 -> RS
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);    // D0
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);    // D1

  /* PORTE */
  init.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
      GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
      GPIO_Pin_15;
  init.GPIO_Mode = GPIO_Mode_AF;
  init.GPIO_Speed = GPIO_Speed_50MHz;
  init.GPIO_OType = GPIO_OType_PP;
  init.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &init);

  /* PORTE */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);     // D4
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);     // D5
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);     // D6
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);    // D7
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);    // D8
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);    // D9
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);    // D10
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);    // D11
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);    // D12
}

/*
 * Initialize NOR/SRAM Bank 1.
 */

void Init_FSMC(void)
{

  FSMC_NORSRAMTimingInitTypeDef timing={0};
  FSMC_NORSRAMInitTypeDef init={0};

  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);

  timing.FSMC_AddressSetupTime=0x00;
  timing.FSMC_DataSetupTime=0x0A;
  timing.FSMC_CLKDivision=0x0;
  timing.FSMC_AccessMode=FSMC_AccessMode_A;

  /*
   * Data/Address MUX = Disable
   * Memory Type = SRAM
   * Data Width = 16bit
   * Write Operation = Enable
   * Extended Mode = Disable
   * Asynchronous Wait = Disable
   */

  init.FSMC_Bank=FSMC_Bank1_NORSRAM1;
  init.FSMC_DataAddressMux=FSMC_DataAddressMux_Disable;
  init.FSMC_MemoryType=FSMC_MemoryType_SRAM;
  init.FSMC_MemoryDataWidth=FSMC_MemoryDataWidth_16b;
  init.FSMC_BurstAccessMode=FSMC_BurstAccessMode_Disable;
  init.FSMC_WaitSignalPolarity=FSMC_WaitSignalPolarity_Low;
  init.FSMC_WrapMode=FSMC_WrapMode_Disable;
  init.FSMC_WaitSignalActive=FSMC_WaitSignalActive_BeforeWaitState;
  init.FSMC_WriteOperation=FSMC_WriteOperation_Enable;
  init.FSMC_WaitSignal=FSMC_WaitSignal_Disable;
  init.FSMC_ExtendedMode=FSMC_ExtendedMode_Disable;
  init.FSMC_WriteBurst=FSMC_WriteBurst_Disable;
  init.FSMC_ReadWriteTimingStruct=&timing;
  init.FSMC_WriteTimingStruct=&timing;
  init.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;

  FSMC_NORSRAMInit(&init);
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1,ENABLE);
}


/*********************************************************************
*
*       LCD_X_InitController
*
* Purpose:
*   Initializes the display controller
*/
void _InitController(void)
{
    /* Set up the display controller and put it into operation. If the
    *  display controller is not initialized by any external routine
    *  this needs to be adapted by the customer.
    */


}

