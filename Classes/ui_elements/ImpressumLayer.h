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


class ImpressumLayer : public cocos2d::CCLayer {
public:
	ImpressumLayer();
	virtual ~ImpressumLayer();
	virtual bool init();
	void DrawHUD();
	void OnMenu(CCObject* pSender);
	CREATE_FUNC(ImpressumLayer);
private:
	cocos2d::CCMenu* levelMenu;
};

#endif /* ImpressumLayer_H_ */
