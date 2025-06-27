/*
 * app_RTC.c
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#include "main.h"
#include "data.h"
#include <stdint.h>
#include "stdio.h"
#include <string.h>
#include "com_RTC.h"
#include "app_RTC.h"
#include "com_terminal.h"


extern I2C_HandleTypeDef hi2c1;
HAL_StatusTypeDef status;
extern UART_HandleTypeDef huart3;


#define RTC_add (0x68 << 1)
#define RTC_wr_add 0xD0
#define RTC_rd_add 0xD1
char timeFg=0;
char dateFg=0;
//decimal to bcd convertion

uint8_t dec_bcd(int val)
{
	return (uint8_t)((val/10<<4)|(val%10));
}

//bcd to decimal convertion

int bcd_dec(uint8_t val)
{
	return ((val >> 4) * 10) + (val & 0x0F);
}





void set_time(uint8_t sec,uint8_t min,uint8_t hr)
{
	if(sec<=60&&min<=60&&hr<=24)
   {
	uint8_t Set_time[3];
	Set_time[0]=dec_bcd(sec);
	Set_time[1]=dec_bcd(min);
	Set_time[2]=dec_bcd(hr);
	HAL_I2C_Mem_Write(&hi2c1, RTC_add, 0x00, I2C_MEMADD_SIZE_8BIT, Set_time, 3, HAL_MAX_DELAY);
   }
//	if(HAL_I2C_Mem_Write(&hi2c1, RTC_add, 0x00, I2C_MEMADD_SIZE_8BIT, Set_time, 3, HAL_MAX_DELAY)== HAL_OK)
//	{
//		uart3_tx("I2C WRITE TIME SUCESSES\r\n");
//	}
//	else
//	{
//		uart3_tx("I2C WRITE TIME not SUCESSES\r\n");
//	}
//  }
//	else
//	{
//		uart3_tx("I2C WRITE TIME VALUES ERROR \r\n");
//	}
}

void set_date(uint8_t day,uint8_t dat,uint8_t mon,uint8_t yr)
{
	if(dat<=31&&mon<=12&&yr<=99)
	{
	uint8_t Set_date[4];
	Set_date[0]=dec_bcd(day);
	Set_date[1]=dec_bcd(dat);
	Set_date[2]=dec_bcd(mon);
	Set_date[3]=dec_bcd(yr);
	HAL_I2C_Mem_Write(&hi2c1, RTC_add, 0x03, 1, Set_date, 4, HAL_MAX_DELAY);
	}
//	if(HAL_I2C_Mem_Write(&hi2c1, RTC_add, 0x03, I2C_MEMADD_SIZE_8BIT, Set_date, 4, HAL_MAX_DELAY)==HAL_OK)
//	{
//		uart3_tx("I2C WRITE DATE SUCESSES\r\n");
//	}
//	else
//	{
//		uart3_tx("I2C WRITE DATE not SUCESSES\r\n");
//	}
//    }
//	else
//	{
//		uart3_tx("I2C WRITE DATE VALUES ERROR \r\n");
//	}
}


uint8_t Get_time[7];
void get_time(struct data *d)
{
//	char WiFi_cloud_data[256];
	uint8_t Get_time[7];
	//HAL_I2C_Mem_Read(&hi2c1, RTC_add, 0x00, I2C_MEMADD_SIZE_8BIT, Get_time, 7, HAL_MAX_DELAY);
	if(HAL_I2C_Mem_Read(&hi2c1, RTC_add, 0x00, I2C_MEMADD_SIZE_8BIT, Get_time, 7, HAL_MAX_DELAY)==HAL_OK)
	{
	d->seconds=bcd_dec(Get_time[0]);
	d->minutes=bcd_dec(Get_time[1]);
	d->hour=bcd_dec(Get_time[2]);
	d->dayofweek=bcd_dec(Get_time[3]);
	d->dayofmonth=bcd_dec(Get_time[4]);
	d->month=bcd_dec(Get_time[5]);
	d->year=bcd_dec(Get_time[6]);
//	sprintf((char*)WiFi_cloud_data,"TIME,%02d:%02d:%02d;DATE,%02d/%02d/%02d;",d->hour,d->minutes,d->seconds,d->dayofmonth,d->month,d->year);
//	HAL_UART_Transmit(&huart3, WiFi_cloud_data, strlen((char*)WiFi_cloud_data), HAL_MAX_DELAY);
//	}
//	else
//	{
//		uart3_tx("I2C READ ERROR\r\n");
	}
}












