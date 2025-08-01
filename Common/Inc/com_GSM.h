/*
 * com_GSM.h
 *
 *  Created on: Sep 11, 2024
 *      Author: KAVYA
 */

#ifndef INC_COM_GSM_H_
#define INC_COM_GSM_H_

#include "app_GSM.h"
#include "main.h"
#include "data.h"
#include<stdbool.h>
void UART2_Init(void);
void dataFromCloud();
void DataToWiFi(struct data *d);
void DataToGSM(struct data *d);
//void ProcessData();
void USERRCV_GPIO_Init();
void cloud_data_receive();
void cloud_Process_Commands(void);
void Process_GPIO_Status();
void extract_cloud_data(char* WifiData);
void cloud_set_output(struct data *d);
void cloud_reset_output(struct data *d1);
void cloud_read_pinstatus(struct data *d2);
void GSM_SendSMS();
bool GSM_Init(void);
bool GSM_SendCommandandSMS(const char *cmd, uint32_t timeout);
void SendSmsBody(void);
bool GSM_WaitForResponse(uint32_t timeout);
void DataToPhone(char * Message);
void DataToGsheet();
bool GSMInitGsheet();
bool HTTPInitAndSend(char *dataToSend);
void ProcessIncomingData(void);
void WiFi_ProcessIncomingData(void);
void extractData(char *a);
bool DatatoESP(char * Data);




//void wifiHAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif /* INC_COM_GSM_H_ */
