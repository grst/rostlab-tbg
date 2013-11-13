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
	CREATE_FUNC(AboutUsLayer);
private:
	cocos2d::CCMenu* levelMenu;
};

#endif /* AboutUsLayer_H_ */
