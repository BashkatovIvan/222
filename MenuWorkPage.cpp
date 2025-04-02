/*
 * MenuWorkPage.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#include "MenuWorkPage.h"
#include "Globals.h"

bool useBlockingKeyboard = true;

void MenuWorkPage::fillCounterValue(DisplayData &d, uint32_t value)
{
	uint32_t remainder = value;
	uint32_t devider = 1000000;
	uint8_t *digits[7] = {&d.digit7, &d.digit6, &d.digit5, &d.digit4, &d.digit3, &d.digit2, &d.digit1 };

	//Для экономи вычислительных ресурсов
	for (uint i = 0; i < 7; i++)
	{
		if (remainder >= devider)
		{
			*digits[i] = remainder / devider;
		}
		else
		{
			*digits[i] = 0;
		}
		remainder = remainder % devider;
		devider /= 10;
	}
}

void MenuWorkPage::handleMainPage(bool switchPage)
{
	DisplayData d;
	Sensors s = press->getSensors();
	SystemError error = press->getError();
	SystemInfo info = press->getInfo();
	SystemWarning warning = press->getWarning();

	d.mode = (uint8_t)(press->getMode());
	d.error = (error != SystemError::NO_ERROR ? (uint8_t)error : warning != SystemWarning::NO_WARNING ? (uint8_t)warning : (uint8_t)info);
	d.lubricant = (uint8_t)(s.SP2 ? 1 : 0);
	d.mainMotor = (uint8_t)(s.KM1_1 ? 1 : 0);
	d.startPosition = (uint8_t)(!s.SQ3 ? 1 : 0);
	d.counter = (uint8_t)press->counterIsEnable();
	d.controlMode = (uint8_t)press->getControlMode();
	d.blowingOffMotor = (uint8_t)press->blowingOffIsStarted();

	fillCounterValue(d, press->getCounterValue());

	if(switchPage)
		d.curPage = (uint8_t)(Pages::MAIN_PAGE);

	hmi->display->updateData(d);
}

void MenuWorkPage::handleDiagnosticPage(bool switchPage)
{
	DisplayData d;
	Sensors s = press->getSensors();
	SystemError error = press->getError();
	SystemInfo info = press->getInfo();
	SystemWarning warning = press->getWarning();

	d.SQ1 = (uint8_t)(s.SQ1 ? 1 : 0);
	d.SQ2 = (uint8_t)(s.SQ2 ? 1 : 0);
	d.SQ3 = (uint8_t)(s.SQ3 ? 1 : 0);
	d.SQ11_SQ12 = (uint8_t)(s.SQ11_SQ12 ? 1 : 0);
	d.SQ13 = (uint8_t)(s.SQ13 ? 1 : 0);
	d.SQ14 = (uint8_t)(s.SQ14 ? 1 : 0);
	d.SQ15 = (uint8_t)(s.SQ15 ? 1 : 0);
	d.SQ16 = (uint8_t)(s.SQ16 ? 1 : 0);
	d.SB3_1 = (uint8_t)(s.SB3_1 ? 1 : 0);
	d.SB3_2_4_2 = (uint8_t)(s.SB3_2_4_2 ? 1 : 0);
	d.SB4_1 = (uint8_t)(s.SB4_1 ? 1 : 0);
	d.SB5 = (uint8_t)(s.SB5 ? 1 : 0);
	d.SB6_1 = (uint8_t)(s.SB6_1 ? 1 : 0);
	d.SB6_2 = (uint8_t)(s.SB6_2 ? 1 : 0);
	d.SL = (uint8_t)(s.SL ? 1 : 0);
	d.SA = (uint8_t)(s.SA ? 1 : 0);
	d.SP1 = (uint8_t)(s.SP1 ? 1 : 0);
	d.SP2 = (uint8_t)(s.SP2 ? 1 : 0);
	d.KM1_1 = (uint8_t)(s.KM1_1 ? 1 : 0);
	d.YV1 = (uint8_t)(s.YV1 ? 1 : 0);
	d.YV2 = (uint8_t)(s.YV2 ? 1 : 0);
	d.error = (error != SystemError::NO_ERROR ? (uint8_t)error : warning != SystemWarning::NO_WARNING ? (uint8_t)warning : (uint8_t)info);

	if(switchPage)
		d.curPage = (uint8_t)(Pages::DIAGNOSTIC_PAGE);

	hmi->display->updateData(d);
}

void MenuWorkPage::handle(int curPage)
{
	if (!(curPage == Pages::MAIN_PAGE || curPage == Pages::DIAGNOSTIC_PAGE))
		return;

	Sensors s = press->getSensors();

	Button key = hmi->keyboard->getKey();
	switch (curPage)
	{
		case Pages::MAIN_PAGE:
			handleMainPage();
			break;
		case Pages::DIAGNOSTIC_PAGE:
			handleDiagnosticPage();
			break;
		default:
			break;
	}

	switch(key.button)
	{
		case KEY_MODE_ADJUSTMENT:
			if (useBlockingKeyboard && !s.SA)
				press->setWarning(SystemWarning::N78_KEYBOARD_IS_LOCKED);
			else
				press->changeMode(PressModes::MODE_ADJUSTMENT);
			break;
		case KEY_MODE_ONE_STEP:
			if (useBlockingKeyboard && !s.SA)
				press->setWarning(SystemWarning::N78_KEYBOARD_IS_LOCKED);
			else
				press->changeMode(PressModes::MODE_ONE_STEP);
			break;
		case KEY_MODE_CONTINUOUS:
			if (useBlockingKeyboard && !s.SA)
				press->setWarning(SystemWarning::N78_KEYBOARD_IS_LOCKED);
			else
				press->changeMode(PressModes::MODE_CONTINUOUS);
			break;
		case KEY_MODE_MANUAL:
			if (useBlockingKeyboard && !s.SA)
				press->setWarning(SystemWarning::N78_KEYBOARD_IS_LOCKED);
			else
				press->changeMode(PressModes::MODE_MANUAL);
			break;
		case KEY_REMOTE_CONTROL:
			if (useBlockingKeyboard && !s.SA)
				press->setWarning(SystemWarning::N78_KEYBOARD_IS_LOCKED);
			else
				press->controlModeOneShot();
			break;
		case KEY_CANCEL_LUBRICANT:
			press->lubricantOneShot();
			break;
		case KEY_BLOWING_OFF:
			press->blowingOffOneShot();
			break;
		case KEY_SWITCH_MENU:
			if (key.longPress)
			{
				if (useBlockingKeyboard && !s.SA)
					press->setWarning(SystemWarning::N78_KEYBOARD_IS_LOCKED);
				else if (press->getState() != PressState::IN_PROCESS)
					hmi->display->updateData({.curPage = (uint8_t)(Pages::SETTINGS_PAGE1)});

			}
			else
			{
				curPage = hmi->display->getCurrentPage();
				if (curPage == Pages::MAIN_PAGE)
				{
					handleDiagnosticPage(true);
				}
				else if (curPage == Pages::DIAGNOSTIC_PAGE)
				{
					handleMainPage(true);
				}
			}
			break;
		case KEY_ZERO_VALUES:
			if (key.longPress)
			{
				press->counterOneShot(true);
			}
			else
			{
				press->counterOneShot(false);
			}
			break;
		default:
			break;
	}
}

