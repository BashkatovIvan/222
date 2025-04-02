/*
 * DisplayUARTRoutines.h
 *
 *  Created on: 20 авг. 2022 г.
 *      Author: alexander
 */

#ifndef INCLUDE_DISPLAYUARTROUTINES_H_
#define INCLUDE_DISPLAYUARTROUTINES_H_
#include "usart.h"

void uartRxEventCallback(struct __UART_HandleTypeDef *huart, uint16_t Pos);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);


#endif /* INCLUDE_DISPLAYUARTROUTINES_H_ */
