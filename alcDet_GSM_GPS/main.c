//inbuilt library

#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//user-defined library

#include "LCD.h"
#include "USART_Interrupt.h"
#include "gsm_alt.h"
#include "gps_alt.h"

//for buzzer and output from the alcohol sensor

#define dataDir_alcohol DDRC
#define alcohol PORTC
#define buzzer 1
#define digital_output 0
//#define gps_vcc 2
#define A1 3
#define A2 4
#define A3 5
#define A4 6
#define button 0

int main (void)
{
	
	dataDir_alcohol &= ~1<<digital_output;
	DDRA &= ~1<<button;
	DDRA &= ~1<<buzzer;
	DDRA |= 1<<A1 | 1<<A2 | 1<<A3 | 1<<A4; 
	
	
	dataDir_alcohol |= 1<<buzzer;
	//dataDir_alcohol |= 1<<gps_vcc ;
	
	PORTA |= 1<<PINA0; //button
	
	PORTA &= ~(1<<A1 | 1<<A4);			//this is for motor ic to stop car
	PORTA &= ~(1<<A2 | 1<<A3);
			
	
	
	//for gsm
	unsigned char number[]="+9779843358599";
	unsigned char sms[]="https://maps.google.com/maps?q=";
	
	
	//
	
	LCD_initial();
	
	//***********************body**************************//
	
	while(1) 														// infinte loop
	{	
		LCD_initial();
		//LCD_clear();
		
		if(bit_is_clear(PINA,0))									//button pressed
		{
			string_location(3,0);
			send_a_string("Starting...");
			_delay_ms(200);
			
			while(1)
			{
				if(!bit_is_clear(PINA,1))
				{
						alcohol |= 1<<buzzer;

						//stopping car
						PORTA &= ~(1<<A1 | 1<<A4);
						PORTA &= ~(1<<A2 | 1<<A3);
		
						
						gps_get();
						
						strcat(sms,lati_final);
						strcat(sms,",");
						strcat(sms,longi_final);
						_delay_ms(1500);
						
						if(gps_value)
						{
							gsm_msg_send(number,sms);									//to send the message
						} 
						else 
						{
							LCD_clear();
							string_location(1,0);
							send_a_string("Couldn't locate:");
							string_location(1,1);
							send_a_string("Check gps");
							_delay_ms(1000);
						}
						break;
				}
				
				else
				{
					alcohol &= ~1<<buzzer;
					alcohol |= 1<<PINC3;
					
					PORTA |= 1<<A1 | 1<<A4;				//to start car
					PORTA &= ~(1<<A2 | 1<<A3);
					
					string_location(3,0);
					send_a_string("No Alcohol");
					string_location(2,1);
					send_a_string("Well Done!!!");
					_delay_ms(1000);
				}
			}					//end o inner while
		}
		else
		{
			string_location(3,0);
			send_a_string("Standing By...");
		}		
	}
}

