/*
 * TBGTarget.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "TBGTarget.h"
#include "helper/MatrixHelper.h"
#include "helper/HelperFunctions.h"
#include "cocos2d.h"
USING_NS_CC;

TBGTarget::TBGTarget() {
	type = MatrixHelper::getRandomAminoAcid();
    //initialize sprite
    m_Sprite = cocos2d::CCSprite::create(
                            MatrixHelper::getImagePathForAcid(type),
                                               CCRectMake(0, 0, 50, 50));
    m_Sprite->setTag(1); //target
    m_Sprite->setZOrder(3);
    //default value: negative
    direction = -1;
}

TBGTarget::~TBGTarget() {
	delete this->m_Sprite;
}

int TBGTarget::getDirection() {
    return this->direction;
}
char TBGTarget::getType() {
    return this->type;
}
//d is an angle between 0 and 360 degree
void TBGTarget::setDirection(int d){
    this->direction = HelperFunctions::mod(d, 360);
}
//type is a valid one-letter IUPAC AminoAcid code
void TBGTarget::setType(char t){
    this->type = t;
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

	// TODO apply Matrix score
	return 1;

}
