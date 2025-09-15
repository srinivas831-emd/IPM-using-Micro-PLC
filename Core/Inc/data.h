/*
 * data.h
 *
 *  Created on: Dec 4, 2024
 *      Author: KAVYA
 */

#ifndef INC_DATA_H_
#define INC_DATA_H_


struct data
{
	float threshold;
	float adc1_value;
	float adc2_value;
	float adc3_value;
	float adc4_value;
	float MDS_value;
	float ch1Max;
	float ch1Min;
	float ch2Max;
	float ch2Min;
	float ch3Max;
	float ch3Min;
	float ch4Max;
	float ch4Min;
	uint8_t GPIO[4];
	uint8_t scan_time;
	uint8_t adcChnlChecker[4];
	char config[4];
	char Status1[10];
	char Status2[10];
	char Status3[10];
	char Status4[10];
	char username[20];
	char password[20];
	char eeprom_data[10];
	char MDS_PIN;
	char online;
	char WiFi;
	char GSM;
	char PhoneNumber[20];
	char ExpectingGsmResponse;
	char Mode;
	char seconds;
	char minutes;
	char hour;
	char dayofweek;
	char dayofmonth;
	char month;
	char year;
	char arr1[10];
	char arr2[20];
	char wifiarr1[10];
	char wifiarr2[20];
	char link[200];
};

extern struct data d;


#endif /* INC_DATA_H_ */
