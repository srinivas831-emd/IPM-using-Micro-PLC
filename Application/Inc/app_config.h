/*
 * app_config.h
 *
 *  Created on: Sep 13, 2024
 *      Author: KAVYA
 */

#ifndef INC_APP_CONFIG_H_
#define INC_APP_CONFIG_H_
#include "data.h"


//void DataTOgsm();
//void gpiostatus(struct status d);
//void setTime(struct time t);
void set_output(struct data *d);
void reset_output(struct data *d);
void read_pinstatus(struct data *d);
void Configurator();
void extract_data(char* command);
void Process_Commands(void);
void pin_config();
void clear_buffer();
void channelGpioPins();


#endif /* INC_APP_CONFIG_H_ */
