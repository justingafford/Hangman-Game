/*Name & E-mail: Justin Gafford jgaff001@ucr.edu
*	Lab Section: 022
*	Assignment: Custom Project(Hangman)
*/

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include<string.h>
#include<stdlib.h>

#include "timeout.h"
#include "LCD.h"
#include "LCD.cpp"
#include "io.h"
#include "io.c"
#include "ADC_H.h"
#include "ADC_C.c"
#include "victory.h"
#include "next.h"
#include "intro.h"
#include "introscreen.h"	
#include "hangmanOne.h"
#include "hangmanTwo.h"
#include "hangmanThree.h"
#include "hangmanFour.h"
#include "hangmanFive.h"
#include "hangmanSix.h"
#include "hangmanSeven.h"
#include "hangmanEight.h"
#include "hangmanNine.h"
#include "hangmanTen.h"
LCD_5110 lcd;

void InitADC(void)
{
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2); //ENABLE ADC, PRESCALER 128
}
uint16_t readadc(uint8_t ch)
{
	ch&=0b00000111;         //ANDing to limit input to 7
	ADMUX = (ADMUX & 0xf8)|ch;  //Clear last 3 bits of ADMUX, OR with ch
	ADCSRA|=(1<<ADSC);        //START CONVERSION
	while((ADCSRA)&(1<<ADSC));    //WAIT UNTIL CONVERSION IS COMPLETE
	return(ADC);        //RETURN ADC VALUE
}

char words1[][13]={"MEMES","OWL","UCR","ARRANGEMENT","MASTICATE","SUMMER","LOSER","COMPUTER","BREEZE","BRICK","CALM","CANAL","WOLF","CAST","CHOSE","CLAWS","HUNDRED","MOON","CASTRO","COOKIES","CUSTOMS","DAMAGE","DANNY","DEEPLY","DEPTH","DISCUSSION","DOLL","GAMER","MINECRAFT","PWM","MACHINE","HUNDRED","DREAD","JUSTIN","SUNDRESS"};
unsigned char pos=0,ran=0,i,count=0,f=0,length;
char a[]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char z[10]="<   >";
uint8_t score = 0;
char buffer[20];

int main(void) {
	
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines
	DDRA=0x00; PORTA=0xFF;
	lcd.lcd_init(&PORTB, PB1, &PORTB, PB2, &PORTB, PB3, &PORTB, PB5, &PORTB, PB7);
	LCD_init();
	uint16_t x;
	uint16_t press;
	InitADC();
	/*
	unsigned char temp1,temp2;
	
	temp1 = 0;
	temp2 = 0;
	for(int i = 0; i < 48; i++) {
		lcd.drawPixel(temp1,temp2,0);
		lcd.drawPixel(temp1,0,0);
		lcd.drawPixel(0,temp2,0);
		temp1++;
		temp2++;
	}
	*/
	lcd.printPictureOnLCD(intro);
	_delay_ms(3000);
	lcd.lcd_clear();
	xy:		unsigned char pos=0,ran=0,i,count=0,f=0,c=0,r;
			r=rand() / (RAND_MAX / 35 + 1);
			char buff[13];
			strcpy(buff,words1[r]);
			length=strlen(buff);
			char disp[13];
			for(i=0;i<length;i++)
			disp[i]='_';
			disp[i]='\0';
			
			//LCD_init();	
			//LCD_DisplayString(1,"Score / Hi-Score");
			//LCD_Cursor(17);
			//LCD_WriteData(score);
			//LCD_DisplayString(18," , ");
			//LCD_WriteData(j);
	while (1) {
		
		lcd.lcd_goto_xy(4,0);
		lcd.lcd_string_format("HANGMAN", 0);
		//_delay_ms(200);
		lcd.lcd_goto_xy(2,2);
		lcd.lcd_string_format(disp, 0);
		//_delay_ms(200);
		lcd.lcd_goto_xy(4,4);
		z[2]=a[pos];
		lcd.lcd_string_format(z, 0);
		_delay_ms(200);
		lcd.lcd_goto_xy(0,5);
		lcd.lcd_string_format("Score: ", 0);
		char buffy [33];
		char* nice = itoa(score,buffy,10);
		lcd.lcd_str(nice);
		lcd.lcd_goto_xy(9,5);
		lcd.lcd_string_format("HS: ", 0);
		char* meme = itoa(eeprom_read_byte(0),buffy,10);
		lcd.lcd_str(meme);
		
		x = readadc(0);
		x = x - 512;
		_delay_ms(200);
		
		if(x > 900)
		{
			joystick:
			x = readadc(0);
			x = x - 512;
			while (x > 900) {
				goto joystick;
			}
			pos++;
			if(pos==26)
			pos=0;
			
		}
		
		else if(x < 100)
		{
			joystick2:
			x = readadc(0);
			x = x - 512;
			while (x < 100) {
				goto joystick2;
			}
			pos--;
			if(pos==-1)
			pos=25;
			
		}
		
		else
		 {
			press = readadc(1);
			press = press - 512;
			_delay_ms(200);
			if(press > 900)
			{
				joystick3:
				readadc(1);
				press = press - 512;
				while (press > 900) {
					goto joystick3;
				}
				for(i=0;i<strlen(buff);i++) {
					if(buff[i]==z[2]) {
						disp[i]=z[2];
						//PORTB=0x02;
						_delay_ms(200);
						//PORTB=0x00;
						f=1;
						if(strcmp(buff,disp) == 0) {
							lcd.printPictureOnLCD(victory);
							//PORTB = 0x02;
							_delay_ms(200);
							score++;
							if(score > eeprom_read_byte(0)) {
								eeprom_update_byte(0,score);
								LCD_init();
								LCD_WriteCommand(! + '0');
								_delay_ms(2000);
								LCD_ClearScreen();
							}
							//PORTB = 0x00;
							_delay_ms(1800);
							lcd.lcd_clear();
							
							goto xy;
						}
					}
				}
				if(f==0) {
					switch(count) {
						case 1:		
							lcd.printPictureOnLCD(hangmanOne);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 2:
							lcd.printPictureOnLCD(hangmanTwo);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 3:		
							lcd.printPictureOnLCD(hangmanThree);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 4:		
							lcd.printPictureOnLCD(hangmanFour);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 5:		
							lcd.printPictureOnLCD(hangmanFive);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 6:		
							lcd.printPictureOnLCD(hangmanSix);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 7:		
							lcd.printPictureOnLCD(hangmanSeven);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 8:		
							lcd.printPictureOnLCD(hangmanEight);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 9:		
							lcd.printPictureOnLCD(hangmanNine);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							break;
					
						case 10:	
							lcd.printPictureOnLCD(hangmanTen);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
						case 11:
							lcd.lcd_goto_xy(4,4);
							lcd.lcd_string_format(words1[r], 0);
							_delay_ms(2000);
							lcd.printPictureOnLCD(next);
							//PORTA=0x02;
							_delay_ms(600);
							//PORTA=0x00;
							_delay_ms(1400);
							lcd.lcd_clear();
							goto xy;
							break;
					}
					count++;
				}
				f=0;
			}
		
		}
	
	}
}
