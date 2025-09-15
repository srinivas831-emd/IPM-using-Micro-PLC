
/**
  ******************************************************************************

  EEPROM.c Using the HAL I2C Functions
  Author:   ControllersTech
  Updated:  Feb 16, 2021

  ******************************************************************************
  Copyright (C) 2017 ControllersTech.com

  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
  of the GNU General Public License version 3 as published by the Free Software Foundation.
  This software library is shared with public for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
  or indirectly by this software, read more about this on the GNU General Public License.

  ******************************************************************************
*/
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "EEPROM.h"
#include "math.h"
#include "string.h"
#include "com_GSM.h"
#include "com_terminal.h"

// Define the I2C
extern I2C_HandleTypeDef hi2c3;;
#define EEPROM_I2C &hi2c3

// EEPROM ADDRESS (8bits)
#define EEPROM_ADDR 0xA0

// Define the Page Size and number of pages
#define PAGE_SIZE 64     // in Bytes
#define PAGE_NUM  128    // number of pages
#define EEPROM_FLAG_PAGE      0
#define EEPROM_FLAG_OFFSET    0

#define EEPROM_COUNT_OFFSET   1
#define EEPROM_MSG_OFFSET     2
#define MAX_MESSAGE_LEN     200
#define EEPROM_LOG_START_PAGE 1





/*****************************************************************************************************************************************/
uint8_t bytes_temp[4];

// function to determine the remaining bytes
uint16_t bytestowrite (uint16_t size, uint16_t offset)
{
	if ((size+offset)<PAGE_SIZE) return size;
	else return PAGE_SIZE-offset;
}

void EEPROM_Write (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
	int paddrposition = log(PAGE_SIZE)/log(2);
	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
	uint16_t numofpages = (endPage-startPage) + 1;
	uint16_t pos=0;

	for (int i=0; i<numofpages; i++)
	{
		uint8_t attempt=0;
		uint16_t MemAddress = startPage<<paddrposition | offset;
		uint16_t bytesremaining = bytestowrite(size, offset);

		while(attempt<2)
		{
			if (HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, &data[pos], bytesremaining, 1000) == HAL_OK)
			{
				uart3_tx((uint8_t*) "EEPROM Write OK\r\n");
				break;
			}
			else
			{
				attempt++;
				uart3_tx((uint8_t*) "EEPROM Write FAILED\r\n");
				HAL_Delay(10);
			}
		}
		startPage += 1;
		offset=0;
		size = size-bytesremaining;
		pos += bytesremaining;
		HAL_Delay (10);
	}

}

void EEPROM_Read (uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
	int paddrposition = log(PAGE_SIZE)/log(2);
	uint16_t startPage = page;
	uint16_t endPage = page + ((size+offset)/PAGE_SIZE);
	uint16_t numofpages = (endPage-startPage) + 1;
	uint16_t pos=0;


	for (int i=0; i<numofpages; i++)
	{
		uint8_t attempt=0;
		uint16_t MemAddress = startPage<<paddrposition | offset;
		uint16_t bytesremaining = bytestowrite(size, offset);

		while(attempt<2)
		{
			if (HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, 	MemAddress, 2, &data[pos], bytesremaining, 1000) == HAL_OK)
			{
				uart3_tx((uint8_t*) "EEPROM Read OK\r\n");
				break;
			}
			else
			{
				attempt++;
				uart3_tx((uint8_t*) "EEPROM Read FAILED\r\n");
				HAL_Delay(10);
			}

		}
		startPage += 1;
		offset=0;
		size = size-bytesremaining;
		pos += bytesremaining;
		HAL_Delay(10);
	}
}



void EEPROM_PageErase (uint16_t page)
{
	int paddrposition = log(PAGE_SIZE)/log(2);
	uint16_t MemAddress = page<<paddrposition;
	uint8_t data[PAGE_SIZE];
	memset(data,0x00,PAGE_SIZE);
	HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, MemAddress, 2, data, PAGE_SIZE, 1000);
	HAL_Delay (10);
}


void EEPROM_StoreMessage(char *message)
{
		uint8_t flag = 0x01;
	    uint8_t count = 0;

	    // Read current count
	    EEPROM_Read(EEPROM_FLAG_PAGE, EEPROM_COUNT_OFFSET, &count, 1);

	    if ((count + 1) * MAX_MESSAGE_LEN > (PAGE_NUM * PAGE_SIZE)) {
	        uart3_tx((uint8_t*)"EEPROM full. Cannot store more messages.\r\n");
	        return;
	    }

	    count++;  // Increment count
	    EEPROM_Write(EEPROM_FLAG_PAGE, EEPROM_FLAG_OFFSET, &flag, 1);
	    EEPROM_Write(EEPROM_FLAG_PAGE, EEPROM_COUNT_OFFSET, &count, 1);

	    // Calculate target address
	    uint16_t msg_address = (count - 1) * MAX_MESSAGE_LEN;
	    uint16_t page = EEPROM_LOG_START_PAGE + (msg_address / PAGE_SIZE);
	    uint16_t offset = msg_address % PAGE_SIZE;

	    // Prepare padded buffer
	    uint8_t buffer[MAX_MESSAGE_LEN] = {0};
	    strncpy((char *)buffer, message, MAX_MESSAGE_LEN - 1);

	    // Write the 200-byte block
	    EEPROM_Write(page, offset, buffer, MAX_MESSAGE_LEN);
}


void EEPROM_ReadAllMessagesAndErase(void)
{
    uint8_t flag = 0;
    uint8_t count = 0;
    bool allUploaded = true;

    EEPROM_Read(EEPROM_FLAG_PAGE, EEPROM_FLAG_OFFSET, &flag, 1);
    EEPROM_Read(EEPROM_FLAG_PAGE, EEPROM_COUNT_OFFSET, &count, 1);


    if (flag != 0x01 || count == 0)
    {
        uart3_tx((uint8_t *)"No valid data in EEPROM.\r\n");
        return;
    }

    char buffer[MAX_MESSAGE_LEN + 1]={0};

    for (uint8_t i = 0; i <count; i++)
    {
        uint16_t msg_address = i * MAX_MESSAGE_LEN;
        uint16_t page = EEPROM_LOG_START_PAGE + (msg_address / PAGE_SIZE);
        uint16_t offset = msg_address % PAGE_SIZE;

        memset(buffer, 0, sizeof(buffer));
        EEPROM_Read(page, offset, (uint8_t *)buffer, MAX_MESSAGE_LEN);
        if(buffer[0]!='\0')
        {
			if(d.WiFi==1&&d.GSM==0)
			{
				if(!DatatoESP(buffer))
				{
					allUploaded=false;
				}
			}
			else if(d.WiFi==0&&d.GSM==1)
			{
				if(d.Mode==0)
				{
					GSM_SendSMS(buffer);
				}
				if(d.Mode==1)
				{
					if(!GSMInitGsheet()||!HTTPInitAndSend(buffer))
					{
						allUploaded=false;
					}

				}
			}
        }
    }

    // Erase all used pages including metadata
    if(allUploaded==true)
    {
		for (uint8_t i = 0; i <= PAGE_NUM; i++)
			{
				EEPROM_PageErase(i);
			}
		uart3_tx((uint8_t *)"EEPROM erased after reading all messages.\r\n");
	}
    else
	{
		uart3_tx((uint8_t *)"Some messages failed to upload. EEPROM not erased.\r\n");
	}
}
