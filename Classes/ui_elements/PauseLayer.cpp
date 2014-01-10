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
#include "../ui_elements/cc-extensions/CCGestureRecognizer/CCSwipeGestureRecognizer.h"

using namespace cocos2d;


PauseLayer::PauseLayer() {
	// TODO Auto-generated constructor stub

}

PauseLayer::~PauseLayer() {
	// TODO Auto-generated destructor stub
}

cocos2d::CCLayer* PauseLayer::create(int level) {
	PauseLayer *layer = PauseLayer::create();
	layer->level = level;
	return layer;
}

bool PauseLayer::init() {

	bool bRet = false;
	do {
		CC_BREAK_IF(!CCLayer::init());
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->setTouchEnabled(true);

		CCLabelTTF* pauseLabel = CCLabelTTF::create("Game paused", "carrois",
				24);
		pauseLabel->setPosition(ccp(winSize.width / 2, winSize.height - 30));
		CC_BREAK_IF(!pauseLabel);

		this->addChild(pauseLabel, 11);

		CCArray * menuIcons = CCArray::create();

		/*
		 // settings
		 CCMenuItemImage *pSettingsItem = CCMenuItemImage::create(
		 "grey/settings.png", "grey/settings.png", this,
		 menu_selector(PauseLayer::OnMenu));
		 pSettingsItem->setTag(3);
		 menuIcons->addObject(pSettingsItem);
		 */

		//music
		CCMenuItemImage * pMusicItemOn = CCMenuItemImage::create(
				"grey/volume_on.png", "grey/volume_on.png", this,
				menu_selector(PauseLayer::OnMenu));

		CCMenuItemImage* pMusicItemOff = CCMenuItemImage::create(
				"grey/volume_muted.png", "grey/volume_muted.png", this,
				menu_selector(PauseLayer::OnMenu));

		CCMenuItemToggle * toggleMenu = CCMenuItemToggle::createWithTarget(this,
				menu_selector(PauseLayer::OnMenu), pMusicItemOn, pMusicItemOff,
				NULL);
		toggleMenu->setTag(2);

		// add current value
		if (!cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey(
				"music_enable", true)) {
			toggleMenu->setSelectedIndex(1);
		}
		menuIcons->addObject(toggleMenu);

		// main menu
		CCMenuItemImage *pMainMenu = CCMenuItemImage::create(
				"grey/mainmenu.png", "grey/mainmenu.png", this,
				menu_selector(PauseLayer::OnMenu));

		pMainMenu->setTag(1);
		menuIcons->addObject(pMainMenu);

		// close app
		CCMenuItemImage *pCloseApp = CCMenuItemImage::create(
				"grey/closeapp.png", "grey/closeapp.png", this,
				menu_selector(PauseLayer::OnMenu));

		pCloseApp->setTag(4);
		menuIcons->addObject(pCloseApp);

		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->alignItemsHorizontallyWithPadding(15);

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 11);

		CCLayerGradient* layer3 = CCLayerGradient::create();
		layer3->setContentSize(CCSizeMake(winSize.width, winSize.height));
		layer3->setPosition(ccp(0, 0));
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

void PauseLayer::didSwipe(CCObject* sender) {

	CCLog("Swipe called.");

}

void PauseLayer::OnMenu(CCObject* pSender) {
	CCMenuItem* pMenuItem = (CCMenuItemImage *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("PauseMenuItem  %d", tag);

	CCScene *pScene1;
	switch (tag) {
	case 1:
		SoundEffectHelper::playClickSound();
		pScene1 = MainScreenScene::create();
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionSlideInT::create(1.0f, pScene1));
		break;
	case 2:
		// toggle music
		HelperFunctions::toggleMusic();
		SoundEffectHelper::playLevelBackgroundMusic(level);
		SoundEffectHelper::pauseBackgroundMusic();
		if (SoundEffectHelper::isSoundEnabled()) {
			SoundEffectHelper::playClickSound();
		}
		break;
	case 3:
		SoundEffectHelper::playClickSound();
		pScene1 = SettingsScreenScene::create();
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionSplitCols::create(1.0f, pScene1));
		break;
	case 4:
		// simulate close Button clicked
		DeeWorld *layer = (DeeWorld*) this->getParent();
		layer->keyBackClicked();
		break;
	}
}

