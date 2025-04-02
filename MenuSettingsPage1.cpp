/*
 * MenuSettingsPage1.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#include "MenuSettingsPage1.h"
#include "Globals.h"
#include "string.h"
#include "Utils.h"

void MenuSettingsPage1::handle(int curPage)
{
	if (this->curPage != curPage && curPage == Pages::SETTINGS_PAGE1)
	{
		dataBaseData = dataBase->getAllFromStorage();
		if (!memcmp(&dataBaseData, &clearData, sizeof(DataBaseData)))
			dataBase->setDefaults(&dataBaseData);

		curItem = 1;
		editFlag = false;
	}
	this->curPage = curPage;

	if (curPage != Pages::SETTINGS_PAGE1)
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
					dataBaseData.hasStopControl = dataBaseData.hasStopControl != 0 ? 0 : 1;
				}
				if (curItem == 3)
				{
					dataBaseData.lubricantType = dataBaseData.lubricantType == LubricantType::LUBRICANT_OFF ? LubricantType::BY_TIME : dataBaseData.lubricantType - 1;
				}
				if (curItem == 4)
				{
					dataBaseData.lubricantOffTime = dataBaseData.lubricantOffTime == 0 ? 100 : dataBaseData.lubricantOffTime - 1;
					dataBaseData.lubricantOffCount = dataBaseData.lubricantOffTime * 10;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 5)
				{
					dataBaseData.lubricantOnTime = dataBaseData.lubricantOnTime < 2 ? 100 : dataBaseData.lubricantOnTime - 1;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
			}

			break;
		}
		case KEY_SWITCH_MENU:
		{
			if (editFlag)
			{
				editFlag = false;
				dataBase->saveAllToStorage(dataBaseData);
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
				hmi->display->updateData({.curPage = (uint8_t)(Pages::SETTINGS_PAGE2)});
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
					dataBaseData.hasStopControl = dataBaseData.hasStopControl != 0 ? 0 : 1;
				}
				if (curItem == 3)
				{
					dataBaseData.lubricantType = dataBaseData.lubricantType == LubricantType::BY_TIME ? LubricantType::LUBRICANT_OFF : dataBaseData.lubricantType + 1;
				}
				if (curItem == 4)
				{
					dataBaseData.lubricantOffTime = dataBaseData.lubricantOffTime >= 100 ? 1 : dataBaseData.lubricantOffTime + 1;
					dataBaseData.lubricantOffCount = dataBaseData.lubricantOffTime * 10;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 5)
				{
					dataBaseData.lubricantOnTime = dataBaseData.lubricantOnTime >= 100 ? 1 : dataBaseData.lubricantOnTime + 1;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
			}
			break;
		}

		default:
			break;
	}

	d.editFlag = (uint8_t)(editFlag ? 1 : 0);
	d.curItem = curItem;
	d.hasStopControl = dataBaseData.hasStopControl;
	d.lubricantType = dataBaseData.lubricantType;
	d.lubricantOnTime = dataBaseData.lubricantOnTime;
	d.lubricantOffTime = htons(dataBaseData.lubricantOffTime);
	d.lubricantOffCount = htons(dataBaseData.lubricantOffCount);

	hmi->display->updateData(d);
}
