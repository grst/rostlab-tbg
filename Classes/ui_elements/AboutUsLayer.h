/*
 * AboutUsLayer.h
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef AboutUsLayer_H_
#define AboutUsLayer_H_

#include "cocos2d.h"
#include "../helper/SoundEffectHelper.h"


class AboutUsLayer : public cocos2d::CCLayer {
public:
	AboutUsLayer();
	virtual ~AboutUsLayer();
	virtual bool init();
	void DrawHUD();
	void OnMenu(CCObject* pSender);
	void didSwipe(CCObject* pSender);
	void initButt();
	void getNextImage();
	CREATE_FUNC(AboutUsLayer);

	// The back key clicked
	 virtual void keyBackClicked();

private:
	cocos2d::CCMenu* levelMenu;
	int posImageCounter;
	cocos2d::CCSprite* pImage;
	cocos2d::CCLabelTTF*  pTitle;
};

#endif /* AboutUsLayer_H_ */
