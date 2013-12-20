/*
 * UIElements.cpp
 *
 *  Created on: Oct 14, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "UIElements.h"
#include "../screens/DeeWorldScene.h"
#include <queue>

UIElements::UIElements() {
	// TODO Auto-generated constructor stub

}

UIElements::~UIElements() {
	// TODO Auto-generated destructor stub
}

void UIElements::loadCache(DeeWorld* scene) {
	scene->_scoreNumber = CCLabelBMFont::create("0", "Arial-num.fnt");
	scene->_scoreNumber->setVisible(false);
}

void UIElements::runDestroyAcidEffect(cocos2d::CCLabelTTF* label) {

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCFiniteTimeAction* actionMove = CCMoveTo::create((float) 0.8,
			ccp(80, visibleSize.height + 100));

	// Sebi: we have to add some dummy parameters otherwise it fails on Android
	CCSequence *readySequence = CCSequence::create(actionMove, NULL, NULL);

	//label->runAction(readySequence);

	// remove from display

	CCSequence* seq = CCSequence::create(actionMove,
	                                     CCCallFunc::create(label, callfunc_selector(CCSprite::removeFromParent)),
	                                     NULL);
	label->runAction(seq);


}

// TODO make it run more fluently
void UIElements::displayScoreEffect(DeeWorld* scene, int scoring) {

	std::string strScoring = static_cast<ostringstream*>(&(ostringstream()
			<< scoring))->str();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	scene->_scoreNumber = CCLabelBMFont::create(strScoring.c_str(),
			"Arial-num.fnt");
	//scene->_scoreNumber->setScale(5);

	scene->_scoreNumber->setPosition(
			ccp(visibleSize.width / 2, visibleSize.height / 2));

	// generates a nice color according to the score
	scene->_scoreNumber->setColor(UIElements::getColorForScore(scoring));

	scene->addChild(scene->_scoreNumber);

	CCFiniteTimeAction* tintToNumber = CCTintTo::create(1.5, 255, 0, 0);
	if (scoring > 0) {
		tintToNumber = CCTintTo::create(1.0, 0, 255, 0);

	} else if (scoring < 0) {
		tintToNumber = CCTintTo::create(1.0, 255, 0, 0);

	} else {
		tintToNumber = CCTintTo::create(1.5, 0, 0, 0);
	}



	CCSequence* seq = CCSequence::create(tintToNumber,
			CCCallFunc::create(scene->_scoreNumber, callfunc_selector(CCSprite::removeFromParent)),
					 NULL);


	CCActionInterval * scaleTo = CCScaleTo::create(1.0, 0.00);
	scene->_scoreNumber->runAction(scaleTo);

	scene->_scoreNumber->runAction(seq);

}

void UIElements::createNewAminoAcid(DeeWorld* scene) {

	char nextAcid = scene->getNextAminoAcid();
	BoardAcid * acid = new BoardAcid();

	if (nextAcid == '0') {

		CCLOG("No AA more available");
		//scene->gameEnd();
		//return;
	} else {

		acid->setAcid(nextAcid);

		char tt[] = "t";
		tt[0] = nextAcid;

		std::string str = string(tt);

		CCLog("adding Acid %c", nextAcid);

		// Create the actions
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

		CCLabelTTF * label = CCLabelTTF::create(str.c_str(), "Helvetica", 24,
				CCSizeMake(100, 30), kCCTextAlignmentRight);
		acid->_label = label;

		acid->_label->setPosition(ccp(80, visibleSize.height - 20));
		acid->_label->setColor(ccc3(255, 255, 255));
		scene->addChild(acid->_label);

	}

	CCLog("moving AAs");

	// color the first AA in red
	if (!scene->_code.empty()) {
		BoardAcid* elRed = scene->_code.front();
		CCFiniteTimeAction* actionMove = CCTintTo::create(1.5, 255, 0, 0);
		// Sebi: we have to add some dummy parameters otherwise it fails on Android
		CCSequence *readySequence = CCSequence::create(actionMove, NULL,
		NULL);
		elRed->_label->runAction(readySequence);
	}

	// move all elements a bit
	std::queue<BoardAcid*> tmpQueue;
	while (!scene->_code.empty()) {
		BoardAcid* el = scene->_code.front();
		tmpQueue.push(el);

		CCFiniteTimeAction* actionMove = CCMoveTo::create((float) 0.8,
				ccp(el->_label->getPositionX() - 20,
						el->_label->getPositionY()));

		// Sebi: we have to add some dummy parameters otherwise it fails on Android
		CCSequence *readySequence = CCSequence::create(actionMove, NULL,
		NULL);
		el->_label->runAction(readySequence);

		scene->_code.pop();
	}

	scene->_code = tmpQueue;

	if (nextAcid != '0') {
		scene->_code.push(acid);
	}

	CCLog("moving AAs finished");

	if (scene->_code.empty()) {
		scene->gameEnd();
		return;
	}
	CCLog("end of created AA");
}

ccColor3B UIElements::getColorForScore(int scoring) {
	ccColor3B colorScore = ccc3(0, 255, 0);
	if (scoring > 5) {
		colorScore = ccc3(0, 255, 0);
	}

	if (scoring < 5) {
		colorScore = ccc3(255, 0, 0);
	}

	switch (scoring) {
	case 5:
		colorScore = ccc3(0, 255, 0);
		break;
	case 4:
		colorScore = ccc3(0, 200, 0);
		break;
	case 3:
		colorScore = ccc3(0, 160, 0);
		break;
	case 2:
		colorScore = ccc3(0, 100, 0);
		break;
	case 1:
		colorScore = ccc3(0, 50, 0);
		break;
	case 0:
		colorScore = ccc3(0, 0, 255);
		break;
	case -1:
		colorScore = ccc3(50, 0, 0);
		break;
	case -2:
		colorScore = ccc3(100, 0, 0);
		break;
	case -3:
		colorScore = ccc3(160, 0, 0);
		break;
	case -4:
		colorScore = ccc3(200, 0, 0);
		break;
	case -5:
		colorScore = ccc3(255, 0, 0);
		break;
	}
	return colorScore;
}

