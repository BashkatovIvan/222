/*
 * MCU.h
 *
 *  Created on: 2 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef MCU_H_
#define MCU_H_

#include <list>
#include "InterruptHandler.h"
#include "tim.h"
#include <time.h>

class MCU {

	static std::list<InterruptHandler*> tim2Callbacks;
	static std::list<InterruptHandler*> tim21Callbacks;
public:
	MCU();

	void tim2Start();
	void tim2Stop();
	static void tim2Callback(TIM_HandleTypeDef *htim);
	void tim2AddCallback(InterruptHandler*);

	void tim21Start();
	void tim21Stop();
	static void tim21Callback(TIM_HandleTypeDef *htim);
	void tim21AddCallback(InterruptHandler*);
	void IWDGRefresh();
	bool isIWDGRestart();
	struct tm getRTCTime();
	void clearRTCDate();
	void IWDGRun();
};

#endif /* MCU_H_ */
