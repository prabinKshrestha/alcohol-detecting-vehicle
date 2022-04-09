#ifndef LCD
#define LCD

#include<avr/io.h>
#include<util/delay.h>
#include<stdlib.h>


// define
#define MrLCDsCrib				    PORTB
#define dataDir_MrLCDsCrib			DDRB
#define MrLCDsControl 	 		    PORTD
#define dataDir_MrLCDsControl		DDRD
#define LightSwitch		 		   	5
#define ReadWrite 			        7
#define BipolarMood 			    2 

//function declaration

void LCD_initial(void);
void check_if_MrLCD_is_busy(void);
void peek_a_boo(void);
void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(char *string_of_characters);
void LCD_clear(void);
void string_location(uint8_t x,uint8_t y);
char first_column_of_display[2] = {0, 64}; //location in LCD : 




void LCD_initial(void)
{
	dataDir_MrLCDsControl |=1<<LightSwitch | 1<<ReadWrite | 1<<BipolarMood ; // all are output
	_delay_ms(15);
		
	send_a_command(0x01); 						// clear the screen 0x01 = 0b000000001
	_delay_ms(2);
		
	send_a_command(0x38);						// 8 bit Data
	_delay_us(50);
	
	send_a_command(0b00001110);					// cursor related
	_delay_us(50);
}
void LCD_clear()
{
	dataDir_MrLCDsControl |=1<<LightSwitch | 1<<ReadWrite | 1<<BipolarMood ; // all are output
	_delay_ms(15);
	send_a_command(0x01);
	_delay_ms(2);
}

void check_if_MrLCD_is_busy(void)
{
	dataDir_MrLCDsCrib =0;					// all input
	MrLCDsControl |=1<<ReadWrite;			//RW=high
	MrLCDsControl &= ~1<<BipolarMood;		//RS=0 command mode 
	while(MrLCDsCrib >=0x80)				// if busy then is greater than 0x80 0b10000000
	{
		peek_a_boo();						// light goeas on and off
	}
	dataDir_MrLCDsCrib =0xFF;				// all output
}
void peek_a_boo()
{
	MrLCDsControl |=1<<LightSwitch;			// en=on
	asm volatile ("nop");
	asm volatile ("nop");
	MrLCDsControl &= ~1<<LightSwitch;		// en=off
}

void send_a_command(unsigned char command)
{
	check_if_MrLCD_is_busy();								//checking if lcd is busy
	MrLCDsCrib = command;									//setting portb as command because PORT acts as output
	MrLCDsControl &= ~(1<<ReadWrite|1<<BipolarMood);		//RW off for readwrite and RS off for command
	peek_a_boo();	
	MrLCDsCrib=0;											//portB 0 so no error
}
void send_a_character(unsigned char character)
{
	check_if_MrLCD_is_busy();
	MrLCDsCrib = character;
	MrLCDsControl &= ~(1<<ReadWrite);
	MrLCDsControl |= 1<<BipolarMood;						//RS high for character
	peek_a_boo();
	MrLCDsCrib=0;	
}

void string_location(uint8_t x,uint8_t y)
{
	send_a_command(0x80+first_column_of_display[y]+x);
}

void send_a_string(char *string_of_characters)
{
	while(*string_of_characters>0)
	{
		send_a_character(*string_of_characters++);
	}
}

#endif
