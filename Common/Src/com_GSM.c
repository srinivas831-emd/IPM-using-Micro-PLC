/*
 * com_GSM.c
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */


#include "main.h"
#include "data.h"
#include "app_GSM.h"
#include "com_GSM.h"
#include "app_ADC.h"
#include "app_RTC.h"
#include "com_ADC.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ctype.h"
#include "com_config.h"
#include "circular_queue.h"
#include "app_config.h"
#include"com_terminal.h"
#include"EEPROM.h"
#define GSM_RX_BUFFER_SIZE 512

extern char CH1[10];
extern char CH2[10];
extern char CH3[10];
extern char CH4[10];
extern char MDS[10];
uint8_t WiFi_cloud_data[256];
uint8_t GSM_cloud_data[256];

//CircularQueue wifirxQueue;
char *wifitoken;

static char WifiData[1000];
uint8_t rxwifiBuffer;
CircularQueue rxwifiQueue;
CircularQueue GSMQueue;
char wifiarr1[10]   ;
char wifiarr2[10]   ;
int wifiswitch_val=0;
char wificonfig[4]={4,4,4,4};
uint8_t wifiadcChnlChecker[4];
extern struct data d;
extern uint8_t ch;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

UART_HandleTypeDef huart4;


void UART4_Init(void)
{


	/* USER CODE BEGIN UART4_Init 0 */

	/* USER CODE END UART4_Init 0 */

	/* USER CODE BEGIN UART4_Init 1 */

	/* USER CODE END UART4_Init 1 */
	huart4.Instance = UART4;
	if((d.WiFi==1)&&(d.GSM==0))
	{
		huart4.Init.BaudRate = 9600;
	}
	if((d.WiFi==0)&&(d.GSM==1))
	{
		huart4.Init.BaudRate = 115200;
	}
    else
    {
        huart4.Init.BaudRate = 9600;  // Default Fallback
    }
	huart4.Init.WordLength = UART_WORDLENGTH_8B;
	huart4.Init.StopBits = UART_STOPBITS_1;
	huart4.Init.Parity = UART_PARITY_NONE;
	huart4.Init.Mode = UART_MODE_TX_RX;
	huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart4.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart4) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN UART4_Init 2 */

	/* USER CODE END UART4_Init 2 */

}


void USERRCV_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);

	/*Configure GPIO pin : PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PC6 PC7 PC8 */
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

void DataToGSM(struct data *d)
{
	char Gsheetdata[200];
	char fullMessage[200];
	snprintf(fullMessage, sizeof(fullMessage),
    "TIME=%02d:%02d:%02d\r\n"
    "DATE=%02d/%02d/%02d\r\n"
    "CH1=%s\r\nCH2=%s\r\nCH3=%s\r\nCH4=%s\r\n"
    "MDS=%s\r\n"
    "GPIO0=%s\r\nGPIO1=%s\r\nGPIO2=%s\r\nGPIO3=%s\r\n",
    d->hour, d->minutes, d->seconds,
    d->dayofmonth, d->month, d->year,
    CH1, CH2, CH3, CH4, MDS,
    d->Status1, d->Status2, d->Status3, d->Status4);

	snprintf(Gsheetdata, sizeof(Gsheetdata),
	"TIME=%02d:%02d:%02d&DATE=%02d/%02d/%02d&CH1=%s&CH2=%s&CH3=%s&CH4=%s&MDS=%s&"
	"GPIO0=%s&GPIO1=%s&GPIO2=%s&GPIO3=%s",
	d->hour, d->minutes, d->seconds,
	d->dayofmonth, d->month, d->year,
	CH1, CH2, CH3, CH4, MDS,
	d->Status1, d->Status2, d->Status3, d->Status4);

	if (d->Mode==1)
	{
		DataToGsheet(Gsheetdata);
	}
	if (d->Mode==0)
	{
		DataToPhone(fullMessage);
	}

}


void DataToPhone(char * Message)
{
	  if(GSM_Init())
	  {
		  EEPROM_ReadAllMessagesAndErase();
		  GSM_SendSMS(Message);
		  uart3_tx((uint8_t*)"\r\nSMS SENT SUCCESFULLY\r\n");
	  }

	  else
	  {

		  EEPROM_StoreMessage(Message);
		  uart3_tx((uint8_t*)"\r\nGSM ERROR DID NOT SEND SMS\r\n");
		  return;
	  }
}

void DataToGsheet(char *Data)
{
	if(GSMInitGsheet()&&HTTPInitAndSend(Data))
	{
		EEPROM_ReadAllMessagesAndErase();
		uart3_tx((uint8_t *)"Success");
	}
	else
	{
		EEPROM_StoreMessage(Data);
		uart3_tx((uint8_t*)"\r\nGSM ERROR DID NOT SEND DATA TO GSHEET\r\n");
		return;
	}
}

bool GSMInitGsheet()
{
    CircularQueue_Init(&rxwifiQueue);  // Clear residual data
    HAL_Delay(500);

    if (!GSM_SendCommandandSMS("AT\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rATE0 failed\r");
        return false;
    }

    HAL_Delay(500);
    if (!GSM_SendCommandandSMS("AT+CPIN?\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CPIN? failed\r");
        return false;
    }

    HAL_Delay(500);
    if (!GSM_SendCommandandSMS("AT+CSQ\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CSQ failed\r");
        return false;
    }

    HAL_Delay(500);
    if (!GSM_SendCommandandSMS("AT+CGREG?\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CGREG? failed\r");
        return false;
    }

    HAL_Delay(500);
    if (!GSM_SendCommandandSMS("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CGDCONT failed\r");
        return false;
    }

    HAL_Delay(500);
    if (!GSM_SendCommandandSMS("AT+CGATT=1\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CGATT=1 failed\r");
        return false;
    }

    HAL_Delay(500);
    if (!GSM_SendCommandandSMS("AT+CGACT=1,1\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CGACT=1,1 failed\r");
        return false;
    }

    HAL_Delay(5000);  // Wait for PDP to become active
    return true;
}

bool HTTPInitAndSend(char *dataToSend)
{
    char url_buffer[350] = {0};

    GSM_SendCommandandSMS("AT+HTTPTERM\r\n", 1000);  // Ignore fail
    HAL_Delay(200);

    if (!GSM_SendCommandandSMS("AT+HTTPINIT\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+HTTPINIT failed\r");
        return false;
    }

    HAL_Delay(500);

    // Set URL
    snprintf(url_buffer, sizeof(url_buffer),
    "AT+HTTPPARA=\"URL\",\"https://script.google.com/macros/s/AKfycbzX2CX2NH_qEUK3xxXWb4E22FBTYOP-szZ-fblhGSe9peR-hir6QgeF7Z889WJzdeggZg/exec?%s\"\r\n",dataToSend);
    uart3_tx((uint8_t*)url_buffer);
    if (!GSM_SendCommandandSMS(url_buffer, 5000))
    {
        uart3_tx((uint8_t *)"\rAT+HTTPPARA URL failed\r");
        GSM_SendCommandandSMS("AT+HTTPTERM\r\n", 1000);  // Always close
        HAL_Delay(500);
        return false;
    }

    HAL_Delay(500);

    GSM_SendCommandandSMS("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n", 1000);

    if (!GSM_SendCommandandSMS("AT+HTTPACTION=0\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+HTTPACTION=0 failed\r");
        return false;
    }

    HAL_Delay(10000);
    GSM_SendCommandandSMS("AT+HTTPREAD\r\n", 2000);  // Read response


    GSM_SendCommandandSMS("AT+HTTPTERM\r\n", 1000);  // Always close
    HAL_Delay(10000);

    return true;
}

bool GSM_WaitForResponse(uint32_t timeout)
{
    uint32_t startTime = HAL_GetTick();
    uint8_t gsmBuffer[GSM_RX_BUFFER_SIZE] = {0};
    uint16_t idx = 0;
    uint8_t ch;

    while ((HAL_GetTick() - startTime) < timeout)
    {
        while (!CircularQueue_IsEmpty(&rxwifiQueue))
        {
            if (CircularQueue_Dequeue(&rxwifiQueue, &ch))
            {
                gsmBuffer[idx++] = ch;
                if (idx >= GSM_RX_BUFFER_SIZE-1) idx = 0; // prevent overflow
                gsmBuffer[idx] = '\0';

                if (strstr((char *)gsmBuffer, "ERROR") ||
                    strstr((char *)gsmBuffer, "FAIL"))
                {
                    return false;
                }
                // Add more success conditions
                else if (strstr((char *)gsmBuffer, "OK\r\n") ||
                    strstr((char *)gsmBuffer, "+CMGS:") ||
                    strstr((char *)gsmBuffer, "\r\n>") ||
                    strstr((char *)gsmBuffer, "+HTTPACTION:") ||
                    strstr((char *)gsmBuffer, "+HTTPREAD:"))
                {
                    return true;
                }
            }
        }
        HAL_Delay(10); // Small delay to prevent busy waiting
    }
    return false;  // Timeout
}

bool GSM_SendCommandandSMS(const char *cmd, uint32_t timeout)
{
    for (int attempt = 0; attempt < 2; attempt++)
    {
        HAL_UART_Transmit(&huart4, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);
        HAL_UART_Receive_IT(&huart4,&rxwifiBuffer,1);
        bool success = GSM_WaitForResponse(timeout);
        if (success) return true;

        if (attempt == 0)
        {
            uart3_tx((uint8_t *)"Retrying GSM command...\r\n");
            HAL_Delay(200);
        }
    }
    return false;
}

bool GSM_Init(void)
{
    CircularQueue_Init(&rxwifiQueue);  // Clear residual data


    if (!GSM_SendCommandandSMS("ATE0\r\n", 1000))
    {
    	uart3_tx((uint8_t *)"\rATE0 failed\r");
    	return false;
    }
    HAL_Delay(1000);
    if (!GSM_SendCommandandSMS("AT\r\n", 1000))
    {
    	uart3_tx((uint8_t *)"\rAT failed\r");
    	return false;
    }
    HAL_Delay(1000);
    if (!GSM_SendCommandandSMS("AT+CMGF=1\r\n", 1000))
    {
    	uart3_tx((uint8_t *)"AT+CMGF=1\r");
    	return false;
    }
    return true;
}


void GSM_SendSMS(char *a)
{
	char cmd[64];
	sprintf(cmd, "AT+CMGS=\"+918317370381\"\r");

	// Phase 1: Send recipient number
	GSM_SendCommandandSMS(cmd, 2000);

	uart3_tx((uint8_t *)a);
	// Critical: Set response routing BEFORE transmission
	CircularQueue_Init(&rxwifiQueue);

	HAL_UART_Transmit(&huart4, (uint8_t*)a, strlen(a), HAL_MAX_DELAY);
	uint8_t ctrlZ = 0x1A;
	HAL_UART_Transmit(&huart4, &ctrlZ, 1, HAL_MAX_DELAY);
	CircularQueue_Init(&rxwifiQueue);  // Clear residual data
	// Optional memory cleanup (remove if causing issues)
	GSM_SendCommandandSMS("AT+CMGD=1,4\r", 2000);
}



void DataToWiFi(struct data *d)
{
	sprintf((char*)WiFi_cloud_data,"TIME,%02d:%02d:%02d;DATE,%02d/%02d/%02d;CH1,%s;CH2,%s;CH3,%s;CH4,%s;MDS,%s;GPIO0,%s;GPIO1,%s;GPIO2,%s;GPIO3,%s\r\n",d->hour,d->minutes,d->seconds,d->dayofmonth,d->month,d->year,CH1,CH2,CH3,CH4,MDS,d->Status1,d->Status2,d->Status3,d->Status4);
	HAL_UART_Transmit(&huart4, WiFi_cloud_data, strlen((char*)WiFi_cloud_data), HAL_MAX_DELAY);
	HAL_Delay(5);
}

void dataFromCloud()
{
	cloud_data_receive();
	cloud_Process_Commands();
	cloud_reset_output(&d);
	cloud_set_output(&d);
	cloud_read_pinstatus(&d);
}

void cloud_data_receive()
{
	if (HAL_UART_Receive_IT(&huart4, &rxwifiBuffer, 1) == HAL_ERROR)
	{
		Error_Handler();
	}
}


void cloud_Process_Commands(void)
{
	static uint8_t cmdIndex = 0;  // Persistent index to track the current command
	uint8_t byte;

	while (!CircularQueue_IsEmpty(&rxwifiQueue))
	{
		CircularQueue_Dequeue(&rxwifiQueue, &byte);

//		HAL_UART_Transmit(&huart1, &byte, 1, HAL_MAX_DELAY); // For debugging
		if(cmdIndex == 0)
		{
			memset(WifiData, 0, sizeof(WifiData));
		}

		// Add byte to the command
		if (byte != ';')
		{
			if (cmdIndex < sizeof(WifiData) - 1)
			{

				WifiData[cmdIndex++] = byte;
			}

			else

			{
				// Handle command overflow (optional)
				cmdIndex = 0;  // Reset on overflow
			}
		}

		else

		{
			// Command completed when '\n' is received
			WifiData[cmdIndex] = '\0';  // Null-terminate the command
			cmdIndex = 0;
			extract_cloud_data(WifiData);
			Process_GPIO_Status();
		}

	}
}





void extract_cloud_data(char* WifiData)
{
	wifitoken = strtok(WifiData, ",");


	if (wifitoken != NULL)
	{
		strncpy(wifiarr1, wifitoken, sizeof(wifiarr1) - 1);

		wifiarr1[sizeof(wifiarr1) - 1] = '\0';// Ensure null-termination
		strcpy(d.wifiarr1,wifiarr1);


		wifitoken = strtok(NULL, ",");
	}

	// Get the second token and store it in arr2
	if (wifitoken != NULL)
	{
		strncpy(wifiarr2, wifitoken, sizeof(wifiarr2) - 1);

		wifiarr2[sizeof(wifiarr2) - 1] = '\0'; // Ensure null-termination
		strcpy(d.wifiarr2,wifiarr2);

		//token = strtok(NULL, ",");
	}
}

void cloud_set_output(struct data *d)
{
	if(d->config[0] == 0)
	{
		d->GPIO[0] = write_gpio(GPIOB,GPIO_PIN_2, PIN_SET);
	}

	if(d->config[1] == 0)
	{
		d->GPIO[1] = write_gpio(GPIOC,GPIO_PIN_1, PIN_SET);
	}

	if(d->config[2] == 0)
	{
		d->GPIO[2] = write_gpio(GPIOB,GPIO_PIN_4, PIN_SET);
	}

	if(d->config[3] == 0)
	{
		d->GPIO[3] = write_gpio(GPIOB,GPIO_PIN_5, PIN_SET);
	}
	//	HAL_UART_Transmit(&huart2, (uint8_t *)d.GPIO, 4,1000);
}

void cloud_reset_output(struct data *d1)
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

void cloud_read_pinstatus(struct data *d2)
{
	if(d2->config[0]==1)
	{
		d2->GPIO[0]=read_gpio( GPIOB,GPIO_PIN_2);
	}
	if(d2->config[1]==1)
	{
		d2->GPIO[1]=read_gpio(GPIOC,GPIO_PIN_1);
	}
	if(d2->config[2]==1)
	{
		d2->GPIO[2]=read_gpio( GPIOB,GPIO_PIN_4);
	}
	if(d2->config[3]==1)
	{
		d2->GPIO[3]=read_gpio( GPIOB,GPIO_PIN_5);
	}
}


void Process_GPIO_Status()
{
		if(strcmp(wifiarr1,"B2")||strcmp(wifiarr1,"GPIO0")==0)
		{
			wifiswitch_val=1;
		}

		else if(strcmp(wifiarr1,"C1")||strcmp(wifiarr1,"GPIO1")==0)
		{
			wifiswitch_val=2;
		}

		else if(strcmp(wifiarr1,"B4")||strcmp(wifiarr1,"GPIO2")==0)
		{
			wifiswitch_val=3;
		}

		else if(strcmp(wifiarr1,"B5")||strcmp(wifiarr1,"GPIO3")==0)
		{
			wifiswitch_val=4;
		}

		else if(strcmp(wifiarr1,"ADC1")==0)
		{
			wifiswitch_val=5;
		}
		else if(strcmp(wifiarr1,"DISABLE")==0)
		{
			wifiswitch_val=6;
		}

		else if(strcmp(wifiarr1,"TIME")==0)
		{
			wifiswitch_val=7;
		}

		else if(strcmp(wifiarr1,"DATE")==0)
		{
			wifiswitch_val=8;
		}

		else if(strcmp(wifiarr1,"SCANTIME")==0)
		{
			wifiswitch_val=9;
		}

	switch(wifiswitch_val)
		{

	//PB2
		case 1:
			if(strcmp(wifiarr2,"HIGH")==0)
			{
				user_GPIO_Init(GPIOB,GPIO_PIN_2,OUTPUT);
				d.config[0] = 0;
			}

			else if(strcmp(wifiarr2,"LOW")==0)
			{
					user_GPIO_Init(GPIOB,GPIO_PIN_2,OUTPUT);
					d.config[0] = 2;
			}

			else if(strcmp(wifiarr2,"INPUT")==0)
			{
					user_GPIO_Init(GPIOB,GPIO_PIN_2,INPUT);
					d.config[0] = 1;
			}
			break;

	//PC1
		case 2:
			if(strcmp(wifiarr2,"HIGH")==0)
			{
				user_GPIO_Init(GPIOC,GPIO_PIN_1,OUTPUT);
				d.config[1]=0;
			}

			else if(strcmp(wifiarr2,"LOW")==0)
			{
				user_GPIO_Init(GPIOC,GPIO_PIN_1,OUTPUT);
				d.config[1]=2;
			}

			else if(strcmp(wifiarr2,"INPUT")==0)
			{
				user_GPIO_Init(GPIOC,GPIO_PIN_1,INPUT);
				d.config[1]=1;
			}
			break;


	//PB4
		case 3:
			if(strcmp(wifiarr2,"HIGH")==0)
			{
				user_GPIO_Init(GPIOB,GPIO_PIN_4,OUTPUT);
				d.config[2]=0;
			}
			else if(strcmp(wifiarr2,"LOW")==0)
			{
				user_GPIO_Init(GPIOB,GPIO_PIN_4,OUTPUT);
				d.config[2]=2;
			}

			else if(strcmp(wifiarr2,"INPUT")==0)
			{
				user_GPIO_Init(GPIOB,GPIO_PIN_4,INPUT);
				d.config[2]=1;
			}

			break;

	//PB5
		case 4:
			if(strcmp(wifiarr2,"HIGH")==0)
			{
				user_GPIO_Init(GPIOB,GPIO_PIN_5,OUTPUT);
				d.config[3]=0;
			}
			else if(strcmp(wifiarr2,"LOW")==0)
			{
				user_GPIO_Init(GPIOB,GPIO_PIN_5,OUTPUT);
				d.config[3]=2;
			}
			else if(strcmp(wifiarr2,"INPUT")==0)
			{
				user_GPIO_Init(GPIOB,GPIO_PIN_5,INPUT);
				d.config[3]=1;
			}
			break;


		case 5:
			if(strcmp(wifiarr2,"CH1")==0)
			{
				ADC_select_CH1();
				d.adcChnlChecker[0] = 1;
			}

			else if(strcmp(wifiarr2,"CH2")==0)
			{
				ADC_select_CH2();
				d.adcChnlChecker[1] = 1;
			}

			else if(strcmp(wifiarr2,"CH3")==0)
			{
				ADC_select_CH3();
				d.adcChnlChecker[2] = 1;
			}

			else if(strcmp(wifiarr2,"CH4")==0)
			{
				ADC_select_CH4();
				d.adcChnlChecker[3] = 1;
			}

			break;

		case 6:
			if(strcmp(wifiarr2,"CH1")==0)
			{
				ADC_select_CH1();
				d.adcChnlChecker[0] = 0;
			}

			else if(strcmp(wifiarr2,"CH2")==0)
			{
				ADC_select_CH2();
				d.adcChnlChecker[1] = 0;
			}

			else if(strcmp(wifiarr2,"CH3")==0)
			{
				ADC_select_CH3();
				d.adcChnlChecker[2] = 0;
			}

			else if(strcmp(wifiarr2,"CH4")==0)
			{
				ADC_select_CH4();
				d.adcChnlChecker[3] = 0;
			}

			break;

		case 7:

			wifitoken = strtok(wifiarr2, ":");

			// Get the first token and convert to integer
			if (wifitoken != NULL)
			{
				d.hour = atoi(wifitoken);
				wifitoken = strtok(NULL, ":");

			}

			// Get the second token and convert to integer
			if (wifitoken != NULL)
			{
				d.minutes = atoi(wifitoken);
				wifitoken = strtok(NULL, ":");

			}

			// Get the third token and convert to integer
			if (wifitoken != NULL)
			{
				d.seconds = atoi(wifitoken);
			}
			//				 Set_Time(time);

			set_time(d.seconds,d.minutes,d.hour);

			break;

		case 8:

			wifitoken = strtok(wifiarr2, ":");

			// Get the first token and convert to integer
			if (wifitoken != NULL)
			{
				d.dayofmonth = atoi(wifitoken);
				wifitoken = strtok(NULL, ":");
			}

			// Get the second token and convert to integer
			if (wifitoken != NULL)
			{
				d.month = atoi(wifitoken);
				wifitoken = strtok(NULL, ":");
			}

			// Get the third token and convert to integer
			if (wifitoken != NULL)
			{
				d.year = atoi(wifitoken);
			}
			//				 Set_Time(time);

			set_date(1,d.dayofmonth,d.month,d.year);
			break;


		case 9:
			d.scan_time = atoi(wifiarr2);
			break;
		}
}

