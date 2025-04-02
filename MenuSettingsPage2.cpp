/*
 * MenuSettingsPage1.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#include "MenuSettingsPage2.h"
#include "Globals.h"
#include "string.h"
#include "Utils.h"

void MenuSettingsPage2::handle(int curPage)
{
	if (this->curPage != curPage && curPage == Pages::SETTINGS_PAGE2)
	{
		dataBaseData = dataBase->getAllFromStorage();
		if (!memcmp(&dataBaseData, &clearData, sizeof(DataBaseData)))
			dataBase->setDefaults(&dataBaseData);

		curItem = 1;
		editFlag = false;
	}
	this->curPage = curPage;

	if (curPage != Pages::SETTINGS_PAGE2)
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
					dataBaseData.lubricantCount = dataBaseData.lubricantCount < 1 ? 100 : dataBaseData.lubricantCount - 1;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 3)
				{
					dataBaseData.lubricantTime = dataBaseData.lubricantTime < 1 ? 100 : dataBaseData.lubricantTime - 1;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 4)
				{
					dataBaseData.browingOffEnabled = dataBaseData.browingOffEnabled == 0 ? 1 : 0;
				}
				if (curItem == 5)
				{
					dataBaseData.browingOffWorkTime = dataBaseData.browingOffWorkTime < 100 ? 10000 : dataBaseData.browingOffWorkTime - 100;

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
				hmi->display->updateData({.curPage = (uint8_t)(Pages::SETTINGS_PAGE3)});
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
					dataBaseData.lubricantCount = dataBaseData.lubricantCount >= 100 ? 0 : dataBaseData.lubricantCount + 1;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 3)
				{
					dataBaseData.lubricantTime = dataBaseData.lubricantTime >= 100 ? 0 : dataBaseData.lubricantTime + 1;

					if (key.longPress && hmi->keyboard->getMode() != KeyboardMode::KEYBOARD_ITER)
					{
						hmi->keyboard->setMode(KeyboardMode::KEYBOARD_ITER);
					}
				}
				if (curItem == 4)
				{
					dataBaseData.browingOffEnabled = dataBaseData.browingOffEnabled == 0 ? 1 : 0;
				}
				if (curItem == 5)
				{
					dataBaseData.browingOffWorkTime = dataBaseData.browingOffWorkTime >= 10000 ? 0 : dataBaseData.browingOffWorkTime + 100;

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
	d.lubricantCount = dataBaseData.lubricantCount;
	d.lubricantTime = dataBaseData.lubricantTime;
	d.browingOffEnabled = dataBaseData.browingOffEnabled;
	d.browingOffWorkTime = dataBaseData.browingOffWorkTime / 100;

	hmi->display->updateData(d);
}
