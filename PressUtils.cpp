/*
 * PressUtils.cpp
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */
#include "Press.h"

bool Press::checkProtectiveShieldState(Sensors *s, PressState saveState, SystemError err)
{
	if (s->SQ14 == sensors.SQ14 && !s->SQ14)
	{
		error = err;
		this->saveState = saveState;
		state = PressState::PRESS_ERROR;
		return false;
	}
	return true;
}

bool Press::checkBlockingState(Sensors *s, PressState saveState)
{
	if (s->SQ15 == sensors.SQ15 && !s->SQ15)
	{
		error = SystemError::N21_BLOCK_SENSOR_EVENT;
		this->saveState = saveState;
		state = PressState::PRESS_ERROR;
		return false;
	}
	return true;
}

bool Press::checkPressOverloadState(Sensors *s, PressState saveState)
{
	if (s->SQ16 == sensors.SQ16 && !s->SQ16)
	{
		error = SystemError::N10_PRESS_OVERLOAD;
		this->saveState = saveState;
		state = PressState::PRESS_ERROR;
		return false;
	}
	return true;
}

bool Press::checkCondition(bool condition, SystemError errorIfconditionTrue, PressState saveStateIfConditionTrue)
{
	if (condition)
	{
		error = errorIfconditionTrue;
		this->saveState = saveStateIfConditionTrue;
		state = PressState::PRESS_ERROR;
		return true;
	}
	return false;
}

