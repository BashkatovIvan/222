/*
 * PressErrorHandler.cpp
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */

#include "Press.h"

PressState Press::errorHandler(Sensors *s)
{
	slideBlock->stop();
	return state;
}


