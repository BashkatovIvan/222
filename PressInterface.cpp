/*
 * PressInterface.cpp
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */
#include "Press.h"

Sensors Press::getSensors()
{
	return sensors;
};

SystemError Press::getError()
{
	return error;
};

SystemWarning Press::getWarning() {
	return warning;
}
SystemInfo Press::getInfo(){
	return info;
}

PressState Press::getState()
{
	return state;
};
PressModes Press::getMode()
{
	return mode;
};

bool Press::mainMotorIsStarted()
{
	return mainMotor->getState() == MainMotorState::STARTED;
};

bool Press::lubricantIsStarted()
{
	return lubricantMotor->getState() == LubricantMotorState::IN_PROGRESS;
};

void Press::lubricantOneShot()
{
	lubricantMotor->oneShot();
};

bool Press::blowingOffIsStarted()
{
	return blowingOffMotor->getState() == BlowingOffMotorState::BLOWING_OFF_MOTOR_STARTED;
}

void Press::blowingOffOneShot()
{
	if (warning != SystemWarning::NO_WARNING || state == PressState::PRESS_ERROR)
	{
		clearErrors();
	}
	else
	{
		blowingOffMotor->oneShot();
	}
}

void Press::clearErrors()
{
	if (state == PressState::PRESS_ERROR)
	{
		error = SystemError::NO_ERROR;
		lubricantMotor->clearErrors();
		mainMotor->clearErrors();
		slideBlock->clearErrors();
		controlPanel->clearErrors();
		state = saveState == PressState::INIT ? PressState::INIT : PressState::PRESS_IDLE;
	}

	if (warning != SystemWarning::NO_WARNING)
	{
		lubricantMotor->clearErrors();
		warning = SystemWarning::NO_WARNING;
	}

	if (info != SystemInfo::NO_INFO)
	{
		info = SystemInfo::NO_INFO;
	}
}

void Press::controlModeOneShot()
{
	if (state == PressState::IN_PROCESS)
		return;

	if (this->mode != PressModes::MODE_MANUAL && this->mode != PressModes::MODE_ADJUSTMENT)
	{
		controlPanel->setMode(controlPanel->getMode() == ControlPanelMode::BUTTONS_CONTROL ? ControlPanelMode::PEDAL_CONTROL : ControlPanelMode::BUTTONS_CONTROL);
	}
}

ControlPanelMode Press::getControlMode()
{
	return controlPanel->getMode();
};

void Press::changeMode(PressModes mode)
{
	if (this->mode == mode)
		return;

	if (state == PressState::PRESS_IDLE || state == PressState::INIT)
	{
		this->mode = mode;
	}
	else if (state == PressState::PRESS_ERROR)
	{
		clearErrors();
		this->mode = mode;
	}
	else
	{
		return;
	}

	//В этих режимах управление от педали запрещено, поэтому при смене режима, автоматом устанавливается управление от кнопок
	if (mode == PressModes::MODE_MANUAL || mode == PressModes::MODE_ADJUSTMENT)
		controlPanel->setMode(ControlPanelMode::BUTTONS_CONTROL);

	//Для режимов "Ручной проворот" и "Наладка" отключить проверки для муфты, она может вращаться как угодно, что приведет к ненужным ошибкам
	if (mode == PressModes::MODE_MANUAL || mode == PressModes::MODE_ADJUSTMENT)
		slideBlock->checkErrorsDisable();
	else
		slideBlock->checkErrorsEnable();

	//Обязательно
	modePrepare = true;
}




