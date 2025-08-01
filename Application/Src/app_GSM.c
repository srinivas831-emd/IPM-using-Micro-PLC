/*
 * app_GSM.c
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#include "main.h"
#include "app_GSM.h"
#include "com_GSM.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"
#include "data.h"
#include "app_terminal.h"
#include "com_terminal.h"

char buffer_Time[15];
char buffer_Date[15];
char CH1[10];
char CH2[10];
char CH3[10];
char CH4[10];
char MDS[10];


//extern struct data d;


UART_HandleTypeDef huart2;

void DataToCloud(struct data *d)
{

	// LATEST CODE
		static uint8_t last_triggered_minute = 255;
		static uint8_t last_triggered_hour = 255;
		static uint8_t base_minute = 0;
	    //tatic uint8_t base_second = 0;
	    static uint8_t base_hour = 0;
	    static uint8_t initialized = 0;
	    static uint8_t previous_scan_time = 0;
	    char uart_buf[32];

	    uint8_t curr_scantime=d->scan_time;
	    uint8_t curr_min = d->minutes;
	    //uint8_t curr_sec = d->seconds;
	    uint8_t curr_hour = d->hour;


	    // Format data for transmission
			sprintf(buffer_Time, "%02d:%02d:%02d", d->hour, d->minutes, d->seconds);
			sprintf(buffer_Date, "%02d/%02d/%02d", d->dayofmonth, d->month, d->year);
	    	sprintf((char*)CH1,"%f",d->adc1_value);
	    	sprintf((char*)CH2,"%f",d->adc2_value);
	    	sprintf((char*)CH3,"%f",d->adc3_value);
	    	sprintf((char*)CH4,"%f",d->adc4_value);
	    	sprintf((char*)MDS,"%f",d->MDS_value);

//////////////////////////////////////STATUS 1//////////////////////////////////////////////////////////////////////////////////////////STATUS 1//////////////////////////////////////////////////////
	    		if(d->config[0]== 1)
	    		{
	    			strcpy(d->Status1, "HIGH");
	    		}
	    		else if (d->config[0]==2)
	    		{
	    			strcpy(d->Status1, "LOW");
	    		}
	    		else if((d->GPIO[0]==1)&&(d->config[0]==3))
	    		{
	    			strcpy(d->Status1, "HIGH");
	    		}
	    		else if((d->GPIO[0]==0)&&(d->config[0]==3))
				{
					strcpy(d->Status1, "LOW");
				}
	    		else
	    		{
	    			strcpy(d->Status1, "UNKNOWN");
	    		}

//////////////////////////////////////STATUS 2//////////////////////////////////////////////////////////////////////////////////////////STATUS 2////////////////////////////////////////////////////
	    		if(d->config[1]== 1)
				{
					strcpy(d->Status2, "HIGH");
				}
				else if (d->config[1]==2)
				{
					strcpy(d->Status2, "LOW");
				}
				else if((d->GPIO[1]==1)&&(d->config[1]==3))
				{
					strcpy(d->Status2, "HIGH");
				}
				else if((d->GPIO[1]==0)&&(d->config[1]==3))
				{
					strcpy(d->Status2, "LOW");
				}
				else
				{
					strcpy(d->Status2, "UNKNOWN");
				}

//////////////////////////////////////STATUS 3//////////////////////////////////////////////////////////////////////////////////////////STATUS 3////////////////////////////////////////////////////
	    		if(d->config[2]== 1)
				{
					strcpy(d->Status3, "HIGH");
				}
				else if (d->config[2]==2)
				{
					strcpy(d->Status3, "LOW");
				}
				else if((d->GPIO[2]==1)&&(d->config[2]==3))
				{
					strcpy(d->Status3, "HIGH");
				}
				else if((d->GPIO[2]==0)&&(d->config[2]==3))
				{
					strcpy(d->Status3, "LOW");
				}
				else
				{
					strcpy(d->Status3, "UNKNOWN");
				}

//////////////////////////////////////STATUS 4//////////////////////////////////////////////////////////////////////////////////////////STATUS 4////////////////////////////////////////////////////
	    		if(d->config[3]== 1)
				{
					strcpy(d->Status4, "HIGH");
				}
				else if (d->config[3]==2)
				{
					strcpy(d->Status4, "LOW");
				}
				else if((d->GPIO[3]==1)&&(d->config[3]==3))
				{
					strcpy(d->Status4, "HIGH");
				}
				else if((d->GPIO[3]==0)&&(d->config[3]==3))
				{
					strcpy(d->Status4, "LOW");
				}
				else
				{
					strcpy(d->Status4, "UNKNOWN");
				}
//////////////////////////////////////STATUS 4///////////////////////////////////////////////////////////////////////////////////////STATUS 4/////////////////////////////////////////////////////////

	    if (!initialized)
	    {
	        base_minute = curr_min;
	        base_hour = curr_hour;
	        initialized = 1;
	        return;
	    }

	    if (curr_scantime != previous_scan_time)
	    	{
	    		base_minute = curr_min;
	    		base_hour = curr_hour;
	    		previous_scan_time = curr_scantime;
	    		return;
	    	}

	    // Compute expected minute considering wrap-around
	    uint8_t expected_minute = (base_minute + curr_scantime) % 60;

	    if (curr_min == expected_minute&&((curr_min != last_triggered_minute)||(curr_hour!=last_triggered_hour)))// && curr_sec == base_second)
	    {
	        uart3_tx((uint8_t *)"\r\ncurr_scantime: ");
	        sprintf(uart_buf, "%u",curr_scantime);
	        uart3_tx((uint8_t *)uart_buf);

	        uart3_tx((uint8_t *)"\r\nbase_minute: ");
	        sprintf(uart_buf, "%u", base_minute);
	        uart3_tx((uint8_t *)uart_buf);

	        uart3_tx((uint8_t *)"\r\ncurr_min: ");
	        sprintf(uart_buf, "%u", curr_min);
	        uart3_tx((uint8_t *)uart_buf);

	        uart3_tx((uint8_t *)"\r\nexpected_minute: ");
	        sprintf(uart_buf, "%u", expected_minute);
	        uart3_tx((uint8_t *)uart_buf);

	        uart3_tx((uint8_t *)"\r\nlast_triggered_minute: ");
			sprintf(uart_buf, "%u", last_triggered_minute);
			uart3_tx((uint8_t *)uart_buf);
	        uart3_tx((uint8_t *)"\r\n");


	        DataToPass();
	        //dataToUart();
	        last_triggered_minute = curr_min;
	        last_triggered_hour = curr_hour;
			base_minute = curr_min;   // Update reference for next transmission
			base_hour = curr_hour;

	    }
}

void DataToPass()
{
	if ((d.WiFi==1)&&(d.GSM==0))
	{
		DataToWiFi(&d);
	}
	else if((d.WiFi==0)&&(d.GSM==1))
	{
		DataToGSM(&d);
	}
}


void dataToUart()
{
	terminal(&d);
}
