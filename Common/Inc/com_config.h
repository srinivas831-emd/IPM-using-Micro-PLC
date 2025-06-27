/*
 * com_config.h
 *
 *  Created on: Sep 13, 2024
 *      Author: KAVYA
 */

#ifndef INC_COM_CONFIG_H_
#define INC_COM_CONFIG_H_

#include "main.h"
#define RX_BUFFER_SIZE 14  // UART receive buffer size
typedef enum
{
	OUTPUT,
	INPUT
}mode;

typedef enum
{
  PIN_RESET = 0,
  PIN_SET
} myPinState;

void user_SystemClock_Config(void);
void user_output_GPIO_Init(void);
void user_GPIO_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,mode PIN_mode);
void gpio_output(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,mode PIN_mode);
void gpio_input(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, mode PIN_mode);
void user_USART1_UART_Init(void);
void data_receive();
void status_transmit();
myPinState read_gpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
myPinState write_gpio(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, myPinState PinState);



#endif /* INC_COM_CONFIG_H_ */
