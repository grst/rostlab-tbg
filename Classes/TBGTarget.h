/*
 * TBGTarget.h
 *
 *  Created on: Oct 3, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef TBGTARGET_H_
#define TBGTARGET_H_

#include "cocos2d.h"
#include "cocoa/CCObject.h"

//NS_CC_BEGIN
//CC_DLL

class  TBGTarget : public cocos2d::CCObject {
public:
	TBGTarget();
	~TBGTarget();
	void setSprite(cocos2d::CCSprite* sprite);
	cocos2d::CCSprite * getSprite();
	float velX;
	float velY;
private:
	cocos2d::CCSprite * m_Sprite;

};

#endif /* TBGTARGET_H_ */
