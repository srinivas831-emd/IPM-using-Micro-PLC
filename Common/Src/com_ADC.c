/*
 * com_ADC.c
 *
 *  Created on: Sep 14, 2024
 *      Author: KAVYA
 */



#include "main.h"
#include<stdio.h>
#include<stdlib.h>
#include"com_adc.h"
#include"app_adc.h"
#include "app_config.h"
#include <string.h>
uint32_t ss[4];

extern char arr1[10];
ADC_ChannelConfTypeDef sConfig = {0};
extern ADC_HandleTypeDef hadc1;
//void USER_SystemClock_Config(void)
//{
//	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//
//	/** Configure the main internal regulator output voltage
//	 */
//	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Configure LSE Drive Capability
//	 */
//	HAL_PWR_EnableBkUpAccess();
//	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
//
//	/** Initializes the RCC Oscillators according to the specified parameters
//	 * in the RCC_OscInitTypeDef structure.
//	 */
//	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
//	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
//	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//	RCC_OscInitStruct.MSICalibrationValue = 0;
//	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
//	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
//	RCC_OscInitStruct.PLL.PLLM = 1;
//	RCC_OscInitStruct.PLL.PLLN = 16;
//	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Initializes the CPU, AHB and APB buses clocks
//	 */
//	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
//	{
//		Error_Handler();
//	}
//
//	/** Enable MSI Auto calibration
//	 */
//	HAL_RCCEx_EnableMSIPLLMode();
//}

void USER_ADC1_Init(void)
{

	  /* USER CODE BEGIN ADC1_Init 0 */
	//
	  /* USER CODE END ADC1_Init 0 */



	  /* USER CODE BEGIN ADC1_Init 1 */
	//
	  /* USER CODE END ADC1_Init 1 */

	  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	  */
	  hadc1.Instance = ADC1;
	  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	  hadc1.Init.ScanConvMode = ENABLE;
	  hadc1.Init.ContinuousConvMode = ENABLE;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.NbrOfConversion = 4;
	  hadc1.Init.DMAContinuousRequests = DISABLE;
	  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	  if (HAL_ADC_Init(&hadc1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
//	  sConfig.Channel = ADC_CHANNEL_5;
//	  sConfig.Rank = 1;
//	  sConfig.SamplingTime = ADC_SAMPLETIME_112CYCLES;
//	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
//	  sConfig.Channel = ADC_CHANNEL_6;
//	  sConfig.Rank = 2;
//	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
//	  sConfig.Channel = ADC_CHANNEL_9;
//	  sConfig.Rank = 3;
//	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }

	  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	  */
//	  sConfig.Channel = ADC_CHANNEL_10;
//	  sConfig.Rank = 4;
//	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
	  /* USER CODE BEGIN ADC1_Init 2 */
	//
	  /* USER CODE END ADC1_Init 2 */

	}


void USER_GPIO_Init(void)
{
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}
//ADC_ChannelConfTypeDef sConfig = {0};

void ADC_select_CH1(void)
{
	if(strcmp(arr1,"DISABLE")==0)
	{
		sConfig.Channel = ADC_CHANNEL_5;
		sConfig.Rank = 0;
		sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}

	else
	{
	sConfig.Channel = ADC_CHANNEL_5;
		  sConfig.Rank = 1;
		  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		  {
		    Error_Handler();
		  }
	}
}

void ADC_select_CH2(void)
{
	if(strcmp(arr1,"DISABLE")==0)
	{
		sConfig.Channel = ADC_CHANNEL_6;
		sConfig.Rank = 0;
		sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}

	else
	{
	sConfig.Channel = ADC_CHANNEL_6;
		  sConfig.Rank = 1;
		  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		  {
		    Error_Handler();
		  }
	}
}


void ADC_select_CH3(void)
{
	if(strcmp(arr1,"DISABLE")==0)
	{
		sConfig.Channel = ADC_CHANNEL_9;
		sConfig.Rank = 0;
		sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}

	else
	{
	sConfig.Channel = ADC_CHANNEL_9;
		  sConfig.Rank = 1;
		  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		  {
		    Error_Handler();
		  }
	}
}


void ADC_select_CH4(void)
{
	if(strcmp(arr1,"DISABLE")==0)
	{
		sConfig.Channel = ADC_CHANNEL_10;
		sConfig.Rank = 0;
		sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
	}

	else
	{
	sConfig.Channel = ADC_CHANNEL_10;
		  sConfig.Rank = 1;
		  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
		  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		  {
		    Error_Handler();
		  }
	}
}


//void ADC_channel_set( ADC_Channel CH_no)
//{
//	if(CH_no == CHANNEL_5)
//	{
//	  ss[0] = CH_no;
//	}
//	if(CH_no == CHANNEL_6)
//	{
//	  ss[1] = CH_no;
//	}
//
//
//	sConfig.Channel = CH_no;
//	sConfig.Rank = ADC_REGULAR_RANK_1;
//	sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
//	sConfig.SingleDiff = ADC_SINGLE_ENDED;
//	sConfig.OffsetNumber = ADC_OFFSET_NONE;
//	sConfig.Offset = 0;
//	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//	{
//		Error_Handler();
//	}
//}



void adc_start(void)
{
	HAL_ADC_Start(&hadc1);
}

void adc_conversion(void)
{
	HAL_ADC_PollForConversion(&hadc1,1000);
}

uint16_t adc_value(void)
{
  return HAL_ADC_GetValue(&hadc1);
}

void adc_stop(void)
{
	HAL_ADC_Stop(&hadc1);
}

