/*
 * HMI.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_HMI_H_
#define SRC_HMI_H_

#include "Display.h"
#include "Keyboard.h"

class HMI {
public:
	Display *display;
	Keyboard *keyboard;
};

#endif /* SRC_HMI_H_ */
