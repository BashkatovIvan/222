/*
 * СontrolPanel.h
 *
 * Класс панели управления (кнопки двурукого включения, педаль, кнопка выключения непрерывных ходов)
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: alexander
 */

#ifndef СONTROLPANEL_H_
#define СONTROLPANEL_H_

#include "Sensors.h"
#include "InterruptHandler.h"
#include "SystemErrors.h"
#include "PressInterface.h"

typedef enum _ControlPanelState
{
	CONTROL_PANEL_WAIT_FOR_EVENT,
	CONTROL_PANEL_ERROR,
	CONTROL_PANEL_START,
	CONTROL_PANEL_STOP

} ControlPanelState;

class ControlPanel : public InterruptHandler
{
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;

	Sensors sensors;
	ControlPanelMode mode = ControlPanelMode::BUTTONS_CONTROL;
	ControlPanelState state = CONTROL_PANEL_WAIT_FOR_EVENT;
	SystemError error = SystemError::NO_ERROR;
	int SB3time = 0;
	int SB4time = 0;
	int SB6_1_SB6_2time = 0;
	int fixTime = 0;
	bool needToFixFirstState = true;

	void handle() override;
	ControlPanelState postInterruptHandler(Sensors *s);
public:
	ControlPanel();
	ControlPanelState updateState(Sensors *s);
	void setMode(ControlPanelMode mode);
	ControlPanelMode getMode();
	ControlPanelState getState();
	SystemError getError();
	void clearErrors();
};

#endif /* СONTROLPANEL_H_ */
