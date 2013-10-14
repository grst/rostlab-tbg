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

void UIElements::fancyMovement(std::queue<cocos2d::CCDrawNode*> movementLines,
		cocos2d::CCPoint pt, cocos2d::CCPoint playerPoint, DeeWorld* scene) {

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

