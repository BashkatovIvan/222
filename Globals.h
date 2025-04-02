/*
 * Globals.h
 *
 *  Created on: 26 авг. 2022 г.
 *      Author: alexander
 */

#ifndef INCLUDE_GLOBALS_H_
#define INCLUDE_GLOBALS_H_

#include "DataBase.h"
#include "RTClock.h"
#include "HMI.h"

#define PROGRAM_VERSION 39
//#define DEBUG_VERSION

extern DataBase *dataBase;
extern HMI hmi;
extern RTClock *rtClock;


#endif /* INCLUDE_GLOBALS_H_ */
