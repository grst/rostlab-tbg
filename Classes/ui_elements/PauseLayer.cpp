/*
 * PauseLayer.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "PauseLayer.h"
#include "../helper/HelperFunctions.h"
#include "../screens/MainScreenScene.h"
#include "../screens/SettingsScreenScene.h"
#include "../screens/SplashScreenScene.h"
#include "../screens/DeeWorldScene.h"
#include "../Globals.h"

using namespace cocos2d;

PauseLayer::PauseLayer() {
	// TODO Auto-generated constructor stub

}

PauseLayer::~PauseLayer() {
	// TODO Auto-generated destructor stub
}


bool PauseLayer::init()
{

	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		this->setTouchEnabled(true);

		CCLabelTTF* pauseLabel = CCLabelTTF::create("Game paused", "Arial", 24);
		CC_BREAK_IF(! pauseLabel);


		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		pauseLabel->setPosition(ccp(pauseLabel->getContentSize().width/2,winSize.height - pauseLabel->getContentSize().height/2));
		// add this to the layer
		this->addChild(pauseLabel, 11);


		CCArray * menuIcons = CCArray::create();

		// settings
		CCMenuItemImage *pSettingsItem = CCMenuItemImage::create(
							"settings.png", "settings.png", this,
							menu_selector(PauseLayer::OnMenu));
		pSettingsItem->setPosition(ccp(winSize.width * 1/ 5+ pSettingsItem->getContentSize().width ,
						winSize.height -  pSettingsItem->getContentSize().height));
		pSettingsItem->setTag(3);
		menuIcons->addObject(pSettingsItem);

		//music
		CCMenuItemImage *	pMusicItemOn = CCMenuItemImage::create(
										"volume_on.png", "volume_on.png", this,
										menu_selector(PauseLayer::OnMenu));

		CCMenuItemImage*	pMusicItemOff = CCMenuItemImage::create(
										"volume_muted.png", "volume_muted.png", this,
										menu_selector(PauseLayer::OnMenu));

		CCMenuItemToggle * toggleMenu = CCMenuItemToggle::createWithTarget(this,menu_selector(PauseLayer::OnMenu), pMusicItemOn, pMusicItemOff, NULL );
		toggleMenu->setPosition(ccp(winSize.width / 5+ toggleMenu->getContentSize().width * 5 / 2 ,
				winSize.height - toggleMenu->getContentSize().height));
		toggleMenu->setTag(2);
		// add current value
		if( !	cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey(
					"music_enable", true)){
			toggleMenu->setSelectedIndex(1);
		}
		menuIcons->addObject(toggleMenu);



		// main menu
		CCMenuItemImage *pMainMenu = CCMenuItemImage::create(
							"mainmenu.png", "mainmenu.png", this,
							menu_selector(PauseLayer::OnMenu));
		pMainMenu->setPosition(ccp(winSize.width * 1/5 + pMainMenu->getContentSize().width   * 9 / 2,
						winSize.height - pMainMenu->getContentSize().height));
		pMainMenu->setTag(1);
		menuIcons->addObject(pMainMenu);

		// close app
		CCMenuItemImage *pCloseApp = CCMenuItemImage::create(
							"closeapp.png", "closeapp.png", this,
							menu_selector(PauseLayer::OnMenu));
		pCloseApp->setPosition(ccp(winSize.width * 1/ 5+ pCloseApp->getContentSize().width  * 13 / 2,
						winSize.height - pCloseApp->getContentSize().height));
		pCloseApp->setTag(4);
		menuIcons->addObject(pCloseApp);



		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(CCPointZero);

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 11);



		CCLayerGradient* layer3 = CCLayerGradient::create();
		layer3->setContentSize(CCSizeMake(winSize.width, winSize.height));
		layer3->setPosition(ccp(0,0));
		layer3->setStartColor(ccc3(100, 100, 100));
		layer3->setEndColor(ccc3(120, 120, 120));
		layer3->setStartOpacity(160);
		layer3->setEndOpacity(240);
		ccBlendFunc blend;
		blend.src = GL_SRC_ALPHA;
		blend.dst = GL_ONE_MINUS_SRC_ALPHA;
		layer3->setBlendFunc(blend);
		addChild(layer3, 10);


		bRet = true;
	} while (0);

	return bRet;


}

void PauseLayer::OnMenu(CCObject* pSender)
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
	case 4:
		// simulate close Button clicked
		DeeWorld *layer = (DeeWorld*) this->getParent();
		layer->keyBackClicked();
		break;
	}
}

