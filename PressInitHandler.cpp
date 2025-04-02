/*
 * PressInitHandler.cpp
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */
#include "Press.h"

PressState Press::initHandler(Sensors *s)
{
	if (lubricantMotor->getCurMode() == LubricantMode::PUMPING)
	{
		info = SystemInfo::N31_LUBRICANT_ON_STARTED;
		if (lubricantMotor->getState() != LubricantMotorState::LUBRICANT_ERROR)
		{
			lubricantMotor->run();
		}
	}
	else
	{
		if (state != PressState::PRESS_ERROR)
			state = PressState::PRESS_IDLE;
	}

	return state;
}



