/*
 * ImpressumLayer.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "ImpressumLayer.h"
#include "../helper/HelperFunctions.h"
#include "../screens/MainScreenScene.h"
#include "../screens/SettingsScreenScene.h"
#include "../Globals.h"

using namespace cocos2d;

ImpressumLayer::ImpressumLayer() {
	// TODO Auto-generated constructor stub

}

ImpressumLayer::~ImpressumLayer() {
	// TODO Auto-generated destructor stub
}


bool ImpressumLayer::init()
{

	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		this->setTouchEnabled(true);

		CCLabelTTF* pauseLabel = CCLabelTTF::create("About us", "Arial", 24);
		CC_BREAK_IF(! pauseLabel);


		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		pauseLabel->setPosition(ccp(pauseLabel->getContentSize().width/2,winSize.height - pauseLabel->getContentSize().height/2));

		CCArray * menuIcons = CCArray::create();




		// main menu
		CCMenuItemImage *pMainMenu = CCMenuItemImage::create(
							"ic_action_gamepad.png", "ic_action_gamepad.png", this,
							menu_selector(ImpressumLayer::OnMenu));
		pMainMenu->setPosition(ccp(winSize.width / 5+ pMainMenu->getContentSize().width  +200,
						winSize.height - pMainMenu->getContentSize().height));
		pMainMenu->setTag(1);
		menuIcons->addObject(pMainMenu);



		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(CCPointZero);

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 11);


		// add this to the layer
		this->addChild(pauseLabel, 11);


		CCLayerGradient* layer3 = CCLayerGradient::create();
		layer3->setContentSize(CCSizeMake(winSize.width, winSize.height));
		layer3->setPosition(ccp(0,0));
		layer3->setStartColor(ccc3(100, 100, 100));
		layer3->setEndColor(ccc3(120, 120, 120));
		layer3->setStartOpacity(255);
		layer3->setEndOpacity(255);
		ccBlendFunc blend;
		blend.src = GL_SRC_ALPHA;
		blend.dst = GL_ONE_MINUS_SRC_ALPHA;
		layer3->setBlendFunc(blend);
		addChild(layer3, 10);


		bRet = true;
	} while (0);

	return bRet;


}

void ImpressumLayer::OnMenu(CCObject* pSender)
{



	CCMenuItem* pMenuItem = (CCMenuItemImage *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("PauseMenuItem  %d", tag);

	CCScene *pScene1;
	switch(tag){
	case 1:
		 SoundEffectHelper::playClickSound();
		 pScene1 = MainScreenScene::create();
		 CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pScene1));
		break;
	case 2:
		// toggle music
		HelperFunctions::toggleMusic();
		SoundEffectHelper::playLevelBackgroundMusic(Globals::level);
		SoundEffectHelper::pauseBackgroundMusic();
		if(SoundEffectHelper::isSoundEnabled()){
			SoundEffectHelper::playClickSound();
		}
		break;
	case 3:
		SoundEffectHelper::playClickSound();
		pScene1 = SettingsScreenScene::create();
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pScene1));
		break;
	}

//	CCScene* nextScene = MainMenu::scene();
	//CCDirector::sharedDirector()->replaceScene(nextScene);
}

