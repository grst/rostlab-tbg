/*
 * PositionHelper.cpp
 *
 *  Created on: Oct 5, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "PositionHelper.h"
#include "AminoAcid.h"

USING_NS_CC;

PositionHelper::PositionHelper() {
	// TODO Auto-generated constructor stub

}

PositionHelper::~PositionHelper() {
	// TODO Auto-generated destructor stub
}

cocos2d::CCPoint PositionHelper::calculateNewPos(AminoAcid *aa, cocos2d::CCSize winSize) {

    cocos2d::CCPoint pos = aa->getPosition();
    cocos2d::CCSize tarSize = aa->getContentSize();
    int direction = aa->getDirection();
	//int edge = 5;
	//int corner = 0;
    CCLog("current direction: %i", direction);
    
    if(direction != -1) {
        //mirror at current edge.
        int edge = detectEdge(pos, winSize);
        if(edge%2 == 0) {
            //upper or lower
            direction = HelperFunctions::mod(-direction, 360);
        } else {
            //left or right
            direction = HelperFunctions::mod(180-direction, 360);
        }
        aa->setDirection(direction);
    } else {
        //set initial direction
        //TODO: possibly there are issues when the AA is placed at an edge and would fly into the off
        //direction = rand()%360;
        direction = 30;
        aa->setDirection(direction);
    }
    
    float radius = sqrt(pow(winSize.height, 2) + pow(winSize.width, 2));
    CCLog("r=%f,a=%i", radius, direction);
    CCPoint coords = polarToCartesian(direction, (double) radius);
    CCLog("current position: x=%f,y=%f", pos.x, pos.y);
    CCLog("target position: x=%f,y=%f", coords.x, coords.y);
    //add destination to current position.
    return ccp(coords.x + pos.x, coords.y + pos.y);


//	int endX = 0;
//	int endY = 0;
//
//	cocos2d::CCLog("edge type %d", edge);

//	int randomX = winSize.width
//			- int(rand()) % int((winSize.width - tarSize.width / 2));
//	int randomY = winSize.height
//			- int(rand()) % int((winSize.height - tarSize.height));
//
//	if (edge > 4) {
//		edge = rand() % 4;
//	}
//
//	switch (edge) {
//
//	// right
//	case 0:
//		switch (rand() % 3) {
//		case 0:
//			endX = randomX / 2;
//			endY = 0;
//			break;
//		case 1:
//			endX = randomX / 2;
//			endY = winSize.height;
//			break;
//		case 2:
//			endX = 0;
//			endY = randomY;
//			break;
//		}
//		break;
//
//		// lower
//	case 1:
//		switch (rand() % 4) {
//		case 0:
//		case 1:
//			endX = randomX;
//			endY = winSize.height;
//			break;
//		case 2:
//			endX = tarSize.width / 2;
//			endY = winSize.height - randomY / 2;
//			break;
//		case 3:
//			endX = winSize.width;
//			endY = winSize.height - randomY / 2;
//			break;
//		}
//
//		break;
//
//		// left
//	case 2:
//
//		switch (rand() % 3) {
//		case 0:
//			endX = winSize.width - randomX / 2;
//			endY = 0;
//			break;
//		case 1:
//			endX = winSize.width - randomX / 2;
//			endY = winSize.height;
//			break;
//		case 2:
//			endX = winSize.width;
//			endY = randomY;
//			break;
//		}
//		break;
//
//		// upper
//	case 3:
//		switch (rand() % 4) {
//		case 0:
//		case 1:
//			endX = randomX;
//			endY = 0;
//			break;
//		case 2:
//			endX = tarSize.width / 2;
//			endY = randomY / 2;
//			break;
//		case 3:
//			endX = winSize.width;
//			endY = randomY / 2;
//			break;
//		}
//
//		break;
//	}
//
//	return ccp(endX, endY);

}

CCPoint PositionHelper::polarToCartesian(double angle, double radius) {
    angle = HelperFunctions::radian(angle);
    return ccp( cos(angle)*radius,  sin(angle)*radius);
}

int PositionHelper::detectEdge(cocos2d::CCPoint pos,cocos2d::CCSize winSize) {
    //////////
    //detect Position
	int sensi = 5;
    int edge = 5;
    int corner = 0;
	int x = int(pos.x);
	int y = int(pos.y);
	//left edge
	if (x <= sensi) {
		edge = 1;
	}
	// right
	if (x >= winSize.width - sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 3;
	}

	//upper edge
	if (y >= winSize.height - sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 2;

	}

	// lower edge
	if (y <= sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 0;
	}
    
    return edge;

}
