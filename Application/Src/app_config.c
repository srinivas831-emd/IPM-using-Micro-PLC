/*
 * app_config.c
 *
 *  Created on: Sep 13, 2024
 *      Author: KAVYA
 */

#include <stdio.h>
#include<string.h>
#include "com_config.h"
#include "app_config.h"
#include "app_ADC.h"
#include "app_RTC.h"
#include "com_ADC.h"
#include <string.h>
#include "data.h"
#include "main.h"
#include <stdlib.h>
#include "circular_queue.h"
#include "com_GSM.h"

int switch_val=0;
//char tx_data[15];
UART_HandleTypeDef huart1;
char arr1[10];
char arr2[20];
char *token;
int flag;
uint8_t* pin_status;

uint8_t s[4];
extern struct data d;

uint8_t rxBuffer;  // UART receive buffer
CircularQueue rxQueue;// Circular queue for storing received data
static char command[60];  // Temporary storage for a single command

extern uint8_t rxwifiBuffer;
uint8_t ch;
extern CircularQueue rxwifiQueue;
extern CircularQueue GSMQueue;

extern  UART_HandleTypeDef huart4;

void Configurator()
{
	data_receive();
	Process_Commands();
	reset_output(&d);
	set_output(&d);
	read_pinstatus(&d);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		// Enqueue received bytes into the circular queue

		if (!CircularQueue_Enqueue(&rxQueue, rxBuffer))
		{
			// Handle queue overflow if necessary (optional)
		}

		// Restart UART reception for the next chunk of data
		HAL_UART_Receive_IT(&huart1, &rxBuffer, 1);

	}

	if (huart->Instance == UART4)
	{

		if(!CircularQueue_Enqueue(&rxwifiQueue, rxwifiBuffer))
		{

		}
	      HAL_UART_Receive_IT(&huart4, &rxwifiBuffer, 1);

	}
}


void Process_Commands(void)
{
	static uint8_t cmdIndex = 0;  // Persistent index to track the current command
	uint8_t byte;

	while (!CircularQueue_IsEmpty(&rxQueue))
		{
		CircularQueue_Dequeue(&rxQueue, &byte);

		// Add byte to the command
		if (byte != ';')
		{
			if (cmdIndex < sizeof(command) - 1)
			{
				command[cmdIndex++] = byte;
			} else
			{
				// Handle command overflow (optional)
				cmdIndex = 0;  // Reset on overflow
			}
		} else
		{
			// Command completed when '\n' is received
			command[cmdIndex] = '\0';  // Null-terminate the command
			cmdIndex = 0;

			// Process the command
			extract_data(command);
			pin_config();
		}
	}
}


void set_output(struct data *d)
{
	if(d->config[0] == 1)
	{
		d->GPIO[0] = write_gpio(GPIOB,GPIO_PIN_2, PIN_SET);
	}

	if(d->config[1] == 1)
	{
		d->GPIO[1] = write_gpio(GPIOC,GPIO_PIN_1, PIN_SET);
	}

	if(d->config[2] == 1)
	{
		d->GPIO[2] = write_gpio(GPIOB,GPIO_PIN_4, PIN_SET);
	}

	if(d->config[3] == 1)
	{
		d->GPIO[3] = write_gpio(GPIOB,GPIO_PIN_5, PIN_SET);
	}
	//	HAL_UART_Transmit(&huart2, (uint8_t *)d.GPIO, 4,1000);
}

void reset_output(struct data *d1)
{
	if(d1->config[0] == 2)
	{
		d1->GPIO[0] = write_gpio(GPIOB,GPIO_PIN_2, PIN_RESET);
	}

	if(d1->config[1] == 2)
	{
		d1->GPIO[1]=write_gpio(GPIOC,GPIO_PIN_1, PIN_RESET);
	}

	if(d1->config[2] == 2)
	{
		d1->GPIO[2] = write_gpio(GPIOB,GPIO_PIN_4, PIN_RESET);
	}

	if(d1->config[3] == 2)
	{
		d1->GPIO[3] = write_gpio(GPIOB,GPIO_PIN_5, PIN_RESET);
	}
}

void read_pinstatus(struct data *d2)
{
	if(d2->config[0]==3)
	{
		d2->GPIO[0]=read_gpio( GPIOB,GPIO_PIN_2);
	}
	if(d2->config[1]==3)
	{
		d2->GPIO[1]=read_gpio(GPIOC,GPIO_PIN_1);
	}
	if(d2->config[2]==3)
	{
		d2->GPIO[2]=read_gpio( GPIOB,GPIO_PIN_4);
	}
	if(d2->config[3]==3)
	{
		d2->GPIO[3]=read_gpio( GPIOB,GPIO_PIN_5);
	}
	//	 return d->GPIO;
}


void extract_data(char* command)
{
	token = strtok(command, ",");


	if (token != NULL)
	{
		strncpy(arr1, token, sizeof(arr1) - 1);

		arr1[sizeof(arr1) - 1] = '\0';// Ensure null-termination
		strcpy(d.arr1,arr1);


		token = strtok(NULL, ",");
	}

	// Get the second token and store it in arr2
	if (token != NULL)
	{
		strncpy(arr2, token, sizeof(arr2) - 1);

		arr2[sizeof(arr2) - 1] = '\0'; // Ensure null-termination
		strcpy(d.arr2,arr2);

		//token = strtok(NULL, ",");
	}


}


void pin_config()
{

	if(strcmp(arr1,"B2")==0)
	{
		switch_val=1;
	}
	else if(strcmp(arr1,"GPIO0")==0)
	{
		switch_val=1;
	}

	else if(strcmp(arr1,"C1")==0)
	{
		switch_val=2;
	}

	else if(strcmp(arr1,"GPIO1")==0)
	{
		switch_val=2;
	}

	else if(strcmp(arr1,"B4")==0)
	{
		switch_val=3;
	}
	else if(strcmp(arr1,"GPIO2")==0)
	{
		switch_val=3;
	}

	else if(strcmp(arr1,"B5")==0)
	{
		switch_val=4;
	}
	else if(strcmp(arr1,"GPIO3")==0)
	{
		switch_val=4;
	}


	else if(strcmp(arr1,"ADC1")==0)
	{
		switch_val=5;
	}
	else if(strcmp(arr1,"ENABLE")==0)
	{
		switch_val=5;
	}

	else if(strcmp(arr1,"DISABLE")==0)
	{
		switch_val=6;
	}

	else if(strcmp(arr1,"TIME")==0)
	{
		switch_val=7;
	}

	else if(strcmp(arr1,"DATE")==0)
	{
		switch_val=8;
	}

	else if(strcmp(arr1,"SCANTIME")==0)
	{
		switch_val=9;
	}
	else if(strcmp(arr1,"USERNAME")==0)
	{
		switch_val=10;
	}
	else if(strcmp(arr1,"PASSWORD")==0)
	{
		switch_val=11;
	}
	else if(strcmp(arr1,"EEPROM")==0)
	{
		switch_val=12;
	}
	else if (strcmp(arr1,"PHNO")==0)
	{
		switch_val=13;
	}
	else if (strcmp(arr1,"MODE")==0)
	{
		switch_val=14;
	}
	else if (strcmp(arr1,"THRESHOLD")==0)
	{
		switch_val=15;
	}
	else if (strcmp(arr1,"LINK")==0)
	{
		switch_val=16;
	}



	switch(switch_val)
	{

//PB2
	case 1:
		if(strcmp(arr2,"HIGH")==0)
		{
				user_GPIO_Init(GPIOB,GPIO_PIN_2,OUTPUT);
				d.config[0] = 1;
		}

		else if(strcmp(arr2,"LOW")==0)
		{
				user_GPIO_Init(GPIOB,GPIO_PIN_2,OUTPUT);
				d.config[0] = 2;
		}

		else if(strcmp(arr2,"INPUT")==0)
		{
				user_GPIO_Init(GPIOB,GPIO_PIN_2,INPUT);
				d.config[0] = 3;
		}
		break;

//PC1
	case 2:
		if(strcmp(arr2,"HIGH")==0)
		{
			user_GPIO_Init(GPIOC,GPIO_PIN_1,OUTPUT);
			d.config[1]=1;
		}

		else if(strcmp(arr2,"LOW")==0)
		{
			user_GPIO_Init(GPIOC,GPIO_PIN_1,OUTPUT);
			d.config[1]=2;
		}

		else if(strcmp(arr2,"INPUT")==0)
		{
			user_GPIO_Init(GPIOC,GPIO_PIN_1,INPUT);
			d.config[1]=3;
		}
		break;


//PB4
	case 3:
		if(strcmp(arr2,"HIGH")==0)
		{
			user_GPIO_Init(GPIOB,GPIO_PIN_4,OUTPUT);
			d.config[2]=1;
		}
		else if(strcmp(arr2,"LOW")==0)
		{
			user_GPIO_Init(GPIOB,GPIO_PIN_4,OUTPUT);
			d.config[2]=2;
		}

		else if(strcmp(arr2,"INPUT")==0)
		{
			user_GPIO_Init(GPIOB,GPIO_PIN_4,INPUT);
			d.config[2]=3;
		}

		break;

//PB5
	case 4:
		if(strcmp(arr2,"HIGH")==0)
		{
			user_GPIO_Init(GPIOB,GPIO_PIN_5,OUTPUT);
			d.config[3]=1;
		}
		else if(strcmp(arr2,"LOW")==0)
		{
			user_GPIO_Init(GPIOB,GPIO_PIN_5,OUTPUT);
			d.config[3]=2;
		}
		else if(strcmp(arr2,"INPUT")==0)
		{
			user_GPIO_Init(GPIOB,GPIO_PIN_5,INPUT);
			d.config[3]=3;
		}
		break;


	case 5:
		if(strcmp(arr2,"CH1")==0)
		{
			ADC_select_CH1();
			d.adcChnlChecker[0] = 1;
		}

		else if(strcmp(arr2,"CH2")==0)
		{
			ADC_select_CH2();
			d.adcChnlChecker[1] = 1;
		}

		else if(strcmp(arr2,"CH3")==0)
		{
			ADC_select_CH3();
			d.adcChnlChecker[2] = 1;
		}

		else if(strcmp(arr2,"CH4")==0)
		{
			ADC_select_CH4();
			d.adcChnlChecker[3] = 1;
		}

		break;

	case 6:
		if(strcmp(arr2,"CH1")==0)
		{
			ADC_select_CH1();
			d.adcChnlChecker[0] = 0;
		}

		else if(strcmp(arr2,"CH2")==0)
		{
			ADC_select_CH2();
			d.adcChnlChecker[1] = 0;
		}

		else if(strcmp(arr2,"CH3")==0)
		{
			ADC_select_CH3();
			d.adcChnlChecker[2] = 0;
		}

		else if(strcmp(arr2,"CH4")==0)
		{
			ADC_select_CH4();
			d.adcChnlChecker[3] = 0;
		}

		break;

	case 7:

		token = strtok(arr2, ":");

		// Get the first token and convert to integer
		if (token != NULL)
		{
			d.hour = atoi(token);
			token = strtok(NULL, ":");

		}

		// Get the second token and convert to integer
		if (token != NULL)
		{
			d.minutes = atoi(token);
			token = strtok(NULL, ":");

		}

		// Get the third token and convert to integer
		if (token != NULL)
		{
			d.seconds = atoi(token);
		}
		//				 Set_Time(time);

		set_time(d.seconds,d.minutes,d.hour);

		break;

	case 8:

		token = strtok(arr2, ":");

		// Get the first token and convert to integer
		if (token != NULL)
		{
			d.dayofmonth = atoi(token);
			token = strtok(NULL, ":");
		}

		// Get the second token and convert to integer
		if (token != NULL)
		{
			d.month = atoi(token);
			token = strtok(NULL, ":");
		}

		// Get the third token and convert to integer
		if (token != NULL)
		{
			d.year = atoi(token);
		}
		//				 Set_Time(time);

		set_date(1,d.dayofmonth,d.month,d.year);
		break;


	case 9:
		d.scan_time = atoi(arr2);
		break;
	case 10:
		strcpy(d.username,arr2);
		break;

	case 11:
		strcpy(d.password,arr2);
		break;
	case 12:
		strcpy(d.eeprom_data,arr2);
		break;
	case 13:
		strncpy(d.PhoneNumber,arr2,strlen(arr2));
		break;
	case 14:
		if(strcmp(arr2,"SMS")==0)
		{
			d.Mode=0;
		}

		else if(strcmp(arr2,"GSHEET")==0)
		{
			d.Mode=1;
		}
		break;
	case 15:
		d.threshold = atof(arr2);
		break;
	case 16:
		strncpy(d.link,arr2,strlen(arr2));
		break;
	default:
		break;
	}
}

