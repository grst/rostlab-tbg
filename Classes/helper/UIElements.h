/*
 * UIElements.h
 *
 *  Created on: Oct 14, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef UIELEMENTS_H_
#define UIELEMENTS_H_

#include "cocos2d.h"
#include "../screens/DeeWorldScene.h"
#include <queue>

class UIElements {
public:
	UIElements();
	virtual ~UIElements();

	static void fancyMovement(std::queue<cocos2d::CCDrawNode*> movementLines, cocos2d::CCPoint pt,cocos2d::CCPoint playerPoint, DeeWorld* scene);
};

#endif /* UIELEMENTS_H_ */
