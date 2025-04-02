/*
 * Tests.h
 *
 *  Created on: 18 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_TESTS_H_
#define SRC_TESTS_H_

#include <stdint.h>

class Tests {
public:
	Tests();
	static void testSensors();
	static void printSensorsState(uint16_t state);

};

#endif /* SRC_TESTS_H_ */
