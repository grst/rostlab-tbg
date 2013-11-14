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


bool AboutUsLayer::init()
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

		// add this to the layer
		this->addChild(pauseLabel, 11);


		CCArray * menuIcons = CCArray::create();





		// main menu
		CCMenuItemImage *pMainMenu = CCMenuItemImage::create(
							"mainmenu.png", "mainmenu.png", this,
							menu_selector(AboutUsLayer::OnMenu));
		pMainMenu->setPosition(ccp(winSize.width / 5+ pMainMenu->getContentSize().width ,
						winSize.height - pMainMenu->getContentSize().height));
		pMainMenu->setTag(1);
		menuIcons->addObject(pMainMenu);

		// close layer
		CCMenuItemImage *pCloseLayer = CCMenuItemImage::create(
							"closeapp.png", "closeapp.png", this,
							menu_selector(AboutUsLayer::OnMenu));
		pCloseLayer->setPosition(ccp(winSize.width *2/ 5+ pMainMenu->getContentSize().width ,
						winSize.height - pCloseLayer->getContentSize().height));
		pCloseLayer->setTag(2);
		menuIcons->addObject(pCloseLayer);



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

void AboutUsLayer::OnMenu(CCObject* pSender)
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
			// simulate close Button clicked
			MainScreenLayer *layer = (MainScreenLayer*) this->getParent();
			layer->keyBackClicked();
			break;
	}

//	CCScene* nextScene = MainMenu::scene();
	//CCDirector::sharedDirector()->replaceScene(nextScene);
}

