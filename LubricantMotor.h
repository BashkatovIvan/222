/*
 * LubricantMotor.h
 *
 *	Класс управления двигателем системы смазки
 *
 *  Created on: 11 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_LUBRICANTMOTOR_H_
#define SRC_LUBRICANTMOTOR_H_

#include "PressInterface.h"
#include "DataBase.h"
#include "Sensors.h"
#include "InterruptHandler.h"
#include "SystemErrors.h"
#include <stdint.h>
#include <time.h>

enum LubricantMotorState
{
	IN_PROGRESS,
	LUBRICANT_READY,
	IDLE,
	LUBRICANT_ERROR,
	LUBRICANT_WARNING,
	LUBRICANT_DISABLED
};

enum LubricantMode
{
	NORMAL,
	PUMPING,
};

typedef struct _PumpingModeParams
{
	uint16_t lubricantTime;
	uint16_t time = 0;

	uint8_t lubricantCount;
	uint8_t count = 0;
} PumpingModeParams;

typedef struct _NormalModeParams
{
	uint16_t lubricantOnTime;
	uint16_t onTime = 0;

	/* By time */
	struct tm lubricantOffTime;
	struct tm offTime;

	/* By counter */
	uint16_t lubricantOffCount;
	uint16_t offCount;

	uint8_t type;

} NormalModeParams;


class LubricantMotor : public InterruptHandler
{
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;

	PressInterface *press;

	Sensors sensors;
	SystemError error = SystemError::NO_ERROR;
	SystemWarning warning = SystemWarning::NO_WARNING;

	LubricantMode mode;
	NormalModeParams normalModeParams;
	PumpingModeParams pumpingModeParams;

	LubricantMotorState state;
	bool oneShotFlag = false;

	void handle() override;
	LubricantMotorState postInterruptHandler(Sensors *s);
	LubricantMotorState handleInPumpingMode(Sensors *s);
	LubricantMotorState handleInNormalMode(Sensors *s);
	LubricantMotorState handleByTimeMode(NormalModeParams *p, Sensors *s);
	LubricantMotorState handleByCounterMode(NormalModeParams *p, Sensors *s);

	void startMotor();
	void stopMotor();
	LubricantMotorState setIDLEState();
	LubricantMotorState setReadyState();

public:
	LubricantMotor(PressInterface *press);
	void updateParamsFromDb();
	LubricantMotorState updateState(Sensors *s);
	LubricantMotorState getState();
	LubricantMode getCurMode();
	void oneShot();
	void incrementCounter();
	bool run();

	void clearErrors();
	SystemError getError();
	SystemWarning getWarning();

};

#endif /* SRC_LUBRICANTMOTOR_H_ */
