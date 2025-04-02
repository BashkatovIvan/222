#include "Display.h"
#include "string.h"
#include "usart.h"
#include "Globals.h"

void Display::getCurPageHandler()
{
	uint8_t cmd[] = {0x5A, 0xA5, 0x04, READ_NSTRUCTION, 0x00, 0x14, 0x01};
	memcpy(uart_tx_dma_buffer, cmd, sizeof(cmd));

	curPage = NOT_VALID;
	curWriteOperation = WAIT_REPLY_FOR_CUR_PAGE;
	HAL_UART_Transmit_DMA(&huart2, uart_tx_dma_buffer, sizeof(cmd));

	timeout = 0;
}

void Display::waitReplyForCurPageHandler()
{
	if (curPage != NOT_VALID)
	{
		if (messageBuff[curBuff].curPage != curPage && messageBuff[curBuff].curPage != NOT_VALID)
		{
			curWriteOperation = WRITE_DATA_BEFORE_SWITCH_PAGE;
		}
		else
		{
			curWriteOperation = WRITE_DATA;
		}
		timeout = 0;
	}
	timeout++;
}

void Display::switchPageWriteDataBeforeHandler()
{
	uint8_t len = 0;

	if (messageBuff[curBuff].curPage == LOGO_PAGE)
	{
		curWriteOperation = SWITCH_PAGE;
		return;
	}
	else if (messageBuff[curBuff].curPage == MAIN_PAGE)
	{
		len = writeDataMainPageHandler();
	}
	else if (messageBuff[curBuff].curPage == DIAGNOSTIC_PAGE)
	{
		len = writeDataDiagnosticPageHandler();
	}
	else if (messageBuff[curBuff].curPage == SETTINGS_PAGE1)
	{
		len = writeDataSettingsPage1Handler();
	}
	else if (messageBuff[curBuff].curPage == SETTINGS_PAGE2)
	{
		len = writeDataSettingsPage2Handler();
	}
	else if (messageBuff[curBuff].curPage == SETTINGS_PAGE3)
	{
		len = writeDataSettingsPage3Handler();
	}

	if (len > 0)
	{
		writeOk = false;
		HAL_UART_Transmit_DMA(&huart2, uart_tx_dma_buffer, len);
		curWriteOperation = WAIT_REPLY_OK_WRITE_DATA_BEFORE_SWITCH_PAGE;
	}
	else
	{
		curWriteOperation = WRITE_FINISH;
	}

	timeout = 0;
}

void Display::switchPageWriteDataWaitOKHandler()
{
	if (writeOk)
	{
		curWriteOperation = SWITCH_PAGE;
		timeout = 0;
	}
	timeout++;
}

void Display::switchPageHandler()
{
	if (curPage != NOT_VALID)
	{
		uint8_t cmd[] = {0x5A, 0xA5, 0x07, 0x82, 0x00, 0x84, 0x5A, 0x01, 0x00, messageBuff[curBuff].curPage};
		memcpy(uart_tx_dma_buffer, cmd, sizeof(cmd));

		writeOk = false;
		curWriteOperation = WAIT_REPLY_FOR_SWITCH_PAGE;
		HAL_UART_Transmit_DMA(&huart2, uart_tx_dma_buffer, sizeof(cmd));
		timeout = 0;
	}
	timeout++;
}

void Display::waitReplyForSwitchPageHandler()
{
	if (writeOk)
	{
		lastPage = curPage = curData.curPage = messageBuff[curBuff].curPage;
		curWriteOperation = WRITE_DATA;
		timeout = 0;
	}
	timeout++;
}

void Display::writeDataHandler()
{
	uint8_t len = 0;

	dataIfCopy(&curData, &messageBuff[curBuff]);

	if (curData.curPage == LOGO_PAGE)
	{
		curWriteOperation = WRITE_FINISH;
		return;
	}
	else if (curData.curPage == MAIN_PAGE)
	{
		len = writeDataMainPageHandler();
	}
	else if (curData.curPage == DIAGNOSTIC_PAGE)
	{
		len = writeDataDiagnosticPageHandler();
	}
	else if (curData.curPage == SETTINGS_PAGE1)
	{
		len = writeDataSettingsPage1Handler();
	}
	else if (curData.curPage == SETTINGS_PAGE2)
	{
		len = writeDataSettingsPage2Handler();
	}
	else if (curData.curPage == SETTINGS_PAGE3)
	{
		len = writeDataSettingsPage3Handler();
	}

	if (len > 0)
	{
		writeOk = false;
		HAL_UART_Transmit_DMA(&huart2, uart_tx_dma_buffer, len);
		curWriteOperation = WAIT_REPLY_OK;
	}
	else
	{
		curWriteOperation = WRITE_FINISH;
	}
	timeout = 0;
}

uint8_t Display::writeDataSettingsPage3Handler()
{
	uint8_t len = 0;

	uint8_t editFlag = (!curData.editFlag || curData.editFlag == NOT_VALID) ? 0 : 1;

	uint8_t protectiveShieldPic;
	if (curData.protectiveShieldEnabled == NOT_VALID)
		protectiveShieldPic = 3;
	else
		protectiveShieldPic = curData.protectiveShieldEnabled ? 0 : 3;

	uint8_t cmd[] = {0x5A, 0xA5, 0x11, WRITE_INSTRUCTION, 0x55, 0x00,
			0x00, (uint8_t)(curData.curItem == 0 ? 1 : 0),
			0x00, (uint8_t)(curData.curItem == 1 ? 1 : 0),
			0x00, (uint8_t)(curData.curItem == 2 ? (!editFlag ? 1 : 2) : 0),
			0x00, 0x00,
			0x00, (uint8_t)(curData.curItem == 3 ? (!editFlag ? protectiveShieldPic + 1 : protectiveShieldPic + 2) : protectiveShieldPic),
			0x00, 0x00,
			0x00, curData.version,
	};
	memcpy(cmd + sizeof(cmd) - 8, &curData.adjustmentModeTime, 2);

	len = sizeof(cmd);
	memcpy(uart_tx_dma_buffer, cmd, len);

	return len;
}

uint8_t Display::writeDataSettingsPage2Handler()
{
	uint8_t len = 0;

	uint8_t editFlag = (!curData.editFlag || curData.editFlag == NOT_VALID) ? 0 : 1;

	uint8_t browingOffEnabledPic;
	if (curData.browingOffEnabled == NOT_VALID)
		browingOffEnabledPic = 3;
	else
		browingOffEnabledPic = curData.browingOffEnabled ? 0 : 3;

	uint8_t cmd[] = {0x5A, 0xA5, 0x15, WRITE_INSTRUCTION, 0x54, 0x00,
			0x00, (uint8_t)(curData.curItem == 0 ? 1 : 0),
			0x00, (uint8_t)(curData.curItem == 1 ? 1 : 0),
			0x00, (uint8_t)(curData.curItem == 2 ? (!editFlag ? 1 : 2) : 0),
			0x00, curData.lubricantCount,
			0x00, (uint8_t)(curData.curItem == 3 ? (!editFlag ? 1 : 2) : 0),
			0x00, curData.lubricantTime,
			0x00, (uint8_t)(curData.curItem == 4 ?  (!editFlag ? browingOffEnabledPic + 1 : browingOffEnabledPic + 2) : browingOffEnabledPic),
			0x00, (uint8_t)(curData.curItem == 5 ? (!editFlag ? 1 : 2) : 0),
			0x00, curData.browingOffWorkTime,
	};

	len = sizeof(cmd);
	memcpy(uart_tx_dma_buffer, cmd, len);

	return len;
}

uint8_t Display::writeDataSettingsPage1Handler()
{
	uint8_t len = 0;

	uint8_t editFlag = (!curData.editFlag || curData.editFlag == NOT_VALID) ? 0 : 1;

	uint8_t hasStopControlPic;
	if (curData.hasStopControl == NOT_VALID)
		hasStopControlPic = 3;
	else
		hasStopControlPic = curData.hasStopControl ? 0 : 3;

	uint8_t lubricantTypePic;
	if (curData.lubricantType == NOT_VALID)
		lubricantTypePic = 6;
	else
		lubricantTypePic = curData.lubricantType == LubricantType::BY_COUNTER ? 0 : curData.lubricantType == LubricantType::BY_TIME ? 3 : 6;

	uint8_t cmd[] = {0x5A, 0xA5, 0x13, WRITE_INSTRUCTION, 0x53, 0x00,
			0x00, (uint8_t)(curData.curItem == 0 ? 1 : 0),
			0x00, (uint8_t)(curData.curItem == 1 ? 1 : 0),
			0x00, (uint8_t)(curData.curItem == 2 ? (!editFlag ? hasStopControlPic + 1 : hasStopControlPic + 2) : hasStopControlPic),
			0x00, (uint8_t)(curData.curItem == 3 ? (!editFlag ? lubricantTypePic + 1 : lubricantTypePic + 2) : lubricantTypePic),
			0x00, (uint8_t)(curData.curItem == 4 ? (!editFlag ? 1 : 2) : 0),
			0x00, 0x00,
			0x00, (uint8_t)(curData.curItem == 5 ? (!editFlag ? 1 : 2) : 0),
			0x00, curData.lubricantOnTime,
	};
	memcpy(cmd + sizeof(cmd) - 6, curData.lubricantType == LubricantType::BY_COUNTER ? &curData.lubricantOffCount : &curData.lubricantOffTime, 2);

	len = sizeof(cmd);
	memcpy(uart_tx_dma_buffer, cmd, len);

	return len;
}

uint8_t Display::writeDataMainPageHandler()
{
	uint8_t len = 0;

	uint8_t cmd[] = {0x5A, 0xA5, 0x23, WRITE_INSTRUCTION, 0x50, 0x10,
			0x00, (uint8_t)(curData.mode == NOT_VALID ? 0 : curData.mode),
			0x00, (uint8_t)(curData.controlMode == NOT_VALID ? 0 : curData.controlMode),
			0x00, (uint8_t)(curData.lubricant == NOT_VALID ? 0 : curData.lubricant),
			0x00, 0x00,
			0x00, (uint8_t)(curData.mainMotor == NOT_VALID ? 0 : curData.mainMotor),
			0x00, (uint8_t)(curData.blowingOffMotor == NOT_VALID ? 0 : curData.blowingOffMotor),
			0x00, (uint8_t)(curData.startPosition == NOT_VALID ? 0 : curData.startPosition),
			0x00, (uint8_t)(curData.counter == NOT_VALID ? 0 : curData.counter),
			0x00, (uint8_t)(curData.digit7 == NOT_VALID ? 0 : curData.digit7),
			0x00, (uint8_t)(curData.digit6 == NOT_VALID ? 0 : curData.digit6),
			0x00, (uint8_t)(curData.digit5 == NOT_VALID ? 0 : curData.digit5),
			0x00, (uint8_t)(curData.digit4 == NOT_VALID ? 0 : curData.digit4),
			0x00, (uint8_t)(curData.digit3 == NOT_VALID ? 0 : curData.digit3),
			0x00, (uint8_t)(curData.digit2 == NOT_VALID ? 0 : curData.digit2),
			0x00, (uint8_t)(curData.digit1 == NOT_VALID ? 0 : curData.digit1),
			0x00, (uint8_t)(curData.error == NOT_VALID ? 0 : curData.error),
	};

	len = sizeof(cmd);
	memcpy(uart_tx_dma_buffer, cmd, len);

	return len;
}

uint8_t Display::writeDataDiagnosticPageHandler()
{
	uint8_t len = 0;

	uint8_t cmd[] = {0x5A, 0xA5, 0x3D, WRITE_INSTRUCTION, 0x51, 0x10,
			0x00, (uint8_t)(curData.SQ1 == NOT_VALID ? 0 : curData.SQ1),
			0x00, (uint8_t)(curData.SQ2 == NOT_VALID ? 0 : curData.SQ2),
			0x00, (uint8_t)(curData.SQ3 == NOT_VALID ? 0 : curData.SQ3),
			0x00, (uint8_t)(curData.SB3_1 == NOT_VALID ? 0 : curData.SB3_1),
			0x00, (uint8_t)(curData.SB4_1 == NOT_VALID ? 0 : curData.SB4_1),
			0x00, (uint8_t)(curData.SB3_2_4_2 == NOT_VALID ? 0 : curData.SB3_2_4_2),
			0x00, (uint8_t)(curData.SB6_1 == NOT_VALID ? 0 : curData.SB6_1),
			0x00, (uint8_t)(curData.SB6_2 == NOT_VALID ? 0 : curData.SB6_2),
			0x00, (uint8_t)(curData.SP2 == NOT_VALID ? 0 : curData.SP2),
			0x00, (uint8_t)(curData.SL == NOT_VALID ? 0 : curData.SL),
			0x00, (uint8_t)(curData.SQ13 == NOT_VALID ? 0 : curData.SQ13),
			0x00, (uint8_t)(curData.SQ16 == NOT_VALID ? 0 : curData.SQ16),
			0x00, (uint8_t)(curData.SQ11_SQ12 == NOT_VALID ? 0 : curData.SQ11_SQ12),
			0x00, (uint8_t)(curData.SQ14 == NOT_VALID ? 0 : curData.SQ14),
			0x00, (uint8_t)(curData.SP1 == NOT_VALID ? 0 : curData.SP1),
			0x00, (uint8_t)(curData.SQ15 == NOT_VALID ? 0 : curData.SQ15),
			0x00, (uint8_t)(curData.SB5 == NOT_VALID ? 0 : curData.SB5),
			0x00, (uint8_t)(curData.KM1_1 == NOT_VALID ? 0 : curData.KM1_1),
			0x00, (uint8_t)(curData.SA == NOT_VALID ? 0 : curData.SA),
			0x00, (uint8_t)(curData.YV1 == NOT_VALID ? 0 : curData.YV1),
			0x00, (uint8_t)(curData.YV2 == NOT_VALID ? 0 : curData.YV2),
			0x00, 0x00,
			0x00, 0x00,
			0x00, 0x00,
			0x00, 0x00,
			0x00, 0x00,
			0x00, 0x00,
			0x00, 0x00,
			0x00, (uint8_t)(curData.error == NOT_VALID ? 0 : curData.error)
	};
	len = sizeof(cmd);
	memcpy(uart_tx_dma_buffer, cmd, len);

	return len;
}

void Display::waitReplyOkHandler()
{
	if (writeOk)
	{
		curWriteOperation = WRITE_FINISH;
		timeout = 0;
	}
	timeout++;
}

void Display::writeFinishHadler()
{
	if (dataCompare(&curData, &messageBuff[curBuff], false))
	{
		updateTime = 0;
		curWriteOperation = GET_CUR_PAGE;
	}
	else if ((DISPLAY_INTERRUPT_PRESCALER * updateTime) >= 1000)
	{
		messageBuff[curBuff] = curData;
		updateTime = 0;
		curWriteOperation = GET_CUR_PAGE;
	}
	else
	{
		DisplayData clearData;
		messageBuff[curBuff] = clearData;
		curBuff = curBuff == BUFF_FIRST ? BUFF_SECOND : BUFF_FIRST;
	}
}
