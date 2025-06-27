/*
 * com_RTC.c
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#include <main.h>
#include "app_rtc.h"
#include "data.h"

extern I2C_HandleTypeDef hi2c1;




void USER_I2C1_Init()
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}


// Generic I2C write function
void I2C_Write(I2C_HandleTypeDef *hi2c, uint16_t device_address, uint16_t mem_address, uint8_t mem_address_size, uint8_t *data, size_t data_size, uint32_t timeout)
{
    HAL_I2C_Mem_Write(hi2c, device_address, mem_address, mem_address_size, data, data_size, timeout);
}

//read function for RTC
void I2C_read(uint8_t *data)
   {
	HAL_I2C_Mem_Read(&hi2c1, DS3107_ADDRESS, 0x00, 1, data, 7, HAL_MAX_DELAY);
   }

