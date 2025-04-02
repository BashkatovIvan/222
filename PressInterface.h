/*
 * PressInterface.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_PRESSINTERFACE_H_
#define SRC_PRESSINTERFACE_H_

#include "Sensors.h"
#include "MainMotor.h"
#include "PressInterface.h"
#include "MCU.h"
#include <time.h>

typedef enum _PressModes
{
	MODE_MANUAL = 0,       //режим "Ручной проворот"
	MODE_ADJUSTMENT,       //режим "Наладка"
	MODE_ONE_STEP,         //режим "Одиночный ход"
	MODE_CONTINUOUS,       //режим "Непрерывные хода"
	PRESS_MODES_SIZE
} PressModes;

typedef enum _PressState
{
	PRESS_IDLE = 0,        //пресс находится в простое, ожидает действий оператора
	PRESS_ERROR,           //возникла ошибка в работе пресса, на которую должен обратить внимание оператор. При этом муфта отключается
	INIT,                  //начальная инициализация пресса, при которой выполняется прокачка смазки и запуск главного привода
	IN_PROCESS,            //запущен цикл
	PRESS_STATES_SIZE
} PressState;

typedef enum _ControlPanelMode
{
	BUTTONS_CONTROL,       //двурукое управление с кнопок
	PEDAL_CONTROL          //управление педалью
} ControlPanelMode;

 //Интерфейс, абстрактный класс
class PressInterface {
public:
	 virtual Sensors getSensors() = 0;
	 virtual void changeMode(PressModes mode) = 0;
	 virtual PressModes getMode() = 0;
	 virtual PressState getState() = 0;
	 virtual SystemError getError() = 0;
	 virtual SystemWarning getWarning() = 0;
	 virtual void setWarning(SystemWarning warning) = 0;
	 virtual SystemInfo getInfo() = 0;
	 virtual void clearErrors() = 0;
	 virtual void lubricantOneShot() = 0;
	 virtual void controlModeOneShot() = 0;
	 virtual ControlPanelMode getControlMode() = 0;

	 virtual bool mainMotorIsStarted() = 0;
	 virtual bool lubricantIsStarted() = 0;
	 virtual void counterOneShot(bool clearValue) = 0;
	 virtual bool counterIsEnable() = 0;
	 virtual uint32_t getCounterValue() = 0;
	 virtual void incrementCounter() = 0;
	 virtual void blowingOffOneShot() = 0;
	 virtual bool blowingOffIsStarted() = 0;
	 virtual MCU* getMCU() = 0;
	 virtual ~PressInterface() {};

};

#endif /* SRC_PRESSINTERFACE_H_ */
