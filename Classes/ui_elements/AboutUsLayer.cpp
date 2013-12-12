/*
 * AboutUsLayer.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#include "AboutUsLayer.h"
#include "../helper/HelperFunctions.h"
#include "../screens/MainScreenScene.h"
#include "../screens/SettingsScreenScene.h"
#include "../Globals.h"

using namespace cocos2d;

AboutUsLayer::AboutUsLayer() {
	// TODO Auto-generated constructor stub

}

AboutUsLayer::~AboutUsLayer() {
	// TODO Auto-generated destructor stub
}

bool AboutUsLayer::init() {

	bool bRet = false;
	do {
		CC_BREAK_IF(!CCLayer::init());
		this->setTouchEnabled(true);
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCLabelTTF* pauseLabel = CCLabelTTF::create("HOW-TO", "Arial", 24,
				CCSize(winSize.width / 4, 50), kCCTextAlignmentRight);
		CC_BREAK_IF(!pauseLabel);

		pauseLabel->setPosition(
				ccp(
						winSize.width - pauseLabel->getContentSize().width / 2
								- winSize.width * 1 / 16,
						winSize.height
								- pauseLabel->getContentSize().height / 2));

		// add this to the layer
		this->addChild(pauseLabel, 11);

		CCArray * menuIcons = CCArray::create();
		/*

		 // close layer
		 CCMenuItemImage *pCloseLayer = CCMenuItemImage::create(
		 "grey/closeapp.png", "grey/closeapp.png", this,
		 menu_selector(AboutUsLayer::OnMenu));
		 pCloseLayer->setPosition(
		 ccp(winSize.width * 2 / 5 + pCloseLayer->getContentSize().width,
		 winSize.height - pCloseLayer->getContentSize().height));
		 pCloseLayer->setTag(2);
		 menuIcons->addObject(pCloseLayer);
		 */

		// next Button
		CCMenuItemImage *pNextButton = CCMenuItemImage::create(
				"grey/impressum.png", "grey/impressum.png", this,
				menu_selector(AboutUsLayer::OnMenu));
		pNextButton->setPosition(
				ccp(winSize.width * 3 / 5 + pNextButton->getContentSize().width,
						winSize.height - pNextButton->getContentSize().height));
		pNextButton->setTag(3);
		menuIcons->addObject(pNextButton);

		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(ccp(30, winSize.height - 25));
		levelMenu->alignItemsHorizontallyWithPadding(15);

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 11);

		CCLayerGradient* layer3 = CCLayerGradient::create();
		layer3->setContentSize(CCSizeMake(winSize.width, winSize.height));
		layer3->setPosition(ccp(0, 0));
		layer3->setStartColor(ccc3(100, 100, 100));
		layer3->setEndColor(ccc3(120, 120, 120));
		layer3->setStartOpacity(255);
		layer3->setEndOpacity(220);
		ccBlendFunc blend;
		blend.src = GL_SRC_ALPHA;
		blend.dst = GL_ONE_MINUS_SRC_ALPHA;
		layer3->setBlendFunc(blend);
		addChild(layer3, 10);

		posImageCounter = 0;
		initButt();

		bRet = true;
	} while (0);

	return bRet;

}

void AboutUsLayer::initButt() {

// add Protein
	this->getNextImage();
}

void AboutUsLayer::keyBackClicked(void) {
	// simulate close Button clicked
	MainScreenLayer * layer = (MainScreenLayer*) this->getParent();
	layer->keyBackClicked();
}

void AboutUsLayer::getNextImage() {

	std::string img = "";

	MainScreenLayer *layer;

	CCLOG("NextImage  %d", posImageCounter);

	std::string title;

	switch (posImageCounter) {
	case 0:
		title = "0";
		img = "5PEP.png";
		break;
	case 1:
		title = "1";
		img = "1ASZ.png";
		break;
	case 2:
		title = "2";
		img = "1DGS.png";
		break;
	case 3:
		title = "3";
		img = "1OLG.png";
		break;
	case 4:
		// simulate close Button clicked
		layer = (MainScreenLayer*) this->getParent();
		layer->keyBackClicked();
		return;
	default:
		img = "loading-bar-bg.png";
	}

	CCLOG("Starting remove");
	if (pImage != NULL && posImageCounter > 0) {
		pImage->removeFromParent();
		pTitle->removeFromParent();
	}
	CCLOG("Remove survided");

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	this->pImage = CCSprite::create(img.c_str());

	//scale it proportionally to 70% of the screen
	float scale = 0.7;
	CCSize logoSize = pImage->getContentSize();
	HelperFunctions::resizseSprite(pImage, 0.0, winSize.height * scale);
	pImage->setPosition(ccp(winSize.width * 1 / 3, winSize.height / 2));
	this->addChild(pImage, 12);

	this->pTitle = CCLabelTTF::create(title.c_str(), "Artial", 20,
			CCSizeMake(winSize.width * 4 / 6, 30), kCCTextAlignmentCenter,
			kCCVerticalTextAlignmentTop);
	this->pTitle->setColor(ccc3(0, 0, 0));
	this->pTitle->setPosition(
			ccp(winSize.width * 3 / 6, winSize.height * 11 / 12));
	this->addChild(pTitle, 12);

	posImageCounter = posImageCounter + 1;
}

void AboutUsLayer::OnMenu(CCObject* pSender) {

	CCMenuItem* pMenuItem = (CCMenuItemImage *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("PauseMenuItem  %d", tag);

	CCScene *pScene1;
	MainScreenLayer *layer;
	switch (tag) {
	case 1:
		SoundEffectHelper::playClickSound();
		pScene1 = MainScreenScene::create();
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene1));
		break;
	case 2:
		// simulate close Button clicked
		layer = (MainScreenLayer*) this->getParent();
		layer->keyBackClicked();
		break;
	case 3:
		// get next image
		getNextImage();
		break;
	}

//	CCScene* nextScene = MainMenu::scene();
	//CCDirector::sharedDirector()->replaceScene(nextScene);
}

