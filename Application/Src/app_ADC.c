/*
 * app_ADC.c
 *
 *  Created on: Sep 14, 2024
 *      Author: KAVYA
 */

#include "main.h"
#include<stdio.h>
#include<stdlib.h>
#include"app_adc.h"
#include"com_adc.h"
#include "data.h"
#include "com_config.h"


uint16_t raw_value;


float voltage_cal(void)
{
	 raw_value = adc_value();
	return (raw_value * (3.3 / 4095));
}

void ADC_handler(struct data *d)
{
	if(d->adcChnlChecker[0] == 1)
	{

		ADC_select_CH1();
		adc_start();
		adc_conversion();
		d->adc1_value = voltage_cal();
		adc_stop();
	}
	else if (d->adcChnlChecker[0] == 0)
	{
		d->adc1_value=0.0000;
	}

	if(d->adcChnlChecker[1] == 1)
	{
		ADC_select_CH2();
		adc_start();
		adc_conversion();
		d->adc2_value = voltage_cal();
		adc_stop();
	}
	else if (d->adcChnlChecker[1] == 0)
	{
		d->adc2_value=0.0000;
	}

	if(d->adcChnlChecker[2] == 1)
	{
		ADC_select_CH3();
		adc_start();
		adc_conversion();
		d->adc3_value = voltage_cal();
		adc_stop();
	}
	else if (d->adcChnlChecker[2] == 0)
	{
		d->adc3_value=0.0000;
	}

	if(d->adcChnlChecker[3] == 1)
	{
		ADC_select_CH4();
		adc_start();
		adc_conversion();
		d->adc4_value = (100*(voltage_cal()));
		adc_stop();
	}
	else if (d->adcChnlChecker[3] == 0)
	{
		d->adc4_value=0.0000;
	}


			if(d->adc1_value==0||d->adc2_value==0||d->adc3_value==0)
			{
				d->MDS_value = 0.0000;
			}
			else
			{
			d->MDS_value=(d->adc1_value*d->adc2_value)/d->adc3_value;
			}

			if(d->threshold<=0)
			{
				d->MDS_PIN = write_gpio(GPIOB,GPIO_PIN_12, PIN_RESET);
				HAL_Delay(1000);
			}
			else
			{
				if(d->adc4_value>=d->threshold)
				{
					d->MDS_PIN = write_gpio(GPIOB,GPIO_PIN_12, PIN_SET);
					HAL_Delay(1000);
				}
				else
				{
					d->MDS_PIN = write_gpio(GPIOB,GPIO_PIN_12, PIN_RESET);
					HAL_Delay(1000);
				}
			}
}
