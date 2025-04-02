/*
 * Tests.cpp
 *
 *  Created on: 18 авг. 2022 г.
 *      Author: amaslov
 */

#include "Tests.h"
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "usart.h"
#include "Sensors.h"


Tests::Tests() {

}

void Tests::testSensors()
{
	char buf[200] = {0};

	sprintf(buf, "%s", "Sensors: ");
	Sensors s = Sensors::getState();

	if (s.SQ1) {
		strcat(buf, "SQ1 ");
	}

	if (s.SQ2) {
		strcat(buf, "SQ2 ");
	}

	if (s.SQ3) {
		strcat(buf, "SQ3 ");
	}

	if (s.SQ11_SQ12) {
		strcat(buf, "SQ11_SQ12 ");
	}

	if (s.SQ13) {
		strcat(buf, "SQ13 ");
	}

	if (s.SQ14) {
		strcat(buf, "SQ14 ");
	}

	if (s.SQ15) {
		strcat(buf, "SQ15 ");
	}

	if (s.SQ16) {
		strcat(buf, "SQ16 ");
	}

	if (s.SB3_1) {
		strcat(buf, "SB3_1 ");
	}

	if (s.SB3_2_4_2) {
		strcat(buf, "SB3_2_4_2 ");
	}

	if (s.SB4_1) {
		strcat(buf, "SB4_1 ");
	}

	if (s.SB6_1) {
		strcat(buf, "SB6_1 ");
	}

	if (s.SB6_2) {
		strcat(buf, "SB6_2 ");
	}

	if (s.SL) {
		strcat(buf, "SL ");
	}

	if (s.SA) {
		strcat(buf, "SA ");
	}

	if (s.SP1) {
		strcat(buf, "SP1 ");
	}

	if (s.SP2) {
		strcat(buf, "SP2 ");
	}

	if (s.KM1_1) {
		strcat(buf, "KM1_1 ");
	}

	if (s.SB5) {
		strcat(buf, "SB5 ");
	}

	if (s.POWER) {
		strcat(buf, "POWER ");
	}

	if (s.YV1) {
		strcat(buf, "YV1 ");
	}

	if (s.YV2) {
		strcat(buf, "YV2 ");
	}

	strcat(buf, " \r\n");

	HAL_UART_Transmit(&huart2, (uint8_t *)buf, strlen(buf) + 1, 1000);
	HAL_Delay(100);
}

void Tests::printSensorsState(uint16_t state)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&state, 2, 1000);
}

