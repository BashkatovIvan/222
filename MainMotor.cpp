/*
 * MainMotor.cpp
 *
 *  Created on: 15 авг. 2022 г.
 *      Author: amaslov
 */

#include "MainMotor.h"
#include "gpio.h"
#include <limits.h>

#define MAIN_MOTOR_INTERRUPT_PRESCALER 5
#define CHECK_TIME (10 / MAIN_MOTOR_INTERRUPT_PRESCALER)

MainMotor::MainMotor()
{
	stop();
}

void MainMotor::updateParamsFromDb()
{

}

void MainMotor::switchOn()
{
	HAL_GPIO_WritePin(OUT_22_GPIO_Port, OUT_22_Pin, GPIO_PIN_SET);
}

void MainMotor::switchOff()
{
	HAL_GPIO_WritePin(OUT_22_GPIO_Port, OUT_22_Pin, GPIO_PIN_RESET);
}

bool MainMotor::getCtlState()
{
	return HAL_GPIO_ReadPin(OUT_22_GPIO_Port, OUT_22_Pin) == GPIO_PIN_SET;
}

void MainMotor::resetState()
{
	KM1 = 0;
}

void MainMotor::run()
{
	if (state == MainMotorState::MAIN_MOTOR_ERROR)
		return;

	if (state == MainMotorState::WAIT_FOR_STARTED)
		return;

	if (state == MainMotorState::STARTED)
		return;

	resetState();
	switchOn();
	state = MainMotorState::WAIT_FOR_STARTED;
}

void MainMotor::stop()
{
	if (state == MainMotorState::MAIN_MOTOR_ERROR)
		return;

	if (state == MainMotorState::WAIT_FOR_STOPPED)
		return;

	if (state == MainMotorState::STOPED)
		return;

	resetState();
	switchOff();
	state = MainMotorState::WAIT_FOR_STOPPED;
}

bool MainMotor::isAllowedUpdateState()
{
	if (state == MainMotorState::MAIN_MOTOR_ERROR)
		return false;

	if (state == MainMotorState::MAIN_MOTOR_IDLE)
		return false;

	return true;
}

void MainMotor::handle()
{
	interruptPrescaler = interruptPrescaler >= (MAIN_MOTOR_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (MAIN_MOTOR_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

MainMotorState MainMotor::postInterruptHandler()
{
	if (!interruptFlag)
		return state;

	interruptFlag = false;

	if (isAllowedUpdateState())
	{
		if (sensors.KM1_1)
		{
			KM1 = KM1 < INT_MAX ? KM1 + 1 : INT_MAX;
		}
		else
		{
			KM1 = KM1 > INT_MIN ? KM1 - 1 : INT_MIN;
		}
	}

	return state;
}

int MainMotor::getStartedTime()
{
	int ret = (KM1 - 1);
	ret = ret < 0 ? 0 : ret;

	if (state == MainMotorState::STARTED)
		return ret * MAIN_MOTOR_INTERRUPT_PRESCALER;

	return 0;
}

int MainMotor::getStopedTime()
{
	int ret = -(KM1 - 1);
	ret = ret < 0 ? 0 : ret;

	if (state == MainMotorState::STOPED)
		return ret * MAIN_MOTOR_INTERRUPT_PRESCALER;

	return 0;
}

MainMotorState MainMotor::getState() {
	return state;
}

SystemError MainMotor::getError()
{
	return error;
}

void MainMotor::clearErrors()
{
	if (state == MAIN_MOTOR_ERROR)
	{
		resetState();
		state = MainMotorState::MAIN_MOTOR_IDLE;
		error = SystemError::NO_ERROR;
	}
}

MainMotorState MainMotor::updateState(Sensors *s)
{
	KM1 = s->KM1_1 != sensors.KM1_1 ? 0 : KM1;
	sensors = *s;

	if (isAllowedUpdateState())
	{
		if (KM1 < -CHECK_TIME)
		{
			state = MainMotorState::STOPED;
		}
		else if (KM1 > CHECK_TIME)
		{
			state = MainMotorState::STARTED;
		}
	}

	return postInterruptHandler();
}
