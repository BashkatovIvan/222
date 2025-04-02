/*
 * PressInterruptHandler.cpp
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */
#include "Press.h"

void Press::handle()
{
	fastInterruptPrescaler = fastInterruptPrescaler >= (PRESS_FAST_INTERRUPT_PRESCALER - 1) ? 0 : fastInterruptPrescaler + 1;
	if (fastInterruptPrescaler >= (PRESS_FAST_INTERRUPT_PRESCALER - 1))
		fastInterruptFlag = true;

	slowInterruptPrescaler = slowInterruptPrescaler >= (PRESS_SLOW_INTERRUPT_PRESCALER - 1) ? 0 : slowInterruptPrescaler + 1;
	if (slowInterruptPrescaler >= (PRESS_SLOW_INTERRUPT_PRESCALER - 1))
		slowInterruptFlag = true;
}

PressState Press::postInterruptHandler(Sensors *s)
{
	if (!fastInterruptFlag)
		return state;

	//Сбросить флаг быстрого прерывания
	fastInterruptFlag = false;

	//Обработчик текущего состояния пресса (пресс может находиться либо в простое, либо запущен цикл, либо включена смазка и т.д.)
	if (state < PRESS_STATES_SIZE)
		state = (this->*statesHandlers[state])(s);

	//Сбросить флаг медленного прерывания
	if (slowInterruptFlag)
		slowInterruptFlag = false;

	sensors = *s;

	return state;
}



