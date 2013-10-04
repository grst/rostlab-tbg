/*
 * TBGTarget.h
 *
 *  Created on: Oct 3, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef TBGTARGET_H_
#define TBGTARGET_H_

#include "cocos2d.h"

class TBGTarget {
private:
	cocos2d::CCSprite * m_Sprite;

public:
	TBGTarget();
	~TBGTarget();
   void setSprite(cocos2d::CCSprite* sprite);
   cocos2d::CCSprite * getSprite( );
   float velX;
   float velY;
};


#endif /* TBGTARGET_H_ */
