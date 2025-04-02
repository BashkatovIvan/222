/*
 * press.cpp
 *
 *  Created on: Aug 1, 2022
 *      Author: amaslov
 */

#include "Press.h"
#include "string.h"
#include <stdio.h>
#include "Tests.h"
#include "RTClock.h"
#include <limits.h>

RTClock *rtClock;
DataBase *dataBase;
HMI hmi;

Press::Press()
{
	mcu = new MCU();
	dataBase = new DataBase();
	dataBase->updateData();

//	/*Версионность софта. Если устанавливается новая версия, то настройки сбрасываются по умолчанию, а так же сбрасывается счетчик триальной версии*/
//	if (dataBase->getProgrammVersion() != PROGRAM_VERSION)
//	{
//		dataBase->setDefaults();
//		dataBase->setProgrammVersion(PROGRAM_VERSION);
//		dataBase->saveData();
//	}

	hmi.display = Display::getInstance();
	hmi.keyboard = Keyboard::getInstance();
	watchdog = new WatchdogHW();
	mainMotor = new MainMotor();
	lubricantMotor = new LubricantMotor(this);
	slideBlock = new SlideBlock();
	protectiveShield = new ProtectiveShield();
	controlPanel = new ControlPanel();
	blowingOffMotor = new BlowingOffMotor();

	mcu->tim21AddCallback(this);
	mcu->tim21AddCallback(controlPanel);
	mcu->tim21AddCallback(slideBlock);
	mcu->tim21AddCallback(lubricantMotor);
	mcu->tim21AddCallback(hmi.display);
	mcu->tim21AddCallback(hmi.keyboard);
	mcu->tim21AddCallback(mainMotor);
	mcu->tim21AddCallback(blowingOffMotor);

	menu = new Menu(this, &hmi);

	idleHandlers[MODE_MANUAL] = &Press::idleHandlerForModeManual;
	idleHandlers[MODE_ADJUSTMENT] = &Press::idleHandlerForModeAdjustment;
	idleHandlers[MODE_ONE_STEP] = &Press::idleHandlerForModeOneStep;
	idleHandlers[MODE_CONTINUOUS] = &Press::idleHandlerForModeContinuous;

	inProcessHandlers[MODE_MANUAL] = &Press::inProcessForModeManual;
	inProcessHandlers[MODE_ADJUSTMENT] = &Press::inProcessForModeAdjustment;
	inProcessHandlers[MODE_ONE_STEP] = &Press::inProcessForModeOneStep;
	inProcessHandlers[MODE_CONTINUOUS] = &Press::inProcessForModeContinuous;

	statesHandlers[PRESS_IDLE] = &Press::idleHandler;
	statesHandlers[PRESS_ERROR] = &Press::errorHandler;
	statesHandlers[INIT] = &Press::initHandler;
	statesHandlers[IN_PROCESS] = &Press::inProcessHandler;

	rtClock = new RTClock(mcu);

	mcu->tim21Start();
}

void Press::exec()
{
	mcu->IWDGRun();

	if (mcu->isIWDGRestart())
	{
		while(true)
		{
			DisplayData d;
			d.error = (uint8_t)(N20_STM32_FREEZE);
			d.curPage = Pages::MAIN_PAGE;

			mcu->IWDGRefresh();
			watchdog->blockOutputCircuits();
			hmi.display->updateData(d);
			hmi.display->updateState();
		}
	}

	watchdog->unblockOutputCircuits();

	HAL_GPIO_WritePin(TEST_PIN2_GPIO_Port, TEST_PIN2_Pin, GPIO_PIN_SET);

	while (true)
	{
		HAL_GPIO_TogglePin(TEST_PIN2_GPIO_Port, TEST_PIN2_Pin);
		HAL_GPIO_WritePin(TEST_PIN2_GPIO_Port, TEST_PIN2_Pin, GPIO_PIN_SET);

		mcu->IWDGRefresh();

		updateAll();

		hmi.display->updateState();

		menu->handle();
	}
}

int main(void)
{
	Press p;
	p.exec();
	return 0;
}

