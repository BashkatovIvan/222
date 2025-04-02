/*
 * DisplayUARTRoutines.cpp
 *
 *  Created on: 20 авг. 2022 г.
 *      Author: alexander
 */
#include "Display.h"
#include "DisplayUARTRoutines.h"
#include "Utils.h"
#include "string.h"
#include "Globals.h"

extern DMA_HandleTypeDef hdma_usart2_rx;


void uartRxEventCallback(struct __UART_HandleTypeDef *huart, uint16_t Pos)
{
	if (huart->Instance == USART2)
		Display::getInstance()->privateUartRxEventCallback(huart, Pos);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)
		Display::getInstance()->privateHAL_UART_ErrorCallback(huart);
}


void Display::privateHAL_UART_ErrorCallback(UART_HandleTypeDef *huarts)
{

}

void Display::privateUartRxEventCallback(struct __UART_HandleTypeDef *huart, uint16_t Pos)
{
	hdrReply * hdr;
	uint8_t pos = 0;
	int sz;

	if (Pos > sizeof(uart_rx_buffer))
		return;

	if ((curRxPos + Pos) >= (int)sizeof(uart_rx_buffer))
		curRxPos = 0;

	memcpy(uart_rx_buffer + curRxPos, uart_rx_dma_buffer, Pos);
	curRxPos += Pos;

	for (;;)
	{
		if ((int)(pos + sizeof(hdrReply)) > curRxPos)
			break;

		hdr = (hdrReply*)(&uart_rx_buffer[pos]);

		if (hdr->header == FRAME_HEADER)
		{
			sz = 0;
			if (hdr->instruction == WRITE_INSTRUCTION)
			{
				if (0 == (sz = handleWriteReply(&uart_rx_buffer[pos], curRxPos - pos)))
					break;

				pos += sz;
			}
			else if (hdr->instruction == READ_AND_TOUCH_INSTRUCTION)
			{
				if (0 == (sz = handleReadAndTouchReply(&uart_rx_buffer[pos], curRxPos - pos)))
					break;

				pos += sz;
			}
			else
			{
				pos++;
			}
		}
		else
		{
			pos++;
		}
	}

	pos = pos > sizeof(uart_rx_buffer) ? sizeof(uart_rx_buffer) : pos;

	if (pos > 0)
	{
		memmove(uart_rx_buffer, uart_rx_buffer + pos, sizeof(uart_rx_buffer) - pos);
		memset(uart_rx_buffer + sizeof(uart_rx_buffer) - pos, 0x00, pos);
		curRxPos = curRxPos - pos;
	}

	curRxPos = curRxPos <= 0 ? 0 : curRxPos;
	receiveData();
}

int Display::receiveData()
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, uart_rx_dma_buffer, sizeof(uart_rx_dma_buffer));
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);

	return 0;
}

int Display::handleWriteReply(uint8_t *buf, uint8_t limitPos)
{
	int trimBytes = 0;

	if (limitPos >= sizeof(hdrWriteReply))
	{
		writeOk = true;
		trimBytes = sizeof(hdrWriteReply);
	}

	return trimBytes;
}

int Display::handleReadAndTouchReply(uint8_t *buf, uint8_t limitPos)
{
	int trimBytes = 0;
	hdrReadAndTouchReply* hdr = (hdrReadAndTouchReply*)buf;

	if (limitPos >= sizeof(hdrReadAndTouchReply))
	{
		if (limitPos >= hdr->len + 3)
		{
			switch(htons(hdr->addr))
			{
				case VP_ADDR_PIC_NOW:
				{
					curPage = *((uint8_t*)(buf + 8));
					break;
				}
				default:
					break;

			}
			trimBytes = hdr->len + 3;
		}
	}
	return trimBytes;
}



