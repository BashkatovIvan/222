/*
 * LubricantMotor.cpp
 *
 *  Created on: 11 авг. 2022 г.
 *      Author: amaslov
 */

#include "LubricantMotor.h"
#include "Utils.h"
#include "gpio.h"
#include "Globals.h"
#include <limits.h>

#define LUBRICANT_MOTOR_INTERRUPT_PRESCALER 5
#define CHECK_TIME_MAX (30000 / LUBRICANT_MOTOR_INTERRUPT_PRESCALER)

LubricantMotor::LubricantMotor(PressInterface *press) : press(press)
{
	stopMotor();
	updateParamsFromDb();

	mode = (pumpingModeParams.lubricantTime == 0 || pumpingModeParams.lubricantCount == 0) ? NORMAL : PUMPING;
	if (mode == PUMPING)
	{
		state = LUBRICANT_READY;
	}
	else
	{
		setIDLEState();
	}
}

void LubricantMotor::incrementCounter()
{
	if (normalModeParams.type == LubricantType::BY_COUNTER && state == LubricantMotorState::IDLE)
	{
		normalModeParams.offCount = normalModeParams.offCount >= UINT16_MAX ? UINT16_MAX : normalModeParams.offCount + 1;
	}
	else
	{
		normalModeParams.offCount = 0;
	}
}

void LubricantMotor::updateParamsFromDb()
{
	pumpingModeParams.lubricantTime = dataBase->getLubricantTime() * 1000 / LUBRICANT_MOTOR_INTERRUPT_PRESCALER;
	pumpingModeParams.lubricantCount = dataBase->getLubricantCount();
	pumpingModeParams.count = 1;

	normalModeParams.lubricantOnTime = dataBase->getLubricantOnTime() * 1000 / LUBRICANT_MOTOR_INTERRUPT_PRESCALER;
	normalModeParams.lubricantOffTime = timeInUTC(dataBase->getLubricantOffTime() * 60);
	normalModeParams.lubricantOffCount = dataBase->getLubricantOffCount();
	normalModeParams.offCount = 0;
	normalModeParams.type = dataBase->getLubricantType();
}

void LubricantMotor::startMotor()
{
	HAL_GPIO_WritePin(OUT_27_GPIO_Port, OUT_27_Pin, GPIO_PIN_SET);
}

void LubricantMotor::stopMotor()
{
	HAL_GPIO_WritePin(OUT_27_GPIO_Port, OUT_27_Pin, GPIO_PIN_RESET);
}

LubricantMode LubricantMotor::getCurMode()
{
	return mode;
}

LubricantMotorState LubricantMotor::setIDLEState()
{
	stopMotor();
	mode = NORMAL;
	rtClock->clearRTCDate();
	normalModeParams.offTime = {};
	return state = LubricantMotorState::IDLE;
}

LubricantMotorState LubricantMotor::setReadyState()
{
	mode = NORMAL;
	normalModeParams.onTime = 0;
	return state = LubricantMotorState::LUBRICANT_READY;
}

LubricantMotorState LubricantMotor::handleInPumpingMode(Sensors *s)
{
	PumpingModeParams *p = &pumpingModeParams;
	uint32_t checkTime = (p->lubricantTime / 2) > CHECK_TIME_MAX ? CHECK_TIME_MAX : (p->lubricantTime / 2);

	if (oneShotFlag)
	{
		oneShotFlag = false;
		return setIDLEState();
	}

	if (state == IN_PROGRESS)
	{
		if (p->count <= p->lubricantCount)
		{
			if (p->time <= p->lubricantTime)
			{
				//check pressure for active state
				if ((p->time >= checkTime) && (s->SP2 == sensors.SP2 && !sensors.SP2))
				{
					stopMotor();
					mode = NORMAL;
					error = SystemError::N12_LUBRICANT_PRESSURE_ERRROR;
					return state = LubricantMotorState::LUBRICANT_ERROR;
				}
				else
				{
					//check oil
					if (s->SL == sensors.SL && sensors.SL)
					{
						stopMotor();
						mode = NORMAL;
						error = SystemError::N09_LUBRICANT_OIL_LEVEL;
						return state = LubricantMotorState::LUBRICANT_ERROR;
					}

					startMotor();
				}
			}
			else if (p->time <= (p->lubricantTime * 2))
			{
				stopMotor();
				//check pressure for passive state
				if ((p->time >= (p->lubricantTime + checkTime)) && (s->SP2 == sensors.SP2 && sensors.SP2))
				{
					mode = NORMAL;
					error = SystemError::N13_LUBRICANT_PRESSURE_ERRROR;
					return state = LubricantMotorState::LUBRICANT_ERROR;
				}
			}

			p->time++;
			if (p->time >= (p->lubricantTime * 2))
			{
				p->count++;
				p->time = 0;
				return state;
			}
		}
		else
		{
			return setIDLEState();
		}
	}

	return state;
}

LubricantMotorState LubricantMotor::handleByTimeMode(NormalModeParams *p, Sensors *s)
{
	uint32_t checkTime = (p->lubricantOnTime / 2) > CHECK_TIME_MAX ? CHECK_TIME_MAX : (p->lubricantOnTime / 2);

	if (oneShotFlag)
	{
		oneShotFlag = false;

		if (state == LubricantMotorState::IN_PROGRESS)
		{
			setIDLEState();
		}
		else if (state == LubricantMotorState::IDLE)
		{
			setReadyState();
		}
	}

	if (p->onTime >= p->lubricantOnTime && state == LubricantMotorState::IN_PROGRESS)
	{
		setIDLEState();
	}

	if (timeInSec(p->offTime) >= timeInSec(p->lubricantOffTime) && state == LubricantMotorState::IDLE)
	{
		setReadyState();
	}

	if (state == LubricantMotorState::IN_PROGRESS)
	{
		//check oil
		if (s->SL == sensors.SL && sensors.SL)
		{
			stopMotor();
			warning = SystemWarning::N09_LUBRICANT_OIL_LEVEL_W;
			return state = LubricantMotorState::LUBRICANT_WARNING;
		}

		p->onTime++;
		startMotor();

		//check Pressure
		if ((p->onTime >= checkTime) && (s->SP2 == sensors.SP2 && !sensors.SP2))
		{
			stopMotor();
			warning = SystemWarning::N12_LUBRICANT_PRESSURE_ERRROR_W;
			return state = LubricantMotorState::LUBRICANT_WARNING;
		}
	}
	else if (state == LubricantMotorState::IDLE)
	{
		p->offTime = rtClock->getRTCTime();

		//check Pressure
		if (((timeInSec(p->offTime) * 1000) >= CHECK_TIME_MAX * LUBRICANT_MOTOR_INTERRUPT_PRESCALER) && (s->SP2 == sensors.SP2 && sensors.SP2))
		{
			warning = SystemWarning::N13_LUBRICANT_PRESSURE_ERRROR_W;
			state = LubricantMotorState::LUBRICANT_WARNING;
		}
	}

	return state;
}

LubricantMotorState LubricantMotor::handleByCounterMode(NormalModeParams *p, Sensors *s)
{
	uint32_t checkTime = (p->lubricantOnTime / 2) > CHECK_TIME_MAX ? CHECK_TIME_MAX : (p->lubricantOnTime / 2);

	if (oneShotFlag)
	{
		oneShotFlag = false;

		if (state == LubricantMotorState::IN_PROGRESS)
		{
			setIDLEState();
		}
		else if (state == LubricantMotorState::IDLE)
		{
			setReadyState();
		}
	}

	if (p->onTime >= p->lubricantOnTime && state == LubricantMotorState::IN_PROGRESS)
	{
		setIDLEState();
	}

	if (p->offCount >= p->lubricantOffCount && state == LubricantMotorState::IDLE)
	{
		setReadyState();
		p->offCount = 0;
	}

	if (state == LubricantMotorState::IN_PROGRESS)
	{
		//check oil
		if (s->SL == sensors.SL && sensors.SL)
		{
			stopMotor();
			warning = SystemWarning::N09_LUBRICANT_OIL_LEVEL_W;
			return state = LubricantMotorState::LUBRICANT_WARNING;
		}

		p->onTime++;
		startMotor();

		//check Pressure
		if ((p->onTime >= checkTime) && (s->SP2 == sensors.SP2 && !sensors.SP2))
		{
			stopMotor();
			warning = SystemWarning::N12_LUBRICANT_PRESSURE_ERRROR_W;
			return state = LubricantMotorState::LUBRICANT_WARNING;
		}
	}
	else if (state == LubricantMotorState::IDLE)
	{
		stopMotor();
		p->offTime = rtClock->getRTCTime();

		//check Pressure
		if (((timeInSec(p->offTime) * 1000) >= CHECK_TIME_MAX * LUBRICANT_MOTOR_INTERRUPT_PRESCALER) && (s->SP2 == sensors.SP2 && sensors.SP2))
		{
			warning = SystemWarning::N13_LUBRICANT_PRESSURE_ERRROR_W;
			state = LubricantMotorState::LUBRICANT_WARNING;
		}
	}
	return state;
}

LubricantMotorState LubricantMotor::handleInNormalMode(Sensors *s)
{
	NormalModeParams *p = &normalModeParams;

	if (state == LubricantMotorState::LUBRICANT_ERROR || state == LubricantMotorState::LUBRICANT_WARNING)
	{
		stopMotor();
		return state;
	}

	/*disable lubricant in settings*/
	if (normalModeParams.lubricantOnTime == 0)
	{
		stopMotor();
		return state = LubricantMotorState::LUBRICANT_DISABLED;
	}

	if (p->type == LubricantType::BY_TIME)
	{
		return handleByTimeMode(p, s);
	}
	else if (p->type == LubricantType::BY_COUNTER)
	{
		return handleByCounterMode(p, s);
	}
	else if (p->type == LubricantType::LUBRICANT_OFF)
	{
		return state;
	}

	return state;
}

void LubricantMotor::handle()
{
	interruptPrescaler = interruptPrescaler >= (LUBRICANT_MOTOR_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (LUBRICANT_MOTOR_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

LubricantMotorState LubricantMotor::postInterruptHandler(Sensors *s)
{
	if (!interruptFlag)
		return state;

	interruptFlag = false;

	if (mode == PUMPING)
	{
		state = handleInPumpingMode(s);
	}
	else if (mode == NORMAL)
	{
		state = handleInNormalMode(s);
	}
	sensors = *s;

	return state;
}

void LubricantMotor::oneShot() //startStop button
{
	if (state != LubricantMotorState::LUBRICANT_ERROR && state != LubricantMotorState::LUBRICANT_WARNING)
		oneShotFlag = true;
}

SystemError LubricantMotor::getError()
{
	return error;
}

SystemWarning LubricantMotor::getWarning()
{
	return warning;
}

void LubricantMotor::clearErrors()
{
	if (state == LubricantMotorState::LUBRICANT_ERROR)
	{
		error = SystemError::NO_ERROR;
		setIDLEState();
	}

	else if (state == LubricantMotorState::LUBRICANT_WARNING)
	{
		warning = SystemWarning::NO_WARNING;
		setIDLEState();
	}
}

LubricantMotorState LubricantMotor::updateState(Sensors *s)
{
	return postInterruptHandler(s);
}

bool LubricantMotor::run()
{
	if (state == LubricantMotorState::LUBRICANT_READY)
	{
		state = LubricantMotorState::IN_PROGRESS;
		return true;
	}
	return false;
}

LubricantMotorState LubricantMotor::getState()
{
	return state;
}
