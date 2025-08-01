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

char smsBody[64];
extern char CH1[10];
extern char CH2[10];
extern char CH3[10];
extern char CH4[10];
extern char MDS[10];
char WiFi_cloud_data[300];
uint8_t GSM_cloud_data[256];

//CircularQueue wifirxQueue;
char *wifitoken;

uint8_t rxwifiBuffer;
CircularQueue rxwifiQueue;

char wifiarr1[10]   ;
char wifiarr2[10]   ;
int wifiswitch_val=0;
char wificonfig[4]={4,4,4,4};
uint8_t wifiadcChnlChecker[4];
extern struct data d;
extern uint8_t ch;
#define ALL_DATA_BUFFER_SIZE 1024

char allDataBuffer[ALL_DATA_BUFFER_SIZE];
static uint16_t allDataIndex = 0;
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
		uart3_tx((uint8_t *)"SUCSESS\r\n");
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

    if (!GSM_SendCommandandSMS("AT\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT failed\r");
        return false;
    }

    if (!GSM_SendCommandandSMS("AT+CPIN?\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CPIN? failed\r");
        return false;
    }

    if (!GSM_SendCommandandSMS("AT+CSQ\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CSQ failed\r");
        return false;
    }

    if (!GSM_SendCommandandSMS("AT+CGREG?\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CGREG? failed\r");
        return false;
    }

    if (!GSM_SendCommandandSMS("AT+CGDCONT=1,\"IP\",\"airtelgprs.com\"\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+CGDCONT failed\r");
        return false;
    }

    if (!GSM_SendCommandandSMS("AT+CGATT=1\r\n", 2000))
    {
        uart3_tx((uint8_t *)"\rAT+CGATT=1 failed\r");
        return false;
    }

    if (!GSM_SendCommandandSMS("AT+CGACT=1,1\r\n", 2000))
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

    if (!GSM_SendCommandandSMS("AT+HTTPINIT\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+HTTPINIT failed\r");
        return false;
    }

    // Set URL
    snprintf(url_buffer, sizeof(url_buffer),"AT+HTTPPARA=\"URL\",\"%s?%s\"\r\n",d.link,dataToSend);

    uart3_tx((uint8_t*)url_buffer);
    if (!GSM_SendCommandandSMS(url_buffer, 5000))
    {
        return false;
    }

    GSM_SendCommandandSMS("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n", 1000);

    if (!GSM_SendCommandandSMS("AT+HTTPACTION=0\r\n", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+HTTPACTION=0 failed\r");
        return false;
    }

    HAL_Delay(10000);

    if (!GSM_SendCommandandSMS("", 1000))
    {
        uart3_tx((uint8_t *)"\rAT+HTTPACTION=0 failed\r");
        return false;
    }

    GSM_SendCommandandSMS("AT+HTTPTERM\r\n", 1000);  // Always close

	GSM_SendCommandandSMS("AT+CMGD=1,4\r", 2000);

	CircularQueue_Init(&rxwifiQueue);
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

                if(strstr((char *)gsmBuffer, "*") ||
                        strstr((char *)gsmBuffer, "#"))
                {
                	extractData((char *)gsmBuffer);
                }


                if (strstr((char *)gsmBuffer, "ERROR") ||
                    strstr((char *)gsmBuffer, "FAIL")||
                    strstr((char *)gsmBuffer, "+CSQ: 99,99")||
                    strstr((char *)gsmBuffer, "+CGREG: 0,2")||
                    strstr((char *)gsmBuffer, "+CGREG: 0,3")||
                    strstr((char *)gsmBuffer, "+CGREG: 0,0")||
                    strstr((char *)gsmBuffer, "+CGREG: 0,4")||
                    strstr((char *)gsmBuffer, "+CGATT: 0"))
                {
                    uart3_tx((uint8_t*)"\r\ngsmBuffer: ");
                    uart3_tx((uint8_t*)gsmBuffer);
                    return false;
                }
                // Add more success conditions
                else if (strstr((char *)gsmBuffer, "OK\r\n") ||
                    strstr((char *)gsmBuffer, "+CMGS:") ||
                    strstr((char *)gsmBuffer, "\r\n>") ||
                    strstr((char *)gsmBuffer, "+HTTPACTION:")||
                    strstr((char *)gsmBuffer, "+CGATT: 1")||
                    strstr((char *)gsmBuffer, "SUCCESS"))
                {
                    uart3_tx((uint8_t*)"\r\ngsmBuffer: ");
                    uart3_tx((uint8_t*)gsmBuffer);
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
	char discmd[350];
	int noofatm=2;
    for (int attempt = 0; attempt < noofatm; attempt++)
    {
        HAL_UART_Transmit(&huart4, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);
        HAL_UART_Receive_IT(&huart4,&rxwifiBuffer,1);
        bool success = GSM_WaitForResponse(timeout);
        if (success) return true;

        if (attempt >= 0)
        {
        	sprintf(discmd,"Retrying %s...",cmd);
            uart3_tx((uint8_t *)discmd);
	        uart3_tx((uint8_t *)"\r\n");
            HAL_Delay(200);
        }
    }
    return false;
}

bool GSM_Init(void)
{
    if (!GSM_SendCommandandSMS("AT\r\n", 1000))
    {
    	uart3_tx((uint8_t *)"\rAT failed\r");
    	return false;
    }
    if (!GSM_SendCommandandSMS("ATE0\r\n", 1000))
    {
    	uart3_tx((uint8_t *)"\rATE0 failed\r");
    	return false;
    }
    if (!GSM_SendCommandandSMS("AT+CMGF=1\r\n", 1000))
    {
    	uart3_tx((uint8_t *)"AT+CMGF=1\r");
    	return false;
    }
    if(!GSM_SendCommandandSMS("AT+CNMI=2,2,0,0,0\r\n",500))
    {
    	uart3_tx((uint8_t *)"AT+CNMI=2,2,0,0,0\r");
    	return false;
    }
    return true;
}

void GSM_SendSMS(char *a)
{
	char cmd[64];
	sprintf(cmd, "AT+CMGS=\"%s\"\r",d.PhoneNumber);

	// Phase 1: Send recipient number
	GSM_SendCommandandSMS(cmd, 2000);

	uart3_tx((uint8_t *)a);
	// Critical: Set response routing BEFORE transmission

	HAL_UART_Transmit(&huart4, (uint8_t*)a, strlen(a), HAL_MAX_DELAY);
	uint8_t ctrlZ = 0x1A;
	HAL_UART_Transmit(&huart4, &ctrlZ, 1, HAL_MAX_DELAY);
	// Optional memory cleanup (remove if causing issues)
	CircularQueue_Init(&rxwifiQueue);
	GSM_SendCommandandSMS("AT+CMGD=1,4\r", 2000);
}



void DataToWiFi(struct data *d)
{
	snprintf(WiFi_cloud_data,sizeof(WiFi_cloud_data),"DATE,%0d/%02d/%02d;TIME,%02d:%02d:%02d;CH1,%s;CH2,%s;CH3,%s;CH4,%s;MDS,%s;GPIO0,%s;GPIO1,%s;GPIO2,%s;GPIO3,%s;?\r\n",d->dayofmonth,d->month,d->year,d->hour,d->minutes,d->seconds,CH1,CH2,CH3,CH4,MDS,d->Status1,d->Status2,d->Status3,d->Status4);
	if(DatatoESP(WiFi_cloud_data))
	{
			HAL_Delay(1000);
			EEPROM_ReadAllMessagesAndErase();
			uart3_tx((uint8_t *)"SUCCESS\r\n");
	}
	else
	{
			HAL_Delay(1000);
			EEPROM_StoreMessage(WiFi_cloud_data);
			uart3_tx((uint8_t*)"\r\nWIFI ERROR DID NOT SEND DATA TO GSHEET\r\n");
			return;
	}
	memset(WiFi_cloud_data,0,sizeof(WiFi_cloud_data));
}


bool DatatoESP(char * Data)
{
	uart3_tx((uint8_t*)"\r\nData");
	uart3_tx((uint8_t*)Data);
	if(!GSM_SendCommandandSMS(Data,15000))
	{
		HAL_Delay(5000);
		return false;
	}
	HAL_Delay(5000);
	return true;
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
	ProcessIncomingData();
}


void ProcessIncomingData(void)
{
	 uint8_t byte;

	    while (!CircularQueue_IsEmpty(&rxwifiQueue))
	    {
	        CircularQueue_Dequeue(&rxwifiQueue, &byte);

	        // Add byte to buffer (if space)
	        if (allDataIndex < ALL_DATA_BUFFER_SIZE - 1)
	        {
	            allDataBuffer[allDataIndex++] = byte;
	            allDataBuffer[allDataIndex] = '\0'; // Null-terminate
	        }
	        else
	        {
	            // Buffer full — optional: reset or overwrite
	            allDataIndex = 0;
	            memset(allDataBuffer, 0, sizeof(allDataBuffer));
	        }
	    }
	    extractData(allDataBuffer);
	    allDataIndex = 0;
	    memset(allDataBuffer, 0, sizeof(allDataBuffer));
}




void extractData(char *unprocessedSMS)
{
		int i = 0;
	    char processedSMS[200];  // One command at a time

	    while (unprocessedSMS[i] != '#' && unprocessedSMS[i] != '\0')  // Stop at '#' or end of string
	    {
	        // Wait for '*'
	        if (unprocessedSMS[i] == '*')
	        {
	            i++; // Skip '*'
	            int j = 0;

	            // Copy until ';' or end
	            while (unprocessedSMS[i] != ';' && unprocessedSMS[i] != '\0' && j < sizeof(processedSMS) - 1)
	            {
	            	processedSMS[j++] = unprocessedSMS[i++];
	            }

	            processedSMS[j] = '\0';  // Null-terminate

	            if (j > 0)
	            {
	                extract_data(processedSMS);
	                pin_config();
	            }

	            if (unprocessedSMS[i] == ';') i++;  // Skip ';'
	        }
	        else
	        {
	            i++;  // Skip characters until next '*'
	        }
	    }
}



void cloud_set_output(struct data *d)
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
}


