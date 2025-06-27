/*
 * app_EEPROM.c
 *
 *  Created on: Dec 4, 2024
 *      Author: KAVYA
 */
#include "main.h"
#include "string.h"
#include "EEPROM.h"
#include "stdio.h"
#include "app_EEPROM.h"
#include "com_EEPROM.h"
#include "data.h"



uint8_t DataRead[14400];
char eeprom_data[200];
extern char CH1[10];
extern char CH2[10];
extern char CH3[10];
extern char CH4[10];
extern char MDS[10];

static uint8_t Old_value = 0; // Retain value between function calls
static uint8_t count = 0; // Flag to indicate if data has been transmitted this minute
static uint8_t counth=0;
static uint8_t Old_valueh = 0;
extern uint8_t page_number;

void EEPROM_transmit(struct data *d)
{
	uint8_t New_value=d->minutes;
	uint8_t New_valueh=d->hour;

	sprintf((char *)eeprom_data,"Time:%02d:%02d:%02d,Date:%02d:%02d:%02d,CH1:%.2f,CH2:%.2f,CH3:%.2f,CH4:%.2f,MDS:%.2f,Status1:%s,Status2:%s,Status3:%s,Status4:%s",
			d->hour, d->minutes, d->seconds,d->dayofmonth, d->month, d->year,
			d->adc1_value, d->adc2_value, d->adc3_value, d->adc4_value,
			d->MDS_value, d->Status1, d->Status2, d->Status3, d->Status4);
//	for (int i=0; i<300; i++)
//	{
//		EEPROM_PageErase(i);
//	}
//	EEPROM_Read(0,0,DataRead,650);
//		Data_To_EEPROM(eeprom_data);
//		EEPROM_Read(0,0,DataRead,650);


	if (New_value != Old_value) {
		count++;
	}


	if (count >= d->scan_time) {
		// Transmit data
		if(d->scan_time>0)
		{
			Data_To_EEPROM(eeprom_data);
			count = 0; // Reset the counter after transmission
		}
	}

	Old_value = New_value;

//	EEPROM_Read(0,0,DataRead,14400);

	if (New_valueh != Old_valueh) {
		counth++;
	}

	if (counth >= 1) {
		if(1>0)
		{
			for (int i=0; i<300; i++)
			{
				EEPROM_PageErase(i);
			}

			counth=0;
			page_number = 0;
		}
	}
	Old_valueh = New_valueh;
//	EEPROM_Read(0,0,DataRead,14400);

	if(strcmp(d->eeprom_data,"READDATA")==0)
	{
	EEPROM_Read(0,0,DataRead,14400);

	}
	memset(d->eeprom_data,0,sizeof (d->eeprom_data));
}

