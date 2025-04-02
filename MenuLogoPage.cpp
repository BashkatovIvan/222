/*
 * MenuLogoPage.cpp
 *
 *  Created on: 9 сент. 2022 г.
 *      Author: amaslov
 */

#include <limits.h>
#include "MenuLogoPage.h"
#define MENU_LOGO_PAGE_INTERRUPT_PRESCALER 5

/*From MenuPage*/
void MenuLogoPage::handle(int curPage)
{
	if (curPage != Pages::LOGO_PAGE)
		return;

	postInterruptHandler();

	Button key = hmi->keyboard->getKey();
	if (key.button == KEY_SWITCH_MENU || ((time * MENU_LOGO_PAGE_INTERRUPT_PRESCALER) >= 5000)) //5 sec
	{
		hmi->display->updateData({.curPage = (uint8_t)(Pages::MAIN_PAGE)});
	}
}

/*From InterruptHandler*/
void MenuLogoPage::handle()
{
	interruptPrescaler = interruptPrescaler >= (MENU_LOGO_PAGE_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (MENU_LOGO_PAGE_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

void MenuLogoPage::postInterruptHandler()
{
	if (!interruptFlag)
		return;

	interruptFlag = false;

	time = time < UINT32_MAX ? time + 1 : time;
}
