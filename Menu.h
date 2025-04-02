/*
 * Menu.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include "HMI.h"
#include "MenuPage.h"
#include "MenuLogoPage.h"
#include "MenuWorkPage.h"
#include "MenuSettingsPage1.h"
#include "MenuSettingsPage2.h"
#include "MenuSettingsPage3.h"

class Menu
{
	HMI *hmi;
	std::list<MenuPage*> pages;
	bool firstTime = true;
public:
	Menu(PressInterface *press, HMI *hmi);
	void handle();
};

#endif /* SRC_MENU_H_ */
