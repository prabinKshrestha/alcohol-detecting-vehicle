#ifndef gsm_alt
#define gsm_alt

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "LCD.h"
#include "USART_Interrupt.h"


void gsm_msg_send(unsigned char cmd_5[],unsigned char cmd_4[]);

void gsm_msg_send(unsigned char cmd_5[],unsigned char cmd_4[])
{
	unsigned int z;

	unsigned char cmd_1[]={"AT"};
	unsigned char cmd_2[]={"AT+CMGF=1"};
	unsigned char cmd_3[]={"AT+CMGS="};
	//unsigned char cmd_4[]={"Your Driver is Drunk"};
	//unsigned char cmd_5[]={"+9779843358599"};

	LCD_clear();
	USART_Init_GSM(9600);	
	
	//sei();
	LCD_clear();
	_delay_ms(100);
	
	string_location(1,0);
	send_a_string("Got Location");
	_delay_ms(100);
	string_location(1,1);
	send_a_string("GSM Working...");
	
	/*for(int x=79;x>=64;x--)
				{
					string_location(x,0);
					send_a_string("Sending Message");
					_delay_ms(40);					
				}
	_delay_ms(100);
	*/		
			for(z=0;cmd_1[z]!='\0';z++)				//for at command
			{
				UDR = cmd_1[z];
				_delay_ms(100);
			}		
			UDR = ('\r');								//to enter the command
			_delay_ms(500);
			
			for(z=0;cmd_2[z]!='\0';z++)				//for sms mode command
			{
				UDR = cmd_2[z];
				_delay_ms(100);
			}			
			UDR = ('\r');
			_delay_ms(500);
			
			for(z=0;cmd_3[z]!='\0';z++)				//for sms sending command
			{
				UDR = cmd_3[z];
				_delay_ms(100);
			}			
			UDR = ('"');								///for " before number
			_delay_ms(100);
			
			for(z=0;cmd_5[z]!='\0';z++)				//for number writing
			{
				UDR = cmd_5[z];
				_delay_ms(100);
			}			
			UDR = ('"');								//for " after number
			_delay_ms(100);			
			UDR = ('\r');
			_delay_ms(500);
			
			for(z=0;cmd_4[z]!='\0';z++)				//for sms wrting
			{
				UDR = cmd_4[z];
				_delay_ms(100);
			}			
			UDR = (26);                 			  // ctrlZ-> to send the message
			_delay_ms(500);
			
			LCD_clear();
			string_location(1,0);
			send_a_string("Done!");
			//_delay_ms(4000);
			string_location(1,1);
			send_a_string("Message Sent!");
			_delay_ms(1000);
			LCD_clear();
}   


#endif