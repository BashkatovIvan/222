/*
 * Keyboard.h
 *
 *  Created on: 5 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_KEYBOARD_H_
#define SRC_KEYBOARD_H_

#include "spi.h"
#include "InterruptHandler.h"

#define KEY_MODE_ADJUSTMENT_CODE 	0xC0
#define	KEY_MODE_ONE_STEP_CODE		0x82
#define KEY_MODE_CONTINUOUS_CODE 	0x44
#define KEY_MODE_MANUAL_CODE 		0x24
#define KEY_REMOTE_CONTROL_CODE 	0x30
#define KEY_CANCEL_LUBRICANT_CODE 	0x18
#define KEY_BLOWING_OFF_CODE 		0x0C
#define KEY_SWITCH_MENU_CODE 		0x06
#define KEY_ZERO_VALUES_CODE 		0x81

#define KEY_MODE_ADJUSTMENT 		1
#define	KEY_MODE_ONE_STEP 			2
#define KEY_MODE_CONTINUOUS			4
#define KEY_MODE_MANUAL 			8
#define KEY_REMOTE_CONTROL 			16
#define KEY_CANCEL_LUBRICANT 		32
#define KEY_BLOWING_OFF 			64
#define KEY_SWITCH_MENU 			128
#define KEY_ZERO_VALUES 			256

class Button
{
public:
	uint16_t button;
	bool longPress;
	bool ready;
	Button() {
		button = 0;
		longPress = false;
		ready = false;
	};
};

enum KeyboardMode {
	KEYBOARD_NORMAL,
	KEYBOARD_ITER
};

void spiTxRxCpltCallback(SPI_HandleTypeDef *hspi);

class Keyboard : public InterruptHandler
{
	void handle() override;
	volatile bool complete = true;
	volatile int8_t interruptPrescaler = 0;
	volatile uint16_t code = 0;
	volatile uint8_t pos = 0;
	uint8_t spiTxBuf[1];
	uint8_t spiRxBuf[1];
	uint8_t recv_data = 0;
	volatile uint8_t send_data = 0;
	void spiWrite();
	void scanKeys(uint16_t code);

	KeyboardMode keyboardMode = KEYBOARD_NORMAL;

	volatile uint16_t set_key_count_on = 0;
	volatile uint16_t set_key_count_off = 0;
	volatile Button resKey;
	volatile Button key;
	volatile Button prevKey;

	void clearKeyboardState();
	friend void spiTxRxCpltCallback(SPI_HandleTypeDef *hspi);
	void privateSpiTxRxCpltCallback(SPI_HandleTypeDef *hspi);


	Keyboard();
	Keyboard(const Keyboard&);
	Keyboard& operator=(Keyboard&);

public:
	static Keyboard* getInstance() {
        static Keyboard instance;
        return &instance;
	}
	Button getKey();
	void setMode(KeyboardMode mode)
	{
		keyboardMode = mode;
		set_key_count_on = 0;
		set_key_count_off = 0;
		clearKeyboardState();
	}
	KeyboardMode getMode() {
		return keyboardMode;
	}

};

#endif /* SRC_KEYBOARD_H_ */
