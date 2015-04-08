/*
 *	LCD interface header file
 *	See lcd.c for more info
 */

/* turn on or off backlight */

extern void lcdBl(char status);

/* write a byte to the LCD in 4 bit mode */

extern void lcdWrite(unsigned char);

/* Clear and home the LCD */

extern void lcd_clear(void);

/* write a string of characters to the LCD */

extern void lcd_puts(const char * s);

/* Go to the specified position */

extern void lcdGoto(unsigned char pos);
	
/* intialize the LCD - call before anything else */

extern void lcdInit(void);

extern void lcdPutch(char);

/*	Set the cursor position */

#define	lcd_cursor(x)	lcd_write(((x)&0x7F)|0x80)
