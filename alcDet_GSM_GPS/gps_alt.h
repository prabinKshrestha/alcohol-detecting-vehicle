#ifndef gps_alt
#define gps_alt

#include <avr/io.h>
#include <util/delay.h>
#include "USART_Interrupt.h"
#include "LCD.h"

/*
#define F_CPU 8000000UL
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)*/


unsigned char value,lati_value[10],lati_dir, longi_value[11], longi_dir, time[6],IST_time[4];  //date[6],check[20],test[10];
//unsigned char speed[7],corse_angle[7];
unsigned char lati_final[12], longi_final[12];
unsigned int t;
bool gps_value = false;

void gps_get(void);

void display_time(void);
void convert_to_IST(void);

void display_latitude(void);
void display_longitude(void);


void gps_get()
{
	LCD_initial(); //initialization of LCD
	string_location(1,0);
	send_a_string("You're Drunk!");
	string_location(1,1);
	send_a_string("DO NOT DRIVE!");
	_delay_ms(1000);
	USART_Init(12); // initialization of USART
	string_location(1,0);
	
	LCD_clear();
	
	unsigned char line[100];
	int  i = 0;
	
	while(!gps_value){
	
		while(1)
		{
			
			line[i] = usart_getch();
			if (line[i] == '\n') break;
			i++;
		
			
			
		}
		
		line[i] = 0;

		i=0;
		
		while(i>=0){
		
			if(*(line+i)=='$')
			{
				i++;
				if(*(line+i)=='G')
				{	
					i++;
					if(*(line+i)=='P')
					{
						i++;
						if(*(line+i)=='R')
						{
							i++;
							if(*(line+i)=='M')
							{
								i++;	
								if(*(line+i)=='C')
								{
									//gps_value=true;
									//if(gps_value)
									
									//{
										i++;
										if(*(line+i)==',')
										{
											i++;
											*(time+0) = *(line+i);
											for(t=1;*(line+i)!=',';t++)
											{
												i++;
												*(time+t) = *(line+i);
											}
											
											i++;
											
											while(*(line+i)!=',')				// wait upto next ","
											{
												i++;
											}
											i++;
											
											*(lati_value+0) = *(line+i);
											for(t=1;*(line+i)!=',';t++)
											{
												i++;
												*(lati_value+t) = *(line+i);
											}
											
											i++;
											lati_dir=*(line+i);									
											
											i++;
											
											while(*(line+i)!=',')				// wait upto next ","
											{
												i++;
											}
											i++;
											
											*(longi_value+0) = *(line+i);
											for(t=1;*(line+i)!=',';t++)
											{
												i++;
												*(longi_value+t) = *(line+i);
											}
											
											i++;
											longi_dir=*(line+i);									
											
											
											/*
											
											speed[0] = usart_getch();
											value = speed[0];
											for(t=1;value!=',';t++)
											{
												speed[t] = usart_getch();
												value = speed[t];
											}
											
											corse_angle[0] = usart_getch();
											value = corse_angle[0];
											for(t=1;value!=',';t++)
											{
												corse_angle[t] = usart_getch();
												value = corse_angle[t];
											}
											
											date[0] = usart_getch();
											value = date[0];
											for(t=1;value!=',';t++)
											{
												date[t] = usart_getch();
												value = date[t];
											}
											*/
											
											
											LCD_clear();
											_delay_ms(100);
											display_time();
											_delay_ms(500);
											display_latitude();
											display_longitude();
											_delay_ms(500);
											LCD_clear();
											//_delay_ms(4000);
											//LCD_sendcmd(0x01);
											//_delay_ms(100);
											//display_speed();
											//display_corse();
											//_delay_ms(2000);
											
											//get_latitude();
											gps_value=true;
											i=-1;
										}
									//}
								}
								
							}
							
						}
						
					}
					
				}
				
				
			}
			else{
				i=-1;
			}
		}	
	}
	
		
}




void display_time()
{
	
	
	LCD_clear();
	string_location(1,0);
	
	
	
	
	//send_a_command(0x90);
	send_a_string("NST");
	convert_to_IST();
	send_a_character(*(IST_time+0));
	send_a_character(*(IST_time+1));
	send_a_character(':');
	send_a_character(*(IST_time+2));
	send_a_character(*(IST_time+3));
	//send_a_character(':');
	//send_a_character(*(IST_time+4));
	//send_a_character(*(IST_time+5));
	
	
	
}




void convert_to_IST()
{
	unsigned int UTC_hour, UTC_minute,IST_hour,IST_minute,tmp,temp;
	UTC_hour = (*(time+0)-0x30)*10 + (*(time+1)-0x30);
	UTC_minute = (*(time+2)-0x30)*10 + (*(time+3)-0x30);
	IST_hour = UTC_hour+5;
	if(IST_hour>24) IST_hour-=24;
	IST_minute = UTC_minute+45;
	if(IST_minute>=60)
	{
		IST_minute-=60;
		IST_hour+=1;
	}
	if(IST_hour>=10)//if(IST_hour>10)
	{
		tmp=IST_hour%10;
		*(IST_time+1) = tmp+0x30;
		temp = IST_hour/10;
		*(IST_time+0) = temp+0x30;
	}
	else
	{
		*(IST_time+0) = 0x30;
		tmp=IST_hour%10;
		*(IST_time+1) = tmp+0x30;
	}
	if(IST_minute>=10)//if(IST_minute>10)
	{
		tmp=IST_minute%10;
		*(IST_time+3) = tmp+0x30;
		temp = IST_minute/10;
		*(IST_time+2) = temp+0x30;
	}
	else
	{
		*(IST_time+2) = 0x30;
		tmp=IST_minute%10;
		*(IST_time+3) = tmp+0x30;
	}
}





void display_latitude()
{
	int i=0;
	int lati_count=0;
	for(i=0;i<10;i++){
		
		if(i==2){
			lati_final[i]=0x2B;
			i++;
			lati_final[i]=*(lati_value+lati_count);
		}
		else if(i==5){
			lati_final[i]=0x2E;
		}
		else {
			lati_final[i]=*(lati_value+lati_count);
		}
		lati_count++;
			
	}
	lati_final[i]=lati_dir;
	i++;
	lati_final[i]=0;
	
	LCD_clear();
	string_location(1,0);
	
	send_a_string(lati_final);
	/*
	
	//send_a_command(0x80);
	send_a_character(*(lati_value+0));
	send_a_character(*(lati_value+1));
	send_a_character(0xDF);
	send_a_character(*(lati_value+2));
	send_a_character(*(lati_value+3));
	send_a_character(0x27);
	send_a_character(*(lati_value+5));
	send_a_character(*(lati_value+6));
	send_a_character(*(lati_value+7));
	send_a_character(*(lati_value+8));
	send_a_character(0x22);
	send_a_character(0x20);
	send_a_character(lati_dir);
	
	*/
}
void display_longitude()
{
	int i=0;
	int longi_count=0;
	for(i=0;i<11;i++){
		
		if(i==3){
			longi_final[i]=0x2B;
			i++;
			longi_final[i]=*(longi_value+longi_count);
		}
		else if(i==6){
			longi_final[i]=0x2E;
		}
		else {
			longi_final[i]=*(longi_value+longi_count);
		}
		longi_count++;
			
	}
	longi_final[i]=longi_dir;
	i++;
	longi_final[i]=0;
	
	string_location(1,1);
	
	send_a_string(longi_final);
	

	/*
	string_location(1,1);
	
	send_a_command(0xC0);
	send_a_character(*(longi_value+0));
	send_a_character(*(longi_value+1));
	send_a_character(*(longi_value+2));
	send_a_character(0xDF);
	send_a_character(*(longi_value+3));
	send_a_character(*(longi_value+4));
	send_a_character(0x27);
	send_a_character(*(longi_value+6));
	send_a_character(*(longi_value+7));
	send_a_character(*(longi_value+8));
	send_a_character(*(longi_value+9));
	send_a_character(0x22);
	send_a_character(0x20);
	send_a_character(longi_dir);

	*/
}




/*
void display_date()
{
	LCD_sendcmd(0xD0);
	LCD_write_string("Date:");
	LCD_senddata(date[0]);
	LCD_senddata(date[1]);
	LCD_senddata('/');
	LCD_senddata(date[2]);
	LCD_senddata(date[3]);
	LCD_senddata('/');
	LCD_senddata(date[4]);
	LCD_senddata(date[5]);
}
void display_speed()
{
	int i;
	LCD_sendcmd(0x80);
	LCD_write_string("Speed:");
	LCD_sendcmd(0xC0);
	for(i=0;i<5;i++)
	{
		LCD_senddata(speed[i]);
	}
	LCD_write_string(" knots");
}
void display_corse()
{
	int i;
	LCD_sendcmd(0x90);
	LCD_write_string("Direction:");
	LCD_sendcmd(0xD0);
	for(i=0;i<5;i++)
	{
		LCD_senddata(corse_angle[i]);
	}
	LCD_senddata(0xDF);
}

*/

#endif
