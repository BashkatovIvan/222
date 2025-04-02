/*
 * ProtectiveShield.cpp
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: alexander
 */

#include "ProtectiveShield.h"

ProtectiveShield::ProtectiveShield() {
}

void ProtectiveShield::updateState(Sensors *s)
{
	sensors = *s;
}

