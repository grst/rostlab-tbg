/*
 * PositionHelper.h
 *
 *  Created on: Oct 5, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef POSITIONHELPER_H_
#define POSITIONHELPER_H_

#include "cocos2d.h"

class PositionHelper {
public:
	PositionHelper();
	virtual ~PositionHelper();
	static  cocos2d::CCPoint calculateNewPos(cocos2d::CCPoint pos, cocos2d::CCSize tarSize, cocos2d::CCSize winSize);
};

#endif /* POSITIONHELPER_H_ */
