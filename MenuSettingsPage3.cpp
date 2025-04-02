/*
 * MenuSettingsPage1.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#include "MenuSettingsPage3.h"
#include "Globals.h"
#include "string.h"
#include "Utils.h"

void MenuSettingsPage3::handle(int curPage)
{
	if (this->curPage != curPage && curPage == Pages::SETTINGS_PAGE3)
	{
		dataBaseData = dataBase->getAllFromStorage();
		if (!memcmp(&dataBaseData, &clearData, sizeof(DataBaseData)))
			dataBase->setDefaults(&dataBaseData);

		curItem = 1;
		editFlag = false;
	}
	this->curPage = curPage;

	if (curPage != Pages::SETTINGS_PAGE3)
		return;

	Button key = hmi->keyboard->getKey();
	switch(key.button)
	{
		case KEY_BLOWING_OFF:
		{
			if (!editFlag)
			{
				curItem = curItem == 0 ? curItem : curItem - 1;
			}
			else
			{
				if (curItem == 2)
				{
					dataBaseData.adjustmentModeTime = dataBaseData.adjustmentModeTime < 10 ? 1000 : dataBaseData.adjustmentModeTime - 10;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 3)
				{
					dataBaseData.protectiveShieldEnabled = dataBaseData.protectiveShieldEnabled == 0 ? 1 : 0;
				}
			}
			break;
		}
		case KEY_SWITCH_MENU:
		{
			if (editFlag)
			{
				dataBase->saveAllToStorage(dataBaseData);
				editFlag = false;
				break;
			}

			if (curItem > 1)
			{
				editFlag = true;
				break;
			}

			if (curItem == 0)
			{
				dataBase->saveAllToStorage(dataBaseData);
				hmi->display->updateData({.curPage = (uint8_t)(Pages::MAIN_PAGE)});
				break;
			}

			if (curItem == 1)
			{
				dataBase->saveAllToStorage(dataBaseData);
				hmi->display->updateData({.curPage = (uint8_t)(Pages::SETTINGS_PAGE1)});
				break;
			}
			break;
		}
		case KEY_ZERO_VALUES:
		{
			if (!editFlag)
			{
				curItem = curItem == MAX_INDEX ? 1 : curItem + 1;
			}
			else
			{
				if (curItem == 2)
				{
					dataBaseData.adjustmentModeTime = dataBaseData.adjustmentModeTime >= 1000 ? 0 : dataBaseData.adjustmentModeTime + 10;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 3)
				{
					dataBaseData.protectiveShieldEnabled = dataBaseData.protectiveShieldEnabled == 0 ? 1 : 0;
				}
			}
			break;
		}

		default:
			break;
	}

	d.editFlag = (uint8_t)(editFlag ? 1 : 0);
	d.curItem = curItem;
	d.adjustmentModeTime = htons((uint16_t)(dataBaseData.adjustmentModeTime / 10));
	d.protectiveShieldEnabled = dataBaseData.protectiveShieldEnabled;
	d.version = (uint8_t)PROGRAM_VERSION;

	hmi->display->updateData(d);
}
