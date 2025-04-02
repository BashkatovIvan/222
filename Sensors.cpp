/*
 * Sensors.cpp
 *
 *  Created on: Aug 8, 2022
 *      Author: amaslov
 */

#include "Sensors.h"
#include "Tests.h"
#include "Utils.h"
#include "main.h"
#include "gpio.h"
#include "usart.h"

const uint16_t Sensors::SQ1_CODE = 0x0101;
const uint16_t Sensors::SQ2_CODE = 0x0102;
const uint16_t Sensors::SQ3_CODE = 0x0104;
const uint16_t Sensors::SQ11_SQ12_CODE = 0x0801;
const uint16_t Sensors::SQ13_CODE = 0x0404;
const uint16_t Sensors::SQ14_CODE = 0x1001;
const uint16_t Sensors::SQ15_CODE = 0x1004;
const uint16_t Sensors::SQ16_CODE = 0x0408;
const uint16_t Sensors::SB3_1_CODE = 0x0108;
const uint16_t Sensors::SB3_2_4_2_CODE = 0x0202;
const uint16_t Sensors::SB4_1_CODE = 0x0201;
const uint16_t Sensors::SB6_1_CODE = 0x0204;
const uint16_t Sensors::SB6_2_CODE = 0x0208;
const uint16_t Sensors::SB5_CODE = 0x1008;
const uint16_t Sensors::SL_CODE = 0x0402;
const uint16_t Sensors::SA_CODE = 0x2002;
const uint16_t Sensors::SP1_CODE = 0x1002;
const uint16_t Sensors::SP2_CODE = 0x0401;
const uint16_t Sensors::KM1_1_CODE = 0x2001;
const uint16_t Sensors::YV1_CODE = 0x0802;
const uint16_t Sensors::YV2_CODE = 0x0804;
const uint16_t Sensors::POWER_CODE = 0x0808;

Sensors Sensors::getState()
{
	uint8_t pos = 0;
	uint16_t state = 0;

	Sensors s;

	while(pos < 6)
	{
		state = 0;

		((uint8_t *)(&state))[1] = 1 << pos;
		HAL_GPIO_WritePin(GPIOB, OUT_33_Pin|OUT_34_Pin|OUT_35_Pin|OUT_36_Pin|OUT_8_Pin|OUT_5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, OUT_33_Pin << pos, GPIO_PIN_RESET);

		nops(5);

		((uint8_t *)(&state))[0] |= (HAL_GPIO_ReadPin(IN_37_GPIO_Port, IN_37_Pin) == GPIO_PIN_RESET ? 1 : 0) << 0;
		((uint8_t *)(&state))[0] |= (HAL_GPIO_ReadPin(IN_38_GPIO_Port, IN_38_Pin) == GPIO_PIN_RESET ? 1 : 0) << 1;
		((uint8_t *)(&state))[0] |= (HAL_GPIO_ReadPin(IN_39_GPIO_Port, IN_39_Pin) == GPIO_PIN_RESET ? 1 : 0) << 2;
		((uint8_t *)(&state))[0] |= (HAL_GPIO_ReadPin(IN_40_GPIO_Port, IN_40_Pin) == GPIO_PIN_RESET ? 1 : 0) << 3;


		s.SQ1 = (state & SQ1_CODE) == SQ1_CODE ? true : s.SQ1;
		s.SQ2 = (state & SQ2_CODE) == SQ2_CODE ? true : s.SQ2;
		s.SQ3 = (state & SQ3_CODE) == SQ3_CODE ? true : s.SQ3;
		s.SQ11_SQ12 = (state & SQ11_SQ12_CODE) == SQ11_SQ12_CODE ? true : s.SQ11_SQ12;
		s.SQ13 = (state & SQ13_CODE) == SQ13_CODE ? true : s.SQ13;
		s.SQ14 = (state & SQ14_CODE) == SQ14_CODE ? true : s.SQ14;
		s.SQ15 = (state & SQ15_CODE) == SQ15_CODE ? true : s.SQ15;
		s.SQ16 = (state & SQ16_CODE) == SQ16_CODE ? true : s.SQ16;
		s.SB3_1 = (state & SB3_1_CODE) == SB3_1_CODE ? true : s.SB3_1;
		s.SB3_2_4_2 = (state & SB3_2_4_2_CODE) == SB3_2_4_2_CODE ? true : s.SB3_2_4_2;
		s.SB4_1 = (state & SB4_1_CODE) == SB4_1_CODE ? true : s.SB4_1;
		s.SB5 = (state & SB5_CODE) == SB5_CODE ? true : s.SB5;
		s.SB6_1 = (state & SB6_1_CODE) == SB6_1_CODE ? true : s.SB6_1;
		s.SB6_2 = (state & SB6_2_CODE) == SB6_2_CODE ? true : s.SB6_2;
		s.SL = (state & SL_CODE) == SL_CODE ? true : s.SL;
		s.SA = (state & SA_CODE) == SA_CODE ? true : s.SA;
		s.SP1 = (state & SP1_CODE) == SP1_CODE ? true : s.SP1;
		s.SP2 = (state & SP2_CODE) == SP2_CODE ? true : s.SP2;
		s.KM1_1 = (state & KM1_1_CODE) == KM1_1_CODE ? true : s.KM1_1;
		s.YV1 = (state & YV1_CODE) == YV1_CODE ? true : s.YV1;
		s.YV2 = (state & YV2_CODE) == YV2_CODE ? true : s.YV2;
		s.POWER = (state & POWER_CODE) == POWER_CODE ? true : s.POWER;

		//Tests::printSensorsState(state);

		pos++;
	}

	return s;
}


