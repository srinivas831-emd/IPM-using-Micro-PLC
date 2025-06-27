/*
 * com_RTC.h
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#ifndef INC_COM_RTC_H_
#define INC_COM_RTC_H_

#include  "main.h"

void USER_I2C1_Init();
//void I2C_write(uint8_t *arr);
void I2C_Write(I2C_HandleTypeDef *hi2c, uint16_t device_address, uint16_t mem_address, uint8_t mem_address_size, uint8_t *data, size_t data_size, uint32_t timeout);


void I2C_read(uint8_t *data);


#endif /* INC_COM_RTC_H_ */
