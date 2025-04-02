/*
 * MenuWorkPage.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_MENUWORKPAGE_H_
#define SRC_MENUWORKPAGE_H_

#include "PressInterface.h"
#include "MenuPage.h"
#include "HMI.h"

class MenuWorkPage : public MenuPage
{
	PressInterface *press;
	HMI *hmi;
	void handle(int curPage) override;
	void handleMainPage(bool switchPage = false);
	void handleDiagnosticPage(bool switchPage = false);
	void fillCounterValue(DisplayData &d, uint32_t value);
public:
	MenuWorkPage(PressInterface *press, HMI *hmi) : press(press), hmi(hmi){};
};

#endif /* SRC_MENUWORKPAGE_H_ */
