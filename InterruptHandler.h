/*
 * InterruptHandler.h
 *
 *  Created on: Mar 12, 2022
 *      Author: alexander
 */

#ifndef INTERRUPTHANDLER_H_
#define INTERRUPTHANDLER_H_

class InterruptHandler {
public:
	virtual void handle() = 0;
	virtual ~InterruptHandler(){};
};

#endif /* INTERRUPTHANDLER_H_ */
