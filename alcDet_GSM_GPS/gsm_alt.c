
#ifndef gsm_alt
#define gsm_alt

#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "LCD.h"
#include "USART_Interrupt.h"

// Standard Input/Output functions
#include <stdio.h>


unsigned int z;

unsigned char cmd_1[]={"AT"};
unsigned char cmd_2[]={"AT+CMGF=1"};
unsigned char cmd_3[]={"AT+CMGS="};
unsigned char cmd_4[]={"Your Driver Is Drunk"};
unsigned char cmd_5[]={"+9779843358599"};

void gsm_msg_send();

void gsm_msg_send()
{

	LCD_initial();
	LCD_clear();
	string_location(2,0);
	lcd_putsf("GSM Working:");
	_delay_ms(4000);
	//LCD_clear();
  
			LCD_clear();
			string_location(1,1);
			send_a_string("Sending Msg...");
			
			for(z=0;cmd_1[z]!='';z++)
			{
				UDR = cmd_1[z];
				_delay_ms(100);
			}
			
			UDR = ('\r');
			_delay_ms(500);
			
			for(z=0;cmd_2[z]!='';z++)
			{
				UDR = cmd_2[z];
				_delay_ms(100);
			}
			
			UDR = ('\r');
			_delay_ms(500);
			
			for(z=0;cmd_3[z]!='';z++)
			{
				UDR = cmd_3[z];
				_delay_ms(100);
			}
			
			UDR = ('"');
			_delay_ms(100);
			
			for(z=0;cmd_5[z]!='';z++)
			{
				UDR = cmd_5[z];
				_delay_ms(100);
			}
			
			UDR = ('"');
			_delay_ms(100);
			
			UDR = ('\r');
			_delay_ms(500);
			
			for(z=0;cmd_4[z]!='';z++)
			{
				UDR = cmd_4[z];
				_delay_ms(100);
			}
			
			UDR = (26);                   // ctrlZ-> to send the message
			_delay_ms(500);
			
			LCD_clear();
			string_location(2,0);
			lcd_putsf("GSM Working:");
			//_delay_ms(4000);
			string_location(0,1);
			lcd_putsf("Message Sent.");
			delay_ms(2000);
			LCD_clear();
}   


#endif