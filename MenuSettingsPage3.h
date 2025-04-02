/*
 * MenuSettingsPage3.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_MENUSETTINGSPAGE3_H_
#define SRC_MENUSETTINGSPAGE3_H_

#include "PressInterface.h"
#include "MenuPage.h"
#include "Globals.h"

class MenuSettingsPage3 : public MenuPage
{
	PressInterface *press;
	HMI *hmi;
	int curPage = 255;
	void handle(int curPage) override;
	DataBaseData dataBaseData;
	DataBaseData clearData;

	uint8_t curItem = 0;
	bool editFlag = false;
	static const uint8_t MAX_INDEX = 3;
	DisplayData d;
public:
	MenuSettingsPage3(PressInterface *press, HMI *hmi) : press(press), hmi(hmi){};
};

#endif /* SRC_MENUSETTINGSPAGE3_H_ */
