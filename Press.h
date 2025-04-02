/*
 * press.h
 *
 *	Класс пресса
 *
 *  Created on: Aug 1, 2022
 *      Author: amaslov
 */

#ifndef SRC_PRESS_H_
#define SRC_PRESS_H_


#include "MCU.h"
#include "HMI.h"
#include "WatchdogHW.h"
#include "Menu.h"
#include "PressInterface.h"
#include "SlideBlock.h"
#include "ProtectiveShield.h"
#include "ControlPanel.h"
#include "LubricantMotor.h"
#include "DataBase.h"
#include "InterruptHandler.h"
#include "BlowingOffMotor.h"

#define PRESS_FAST_INTERRUPT_PRESCALER 1
#define PRESS_SLOW_INTERRUPT_PRESCALER 5

class Press : public PressInterface, public InterruptHandler
{
	MCU *mcu;
	WatchdogHW *watchdog;
	Sensors sensors;
	MainMotor *mainMotor;
	LubricantMotor *lubricantMotor;
	SlideBlock *slideBlock;
	ProtectiveShield *protectiveShield;
	ControlPanel *controlPanel;
	BlowingOffMotor *blowingOffMotor;

	Menu *menu;

	bool stopFlag = false;
	uint32_t counterValue = 0;
	bool counterEnabled = false;

	PressModes mode = PressModes::MODE_ONE_STEP;
	bool modePrepare = true;
	PressState state = PressState::INIT;
	PressState saveState = PressState::INIT;
	bool waitStop = false;
	uint32_t adjustmentModeTime = 0;

	SystemError error = SystemError::NO_ERROR;
	SystemInfo info = SystemInfo::NO_INFO;
	SystemWarning warning = SystemWarning::NO_WARNING;

	/*PressStatesHandler.cpp*/
	PressState updateState(Sensors *s);
	SystemError updateAll();

	/*PressErrorHandler.cpp*/
	PressState errorHandler(Sensors *s);

	/*PressInitHandler.cpp*/
	PressState initHandler(Sensors *s);

	/*PressIdleHandler.cpp*/
	PressState idleHandler(Sensors *s);
	PressState idleModePrepareHandler(Sensors *s);
	PressState idleHandlerForModeOneStep(Sensors *s);
	PressState idleHandlerForModeContinuous(Sensors *s);
	PressState idleHandlerForModeAdjustment(Sensors *s);
	PressState idleHandlerForModeManual(Sensors *s);
	PressState (Press::*idleHandlers[PRESS_MODES_SIZE])(Sensors *s);

	/*PressIdleHandler.cpp*/
	PressState inProcessHandler(Sensors *s);
	PressState inProcessForModeOneStep(Sensors *s);
	PressState inProcessForModeContinuous(Sensors *s);
	PressState inProcessForModeAdjustment(Sensors *s);
	PressState inProcessForModeManual(Sensors *s);
	PressState (Press::*inProcessHandlers[PRESS_MODES_SIZE])(Sensors *s);

	PressState (Press::*statesHandlers[PRESS_STATES_SIZE])(Sensors *s);

	/*PressInterruptHandler.cpp*/
	volatile bool fastInterruptFlag = false;
	volatile int8_t fastInterruptPrescaler = 0;
	volatile bool slowInterruptFlag = false;
	volatile int8_t slowInterruptPrescaler = 0;
	void handle() override;
	PressState postInterruptHandler(Sensors *s);

	MainSensorPosition slideBlockCurPosition = MainSensorPosition::MAIN_SENSOR_ON;

	/*PressUtils.cpp*/
	bool checkProtectiveShieldState(Sensors *s, PressState saveState, SystemError err);
	bool checkBlockingState(Sensors *s, PressState saveState);
	bool checkPressOverloadState(Sensors *s, PressState saveState);
	bool checkCondition(bool condition, SystemError errorIfconditionTrue, PressState saveStateIfConditionTrue);
	void checkTrialLimtis();

	/*PressInterface.cpp*/
	Sensors getSensors() override;
	SystemError getError() override;
	SystemWarning getWarning() override;
	SystemInfo getInfo() override;
	PressState getState() override;
	PressModes getMode() override;
	bool mainMotorIsStarted() override;
	bool lubricantIsStarted() override;
	bool blowingOffIsStarted() override;
	ControlPanelMode getControlMode() override;
	void lubricantOneShot() override;
	void blowingOffOneShot() override;
	void counterOneShot(bool clearValue) override;
	bool counterIsEnable() override;
	uint32_t getCounterValue() override;
	void incrementCounter() override;
	void changeMode(PressModes mode) override;
	void clearErrors() override;
	void controlModeOneShot() override;
	void setWarning(SystemWarning warning) override {this->warning = warning;};
	MCU* getMCU() override {return mcu;};

public:
	Press();
	~Press() override {};
	void exec();
};

#endif /* SRC_PRESS_H_ */
