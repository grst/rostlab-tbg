/*
 * PositionHelper.h
 *
 *  Created on: Oct 5, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef POSITIONHELPER_H_
#define POSITIONHELPER_H_

#include "cocos2d.h"
#include "../ui_elements/AminoAcid.h"
#include "math.h"
#include "HelperFunctions.h"

class PositionHelper {
public:
	PositionHelper();
	virtual ~PositionHelper();
	static  cocos2d::CCPoint calculateNewPos(AminoAcid *target, cocos2d::CCSize winSize, int edge);
    static int detectEdge(cocos2d::CCPoint pos, cocos2d::CCSize winSize);
    static cocos2d::CCPoint polarToCartesian(double angle, double radius);
};

#endif /* POSITIONHELPER_H_ */
