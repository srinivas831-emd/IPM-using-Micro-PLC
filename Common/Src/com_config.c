/*
 * com_config.c
 *
 *  Created on: Sep 13, 2024
 *      Author: KAVYA
 */


#include "com_config.h"
#include "main.h"
#include <string.h>
#include "circular_queue.h"



extern uint8_t rxBuffer;  // UART receive buffer
//extern  char  tx_data[15];
extern UART_HandleTypeDef huart1;
GPIO_InitTypeDef GPIO_InitStruct = {0};
uint8_t GPIO[4];

void SystemClock_Config(void)
{
	 RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	  /** Configure the main internal regulator output voltage
	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 16;
	  RCC_OscInitStruct.PLL.PLLN = 336;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	  RCC_OscInitStruct.PLL.PLLQ = 2;
	  RCC_OscInitStruct.PLL.PLLR = 2;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	}

 void user_USART1_UART_Init(void)
 {

	   /* USER CODE BEGIN USART1_Init 0 */

	   /* USER CODE END USART1_Init 0 */

	   /* USER CODE BEGIN USART1_Init 1 */

	   /* USER CODE END USART1_Init 1 */
	   huart1.Instance = USART1;
	   huart1.Init.BaudRate = 9600;
	   huart1.Init.WordLength = UART_WORDLENGTH_8B;
	   huart1.Init.StopBits = UART_STOPBITS_1;
	   huart1.Init.Parity = UART_PARITY_NONE;
	   huart1.Init.Mode = UART_MODE_TX_RX;
	   huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	   huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	   if (HAL_UART_Init(&huart1) != HAL_OK)
	   {
	     Error_Handler();
	   }
	   /* USER CODE BEGIN USART1_Init 2 */

	   /* USER CODE END USART1_Init 2 */

	 }

 void user_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,mode PIN_mode)
 {
   //GPIO_InitTypeDef GPIO_InitStruct = {0};
 /* USER CODE BEGIN MX_GPIO_Init_1 */
 /* USER CODE END MX_GPIO_Init_1 */

   /* GPIO Ports Clock Enable */
	 __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

   /*Configure GPIO pin Output Level */
     HAL_GPIO_WritePin(GPIOx, GPIO_Pin,PIN_mode);

     if(PIN_mode == OUTPUT)
     {
    	gpio_output(GPIOx,GPIO_Pin,PIN_mode);
     }
     else if(PIN_mode == INPUT)
     {
         gpio_input(GPIOx,GPIO_Pin,PIN_mode);
     }

     /*Configure GPIO pin : VCP_RX_Pin */
  // GPIO_InitStruct.Pin = VCP_RX_Pin;
     GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
   //HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

 /* USER CODE BEGIN MX_GPIO_Init_2 */
 /* USER CODE END MX_GPIO_Init_2 */
 }

 void gpio_output(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,mode PIN_mode)
 {
     GPIO_InitTypeDef GPIO_InitStruct = {0};

     /* Configure GPIO pins : PA3 PA4 PA5 PA6 PA7 */
     GPIO_InitStruct.Pin = GPIO_Pin;
     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
     HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
 }

 void gpio_input(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, mode PIN_mode)
 {
     GPIO_InitTypeDef GPIO_InitStruct = {0};

     /* Configure GPIO pins : PA3 PA4 PA5 PA6 */
     GPIO_InitStruct.Pin = GPIO_Pin;
     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
     GPIO_InitStruct.Pull = GPIO_NOPULL;
     HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
 }




void data_receive()
 {
	 if (HAL_UART_Receive_IT(&huart1, &rxBuffer,1) == HAL_ERROR)
	 {
			 Error_Handler();
	 }
 }


//void status_transmit()
// {
//	if ((HAL_UART_Transmit(&huart1, (uint8_t *)gpio, 4,1000)) == HAL_ERROR)
//	 {
//		 Error_Handler();
//	 }
// }

 myPinState read_gpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
 {
	return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

 }

 myPinState write_gpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, myPinState PinState)
 {
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin,PinState);
    return PinState;
 }


