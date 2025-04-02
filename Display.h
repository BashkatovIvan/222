/*
 * Display.h
 *
 *  Created on: Aug 2, 2022
 *      Author: amaslov
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include "InterruptHandler.h"
#include "Mutex.h"
#include <stdint.h>
#include "main.h"

#define DISPLAY_INTERRUPT_PRESCALER 5
#define DISPLAY_TIMEOUT (50 / DISPLAY_INTERRUPT_PRESCALER)

#define UART2_BUFF_SIZE 100
#define NOT_VALID 255
#define NOT_VALID_UINT16_T UINT16_MAX

typedef struct _DisplayData
{
	uint8_t curPage = NOT_VALID;

	/*MainPage Params*/
	uint8_t mode = NOT_VALID;
	uint8_t controlMode = NOT_VALID;
	uint8_t error = NOT_VALID;
	uint8_t lubricant = NOT_VALID;
	uint8_t mainMotor = NOT_VALID;
	uint8_t startPosition = NOT_VALID;
	uint8_t blowingOffMotor = NOT_VALID;
	uint8_t counter = NOT_VALID;
	uint8_t digit1 = NOT_VALID;
	uint8_t digit2 = NOT_VALID;
	uint8_t digit3 = NOT_VALID;
	uint8_t digit4 = NOT_VALID;
	uint8_t digit5 = NOT_VALID;
	uint8_t digit6 = NOT_VALID;
	uint8_t digit7 = NOT_VALID;

	/*DiagnosticPage Params*/
	uint8_t SQ1 = NOT_VALID;
	uint8_t SQ2 = NOT_VALID;
	uint8_t SQ3 = NOT_VALID;
	uint8_t SQ11_SQ12 = NOT_VALID;
	uint8_t SQ13 = NOT_VALID;
	uint8_t SQ14 = NOT_VALID;
	uint8_t SQ15 = NOT_VALID;
	uint8_t SQ16 = NOT_VALID;
	uint8_t SB3_1 = NOT_VALID;
	uint8_t SB3_2_4_2 = NOT_VALID;
	uint8_t SB4_1 = NOT_VALID;
	uint8_t SB5 = NOT_VALID;
	uint8_t SB6_1 = NOT_VALID;
	uint8_t SB6_2 = NOT_VALID;
	uint8_t SL = NOT_VALID;
	uint8_t SA = NOT_VALID;
	uint8_t SP1 = NOT_VALID;
	uint8_t SP2 = NOT_VALID;
	uint8_t KM1_1 = NOT_VALID;
	uint8_t YV1 = NOT_VALID;
	uint8_t YV2 = NOT_VALID;

	/*For settings Pages*/
	uint8_t editFlag = NOT_VALID;
	uint8_t curItem = NOT_VALID;

	/*SettingsPage1 Params*/
	uint8_t hasStopControl = NOT_VALID;
	uint8_t lubricantType = NOT_VALID;
	uint8_t lubricantOnTime = NOT_VALID;
	uint16_t lubricantOffTime = NOT_VALID_UINT16_T;
	uint16_t lubricantOffCount = NOT_VALID_UINT16_T;

	/*SettingsPage2 Params*/
	uint8_t lubricantCount = NOT_VALID;
	uint8_t lubricantTime = NOT_VALID;
	uint8_t browingOffEnabled = NOT_VALID;
	uint8_t browingOffWorkTime = NOT_VALID;

	/*SettingsPage3 Params*/
	uint16_t adjustmentModeTime = NOT_VALID_UINT16_T;
	uint8_t protectiveShieldEnabled = NOT_VALID;
	uint8_t version = NOT_VALID;

} DisplayData;

enum Pages
{
	LOGO_PAGE = 0,
	MAIN_PAGE,
	DIAGNOSTIC_PAGE,
	SETTINGS_PAGE1,
	SETTINGS_PAGE2,
	SETTINGS_PAGE3,
	NULL_PAGE
};

#define READ_NSTRUCTION 0x83
#define TOUCH_NSTRUCTION 0x83
#define READ_AND_TOUCH_INSTRUCTION 0x83
#define WRITE_INSTRUCTION 0x82
#define TOUCH_INSTRUCTION 0x83
#define FRAME_HEADER 0xA55A


typedef struct _hdrReply {
	uint16_t header;
	uint16_t len:8;
	uint16_t instruction:8;
} hdrReply;

typedef struct _hdrWriteInstruction {
	uint16_t header;
	uint16_t len:8;
	uint16_t instruction:8;
	uint16_t addr;
} hdrWriteInstruction;

typedef struct _hdrWriteReply {
	uint16_t header;
	uint16_t len:8;
	uint16_t instruction:8;
	uint16_t ok;
} hdrWriteReply;

typedef struct _hdrReadInstruction {
	uint16_t header;
	uint16_t len:8;
	uint16_t instruction:8;
	uint16_t addr;
} hdrReadInstruction;

typedef struct _hdrReadAndTouchReply {
	uint16_t header;
	uint16_t len:8;
	uint16_t instruction:8;
	uint16_t addr;
} hdrReadAndTouchReply;

#define VP_ADDR_PIC_NOW 0x0014

class Display : public InterruptHandler
{
	/*Interrupts*/
	volatile bool interruptFlag = false;
	volatile int8_t interruptPrescaler = 0;
	uint32_t updateTime = 0;

	/*DisplayHandlers*/
	volatile int timeout = DISPLAY_TIMEOUT;
	volatile bool writeOk = false;
	typedef enum _WriteOperations
	{
		GET_CUR_PAGE = 0,
		WAIT_REPLY_FOR_CUR_PAGE,
		WRITE_DATA_BEFORE_SWITCH_PAGE,
		WAIT_REPLY_OK_WRITE_DATA_BEFORE_SWITCH_PAGE,
		SWITCH_PAGE,
		WAIT_REPLY_FOR_SWITCH_PAGE,
		WRITE_DATA,
		WAIT_REPLY_OK,
		WRITE_FINISH,
		WRITE_OPERATIONS_SIZE
	} WriteOperations;
	void (Display::*operations[WRITE_OPERATIONS_SIZE])();
	WriteOperations curWriteOperation;

	/*DisplayUARTRoutines*/
	volatile int curRxPos = 0;
	uint8_t uart_tx_dma_buffer[UART2_BUFF_SIZE] = {0};
	uint8_t uart_rx_dma_buffer[UART2_BUFF_SIZE] = {0};
	uint8_t uart_rx_buffer[UART2_BUFF_SIZE] = {0};

	/*Display interface Data*/
	typedef enum _BuffParts {
		BUFF_FIRST = 0,
		BUFF_SECOND = 1
	} BuffParts;
	DisplayData messageBuff[2];
	BuffParts curBuff = BUFF_FIRST;
	DisplayData curData;
	static const DisplayData defaultData;
	volatile uint8_t curPage;
	uint8_t lastPage;

	/*Interrupts*/
	void handle() override;
	void postInterruptHandler();

	/*DisplayHandlers*/
	void getCurPageHandler();
	void waitReplyForCurPageHandler();
	void switchPageWriteDataBeforeHandler();
	void switchPageWriteDataWaitOKHandler();
	void switchPageHandler();
	void waitReplyForSwitchPageHandler();
	void writeDataHandler();
	uint8_t writeDataMainPageHandler();
	uint8_t writeDataDiagnosticPageHandler();
	uint8_t writeDataSettingsPage1Handler();
	uint8_t writeDataSettingsPage2Handler();
	uint8_t writeDataSettingsPage3Handler();
	void waitReplyOkHandler();
	void writeFinishHadler();

	/*DisplayUARTRoutines*/
	friend void uartRxEventCallback(struct __UART_HandleTypeDef *huart, uint16_t Pos);
	void privateUartRxEventCallback(struct __UART_HandleTypeDef *huart, uint16_t Pos);
	friend void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
	void privateHAL_UART_ErrorCallback(UART_HandleTypeDef *huarts);
	int handleWriteReply(uint8_t *buf, uint8_t limitPos);
	int handleReadAndTouchReply(uint8_t *buf, uint8_t limitPos);
	int receiveData();

	/*DisplayDataIfCopy*/
	bool dataIfCopy(DisplayData *dst, DisplayData *src);
	bool dataCompare(DisplayData *dst, DisplayData *src, bool flagCopy);
	bool dataCompareForLogoPage(DisplayData *dst, DisplayData *src, bool flagCopy);
	bool dataCompareForMainPage(DisplayData *dst, DisplayData *src, bool flagCopy);
	bool dataCompareForDiagnosticPage(DisplayData *dst, DisplayData *src, bool flagCopy);
	bool dataCompareForSettingsPage1(DisplayData *dst, DisplayData *src, bool flagCopy);
	bool dataCompareForSettingsPage2(DisplayData *dst, DisplayData *src, bool flagCopy);
	bool dataCompareForSettingsPage3(DisplayData *dst, DisplayData *src, bool flagCopy);

	Display();
	Display(const Display&);
	Display& operator=(Display&);

public:
	static Display* getInstance() {
        static Display instance;
        return &instance;
	}
	void updateData(DisplayData data);
	int getCurrentPage();
	void updateState();

};

#endif /* SRC_DISPLAY_H_ */
