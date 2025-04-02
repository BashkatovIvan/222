/*
 * Sensors.h
 *
 * Класс опроса концевиков
 *
 *  Created on: Aug 8, 2022
 *      Author: amaslov
 */

#ifndef SRC_SENSORS_H_
#define SRC_SENSORS_H_
#include "InterruptHandler.h"
#include "stdint.h"


class Sensors
{
	static const uint16_t SQ1_CODE;
	static const uint16_t SQ2_CODE;
	static const uint16_t SQ3_CODE;
	static const uint16_t SQ11_SQ12_CODE;
	static const uint16_t SQ13_CODE;
	static const uint16_t SQ14_CODE;
	static const uint16_t SQ15_CODE;
	static const uint16_t SQ16_CODE;
	static const uint16_t SB3_1_CODE;
	static const uint16_t SB3_2_4_2_CODE;
	static const uint16_t SB4_1_CODE;
	static const uint16_t SB5_CODE;
	static const uint16_t SB6_1_CODE;
	static const uint16_t SB6_2_CODE;
	static const uint16_t SL_CODE;
	static const uint16_t SA_CODE;
	static const uint16_t SP1_CODE;
	static const uint16_t SP2_CODE;
	static const uint16_t KM1_1_CODE;
	static const uint16_t YV1_CODE;
	static const uint16_t YV2_CODE;
	static const uint16_t POWER_CODE;

public:
	bool SQ1 = false;
	bool SQ2 = false;
	bool SQ3 = false;
	bool SQ11_SQ12 = false;
	bool SQ13 = false;
	bool SQ14 = false;
	bool SQ15 = false;
	bool SQ16 = false;
	bool SB3_1 = false;
	bool SB3_2_4_2 = false;
	bool SB4_1 = false;
	bool SB5 = false;
	bool SB6_1 = false;
	bool SB6_2 = false;
	bool SL = false;
	bool SA = false;
	bool SP1 = false;
	bool SP2 = false;
	bool KM1_1 = false;
	bool YV1 = false;
	bool YV2 = false;
	bool POWER = false;
	static Sensors getState();
};

#endif /* SRC_SENSORS_H_ */
