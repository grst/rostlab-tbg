/*
 * BoardAcid.h
 *
 *  Created on: Oct 6, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef BOARDACID_H_
#define BOARDACID_H_

#include "cocos2d.h"

class BoardAcid {
public:
	BoardAcid();
	virtual ~BoardAcid();
	void setAcid(char c);
	cocos2d::CCLabelTTF* _label;
	char acid;
};

#endif /* BOARDACID_H_ */
