/*
 * SlideBlock.h
 *
 *	Класс для управления ползуном
 *
 *  Created on: 18 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_SLIDEBLOCK_H_
#define SRC_SLIDEBLOCK_H_

#include "Clutch.h"
#include "InterruptHandler.h"
#include "SystemErrors.h"

typedef enum _SlideBlockStates
{
	SLIDE_BLOCK_ERROR,
	SLIDE_BLOCK_STARTED,
	SLIDE_BLOCK_STOPPED,
} SlideBlockStates;

typedef enum _MainSensorPosition
{
	MAIN_SENSOR_OFF,
	MAIN_SENSOR_ON,
	MAIN_SENSOR_STOP_CONTROL,
	MAIN_SENSOR_UNKNOWN_POSITION,
} MainSensorPosition;

typedef enum _HandSensorPosition
{
	HAND_SENSOR_OFF,
	HAND_SENSOR_ON,
	HAND_SENSOR_UNKNOWN_POSITION,
} HandSensorPosition;

typedef struct _SQState
{
	uint8_t SQ1 = 0;
	uint8_t SQ2 = 0;
} SQState;

class SlideBlock : public InterruptHandler
{
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;

	SQState CHECK_ERR_N22 = {.SQ1 = 1, .SQ2 = 1};
	SQState CHECK_ERR_N23 = {.SQ1 = 0, .SQ2 = 0};
	int SQ2Timeout = 0;

	uint8_t SQ3 = false;

	SQState CHECK_ERR_N25[2] =
	{
		{.SQ1 = 0, .SQ2 = 1},
		{.SQ1 = 0, .SQ2 = 0},
	};

	SQState CHECK_ERR_N27[2] =
	{
		{.SQ1 = 1, .SQ2 = 0},
		{.SQ1 = 0, .SQ2 = 0},
	};

	SQState stateSamples[6] = {};
	uint8_t samples = 0;
	uint8_t samplesLimit;

	Clutch *clutch;
	Sensors sensors;

	bool checkErrors = true;

	SystemError error = SystemError::NO_ERROR;
	SlideBlockStates state = SlideBlockStates::SLIDE_BLOCK_STOPPED;

	MainSensorPosition mainSensorPosition = MainSensorPosition::MAIN_SENSOR_UNKNOWN_POSITION;
	HandSensorPosition handSensorPosition = HandSensorPosition::HAND_SENSOR_UNKNOWN_POSITION;

	int stateTime = 0;

	void rotateSamples(SQState *buff, int8_t size, int8_t offset);
	void handle() override;
	SlideBlockStates postInterruptHandler(Sensors *s);
public:
	SlideBlock();
	void checkErrorsEnable() {checkErrors = true;}
	void checkErrorsDisable() {checkErrors = false;}
	SystemError getError();
	void clearErrors();
	SlideBlockStates getState();
	SlideBlockStates updateState(Sensors *s);
	MainSensorPosition mainSensorGetPosition();
	HandSensorPosition handSensorGetPosition();
	void run();
	void stop();
};

#endif /* SRC_SLIDEBLOCK_H_ */
