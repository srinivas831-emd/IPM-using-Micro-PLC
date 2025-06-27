/*
 * com_EEPROM.c
 *
 *  Created on: Dec 4, 2024
 *      Author: KAVYA
 */
#include "main.h"
#include "com_EEPROM.h"
#include "app_EEPROM.h"
#include "string.h"
#include "EEPROM.h"

extern I2C_HandleTypeDef hi2c3;

uint8_t page_number;  // Start with page 0
uint8_t offset = 0;       // Start with offset 0

void Data_To_EEPROM(char *eeprom_data)
{


	EEPROM_Write(page_number,offset,(uint8_t*)eeprom_data,strlen(eeprom_data));
	page_number+=3;
}

void USER_I2C3_INIT(void)
{

	  /* USER CODE BEGIN I2C3_Init 0 */
	//
	  /* USER CODE END I2C3_Init 0 */

	  /* USER CODE BEGIN I2C3_Init 1 */
	//
	  /* USER CODE END I2C3_Init 1 */
	  hi2c3.Instance = I2C3;
	  hi2c3.Init.ClockSpeed = 400000;
	  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
	  hi2c3.Init.OwnAddress1 = 0;
	  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c3.Init.OwnAddress2 = 0;
	  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /* USER CODE BEGIN I2C3_Init 2 */
	//
	  /* USER CODE END I2C3_Init 2 */

}
