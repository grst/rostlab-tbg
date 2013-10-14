#include "MainScreenScene.h"
#include "../helper/WebOpNative.h"
#include "SettingsScreenScene.h"
#include "DeeWorldScene.h"

using namespace cocos2d;

bool MainScreenScene::init() {
	if (CCScene::init()) {
		this->_layer = MainScreenLayer::create();
		this->_layer->retain();
		this->addChild(_layer);

		return true;
	} else {
		return false;
	}
}

MainScreenScene::~MainScreenScene() {
	if (_layer) {
		_layer->release();
		_layer = NULL;
	}
}

bool MainScreenLayer::init() {
	if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {

		// enable Android back button
		this->setKeypadEnabled(true);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		this->_label = CCLabelTTF::create("Select your level", "Artial", 32);
		_label->retain();
		_label->setColor(ccc3(20, 0, 0));
		_label->setPosition(ccp(winSize.width / 2, winSize.height - 50));
		this->addChild(_label, 0);

		// Place the menu item bottom-right conner.
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		CCArray * menuIcons = CCArray::create();

		int levels = 8;

		for (int i = 0; i < levels; i++) {

			CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
					"CloseNormal.png", "CloseSelected.png", this,
					menu_selector(MainScreenLayer::menuStartGameCallback));

			// some stupid rectangular order of the menu
			pCloseItem->setPosition(
					ccp(
							origin.x + (i % 4) * (winSize.width / 5)
									+ winSize.width / 5
									- pCloseItem->getContentSize().width / 2,
							origin.y + winSize.height - winSize.height / 3
									- (i / 4) * (winSize.height / 3)

									- pCloseItem->getContentSize().height / 2));
			// maybe we should use the dedicated align method for this?

			pCloseItem->setTag(i);

			menuIcons->addObject(pCloseItem);
		}


		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
				"settings.png", "settings.png", this,
				menu_selector(MainScreenLayer::changeScene));

		// some stupid rectangular order of the menu
		pCloseItem->setPosition(
				ccp(winSize.width- pCloseItem->getContentSize().width /2,
						origin.y + winSize.height
								- pCloseItem->getContentSize().height * 2));
		// maybe we should use the dedicated align method for this?

		pCloseItem->setTag(10);

		menuIcons->addObject(pCloseItem);


		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(CCPointZero);

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 1);

		return true;
	} else {
		return false;
	}
}

void MainScreenLayer::changeScene(CCObject* pSender) {

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("Changing to settings", tag);


	 CCScene *pScene = SettingsScreenScene::create();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
}

void MainScreenLayer::menuStartGameCallback(CCObject* pSender) {

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("Starting level %d", tag);



	 CCScene *pScene = DeeWorld::scene();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
}

void MainScreenLayer::keyBackClicked(void) {
    CCDirector::sharedDirector()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif

}

void MainScreenLayer::keyMenuClicked(void) {

	 CCScene *pScene = SettingsScreenScene::create();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
}

MainScreenLayer::~MainScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
