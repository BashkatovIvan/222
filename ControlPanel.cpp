/*
 * СontrolPanel.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: alexander
 */

#include "ControlPanel.h"
#include <limits.h>

#define CONTROL_PANEL_INTERRUPT_PRESCALER 5
#define CONTROL_PANEL_MAX_TIME (1200 / CONTROL_PANEL_INTERRUPT_PRESCALER)
#define CONTROL_PANEL_FIX_TIME (140 / CONTROL_PANEL_INTERRUPT_PRESCALER)

ControlPanel::ControlPanel()
{

}

void ControlPanel::setMode(ControlPanelMode mode)
{
	this->mode = mode;
	clearErrors();
}

ControlPanelMode ControlPanel::getMode() {
	return mode;
}

ControlPanelState ControlPanel::updateState(Sensors *s)
{
	state = postInterruptHandler(s);
	sensors = *s;
	return state;
}

ControlPanelState ControlPanel::getState() {
	return state;
}

SystemError ControlPanel::getError() {
	return error;
}

void ControlPanel::clearErrors()
{
	error = SystemError::NO_ERROR;
	state = CONTROL_PANEL_WAIT_FOR_EVENT;
	needToFixFirstState = true;
}

void ControlPanel::handle()
{
	interruptPrescaler = interruptPrescaler >= (CONTROL_PANEL_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (CONTROL_PANEL_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

ControlPanelState ControlPanel::postInterruptHandler(Sensors *s)
{
	if (!interruptFlag)
		return state;

	interruptFlag = false;

	if (state == CONTROL_PANEL_ERROR)
		return state;

	if (needToFixFirstState)
	{
		sensors = *s;
		SB3time = SB4time = SB6_1_SB6_2time = 0;
		fixTime = CONTROL_PANEL_FIX_TIME * 2;

		needToFixFirstState = false;
	}

	if (mode == ControlPanelMode::BUTTONS_CONTROL)
	{
		if (s->SB3_1 == sensors.SB3_1 && s->SB4_1 == sensors.SB4_1 && s->SB3_2_4_2 == sensors.SB3_2_4_2)
		{
			fixTime = fixTime < INT_MAX ? fixTime + 1 : INT_MAX;

#ifndef DEBUG_VERSION
//			if (((s->SB3_1 && s->SB4_1 && s->SB3_2_4_2) || (!s->SB3_1 && !s->SB4_1 && !s->SB3_2_4_2)) && fixTime >= (CONTROL_PANEL_FIX_TIME * 3))
//			{
//				error = N01_TWO_BUTTONS_CONTROL_HAS_ONLY_ONE_BUTTON;
//				return state = CONTROL_PANEL_ERROR;
//			}
#endif

			if (s->SB3_1 && s->SB4_1)
			{
				if (SB3time > CONTROL_PANEL_MAX_TIME || SB4time > CONTROL_PANEL_MAX_TIME)
				{
					error = N71_TWO_BUTTONS_CONTROL_OVER_TIME;
					return state = CONTROL_PANEL_ERROR;
				}

				SB3time = SB4time = 0;
				return state = fixTime >= CONTROL_PANEL_FIX_TIME ? CONTROL_PANEL_START : state;
			}
			else if (!s->SB3_1 && !s->SB4_1)
			{
				SB3time = SB4time = 0;
				return state = fixTime >= (CONTROL_PANEL_FIX_TIME / 10) ? CONTROL_PANEL_STOP : state;
			}
			else
			{
				SB3time = s->SB3_1 ? (SB3time < INT_MAX ? SB3time + 1 : INT_MAX) : 0;
				SB4time = s->SB4_1 ? (SB4time < INT_MAX ? SB4time + 1 : INT_MAX) : 0;

				return state = fixTime >= (CONTROL_PANEL_FIX_TIME / 10) ? CONTROL_PANEL_STOP : state;
			}
		}
		else
		{
			fixTime = 0;
		}
	}
	else if (mode == ControlPanelMode::PEDAL_CONTROL)
	{
		if (s->SB6_1 == sensors.SB6_1 && s->SB6_2 == sensors.SB6_2)
		{
			fixTime = fixTime < INT_MAX ? fixTime + 1 : INT_MAX;

			if (s->SB6_1 != s->SB6_2)
			{
				SB6_1_SB6_2time = 0;

				if (s->SB6_1 && !s->SB6_2)
				{
					return state = fixTime >= CONTROL_PANEL_FIX_TIME ? CONTROL_PANEL_START : state;
				}
				else
				{
					return state = fixTime >= CONTROL_PANEL_FIX_TIME / 10 ? CONTROL_PANEL_STOP : state;
				}
			}
			else
			{
				SB6_1_SB6_2time = SB6_1_SB6_2time < INT_MAX ? SB6_1_SB6_2time + 1 : INT_MAX;

				if (SB6_1_SB6_2time > CONTROL_PANEL_MAX_TIME)
				{
					error = N68_PEDAL_CONTROL_OVER_TIME;
					return state = CONTROL_PANEL_ERROR;
				}
			}
		}
		else
		{
			fixTime = 0;
		}
	}

	return state;
}
