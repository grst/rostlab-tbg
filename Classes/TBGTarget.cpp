/*
 * TBGTarget.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "TBGTarget.h"

TBGTarget::TBGTarget() {

}

TBGTarget::~TBGTarget() {
	delete this->m_Sprite;
}

void TBGTarget::setSprite(cocos2d::CCSprite* sprite) {
	this->m_Sprite = sprite;
}
cocos2d::CCSprite * TBGTarget::getSprite() {
	return this->m_Sprite;
}

