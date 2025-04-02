/*
 * PressIdleHandler.cpp
 *
 *  Created on: Aug 22, 2022
 *      Author: amaslov
 */
#include "Press.h"
#include "Globals.h"

PressState Press::inProcessForModeOneStep(Sensors *s)
{
	//Сработал датчик окончания хода или ползун дошел до верхней мертвой точки (SQ3 = 0) - цикл завершен, ожидаем, когда отожмут кнопки
	if (!waitStop && ((slideBlock->mainSensorGetPosition() == MainSensorPosition::MAIN_SENSOR_ON) || (s->SQ3 != sensors.SQ3 && !s->SQ3)))
	{
		incrementCounter();
		slideBlock->stop();

		//Сдуть деталь
		if (dataBase->getBrowingOffEnabled())
			blowingOffMotor->run();

		//Дождаться, когда отожмут кнопки
		waitStop = true;

		return state;
	}

	if (stopFlag)
	{
		if (checkCondition(!waitStop, getControlMode() == ControlPanelMode::BUTTONS_CONTROL ? SystemError::N61_CONTROL_BUTTONS_MUST_BE_ENABLED : SystemError::N52_CONTROL_PEDAL_MUST_BE_ENABLED, PressState::PRESS_IDLE))
		{
			slideBlock->stop();
			return state;
		}
		else
		{
			//Дождались, когда кнопки отжаты(waitStop=true), выход
			state = PressState::PRESS_IDLE;
			return state;
		}
	}

#ifndef DEBUG_VERSION
	//Датчик давления воздуха
	if (checkCondition(s->SP1 == false, SystemError::N08_SP1_MUST_BE_1, PressState::PRESS_IDLE))
	{
		slideBlock->stop();
		return state;
	}

	//Датчик блокировок
	if (!checkBlockingState(s, PressState::PRESS_IDLE))
	{
		slideBlock->stop();
		return state;
	}

	//Датчик защитного экрана
	if (dataBase->getProtectiveShieldEnabled())
	{
		if (!checkProtectiveShieldState(s, PressState::PRESS_IDLE, SystemError::N70_PROTECTIVE_SHIELD_WAS_OFF))
		{
			slideBlock->stop();
			return state;
		}
	}
#endif

	return state;
}

PressState Press::inProcessForModeContinuous(Sensors *s)
{
	MainSensorPosition slideBlockPrevPosition = slideBlockCurPosition;
	slideBlockCurPosition = slideBlock->mainSensorGetPosition();

	//Инкрементировать значение счетчика, если ползун в верхней мертвой точке
	if (slideBlockPrevPosition != slideBlockCurPosition && slideBlockCurPosition == MainSensorPosition::MAIN_SENSOR_ON)
	{
		incrementCounter();

		//Сдуть деталь
		if (dataBase->getBrowingOffEnabled())
			blowingOffMotor->run();
	}

	//Если цикл был остановлен оператором, нужно дождаться, когда ползун окажется в верхней мертвой точки, после чего отключить муфту
	if (waitStop && slideBlockCurPosition == MainSensorPosition::MAIN_SENSOR_ON)
	{
		slideBlock->stop();
		state = PressState::PRESS_IDLE;
		return state;
	}

#ifndef DEBUG_VERSION
	//Датчик давления воздуха
	if (checkCondition(s->SP1 == false, SystemError::N08_SP1_MUST_BE_1, PressState::PRESS_IDLE))
	{
		slideBlock->stop();
		return state;
	}

	//Датчик блокировок
	if (!checkBlockingState(s, PressState::PRESS_IDLE))
	{
		slideBlock->stop();
		return state;
	}

	//Датчик защитного экрана
	if (dataBase->getProtectiveShieldEnabled())
	{
		if (!checkProtectiveShieldState(s, PressState::PRESS_IDLE, SystemError::N70_PROTECTIVE_SHIELD_WAS_OFF))
		{
			slideBlock->stop();
			return state;
		}
	}

	//Нажата кнопка "Стоп непрерывных ходов". Оператор увидит предупреждение об этом
	if (getControlMode() == ControlPanelMode::BUTTONS_CONTROL)
	{
		if (s->SB5 == sensors.SB5 && !sensors.SB5)
		{
			//warning = SystemWarning::N50_SB5_PRESSED; //Пока не отображать
			waitStop = true;
			return state;
		}
	}
#endif

	//Отпущена педаль. Оператор увидит предупреждение об этом
	else if (getControlMode() == ControlPanelMode::PEDAL_CONTROL)
	{
		if (stopFlag)
		{
			//warning = SystemWarning::N69_PEDAL_RELEASED;
			waitStop = true;
			return state;
		}
	}

	//Запуск цикла
	slideBlock->run();
	return state;
}

PressState Press::inProcessForModeAdjustment(Sensors *s)
{
	//Датчик давления воздуха
	if (checkCondition(s->SP1 == false, SystemError::N08_SP1_MUST_BE_1, PressState::PRESS_IDLE))
	{
		slideBlock->stop();
		return state;
	}

	//Датчик блокировок
	if (!checkBlockingState(s, PressState::PRESS_IDLE))
	{
		slideBlock->stop();
		return state;
	}

	//Если время цикла вышло, остановить пресс, завершить цикл. Если в БД время установлено нулевое, то ползун перемещается непрерывно и муфта отключается при отжатии кнопок, а не по времени.
	if (dataBase->getAdjustmentModeTime() > 0)
	{
		if (slowInterruptFlag)
		{
			if (adjustmentModeTime * PRESS_SLOW_INTERRUPT_PRESCALER >= dataBase->getAdjustmentModeTime())
			{
				slideBlock->stop();

				if (stopFlag)
				{
					state = PressState::PRESS_IDLE;
					return state;
				}
			}
			else
			{
				adjustmentModeTime++;
			}
		}
	}

	//Если были отжаты кнопки, остановить пресс, завершить цикл
	else if (stopFlag)
	{
		slideBlock->stop();
		state = PressState::PRESS_IDLE;
		return state;
	}

	return state;
}
PressState Press::inProcessForModeManual(Sensors *s)
{
	//Муфта отключается повторным нажатием кнопок
	if (controlPanel->getState() != ControlPanelState::CONTROL_PANEL_START)
		return state;

	//Не выдавать ошибку, просто дождаться, когда кнопки будут отжаты
	if (!stopFlag)
		return state;

	stopFlag = false;
	slideBlock->stop();
	state = PressState::PRESS_IDLE;
	return state;
}

PressState Press::inProcessHandler(Sensors *s)
{
	info = mode == PressModes::MODE_ADJUSTMENT ? SystemInfo::N37_MODE_ADJUSTMENT :
			mode == PressModes::MODE_ONE_STEP ? SystemInfo::N38_MODE_ONE_STEP :
			mode == PressModes::MODE_CONTINUOUS	? SystemInfo::N39_MODE_CONTINUOUS :
					SystemInfo::N40_MODE_MANUAL;

	if (state == PressState::PRESS_ERROR)
		return state;

	//Флаг остановки обновляется всегда
	if (controlPanel->getState() == ControlPanelState::CONTROL_PANEL_STOP)
		stopFlag = true;

	//Eсли пора включить смазку, запустить двигатель смазки
	if (lubricantMotor->getState() == LubricantMotorState::LUBRICANT_READY)
		lubricantMotor->run();

#ifndef DEBUG_VERSION
	//Датчик перегрузки пресса (во всех режимах)
	if (!checkPressOverloadState(s, PressState::PRESS_IDLE))
	{
		slideBlock->stop();
		return state;
	}
#endif

	if (mode < PRESS_MODES_SIZE)
		state = (this->*inProcessHandlers[mode])(s);

	return state;
}
