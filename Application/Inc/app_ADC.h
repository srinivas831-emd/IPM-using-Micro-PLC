/*
 * app_ADC.h
 *
 *  Created on: Sep 14, 2024
 *      Author: KAVYA
 */

#ifndef INC_APP_ADC_H_
#define INC_APP_ADC_H_

#include "data.h"

float voltage_cal(void);

void ADC_handler(struct data *d);


#endif /* INC_APP_ADC_H_ */
