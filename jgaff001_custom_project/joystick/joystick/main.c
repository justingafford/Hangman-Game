#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD_16x2_H_file.h"
#include "LCD_16x2_C_file.c"
#include "ADC_H.h"
#include "ADC_C.c"

int main(void)
{
	char buffer[20];
	int ADC_Value;
	
	ADC_Init();		/* Initialize ADC */
	LCD_Init();		/* Initialize LCD */

	while(1)
	{
		ADC_Value = ADC_Read(0);/* Read the status on X-OUT pin using channel 0 */
		sprintf(buffer, "X=%d   ", ADC_Value);
		LCD_String_xy(1, 0, buffer);
		
		ADC_Value = ADC_Read(1);/* Read the status on Y-OUT pin using channel 0 */
		sprintf(buffer, "Y=%d   ", ADC_Value);
		LCD_String_xy(1, 8, buffer);
		_delay_ms(500);
		//ADC_Value = ADC_Read(2);/* Read the status on SWITCH pin using channel 0 */
		//if(ADC_Value < 600)
		//LCD_String_xy(2, 0, "Switch pressed   ");
		//else
		//LCD_String_xy(2, 0, "Switch open      ");
	}
}

