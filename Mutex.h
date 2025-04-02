/*
 * Mutex.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: alexander
 */

#ifndef MUTEX_H_
#define MUTEX_H_

class Mutex {
	bool flag = false;
public:
	void lock() {flag = true;};
	void unlock() {flag = false;};
	bool isLock() {return flag;};
};

#endif /* MUTEX_H_ */
