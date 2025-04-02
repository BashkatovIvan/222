/*
 * Menu.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#include "Menu.h"

Menu::Menu(PressInterface *press, HMI *hmi) : hmi(hmi)
{
	pages.push_back(new MenuLogoPage(press, hmi));
	pages.push_back(new MenuWorkPage(press, hmi));
	pages.push_back(new MenuSettingsPage1(press, hmi));
	pages.push_back(new MenuSettingsPage2(press, hmi));
	pages.push_back(new MenuSettingsPage3(press, hmi));
}

void Menu::handle()
{
	int curPage = hmi->display->getCurrentPage();

	for (MenuPage* page : pages)
    	page->handle(curPage);
}
