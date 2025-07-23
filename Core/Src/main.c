/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "com_config.h"
#include "app_config.h"
#include "app_EEPROM.h"
#include "com_EEPROM.h"
#include "app_RTC.h"
#include "com_RTC.h"
#include "com_ADC.h"
#include "app_ADC.h"
#include "app_GSM.h"
#include "com_GSM.h"
#include "ctype.h"
#include "string.h"
#include "data.h"
#include "IPM.h"
#include "com_terminal.h"
#include "app_terminal.h"
#include "circular_queue.h"
#include "com_config.h"
#define LINK "https://script.google.com/macros/s/AKfycbxB0z9f02ZI33mE-tcD5cWHuz50fhhXr3fjb0sssR3Cv01QXV-QjESBZpegjNcUCHuLmA/exec"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

//UART_HandleTypeDef huart4;
//UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
extern CircularQueue rxQueue;
extern CircularQueue rxwifiQueue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  /* USER CODE BEGIN 2 */
    d.WiFi=0;
    d.GSM=1;
    d.scan_time=2;
    d.Mode=1;
    strcpy(d.PhoneNumber,"+918317370381");
    strcpy(d.link,LINK);
	user_USART1_UART_Init();
	USER_I2C3_INIT();
	USER_I2C1_Init();
	USER_ADC1_Init();
	USER_USART3_UART_Init();
	UART4_Init();
	CircularQueue_Init(&rxQueue);
	CircularQueue_Init(&rxwifiQueue);
	USERRCV_GPIO_Init();
	user_GPIO_Init(GPIOB, GPIO_PIN_12,OUTPUT);
	GSM_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  application();
  }
  /* USER CODE END 3 */
}


//  * @brief  This function is executed in case of error occurrence.
//  * @retval None
//  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
