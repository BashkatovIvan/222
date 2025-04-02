/*
 * Display.cpp
 *
 *  Created on: Aug 2, 2022
 *      Author: amaslov
 */


#include "Display.h"
#include "DisplayUARTRoutines.h"
#include "string.h"
#include "Globals.h"
#include "SystemErrors.h"
#include "PressInterface.h"

const DisplayData Display::defaultData =
{
	.curPage = Pages::LOGO_PAGE,

	/*MainPage Params*/
	.mode = PressModes::MODE_ADJUSTMENT,
	.controlMode = ControlPanelMode::BUTTONS_CONTROL,
	.error = SystemError::NO_ERROR,
	.lubricant = 0,
	.mainMotor = 0,
	.startPosition = 0,
	.blowingOffMotor = 0,
	.counter = 0,
	.digit1 = 0,
	.digit2 = 0,
	.digit3 = 0,
	.digit4 = 0,
	.digit5 = 0,
	.digit6 = 0,
	.digit7 = 0,

	/*DiagnosticPage Params*/
	.SQ1 = 0,
	.SQ2 = 0,
	.SQ3 = 0,
	.SQ11_SQ12 = 0,
	.SQ13 = 0,
	.SQ14 = 0,
	.SQ15 = 0,
	.SQ16 = 0,
	.SB3_1 = 0,
	.SB3_2_4_2 = 0,
	.SB4_1 = 0,
	.SB5 = 0,
	.SB6_1 = 0,
	.SB6_2 = 0,
	.SL = 0,
	.SA = 0,
	.SP1 = 0,
	.SP2 = 0,
	.KM1_1 = 0,
	.YV1 = 0,
	.YV2 = 0,

	/*For settings Pages*/
	.editFlag = 0,
	.curItem = 0,

	/*SettingsPage1 Params*/
	.hasStopControl = 0,
	.lubricantType = LubricantType::BY_TIME,
	.lubricantOnTime = 0,
	.lubricantOffTime = 0,
	.lubricantOffCount = 0,

	/*SettingsPage2 Params*/
	.lubricantCount = 0,
	.lubricantTime = 0,
	.browingOffEnabled = 0,
	.browingOffWorkTime = 0,

	/*SettingsPage3 Params*/
	.adjustmentModeTime = 0,
	.protectiveShieldEnabled = 0,
	.version = 0
};


Display::Display()
{
	DataBaseData d = dataBase->getAllFromDataBase();

	operations[GET_CUR_PAGE] = &Display::getCurPageHandler;
	operations[WAIT_REPLY_FOR_CUR_PAGE] = &Display::waitReplyForCurPageHandler;
	operations[WRITE_DATA_BEFORE_SWITCH_PAGE] = &Display::switchPageWriteDataBeforeHandler;
	operations[WAIT_REPLY_OK_WRITE_DATA_BEFORE_SWITCH_PAGE] = &Display::switchPageWriteDataWaitOKHandler;
	operations[SWITCH_PAGE] = &Display::switchPageHandler;
	operations[WAIT_REPLY_FOR_SWITCH_PAGE] = &Display::waitReplyForSwitchPageHandler;
	operations[WRITE_DATA] = &Display::writeDataHandler;
	operations[WAIT_REPLY_OK] = &Display::waitReplyOkHandler;
	operations[WRITE_FINISH] = &Display::writeFinishHadler;

	curData = defaultData;
	curData.version = d.programVersion;
	curData.lubricantTime = d.lubricantTime;
	curData.lubricantCount = d.lubricantCount;
	curData.lubricantOnTime = d.lubricantOnTime;
	curData.lubricantOffTime = d.lubricantOffTime;
	curData.lubricantOffCount = d.lubricantOffCount;
	curData.lubricantType = d.lubricantType;
	curData.hasStopControl = d.hasStopControl;
	curData.adjustmentModeTime = d.adjustmentModeTime;
	curData.browingOffWorkTime = d.browingOffWorkTime;
	curData.browingOffEnabled = d.browingOffEnabled;

	curBuff = BUFF_FIRST;

	curPage = Pages::LOGO_PAGE;
	lastPage = Pages::LOGO_PAGE;
	messageBuff[BUFF_FIRST] = messageBuff[BUFF_SECOND] = curData;
	curWriteOperation = WriteOperations::GET_CUR_PAGE;

	MX_USART2_UART_Init();
	HAL_UART_RegisterRxEventCallback(&huart2, uartRxEventCallback);
	HAL_UART_RegisterCallback(&huart2, HAL_UART_ERROR_CB_ID, HAL_UART_ErrorCallback);

	receiveData();
}

void Display::handle()
{
	interruptPrescaler = interruptPrescaler >= (DISPLAY_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (DISPLAY_INTERRUPT_PRESCALER - 1))
		return;

	interruptFlag = true;
}

void Display::postInterruptHandler()
{
	if (!interruptFlag)
		return;

	interruptFlag = false;

	if (timeout >= DISPLAY_TIMEOUT)
	{
		timeout = 0;

		if (curWriteOperation != WRITE_FINISH)
			curWriteOperation = GET_CUR_PAGE;

		memset(uart_rx_buffer, 0x00, sizeof(uart_rx_buffer));
		curRxPos = 0;

		HAL_UART_DeInit(&huart2);
		MX_USART2_UART_Init();
		HAL_UART_RegisterRxEventCallback(&huart2, uartRxEventCallback);
		receiveData();
	}
	else
	{
		if (curWriteOperation < WRITE_OPERATIONS_SIZE)
			(this->*operations[curWriteOperation])();
	}

	updateTime++;
}

void Display::updateState()
{
	postInterruptHandler();
}

int Display::getCurrentPage() {
	return lastPage;
}

void Display::updateData(DisplayData data)
{
	dataIfCopy(&messageBuff[(curBuff == BUFF_FIRST ? BUFF_SECOND : BUFF_FIRST)], &data);
}






