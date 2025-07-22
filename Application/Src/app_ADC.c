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



//float MDS_value;


//struct ADC_VAL
//{
//	float adc1_value;
//	float adc2_value;
//};
//

uint16_t raw_value;

//float adc1_value,adc2_value,adc3_value,adc4_value;

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
		ADC_select_CH1();
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
		ADC_select_CH2();
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
		ADC_select_CH3();
	}

	if(d->adcChnlChecker[3] == 1)
	{
		ADC_select_CH4();
		adc_start();
		adc_conversion();
		d->adc4_value = voltage_cal();
		adc_stop();
	}
	else if (d->adcChnlChecker[3] == 0)
	{
		ADC_select_CH4();
	}
			if(d->adc1_value==0||d->adc2_value==0||d->adc3_value==0)
			{
				d->MDS_value = 0.0000;
			}
			else
			{
			d->MDS_value=(d->adc1_value*d->adc2_value)/d->adc3_value;
			}
			if(d->MDS_value>=d->threshold)
			{
	    		d->MDS_PIN = write_gpio(GPIOB,GPIO_PIN_12, PIN_SET);
	    		HAL_Delay(1000);
	    	}
	    	else
	    	{
	    		d->MDS_PIN = write_gpio(GPIOB,GPIO_PIN_12, PIN_RESET);
	    		HAL_Delay(1000);
	    	}



//	adc_stop();



}

//void adc_channel(ADC_Channel channel)
//{



//	if(adcChnlChecker[0] == 1)
//	{
//		adc_start();
//		ADC_select_CH5();
//		adc_conversion();
//		adc1_value = voltage_cal();
//	}
//	else
//	{
//		__NOP();
//	}
//	if(adcChnlChecker[1] == 1)
//	{
//		adc_start();
//		ADC_select_CH5();
//		adc_conversion();
//		adc1_value = voltage_cal();
//	}
//	else
//	{
//		__NOP();
//	}

//	if (channel == my_ADC_CHANNEL_5)
//	{
//		adc_start();
//		ADC_select_CH5();
//		adc_conversion();
//		adc1_value = voltage_cal();
//	} else if (channel == my_ADC_CHANNEL_6)
//	{
//		adc_start();
//		ADC_select_CH6();
//		adc_conversion();
//		adc2_value = voltage_cal();
//	}
//	else if(channel == my_ADC_CHANNEL_6)
//	{
//		adc_start();
//		ADC_select_CH5();
//		adc_conversion();
//		adc1_value = voltage_cal();
//
//		adc_start();
//		ADC_select_CH6();
//		adc_conversion();
//		adc2_value = voltage_cal();
//	}
//	adc_stop();
//}

//void adc_handler(ADC_Channel channel[], size_t num_channel)
//void adc_handler(ADC_Channel channel)
//{
////	for (size_t i = 0; i < num_channel; i++) {
////    adc_channel(channel[i]);
//
//
//	adc_channel(channel);
//}


//int A1,A2;
//float d1,d2;
//int switch_case=0;
//uint8_t w[4];
//extern uint32_t ss[4];
//
//void adc1(void)
//{
//
//	adc_start();
//	adc_conversion();
//	switch_channel();
//
//	adc_stop();
//	if(w[0]==0)
//	{
//	  ADC_channel_set(CHANNEL_5);
//	  {
//		  switch_channel();
//	  }
//	}
//	if(w[1]==0)
//	{
//	  ADC_channel_set(CHANNEL_6);
//	  {
//		  switch_channel();
//	  }
//	}
//
//
//}
//
//void switch_channel(void)
//{
//	if(ss[0] == CHANNEL_5 )
//	{
//		switch_case=1;
//    }
//	 if(ss[1] == CHANNEL_6 )
//	{
//	    switch_case=2;
//    }
//
//	switch(switch_case)
//	{
//	case 1: A1 = adc_value();
//		    d1 = (A1 * (3.3/4095));
//            w[0]=0;
//            break;
//
//	case 2: A2 = adc_value();
//		    d2 = (A2 * (3.3/4095));
//            w[1]=0;
//	        break;
//	}
//}
////void adc2(void)
////{
////	ADC_select_CH6();
////	adc_start();
////	adc_conversion();
////    f=voltage_cal();
////     adc_stop();
////}
//
//void adc_handler(void)
//{
//	adc1();
//}

