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
	void OnMenu(CCObject* pSender);
	CREATE_FUNC(ImpressumLayer);
	void getNextText();
private:
	cocos2d::CCMenu* levelMenu;
	int textCounter;
	cocos2d::CCLabelTTF * ttfDesc;
	cocos2d::CCLabelTTF * ttfTitle;
};

#endif /* ImpressumLayer_H_ */
