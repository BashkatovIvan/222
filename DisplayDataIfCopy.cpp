/*
 * DisplayDataIfCopy.cpp
 *
 *  Created on: 20 авг. 2022 г.
 *      Author: alexander
 */
#include "Display.h"

bool Display::dataIfCopy(DisplayData *dst, DisplayData *src) {
	return dataCompare(dst, src, true);
}

bool Display::dataCompare(DisplayData *dst, DisplayData *src, bool flagCopy)
{
	switch(lastPage)
	{
		case Pages::LOGO_PAGE:
			return dataCompareForLogoPage(dst, src, flagCopy);
		case Pages::MAIN_PAGE:
			return dataCompareForMainPage(dst, src, flagCopy);
		case Pages::DIAGNOSTIC_PAGE:
			return dataCompareForDiagnosticPage(dst, src, flagCopy);
		case Pages::SETTINGS_PAGE1:
			return dataCompareForSettingsPage1(dst, src, flagCopy);
		case Pages::SETTINGS_PAGE2:
			return dataCompareForSettingsPage2(dst, src, flagCopy);
		case Pages::SETTINGS_PAGE3:
			return dataCompareForSettingsPage3(dst, src, flagCopy);
	}
	return false;
}

bool Display::dataCompareForSettingsPage3(DisplayData *dst, DisplayData *src, bool flagCopy)
{
	bool ret = false;

	for(;;)
	{
		if (src->curPage != dst->curPage && src->curPage != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curPage = src->curPage;
		}

		if (src->curItem != dst->curItem && src->curItem != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curItem = src->curItem;
		}

		if (src->editFlag != dst->editFlag && src->editFlag != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->editFlag = src->editFlag;
		}

		if (src->adjustmentModeTime != dst->adjustmentModeTime && src->adjustmentModeTime != NOT_VALID_UINT16_T)
		{
			ret = true;
			if (!flagCopy) break;
			dst->adjustmentModeTime = src->adjustmentModeTime;
		}

		if (src->protectiveShieldEnabled != dst->protectiveShieldEnabled && src->protectiveShieldEnabled != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->protectiveShieldEnabled = src->protectiveShieldEnabled;
		}

		if (src->version != dst->version && src->version != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->version = src->version;
		}
		break;
	}

	return ret;
}

bool Display::dataCompareForSettingsPage2(DisplayData *dst, DisplayData *src, bool flagCopy)
{
	bool ret = false;

	for(;;)
	{
		if (src->curPage != dst->curPage && src->curPage != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curPage = src->curPage;
		}

		if (src->curItem != dst->curItem && src->curItem != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curItem = src->curItem;
		}

		if (src->editFlag != dst->editFlag && src->editFlag != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->editFlag = src->editFlag;
		}

		if (src->lubricantCount != dst->lubricantCount && src->lubricantCount != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->lubricantCount = src->lubricantCount;
		}

		if (src->lubricantTime != dst->lubricantTime && src->lubricantTime != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->lubricantTime = src->lubricantTime;
		}

		if (src->browingOffEnabled != dst->browingOffEnabled && src->browingOffEnabled != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->browingOffEnabled = src->browingOffEnabled;
		}

		if (src->browingOffWorkTime != dst->browingOffWorkTime && src->browingOffWorkTime != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->browingOffWorkTime = src->browingOffWorkTime;
		}
		break;
	}

	return ret;
}

bool Display::dataCompareForSettingsPage1(DisplayData *dst, DisplayData *src, bool flagCopy)
{
	bool ret = false;

	for(;;)
	{
		if (src->curPage != dst->curPage && src->curPage != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curPage = src->curPage;
		}

		if (src->curItem != dst->curItem && src->curItem != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curItem = src->curItem;
		}

		if (src->editFlag != dst->editFlag && src->editFlag != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->editFlag = src->editFlag;
		}

		if (src->hasStopControl != dst->hasStopControl && src->hasStopControl != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->hasStopControl = src->hasStopControl;
		}

		if (src->lubricantType != dst->lubricantType && src->lubricantType != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->lubricantType = src->lubricantType;
		}

		if (src->lubricantOnTime != dst->lubricantOnTime && src->lubricantOnTime != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->lubricantOnTime = src->lubricantOnTime;
		}

		if (src->lubricantOffTime != dst->lubricantOffTime && src->lubricantOffTime != NOT_VALID_UINT16_T)
		{
			ret = true;
			if (!flagCopy) break;
			dst->lubricantOffTime = src->lubricantOffTime;
		}

		if (src->lubricantOffCount != dst->lubricantOffCount && src->lubricantOffCount != NOT_VALID_UINT16_T)
		{
			ret = true;
			if (!flagCopy) break;
			dst->lubricantOffCount = src->lubricantOffCount;
		}
		break;
	}

	return ret;
}

bool Display::dataCompareForDiagnosticPage(DisplayData *dst, DisplayData *src, bool flagCopy)
{
	bool ret = false;

	for(;;)
	{
		if (src->curPage != dst->curPage && src->curPage != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curPage = src->curPage;
		}

		if (src->error != dst->error && src->error != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->error = src->error;
		}

		if (src->SQ1 != dst->SQ1 && src->SQ1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ1 = src->SQ1;
		}

		if (src->SQ2 != dst->SQ2 && src->SQ2 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ2 = src->SQ2;
		}

		if (src->SQ3 != dst->SQ3 && src->SQ3 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ3 = src->SQ3;
		}

		if (src->SQ11_SQ12 != dst->SQ11_SQ12 && src->SQ11_SQ12 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ11_SQ12 = src->SQ11_SQ12;
		}

		if (src->SQ13 != dst->SQ13 && src->SQ13 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ13 = src->SQ13;
		}

		if (src->SQ14 != dst->SQ14 && src->SQ14 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ14 = src->SQ14;
		}

		if (src->SQ15 != dst->SQ15 && src->SQ15 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ15 = src->SQ15;
		}

		if (src->SQ16 != dst->SQ16 && src->SQ16 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SQ16 = src->SQ16;
		}

		if (src->SB3_1 != dst->SB3_1 && src->SB3_1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SB3_1 = src->SB3_1;
		}

		if (src->SB3_2_4_2 != dst->SB3_2_4_2 && src->SB3_2_4_2 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SB3_2_4_2 = src->SB3_2_4_2;
		}

		if (src->SB4_1 != dst->SB4_1 && src->SB4_1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SB4_1 = src->SB4_1;
		}

		if (src->SB5 != dst->SB5 && src->SB5 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SB5 = src->SB5;
		}

		if (src->SB6_1 != dst->SB6_1 && src->SB6_1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SB6_1 = src->SB6_1;
		}

		if (src->SB6_2 != dst->SB6_2 && src->SB6_2 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SB6_2 = src->SB6_2;
		}

		if (src->SL != dst->SL && src->SL != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SL = src->SL;
		}

		if (src->SA != dst->SA && src->SA != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SA = src->SA;
		}

		if (src->SP1 != dst->SP1 && src->SP1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SP1 = src->SP1;
		}

		if (src->SP2 != dst->SP2 && src->SP2 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->SP2 = src->SP2;
		}

		if (src->KM1_1 != dst->KM1_1 && src->KM1_1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->KM1_1 = src->KM1_1;
		}

		if (src->YV1 != dst->YV1 && src->YV1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->YV1 = src->YV1;
		}

		if (src->YV2 != dst->YV2 && src->YV2 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->YV2 = src->YV2;
		}

		break;
	}

	return ret;
}

bool Display::dataCompareForMainPage(DisplayData *dst, DisplayData *src, bool flagCopy)
{
	bool ret = false;

	for(;;)
	{
		if (src->curPage != dst->curPage && src->curPage != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curPage = src->curPage;
		}

		if (src->mode != dst->mode && src->mode != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->mode = src->mode;
		}

		if (src->lubricant != dst->lubricant && src->lubricant != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->lubricant = src->lubricant;
		}

		if (src->error != dst->error && src->error != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->error = src->error;
		}

		if (src->mainMotor != dst->mainMotor && src->mainMotor != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->mainMotor = src->mainMotor;
		}

		if (src->blowingOffMotor != dst->blowingOffMotor && src->blowingOffMotor != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->blowingOffMotor = src->blowingOffMotor;
		}

		if (src->controlMode != dst->controlMode && src->controlMode != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->controlMode = src->controlMode;
		}

		if (src->startPosition != dst->startPosition && src->startPosition != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->startPosition = src->startPosition;
		}

		if (src->digit1 != dst->digit1 && src->digit1 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->digit1 = src->digit1;
		}

		if (src->digit2 != dst->digit2 && src->digit2 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->digit2 = src->digit2;
		}

		if (src->digit3 != dst->digit3 && src->digit3 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->digit3 = src->digit3;
		}

		if (src->digit4 != dst->digit4 && src->digit4 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->digit4 = src->digit4;
		}

		if (src->digit5 != dst->digit5 && src->digit5 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->digit5 = src->digit5;
		}

		if (src->digit6 != dst->digit6 && src->digit6 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->digit6 = src->digit6;
		}

		if (src->digit7 != dst->digit7 && src->digit7 != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->digit7 = src->digit7;
		}

		if (src->counter != dst->counter && src->counter != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->counter = src->counter;
		}
		break;
	}
	return ret;
}

bool Display::dataCompareForLogoPage(DisplayData *dst, DisplayData *src, bool flagCopy)
{
	bool ret = false;

	for(;;)
	{
		if (src->curPage != dst->curPage && src->curPage != NOT_VALID)
		{
			ret = true;
			if (!flagCopy) break;
			dst->curPage = src->curPage;
		}
		break;
	}
	return ret;
}



