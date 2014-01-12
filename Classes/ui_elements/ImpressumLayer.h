/*
 * ImpressumLayer.h
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef ImpressumLayer_H_
#define ImpressumLayer_H_

#include "cocos2d.h"
#include "../helper/SoundEffectHelper.h"
#include <string>

class ImpressumLayer : public cocos2d::CCLayer {
public:
	ImpressumLayer();
	virtual ~ImpressumLayer();
	virtual bool init();
	void DrawHUD();
	void didSwipe(CCObject* pSender);
	void OnMenu(CCObject* pSender);
	CREATE_FUNC(ImpressumLayer);
	void getNextText();
	void getPreviousText();
private:
	cocos2d::CCMenu* levelMenu;
	int textCounter;
	cocos2d::CCLabelTTF * ttfDesc;
	cocos2d::CCLabelTTF * ttfTitle;
	void updateImg(int tag, bool dir);

	void updatePaging(int img);
	std::vector<cocos2d::CCSprite *> pagings;
};

#endif /* ImpressumLayer_H_ */
