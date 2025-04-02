/*
 * PressIdleHandler.cpp
 *
 *  Created on: Aug 22, 2022
 *      Author: amaslov
 */
#include "Press.h"
#include "Globals.h"

PressState Press::idleHandlerForModeOneStep(Sensors *s)
{
	//Контроль превышения тормозного пути
	if (checkCondition(slideBlock->mainSensorGetPosition() == MainSensorPosition::MAIN_SENSOR_STOP_CONTROL, SystemError::N17_EXCEEDING_BRAKING_DISTANCE, state))
		return state;

	//Ожидание запуска цикла
	if (controlPanel->getState() != ControlPanelState::CONTROL_PANEL_START)
		return state;

	//Если до этого кнопки не были отжаты
	if (getControlMode() == ControlPanelMode::BUTTONS_CONTROL)
	{
		if (checkCondition(!stopFlag, SystemError::N60_CONTROL_BUTTONS_MUST_BE_DISABLED, state))
			return state;

		stopFlag = false;
	}

	//Если до этого педаль не была отжата
	if (getControlMode() == ControlPanelMode::PEDAL_CONTROL)
	{
		if (checkCondition(!stopFlag, SystemError::N53_CONTROL_PEDAL_MUST_BE_DISABLE, state))
			return state;

		stopFlag = false;
	}

	//Двигатель должен быть включен
	if (checkCondition(mainMotor->getStartedTime() < 10, SystemError::N79_KM1_IS_OFF, state))
		return state;

#ifndef DEBUG_VERSION
	//Датчик давления воздуха
	if (checkCondition(s->SP1 == false, SystemError::N08_SP1_MUST_BE_1, state))
		return state;

	//Датчик защитного экрана
	if (dataBase->getProtectiveShieldEnabled())
	{
		if (!checkProtectiveShieldState(s, state, SystemError::N51_PROTECTIVE_SHIELD_IS_OFF))
			return state;
	}

	//Датчик блокировок
	if (!checkBlockingState(s, state))
		return state;
#endif

	//Ползун должен быть в исходном положении. Если состояние положения еще не обновилось, не выдавать ошибку, просто выйти, дождаться, когда состояние положения обновится
	if (slideBlock->handSensorGetPosition() == HandSensorPosition::HAND_SENSOR_UNKNOWN_POSITION)
		return state;
	if (checkCondition(slideBlock->handSensorGetPosition() != HandSensorPosition::HAND_SENSOR_ON, SystemError::N59_SLIDE_BLOCK_IS_NOT_STARTED_POSITION, state))
		return state;

	slideBlock->run();
	state = PressState::IN_PROCESS;

	return state;
}

PressState Press::idleHandlerForModeContinuous(Sensors *s)
{
	return idleHandlerForModeOneStep(s);
}

PressState Press::idleHandlerForModeAdjustment(Sensors *s)
{
	//В данном режиме пресс управляется только от кнопок
	if (getControlMode() != ControlPanelMode::BUTTONS_CONTROL)
		return state;

	//Ожидание запуска цикла
	if (controlPanel->getState() != ControlPanelState::CONTROL_PANEL_START)
		return state;

	//Если до этого кнопки не были отжаты
	if (checkCondition(!stopFlag, SystemError::N60_CONTROL_BUTTONS_MUST_BE_DISABLED, state))
		return state;

	stopFlag = false;

	//Двигатель должен быть включен
	if (checkCondition(mainMotor->getStartedTime() < 10, SystemError::N79_KM1_IS_OFF, state))
		return state;

#ifndef DEBUG_VERSION
	//Датчик давления воздуха
	if (checkCondition(s->SP1 == false, SystemError::N08_SP1_MUST_BE_1, state))
		return state;

	//Датчик блокировок
	if (!checkBlockingState(s, state))
		return state;
#endif

	adjustmentModeTime = 0;
	slideBlock->run();
	state = PressState::IN_PROCESS;

	return state;
}

PressState Press::idleHandlerForModeManual(Sensors *s)
{
	//В данном режиме пресс управляется только от кнопок
	if (getControlMode() != ControlPanelMode::BUTTONS_CONTROL)
		return state;

	//Ожидание запуска цикла
	if (controlPanel->getState() != ControlPanelState::CONTROL_PANEL_START)
		return state;

	//Не выдавать ошибку, просто дождаться, когда кнопки будут отжаты
	if (!stopFlag)
		return state;

	stopFlag = false;

	//Двигатель должен быть выключен
	if (checkCondition(mainMotor->getStopedTime() < 10, SystemError::N80_KM1_IS_ON, state))
		return state;

	slideBlock->run();
	state = PressState::IN_PROCESS;

	return state;
}

PressState Press::idleModePrepareHandler(Sensors *s)
{
	if (mode != PressModes::MODE_MANUAL)
	{
		//пресс готов к работе только из исходного положения (ползун в Верхней Мертвой Точке).
		if (mode == PressModes::MODE_ONE_STEP || mode == PressModes::MODE_CONTINUOUS)
		{
			//Ползун должен быть в исходном положении. Если состояние положения еще не обновилось, не выдавать ошибку, просто выйти и дождаться, когда состояние положения обновится
			if (slideBlock->handSensorGetPosition() == HandSensorPosition::HAND_SENSOR_UNKNOWN_POSITION)
				return state;
			if (checkCondition(slideBlock->handSensorGetPosition() != HandSensorPosition::HAND_SENSOR_ON, SystemError::N59_SLIDE_BLOCK_IS_NOT_STARTED_POSITION, state))
				return state;
		}

		mainMotor->run();
		modePrepare = false;
	}
	else
	{
		if (mainMotor->getState() != MainMotorState::STOPED)
		{
			mainMotor->stop();
		}
		else
		{
			if (mainMotor->getStopedTime() >= 1000 * 120) //two minutes
				modePrepare = false;
			else
				info = SystemInfo::N36_MODE_MANUAL_STOP_MAIN_MOTOR;
		}
	}
	return state;
}

PressState Press::idleHandler(Sensors *s)
{
	slideBlock->stop();
	waitStop = false;

	if (state == PressState::PRESS_ERROR)
		return state;

	//Флаг остановки обновляется всегда
	if (controlPanel->getState() == ControlPanelState::CONTROL_PANEL_STOP)
		stopFlag = true;

	//Текущая позиция в простое обновляется всегда
	slideBlockCurPosition = slideBlock->mainSensorGetPosition();

	//Eсли пора включить смазку, запустить двигатель смазки
	if (lubricantMotor->getState() == LubricantMotorState::LUBRICANT_READY)
		lubricantMotor->run();

	if (modePrepare)
		return idleModePrepareHandler(s);
	else
		info = mode == PressModes::MODE_ADJUSTMENT ? SystemInfo::N37_MODE_ADJUSTMENT :
				mode == PressModes::MODE_ONE_STEP ? SystemInfo::N38_MODE_ONE_STEP :
				mode == PressModes::MODE_CONTINUOUS	? SystemInfo::N39_MODE_CONTINUOUS :
						SystemInfo::N40_MODE_MANUAL;

#ifndef DEBUG_VERSION
	//Датчик перегрузки пресса (во всех режимах)
	if (!checkPressOverloadState(s, state))
		return state;
#endif

	if (mode < PRESS_MODES_SIZE)
		state = (this->*idleHandlers[mode])(s);

	return state;
}
