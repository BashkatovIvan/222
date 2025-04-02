/*
 * Clutch.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: alexander
 */

#include "Clutch.h"
#include "main.h"
#include <limits.h>
#include "Globals.h"

#define CLUTCH_INTERRUPT_PRESCALER 1
#define CLUTCH_CHECK_TIME 5 /*5ms*/

Clutch::Clutch()
{
	switchOff();
}

void Clutch::handle()
{
	interruptPrescaler = interruptPrescaler >= (CLUTCH_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (CLUTCH_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

ClutchStates Clutch::postInterruptHandler(Sensors *s)
{
	if (!interruptFlag)
		return state;

	interruptFlag = false;

	stateTime = stateTime < INT_MAX ? stateTime + 1 : INT_MAX;

	//Контроль работы силовых ключей
	if (sensors.YV1 == s->YV1 && sensors.YV2 == s->YV2)
	{
		if (stateTime >= CLUTCH_CHECK_TIME)
		{
			if (enabled)
			{
				if (!s->YV1 || !s->YV2)
				{
					error = N14_CLUTCH_TRANSISTOR_BREAKAGE;
					return ClutchStates::CLUTCH_ERROR;

				}
			}
			else
			{
				if (s->YV1 || s->YV2)
				{
					error = (s->YV1 && s->YV2) ? N15_CLUTCH_TRANSISTORS_SHORT_CIRCUIT : N16_CLUTCH_TRANSISTOR_SHORT_CIRCUIT;
					return ClutchStates::CLUTCH_ERROR;
				}
			}
		}
	}

#ifndef DEBUG_VERSION
//	//Датчик согласованной работы клапанов (250 ms)
//	if(stateTime >= (250 / CLUTCH_INTERRUPT_PRESCALER))
//	{
//		if (s->SQ11_SQ12 == sensors.SQ11_SQ12 && !s->SQ11_SQ12)
//		{
//			error = N11_SQ11_IS_OFF;
//			return ClutchStates::CLUTCH_ERROR;
//		}
//	}
#endif

	return enabled ? ClutchStates::CLUTCH_ENABLED : ClutchStates::CLUTCH_DISABLED;
}

void Clutch::switchOn()
{
	if (state != ClutchStates::CLUTCH_ERROR)
	{
		if (!enabled)
		{
			stateTime = 0;
		}
		HAL_GPIO_WritePin(GPIOB, OUT_19_Pin | OUT_20_Pin, GPIO_PIN_SET);
		enabled = true;
	}
}

void Clutch::clearErrors()
{
	if (state == ClutchStates::CLUTCH_ERROR)
	{
		switchOff(); //На всякий случай, чтобы не возникло неоднозначных, ошибочных состояний
		error = SystemError::NO_ERROR;
		state = ClutchStates::CLUTCH_DISABLED;
	}
}

void Clutch::switchOff()
{
	if (enabled)
	{
		stateTime = 0;
	}
	HAL_GPIO_WritePin(GPIOB, OUT_19_Pin | OUT_20_Pin, GPIO_PIN_RESET);
	enabled = false;
}

ClutchStates Clutch::getState() {
	return state;
}

SystemError Clutch::getError() {
	return error;
}

ClutchStates Clutch::updateState(Sensors *s)
{
	for(;;)
	{
		if (state == ClutchStates::CLUTCH_ERROR)
			break;

		if (ClutchStates::CLUTCH_ERROR == (state = postInterruptHandler(s)))
			switchOff();

		break;
	}
	sensors = *s;

	return state;
}

