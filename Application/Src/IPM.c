/*
 * IPM.c
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctype.h"
#include "data.h"
#include "app_EEPROM.h"
#include "com_EEPROM.h"
#include "app_RTC.h"
#include "com_RTC.h"
#include "app_config.h"
#include "com_config.h"
#include "com_ADC.h"
#include "app_ADC.h"
#include "app_GSM.h"
#include "com_GSM.h"
#include "com_terminal.h"
#include "app_terminal.h"
#include "com_config.h"


struct data d;

void application(void)
{
	Configurator();			//FOR OFFLINE
	dataFromCloud();		//DATA FROM GOOGLSHEET OR MOBILE
	get_time(&d);			//GET THE TIME FROM RTC
    ADC_handler(&d);		//ADC INITIALIZATION OR DESABLING
    DataToCloud(&d);		//UPLOAD DATA TO GOOGLE SHEET OR MOBLIE
 //	EEPROM_transmit(&d);
    //terminal(&d);
}
