/*
 * PressCounter.cpp
 *
 *  Created on: 30 авг. 2022 г.
 *      Author: amaslov
 */
#include "Press.h"

void Press::counterOneShot(bool clearValue)
{
	if (clearValue)
		counterValue = 0;
	else
		counterEnabled = !counterEnabled;
}

bool Press::counterIsEnable()
{
	return counterEnabled;
}

uint32_t Press::getCounterValue()
{
	return counterValue;
}

void Press::incrementCounter()
{
	lubricantMotor->incrementCounter();

	if (counterEnabled)
		counterValue = counterValue >= 9999999 ? 1 : counterValue + 1;
}

