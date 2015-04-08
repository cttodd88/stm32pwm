#include "lcd.h"
#include "../cmsis_lib/include/stm32f4xx_gpio.h"
#include "../cmsis_lib/include/stm32f4xx_rcc.h"


extern void Delay(unsigned int nTime);

#define LCD_RS GPIOE,GPIO_Pin_8	 //control/data mode
#define LCD_RW GPIOE,GPIO_Pin_9	 //read/!write
#define LCD_EN GPIOE,GPIO_Pin_10 //enable
#define LCD_BL GPIOE,GPIO_Pin_11 //backlight

#define nop Delay(1);
#define	LCD_STROBE	GPIO_SetBits(LCD_EN);   nop; 	GPIO_ResetBits(LCD_EN)
#define LCD_PORTEN  GPIO_SetBits(LCD_RW);	nop;	GPIO_ResetBits(LCD_RW)


void lcdWrite(unsigned char c)
{
	GPIOE->BSRRH=0xF000;
	GPIOE->BSRRL=(c & 0xF0)<<8;

	LCD_STROBE;
	GPIOE->BSRRH=0xF000;
	GPIOE->BSRRL=(c & 0x0F)<<12;

	LCD_STROBE;
	Delay(1);
}



void lcd_clear(void)
{
	LCD_PORTEN;
	GPIO_WriteBit(LCD_RS,0);
	lcdWrite(0x1);
	Delay(2);
}

void lcdGoto(unsigned char pos)
{
	LCD_PORTEN;
	GPIO_ResetBits(LCD_RS);
	lcdWrite(0x80+pos);		//80 is goto command
}

void lcdPutch(char c)
{
	LCD_PORTEN;
	GPIO_SetBits(LCD_RS);// write characters

	GPIOE->BSRRH=0xF000;
	GPIOE->BSRRL=(c & 0xF0)<<8;		// send MSNibble
	LCD_STROBE;

	GPIOE->BSRRH=0xF000;
	GPIOE->BSRRL=(c & 0x0F)<<12;		// send LSNibble
	LCD_STROBE;
	Delay(1);
}


void lcd_puts(const char * s)
{
	LCD_PORTEN;
	GPIO_SetBits(LCD_RS);	// write characters
	while(*s)
	lcdWrite(*s++);
}

void lcdInit(void){

	GPIO_WriteBit(LCD_EN,0);
	LCD_PORTEN;
	GPIO_WriteBit(LCD_RS,0);
	Delay(30);

	lcdWrite(0x03);
	Delay(5);

	lcdWrite(0x03);
	Delay(5);

	lcdWrite(0x03);
	Delay(5);

	lcdWrite(0x02);
	Delay(5);



	lcdWrite(0x28);
	lcdWrite(0x0c);	// display on cursor off blink off
	lcd_clear();

	lcdWrite(0x06);	// entry mode set increment mode/ entire shift off



}


