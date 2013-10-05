/*
 * PositionHelper.cpp
 *
 *  Created on: Oct 5, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "PositionHelper.h"

PositionHelper::PositionHelper() {
	// TODO Auto-generated constructor stub

}

PositionHelper::~PositionHelper() {
	// TODO Auto-generated destructor stub
}

cocos2d::CCPoint PositionHelper::calculateNewPos(cocos2d::CCPoint pos,
		cocos2d::CCSize tarSize, cocos2d::CCSize winSize) {

	int edge = 5;
	int corner = 0;

	int sensi = 5;
	int x = int(pos.x);
	int y = int(pos.y);
	//left edge
	if (x <= sensi) {
		edge = 2;
	}
	// right
	if (x >= winSize.width - sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 0;
	}

	//upper edge
	if (y >= winSize.height - sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 3;

	}

	// lower edge
	if (y <= sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 2;
	}

	int endX = 0;
	int endY = 0;

	cocos2d::CCLog("edge type %d", edge);

	int randomX = winSize.width
			- int(rand()) % int((winSize.width - tarSize.width / 2));
	int randomY = winSize.height
			- int(rand()) % int((winSize.height - tarSize.height));

	if (edge > 4) {
		edge = rand() % 4;
	}

	switch (edge) {

	// right
	case 0:
		switch (rand() % 3) {
		case 0:
			endX = randomX / 2;
			endY = 0;
			break;
		case 1:
			endX = randomX / 2;
			endY = winSize.height;
			break;
		case 2:
			endX = 0;
			endY = randomY;
			break;
		}
		break;

		// lower
	case 1:
		switch (rand() % 4) {
		case 0:
		case 1:
			endX = randomX;
			endY = winSize.height;
			break;
		case 2:
			endX = tarSize.width / 2;
			endY = winSize.height - randomY / 2;
			break;
		case 3:
			endX = winSize.width;
			endY = winSize.height - randomY / 2;
			break;
		}

		break;

		// left
	case 2:

		switch (rand() % 3) {
		case 0:
			endX = winSize.width - randomX / 2;
			endY = 0;
			break;
		case 1:
			endX = winSize.width - randomX / 2;
			endY = winSize.height;
			break;
		case 2:
			endX = winSize.width;
			endY = randomY;
			break;
		}
		break;

		// upper
	case 3:
		switch (rand() % 4) {
		case 0:
		case 1:
			endX = randomX;
			endY = 0;
			break;
		case 2:
			endX = tarSize.width / 2;
			endY = randomY / 2;
			break;
		case 3:
			endX = winSize.width;
			endY = randomY / 2;
			break;
		}

		break;
	}

	return ccp(endX, endY);

}
