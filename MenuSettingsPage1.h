/*
 * MenuSettingsPage1.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_MENUSETTINGSPAGE1_H_
#define SRC_MENUSETTINGSPAGE1_H_

#include "PressInterface.h"
#include "MenuPage.h"
#include "Globals.h"

class MenuSettingsPage1 : public MenuPage
{
	PressInterface *press;
	HMI *hmi;
	int curPage = 255;
	void handle(int curPage) override;
	DataBaseData dataBaseData;
	DataBaseData clearData;

	uint8_t curItem = 0;
	bool editFlag = false;
	static const uint8_t MAX_INDEX = 5;
	DisplayData d;
public:
	MenuSettingsPage1(PressInterface *press, HMI *hmi) : press(press), hmi(hmi){};
};

#endif /* SRC_MENUSETTINGSPAGE1_H_ */
