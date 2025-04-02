/*
 * BlowingOffMotor.h
 *
 *  Created on: 4 сент. 2022 г.
 *      Author: alexander
 */

#ifndef SRC_BLOWINGOFFMOTOR_H_
#define SRC_BLOWINGOFFMOTOR_H_
#include "InterruptHandler.h"
#include "SystemErrors.h"
#include "Sensors.h"

typedef enum _BlowingOffMotorState
{
	BLOWING_OFF_MOTOR_STARTED,
	BLOWING_OFF_MOTOR_STOPPED,
	BLOWING_OFF_MOTOR_ERROR,
} BlowingOffMotorState;

class BlowingOffMotor : public InterruptHandler
{
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;

	uint32_t browingOffWorkTime;
	bool enabledInPressWorkTime = false;
	uint32_t workTime = 0;
	BlowingOffMotorState state;
	SystemError error = SystemError::NO_ERROR;
	bool oneShotFlag = false;

	void startMotor();
	void stopMotor();

	void handle() override;
	BlowingOffMotorState postInterruptHandler(Sensors *s);

public:
	BlowingOffMotor();
	BlowingOffMotorState updateState(Sensors *s);
	void oneShot();
	void clearErrors();
	void run();

	SystemError getError() {return error;}
	BlowingOffMotorState getState() {return state;}
};

#endif /* SRC_BLOWINGOFFMOTOR_H_ */
