/*
 * MenuLogoPage.h
 *
 *  Created on: 9 сент. 2022 г.
 *      Author: amaslov
 */

#ifndef MENULOGOPAGE_H_
#define MENULOGOPAGE_H_

#include "InterruptHandler.h"
#include "PressInterface.h"
#include "MenuPage.h"
#include "HMI.h"

class MenuLogoPage : public MenuPage, public InterruptHandler
{
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;

	PressInterface *press;
	HMI *hmi;
	uint32_t time = 0;

	/*From MenuPage*/
	void handle(int curPage) override;

	/*From InterruptHandler*/
	void handle() override;

	void postInterruptHandler();
public:
	MenuLogoPage(PressInterface *press, HMI *hmi) : press(press), hmi(hmi)
	{
		press->getMCU()->tim21AddCallback(this);
	};
};

#endif /* MENULOGOPAGE_H_ */
