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
	
	int getScore(TBGTarget* other);
    void setDirection(int d);
    int getDirection();
    void setType(char t);
    char getType();
    
private:
	cocos2d::CCSprite * m_Sprite;
    //moving angle of the amino acid
    int direction;
    //IUPAC one letter code amino acid type
	char type;

};

#endif /* TBGTARGET_H_ */
