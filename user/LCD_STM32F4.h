/*
 * LCD_STM32F4.h
 *
 *  Created on: 11.4.2012
 *  Author: martin, jopl
 */

/*
 * How to connect LCD with STM32F4Discovery:
 *
 * RS    ->  PD11
 * WR    ->  PD5
 * RD    ->  PD4
 * CS    ->  PD7
 * RESET ->  PC13
 *
 * DB0   ->  PD14
 * DB1   ->  PD15
 * DB2   ->  PD0
 * DB3   ->  PD1
 * DB4   ->  PE7
 * DB5   ->  PE8
 * DB6   ->  PE9
 * DB7   ->  PE10
 * DB8   ->  PE11
 * DB9   ->  PE12
 * DB10  ->  PE13
 * DB11  ->  PE14
 * DB12  ->  PE15
 * DB13  ->  PD8
 * DB14  ->  PD9
 * DB15  ->  PD10
 *
 * BL_CNT -> PD12
 *
 * TP_IRQ -> PB12
 * TP_SO -> PB14
 * TP_SI -> PB15
 * TP_SCK -> PB13
 * TP_CS -> PC6
 *
 *
 * Display coordinates:
 *  [x,y]
 *     [0,0]                                 [319, 0]
 *       -----------------------------------------
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       |             TOUCH DISPLAY             |
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       |                                       |
 *       -----------------------------------------
 *   [239,0]                               [319,239]
 */

#ifndef LCD_STM32F4_H_
#define LCD_STM32F4_H_

/* Includes ******************************************************************/

#include "../cmsis_boot/stm32f4xx.h"
#include "stm32f4xx_it.h"


/* Type Definitions **********************************************************/

typedef struct
{
  __IO uint16_t Register;  /* LCD Index Write            Address offset 0x00 */
  __IO uint16_t Data;      /* LCD Data Write             Address offset 0x02 */
}LCD_TypeDef;

/* Definitions ***************************************************************/

/*
 * FSMC Memory Bank 1: 0x60000000 to 6FFFFFFF
 * NAND bank selections:
 *   SECTION NAME      HADDR[17:16]  ADDRESS RANGE
 *   Address section   1X            0x020000-0x03FFFF
 *   Command section   01            0x010000-0x01FFFF
 *   Data section      00            0x000000-0x0FFFF
 */

#define LCD_BASE            ((uint32_t) (0x60000000 | 0x0001FFFE))
#define LCD                 ((LCD_TypeDef *) LCD_BASE)

#define GDDRAM_PREPARE      0x0022  /* Graphic Display Data RAM Register. */


/* Function Prototypes *******************************************************/

void Init_LCD(void);
void Write_Command(uint16_t reg, uint16_t data);

void Init_GPIO(void);
void Init_FSMC(void);
void Init_SysTick(void);
void TimingDelay_Decrement(void);
void _InitController(void);


#endif /* LCD_STM32F4_H_ */
