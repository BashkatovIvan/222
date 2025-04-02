/*
 * RTClock.h
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */

#ifndef SRC_RTCLOCK_H_
#define SRC_RTCLOCK_H_
#include "MCU.h"
#include <time.h>

class RTClock {
	MCU *mcu;

public:
	RTClock(MCU *mcu) : mcu(mcu) {clearRTCDate();}
	struct tm getRTCTime() {return mcu->getRTCTime();}
	void clearRTCDate() {mcu->clearRTCDate();}
};

#endif /* SRC_RTCLOCK_H_ */
