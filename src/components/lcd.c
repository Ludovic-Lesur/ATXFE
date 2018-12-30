/*
 * lcd.c
 *
 *  Created on: 29 dec. 2018
 *      Author: Ludo
 */

#include "lcd.h"

#include "gpio.h"
#include "gpio_reg.h"
#include "mapping.h"
#include "tim.h"

/*** LCD local macros ***/

#define LCD_ROW_IDX_MAX			1
#define LCD_COLUMN_IDX_MAX		7

/*** LCD local functions ***/

/* CREATE A PULSE ON ENABLE SIGNAL TO WRITE LCD REGISTERS.
 * @param:	None.
 * @return:	None.
 */
void LCD_EnablePulse(void) {
	// Pulse width > 300ns.
	GPIO_Write(GPIO_LCD_E, 1);
	TIM22_WaitMilliseconds(2);
	GPIO_Write(GPIO_LCD_E, 0);
}

/* SEND A COMMAND TO LCD SCREEN.
 * @param lcd_command:	Command to send.
 * @return:				None.
 */
void LCD_Command(unsigned char lcd_command) {

	/* Put command on output port */
	GPIOA -> ODR &= 0xFFFFFE01;
	GPIOA -> ODR |= (lcd_command << 1);

	/* Send command */
	GPIO_Write(GPIO_LCD_RS, 0);
	LCD_EnablePulse();
}

/* SEND DATA TO LCD SCREEN.
 * @param lcd_data:	Data to send.
 * @return:			None.
 */
void LCD_Data(unsigned char lcd_data) {

	/* Put data on output port */
	GPIOA -> ODR &= 0xFFFFFE01;
	GPIOA -> ODR |= (lcd_data << 1);

	/* Send command */
	GPIO_Write(GPIO_LCD_RS, 1);
	LCD_EnablePulse();
}

/*** LCD functions ***/

void LCD_Init(void) {

	/* Init GPIOs */
	GPIO_Configure(GPIO_LCD_E, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_RS, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB0, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB1, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB2, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB3, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB4, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB5, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB6, Output, PushPull, HighSpeed, NoPullUpNoPullDown);
	GPIO_Configure(GPIO_LCD_DB7, Output, PushPull, HighSpeed, NoPullUpNoPullDown);

	/* Initialization sequence */
	GPIO_Write(GPIO_LCD_E, 0);
	TIM22_WaitMilliseconds(100);
	LCD_Command(0x30);
	TIM22_WaitMilliseconds(30);
	LCD_Command(0x30);
	TIM22_WaitMilliseconds(10);
	LCD_Command(0x30);
	TIM22_WaitMilliseconds(10);
	LCD_Command(0x38); // 8-bits / 2 lines mode.
	LCD_Command(0x08); // Display off.
	LCD_Command(0x0C); // Display on.
}

/* PRINT A STRING ON LCD SCREEN.
 * @param row :     		Current row (0 or 1).
 * @param position  		Cursor position (0 to 7).
 * @param string:			Char array to print.
 * @param string_length:	Number of characters to print.
 * @return:					None.
 */
void LCD_Print(unsigned char row, unsigned char column, char* string, unsigned char string_length) {

	/* Check parameters */
	if ((row <= LCD_ROW_IDX_MAX) && (column <= LCD_COLUMN_IDX_MAX)) {

		/* Set position */
		LCD_Command(((row * 0x40) + column) + 0x80);

		/* Print string */
		unsigned char column_idx = column;
		unsigned char string_idx = 0;
		// Loop until string is printed or screen edge is reached.
		while ((column_idx <= LCD_COLUMN_IDX_MAX) && (column_idx < (column + string_length))) {
			LCD_Data(string[string_idx]);
			string_idx++;
			column_idx++;
		}
	}
}

/* CLEAR LCD SCREEN.
 * @param:	None.
 * @return:	None.
 */
void LCD_Clear(void) {
	LCD_Command(0x01);
	TIM22_WaitMilliseconds(2);
}