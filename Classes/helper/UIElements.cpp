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



void UIElements::loadCache(DeeWorld* scene){
	scene->_scoreNumber = CCLabelBMFont::create( "0", "Arial-num.fnt" );
	scene->_scoreNumber->setVisible(false);
}

void UIElements::runDestroyAcidEffect(cocos2d::CCLabelTTF* label){

   CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

   CCFiniteTimeAction* actionMove = CCMoveTo::create((float) 0.8,ccp(visibleSize.height, visibleSize.width));

   // Sebi: we have to add some dummy parameters otherwise it fails on Android
   CCSequence *readySequence = CCSequence::create(actionMove, NULL, NULL);
   label->runAction(readySequence);
}

// TODO make it run more fluently
void UIElements::displayScoreEffect(DeeWorld* scene, int scoring) {

	std::string strScoring = static_cast<ostringstream*>(&(ostringstream()<< scoring))->str();
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	scene->_scoreNumber = CCLabelBMFont::create( strScoring.c_str(), "Arial-num.fnt" );
	//scene->_scoreNumber->setScale(5);

	scene->_scoreNumber->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));

	// generates a nice color according to the score
	scene->_scoreNumber->setColor(UIElements::getColorForScore(scoring));

	scene->addChild(scene->_scoreNumber);


	CCActionInterval * tintToNumber;
	if (scoring > 0) {
		tintToNumber = CCTintTo::create(1.0, 0, 255, 0);

	} else if (scoring < 0) {
		tintToNumber = CCTintTo::create(1.0, 255, 0, 0);

	} else {
		tintToNumber = CCTintTo::create(1.5, 0, 0, 0);
	}




	scene->_scoreNumber->runAction(tintToNumber);
	CCActionInterval * scaleTo = CCScaleTo::create(1.0, 0.01);
	scene->_scoreNumber->runAction(scaleTo);

}

/**
 * TODO: is this still necessary? If not --> remove!
 */
void UIElements::fancyMovement(cocos2d::CCPoint pt,
		cocos2d::CCPoint playerPoint, DeeWorld* scene) {

	cocos2d::CCDrawNode *node = cocos2d::CCDrawNode::create();
	ccColor4F color;
	switch (arc4random() % 4) {
	case 0:
		color = ccc4FFromccc4B(ccc4(255, 0, 0, 255));
		break;
	case 1:
		color = ccc4FFromccc4B(ccc4(200, 0, 0, 255));
		break;
	case 2:
		color = ccc4FFromccc4B(ccc4(255, 0, 0, 255));
		break;
	case 3:
		color = ccc4FFromccc4B(ccc4(100, 0, 0, 255));
		break;
	case 4:
		color = ccc4FFromccc4B(
				ccc4(arc4random() % 256, arc4random() % 256, arc4random() % 256,
						255));
		break;
	}

	// TODO color is not working correct (same red!) - ideally we make a nice gradient
	CCLog("r: %f, g, :%f ,b: %f", color.r, color.g, color.b);

	CCPoint points[4];
	points[0] = ccp(0, 0);
	points[1] = ccp(0, std::abs(pt.x - playerPoint.x));
	points[2] = ccp(std::abs(pt.x - playerPoint.x),
			std::abs(pt.y - playerPoint.y));
	points[3] = ccp(std::abs(pt.y - playerPoint.y), 0);

	node->drawPolygon(points, 4, color, 0, ccc4FFromccc4B(ccc4(0, 0, 0, 255)));
	//node->drawDot(pt,5, color);
	node->setPosition(playerPoint.x, playerPoint.y);
	node->draw();

	while (scene->movementLines.size() >= 40) {
		CCDrawNode * nodeDel = scene->movementLines.front();
		scene->movementLines.pop();
		scene->removeChild(nodeDel);
		// TODO why is release not working??
		//nodeDel->release();
		//CC_SAFE_RELEASE(nodeDel);
	}
	scene->movementLines.push(node);

	scene->addChild(node, 1);

	CCActionInterval * fadeOut = CCFadeOut::create(1.0);
	CCActionInterval * move = CCMoveTo::create(2.0, ccp(-10, -10));
	//	CCActionInterval * tilt = CC
	node->runAction(move);
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

		CCLabelTTF * label = CCLabelTTF::create(str.c_str(), "Helvetica", 30,
				CCSizeMake(60, 30),
				kCCTextAlignmentRight);
		acid->_label = label;

		acid->_label->setPosition(ccp(visibleSize.width - 30, 30));
		acid->_label->setColor(ccc3(20, 20, 255));
		scene->addChild(acid->_label);

	}

	CCLog("moving AAs");

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

