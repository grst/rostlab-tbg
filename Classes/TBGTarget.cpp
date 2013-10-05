/*
 * TBGTarget.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "TBGTarget.h"
#include "helper/MatrixHelper.h"

TBGTarget::TBGTarget() {
	velX = rand() % 4 + 1;
	velY = rand() % 4 + 1;
	acidType = MatrixHelper::getRandomAminoAcid();
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

/*
 * calculates the BLOSUM score
 */
int TBGTarget::getScore(TBGTarget * other) {

	// TODO compare
	if (other->acidType == this->acidType) {
		return 10;
	}

	return 1;

}
