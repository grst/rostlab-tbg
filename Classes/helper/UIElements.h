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
#include <string>

class UIElements {
public:
	UIElements();
	virtual ~UIElements();

	static void fancyMovement(cocos2d::CCPoint pt,cocos2d::CCPoint playerPoint, DeeWorld* scene);
	static void createNewAminoAcid(DeeWorld* scene);

	static void loadCache(DeeWorld* scene);
	static void displayScoreEffect(DeeWorld* scene, int score);
	static void runDestroyAcidEffect(cocos2d::CCLabelTTF* label);
	static ccColor3B getColorForScore(int scoring);
};

#endif /* UIELEMENTS_H_ */
