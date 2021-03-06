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
	void getPreviousImage();
	void getNextImage();
	CREATE_FUNC(AboutUsLayer);

	// The back key clicked
	 virtual void keyBackClicked();

	 static void buildCache();

private:
	cocos2d::CCMenu* levelMenu;
	int posImageCounter;
	cocos2d::CCSprite* pImage;

	void updateImg(int img, bool direction);
	static std::string getImg(int tag);

	void updatePaging(int img);
	std::vector<cocos2d::CCSprite *> pagings;

	static bool cacheLoaded;
};

#endif /* AboutUsLayer_H_ */
