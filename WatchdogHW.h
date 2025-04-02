/*
 * WatchdogHW.h
 *
 * Класс аппаратного сторожевого таймера. Дергает ножками, чтобы не блокировались управляющие сигналы
 *
 *  Created on: Aug 8, 2022
 *      Author: amaslov
 */

#ifndef SRC_WATCHDOGHW_H_
#define SRC_WATCHDOGHW_H_

#include "main.h"
#include "gpio.h"

class WatchdogHW
{
public:
	WatchdogHW()
	{
		blockOutputCircuits();
	}
	void blockOutputCircuits()
	{
		HAL_GPIO_WritePin(OUT_29_GPIO_Port, OUT_29_Pin, GPIO_PIN_RESET);
	}
	void unblockOutputCircuits()
	{
		HAL_GPIO_WritePin(OUT_29_GPIO_Port, OUT_29_Pin, GPIO_PIN_SET);
	}
};

#endif /* SRC_WATCHDOGHW_H_ */
