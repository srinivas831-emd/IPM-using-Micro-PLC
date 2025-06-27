/*
 * com_terminal.c
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#include "main.h"
#include "data.h"
#include "ctype.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "app_terminal.h"
#include "com_terminal.h"



extern UART_HandleTypeDef huart3;

void uart3_tx(uint8_t* buf)
{
	 HAL_UART_Transmit(&huart3, buf, strlen((char*)buf), HAL_MAX_DELAY);
	 HAL_Delay(100);
}


void USER_USART3_UART_Init()
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}
