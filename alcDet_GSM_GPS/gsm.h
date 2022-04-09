/*
    SIM900 GSM Interfacing with ATmega16/32
    http://www.electronicwings.com 
 */ 
 
 #ifndef gsm1
#define gsm1


#define F_CPU 8000000UL						/* define Clock Frequency */
#include <avr/io.h>					
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "LCD_16x2_H_file.h"				/* include 16x2 LCD Header file */
#include "USART_Interrupt.h"				/* include USART Header file */

#define SREG   _SFR_IO8(0x3F)



void GSM_Begin(void);							
void GSM_Response(void);
void GSM_Response_Display(void);
void GSM_Msg_Display(void);
void GSM_Send_Msg(char* , char*);


char buff[160];								/* buffer to store responses and messages */
char status_flag = 0;						/* for checking any new message */
volatile int buffer_pointer;
int position = 0;							/* save location of current message */





void GSM_Begin()
{

	while(1)
	{
		send_a_command(0xc0);
		USART_SendString("ATE0\r");			/* send ATE0 to check module is ready or not */
		_delay_ms(500);
		//GSM_Response(); 
		if(strstr(buff,"OK"))
		{
			GSM_Response();                 /* get Response */
			memset(buff,0,160);
			break;
		}
		else
		{
			send_a_string("Error");
		}
	}
	_delay_ms(1000);

	LCD_clear();
	string_location(1,0);
	send_a_string("Text Mode");
	send_a_command(0xc0);
	USART_SendString("AT+CMGF=1\r");/* select message format as text */
	GSM_Response();
	_delay_ms(1000);
}


void GSM_Send_Msg(char *num,char *sms)
{
	char sms_buffer[35];
	buffer_pointer=0;
	sprintf(sms_buffer,"AT+CMGS=\"%s\"\r",num);
	USART_SendString(sms_buffer);                       /*send command AT+CMGS="Mobile No."\r */
	_delay_ms(200);
	while(1)
	{
		if(buff[buffer_pointer]==0x3e)                  /* wait for '>' character*/
		{
			buffer_pointer = 0;
			memset(buff,0,strlen(buff));
			USART_SendString(sms);                      /* send msg to given no. */
			USART_TxChar(0x1a);                         /* send Ctrl+Z then only message will transmit*/
			break;
		}
		buffer_pointer++;
	}
	_delay_ms(300);
	buffer_pointer = 0;
	memset(buff,0,strlen(buff));
	memset(sms_buffer,0,strlen(sms_buffer));
}


void GSM_Response()
{
	unsigned int timeout=0;
	int CRLF_Found=0;
	char CRLF_buff[2];
	int Response_Length=0;
	while(1)
	{
		if(timeout>=60000)								/*if timeout occur then return */
		return;
		Response_Length = strlen(buff);
		if(Response_Length)
		{
			_delay_ms(2);
			timeout++;
			if(Response_Length==strlen(buff))
			{
				for(int i=0;i<Response_Length;i++)
				{
					memmove(CRLF_buff,CRLF_buff+1,1);
					CRLF_buff[1]=buff[i];
					if(strncmp(CRLF_buff,"\r\n",2))
					{
						if(CRLF_Found++==2)				/* search for \r\n in string */
						{
							GSM_Response_Display();		/* display response */
							return;
						}
					}

				}
				CRLF_Found = 0;

			}
			
		}
		_delay_ms(1);
		timeout++;
	}
	status_flag=0;
}

void GSM_Response_Display()
{
	buffer_pointer = 0;
	int lcd_pointer = 0;
	while(1)
	{
		if(buff[buffer_pointer]== '\r' || buff[buffer_pointer]== '\n')  /* search for \r\n in string */
		{
			buffer_pointer++;
		}
		else
			break;
	}
	

	send_a_command(0xc0);
	while(buff[buffer_pointer]!='\r')								   /* display response till "\r" */
	{
		send_a_character(buff[buffer_pointer]);								
		buffer_pointer++;
		lcd_pointer++;
		if(lcd_pointer==15)								               /* check for end of LCD line */
		send_a_command(0x80);
	}
	buffer_pointer=0;
	memset(buff,0,strlen(buff));
}

ISR(USART_RXC_vect)
{
	buff[buffer_pointer] = UDR;							/* copy UDR(received value) to buffer */
	buffer_pointer++;
	status_flag = 1;						            /* flag for new message arrival */
}



#endif