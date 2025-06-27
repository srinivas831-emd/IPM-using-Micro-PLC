/*
 * com_ADC.h
 *
 *  Created on: Sep 14, 2024
 *      Author: KAVYA
 */

#ifndef INC_COM_ADC_H_
#define INC_COM_ADC_H_

//void USER_SystemClock_Config(void);
void USER_ADC1_Init(void);
void USER_GPIO_Init(void);
void ADC_select_CH1(void);
void ADC_select_CH2(void);
void ADC_select_CH3(void);
void ADC_select_CH4(void);
void adc_start(void);
void adc_conversion(void);
uint16_t adc_value(void);
void adc_stop(void);


#endif /* INC_COM_ADC_H_ */
