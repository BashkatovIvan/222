/*
 * PressStatesHandler.cpp
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */

#include "Press.h"

SystemError Press::updateAll()
{
	Sensors s = Sensors::getState();
	SystemError err = SystemError::NO_ERROR;

	for(;;)
	{
		if (state == PressState::PRESS_ERROR)
		{
			err = error;
			break;
		}

		if (state != PressState::INIT)
		{
			if (ControlPanelState::CONTROL_PANEL_ERROR == controlPanel->updateState(&s)) {
				err = controlPanel->getError();
				break;
			}

			if (SlideBlockStates::SLIDE_BLOCK_ERROR == slideBlock->updateState(&s)) {
				err = slideBlock->getError();
				break;
			}

			if (BlowingOffMotorState::BLOWING_OFF_MOTOR_ERROR == blowingOffMotor->updateState(&s)) {
				err = blowingOffMotor->getError();
				break;
			}
		}

		LubricantMotorState lubricantMotorState = lubricantMotor->updateState(&s);
		if (LubricantMotorState::LUBRICANT_ERROR == lubricantMotorState) {
			err = lubricantMotor->getError();
			break;
		}
		if (LubricantMotorState::LUBRICANT_WARNING == lubricantMotorState)
		{
			warning = lubricantMotor->getWarning();
		}

		if (MainMotorState::MAIN_MOTOR_ERROR == mainMotor->updateState(&s)) {
			err = mainMotor->getError();
			break;
		}

		protectiveShield->updateState(&s);

		break;
	}

	/*Проверка на внутренние ошибки подсистем пресса. Если возникнет ошибка, вызов фукции updateState() перейдет в обработчик ошибок, в котором муфта отключается*/
	checkCondition((err != SystemError::NO_ERROR && state != PressState::PRESS_ERROR), err, state);

	if (PressState::PRESS_ERROR == updateState(&s)) {
		err = getError();
	}

	/*Проверка на ошибки в работе пресса. После updateState()*/
	checkCondition((err != SystemError::NO_ERROR && state != PressState::PRESS_ERROR), err, state);

	sensors = s;

	return err;
}

PressState Press::updateState(Sensors *s)
{
	return postInterruptHandler(s);
}

