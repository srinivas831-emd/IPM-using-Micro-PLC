/*
 * app_RTC.h
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#ifndef INC_APP_RTC_H_
#define INC_APP_RTC_H_

#define DS3107_ADDRESS 0xD0
#include <stdint.h>
void app(void);
void set_time(uint8_t sec,uint8_t min,uint8_t hr);
void set_date(uint8_t day,uint8_t dat,uint8_t mon,uint8_t yr);
void get_time();
void data_store();


#endif /* INC_APP_RTC_H_ */
