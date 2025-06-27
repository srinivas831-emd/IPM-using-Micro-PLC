/*
 * app_GSM.h
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#ifndef INC_APP_GSM_H_
#define INC_APP_GSM_H_
#include "main.h"
#include "Com_GSM.h"
#include "data.h"
void DataToCloud(struct data *d);
void DataToPass();
void UART4_Init(void);
void dataToUart();



#endif /* INC_APP_GSM_H_ */
