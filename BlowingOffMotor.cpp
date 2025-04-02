/*
 * BlowingOffMotor.cpp
 *
 *  Created on: 4 сент. 2022 г.
 *      Author: alexander
 */

#include "BlowingOffMotor.h"
#include "main.h"
#include "Globals.h"

#define BLOWING_OFF_MOTOR_INTERRUPT_PRESCALER 5

BlowingOffMotor::BlowingOffMotor()
{
	browingOffWorkTime = dataBase->getBrowingOffWorkTime();
	enabledInPressWorkTime = dataBase->getBrowingOffEnabled() == 0 ? false : true;
	stopMotor();
}

void BlowingOffMotor::startMotor()
{
	HAL_GPIO_WritePin(OUT_28_GPIO_Port, OUT_28_Pin, GPIO_PIN_SET);
}

void BlowingOffMotor::stopMotor()
{
	HAL_GPIO_WritePin(OUT_28_GPIO_Port, OUT_28_Pin, GPIO_PIN_RESET);
	workTime = 0;
}

void BlowingOffMotor::run()
{
	if (state == BlowingOffMotorState::BLOWING_OFF_MOTOR_ERROR || !enabledInPressWorkTime || browingOffWorkTime == 0)
		return;

	state = BlowingOffMotorState::BLOWING_OFF_MOTOR_STARTED;
	workTime = 0;
}

void BlowingOffMotor::oneShot()
{
	if (browingOffWorkTime == 0)
		return;

	oneShotFlag = true;
}

void BlowingOffMotor::clearErrors()
{
	if (state == BlowingOffMotorState::BLOWING_OFF_MOTOR_ERROR)
	{
		error = SystemError::NO_ERROR;
		state = BlowingOffMotorState::BLOWING_OFF_MOTOR_STOPPED;
		oneShotFlag = false;
	}
}

void BlowingOffMotor::handle()
{
	interruptPrescaler = interruptPrescaler >= (BLOWING_OFF_MOTOR_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (BLOWING_OFF_MOTOR_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

BlowingOffMotorState BlowingOffMotor::postInterruptHandler(Sensors *s)
{
	if (!interruptFlag)
		return state;

	interruptFlag = false;

	if (state == BlowingOffMotorState::BLOWING_OFF_MOTOR_ERROR)
		return state;

	if (oneShotFlag)
	{
		state = state == BlowingOffMotorState::BLOWING_OFF_MOTOR_STARTED ? BlowingOffMotorState::BLOWING_OFF_MOTOR_STOPPED : BlowingOffMotorState::BLOWING_OFF_MOTOR_STARTED;
		oneShotFlag = false;
	}

	if (state == BlowingOffMotorState::BLOWING_OFF_MOTOR_STARTED)
	{
		startMotor();
		workTime = workTime < UINT32_MAX ? workTime + 1 : UINT32_MAX;

		if (workTime * BLOWING_OFF_MOTOR_INTERRUPT_PRESCALER >= browingOffWorkTime)
		{
			state = BlowingOffMotorState::BLOWING_OFF_MOTOR_STOPPED;
		}
	}

	if (state == BlowingOffMotorState::BLOWING_OFF_MOTOR_STOPPED)
	{
		stopMotor();
	}

	return state;
}

BlowingOffMotorState BlowingOffMotor::updateState(Sensors *s) {
	return postInterruptHandler(s);
}
