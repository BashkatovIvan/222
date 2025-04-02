/*
 * MainMotor.h
 *
 * Класс главного привода
 *
 *  Created on: 15 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_MAINMOTOR_H_
#define SRC_MAINMOTOR_H_

#include "Sensors.h"
#include "InterruptHandler.h"
#include "SystemErrors.h"
#include <stdint.h>

typedef enum _MainMotorState
{
	WAIT_FOR_STARTED,
	WAIT_FOR_STOPPED,
	MAIN_MOTOR_IDLE,
	STARTED,
	STOPED,
	MAIN_MOTOR_ERROR,
} MainMotorState;

class MainMotor : public InterruptHandler
{
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;

	int KM1 = 0;
	Sensors sensors;
	SystemError error = SystemError::NO_ERROR;
	MainMotorState state = MainMotorState::MAIN_MOTOR_IDLE;

	bool getCtlState();
	void resetState();
	void switchOn();
	void switchOff();
	bool isAllowedUpdateState();

	void handle() override;
	MainMotorState postInterruptHandler();
public:
	MainMotor();
	void run();
	void stop();

	MainMotorState updateState(Sensors *s);
	void updateParamsFromDb();
	void clearErrors();
	SystemError getError();
	MainMotorState getState();
	int getStartedTime();
	int getStopedTime();
};

#endif /* SRC_MAINMOTOR_H_ */
