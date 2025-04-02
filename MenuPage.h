/*
 * MenuPage.h
 *
 *  Created on: 16 авг. 2022 г.
 *      Author: amaslov
 */

#ifndef SRC_MENUPAGE_H_
#define SRC_MENUPAGE_H_

class MenuPage {

public:
	virtual void handle(int curPage) = 0;
};

#endif /* SRC_MENUPAGE_H_ */
