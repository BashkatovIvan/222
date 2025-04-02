/*
 * Clutch.h
 *
 *	Класс для управления Муфтой-тормозом
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: alexander
 *
 */

#ifndef SRC_CLUTCH_H_
#define SRC_CLUTCH_H_

#include "Sensors.h"
#include "SystemErrors.h"
#include <string.h>

typedef enum _ClutchStates
{
	CLUTCH_ERROR,
	CLUTCH_ENABLED,
	CLUTCH_DISABLED,
} ClutchStates;

class Clutch : public InterruptHandler
{
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;

	Sensors sensors;
	ClutchStates state;
	SystemError error = SystemError::NO_ERROR;
	bool enabled = false;
	int stateTime = 0;
	void handle() override;
	ClutchStates postInterruptHandler(Sensors *s);
public:
	Clutch();
	ClutchStates updateState(Sensors *s);
	ClutchStates getState();
	SystemError getError();
	void clearErrors();
	void switchOff();
	void switchOn();
};

#endif /* SRC_CLUTCH_H_ */
