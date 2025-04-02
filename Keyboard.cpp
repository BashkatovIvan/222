/*
 * Keyboard.cpp
 *
 *  Created on: 5 авг. 2022 г.
 *      Author: amaslov
 */

#include "Keyboard.h"
#include "usart.h"
#include <limits.h>

#define KEYBOARD_INTERRUPT_PRESCALER 5

void spiTxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
		Keyboard::getInstance()->privateSpiTxRxCpltCallback(hspi);
}

Keyboard::Keyboard()
{
	MX_SPI1_Init();
	HAL_SPI_RegisterCallback(&hspi1, HAL_SPI_TX_RX_COMPLETE_CB_ID, spiTxRxCpltCallback);
}

void Keyboard::privateSpiTxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	recv_data = ~spiRxBuf[0];

	if ((recv_data & KEY_MODE_ADJUSTMENT_CODE) == KEY_MODE_ADJUSTMENT_CODE)
		code |= KEY_MODE_ADJUSTMENT;

	if ((recv_data & KEY_MODE_ONE_STEP_CODE) == KEY_MODE_ONE_STEP_CODE)
		code |= KEY_MODE_ONE_STEP;

	if ((recv_data & KEY_MODE_CONTINUOUS_CODE) == KEY_MODE_CONTINUOUS_CODE)
		code |= KEY_MODE_CONTINUOUS;

	if ((recv_data & KEY_MODE_MANUAL_CODE) == KEY_MODE_MANUAL_CODE)
		code |= KEY_MODE_MANUAL;

	if ((recv_data & KEY_REMOTE_CONTROL_CODE) == KEY_REMOTE_CONTROL_CODE)
		code |= KEY_REMOTE_CONTROL;

	if ((recv_data & KEY_CANCEL_LUBRICANT_CODE) == KEY_CANCEL_LUBRICANT_CODE)
		code |= KEY_CANCEL_LUBRICANT;

	if ((recv_data & KEY_BLOWING_OFF_CODE) == KEY_BLOWING_OFF_CODE)
		code |= KEY_BLOWING_OFF;

	if ((recv_data & KEY_SWITCH_MENU_CODE) == KEY_SWITCH_MENU_CODE)
		code |= KEY_SWITCH_MENU;

	if ((recv_data & KEY_ZERO_VALUES_CODE) == KEY_ZERO_VALUES_CODE)
		code |= KEY_ZERO_VALUES;

	spiWrite();
}

void Keyboard::spiWrite()
{
	if (pos > 7)
	{
		scanKeys(code);
		code = 0;

		pos = 0;
		complete = true;
	}
	else
	{
		send_data = ~(1 << pos++);
		spiTxBuf[0] = send_data;

		HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive_DMA(&hspi1, spiTxBuf, spiRxBuf, 1);
	}
}

void Keyboard::handle()
{
	interruptPrescaler = interruptPrescaler >= (KEYBOARD_INTERRUPT_PRESCALER - 1) ? 0 : interruptPrescaler + 1;
	if (interruptPrescaler < (KEYBOARD_INTERRUPT_PRESCALER - 1))
		return;

	if (complete)
	{
		complete = false;
		spiWrite();
	}
}

void Keyboard::clearKeyboardState() {
	resKey.ready = false;
}

void Keyboard::scanKeys(uint16_t code)
{
	key.button = code;

	if (key.button && !prevKey.button)
		clearKeyboardState();

	if (key.button == prevKey.button && key.button)
	{
		set_key_count_off = 0;
		set_key_count_on = set_key_count_on < UINT16_MAX ? set_key_count_on + 1 : UINT16_MAX;

		resKey.button = key.button;
		if (set_key_count_on == ((keyboardMode == KEYBOARD_ITER) ? (80 / KEYBOARD_INTERRUPT_PRESCALER): (600 / KEYBOARD_INTERRUPT_PRESCALER)))
		{
			resKey.ready = true;

			if (keyboardMode == KEYBOARD_NORMAL) {
				resKey.longPress = true;
			}

			if (keyboardMode == KEYBOARD_ITER) {
				set_key_count_on = 0;
			}
		}
	}

	if (key.button == prevKey.button && !key.button)
	{
		set_key_count_off = set_key_count_off < UINT16_MAX ? set_key_count_off + 1 : UINT16_MAX;

		if (set_key_count_off > 2)
		{
			if (set_key_count_on > 2 && set_key_count_on < 600 / KEYBOARD_INTERRUPT_PRESCALER)
			{
				resKey.ready = true;
				resKey.longPress = false;
			}
			set_key_count_on = 0;
			keyboardMode = KEYBOARD_NORMAL;
		}
	}

	prevKey.button = key.button;
}

Button Keyboard::getKey()
{
	if (!resKey.ready)
		return Button();

	Button res;

	res.button = resKey.button;
	res.longPress = resKey.longPress;

	clearKeyboardState();

	return res;
}

