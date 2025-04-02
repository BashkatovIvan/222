/*
 * ProtectiveShield.h
 *
 * Класс защитного экрана
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: alexander
 */

#ifndef SRC_PROTECTIVESHIELD_H_
#define SRC_PROTECTIVESHIELD_H_

#include "Sensors.h"

class ProtectiveShield {
	Sensors sensors;
public:
	ProtectiveShield();
	void updateState(Sensors *s);
};

#endif /* SRC_PROTECTIVESHIELD_H_ */
