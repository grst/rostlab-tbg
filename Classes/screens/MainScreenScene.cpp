#include "MainScreenScene.h"
#include "../helper/WebOpNative.h"

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

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// add "MainScreen" splash screen"
		CCSprite* pSpriteBackground = CCSprite::create("stripes.png");

		pSpriteBackground->setScale(5.0);

		// position the sprite on the center of the screen
		pSpriteBackground->setPosition(
				ccp(winSize.width / 2, winSize.height / 2));

		// add the sprite as a child to this layer
		//	this->addChild(pSpriteBackground, 0);

		// NEW /////////
		CCSprite* informationSprite = CCSprite::createWithSpriteFrameName(
				"CloseNormal.png");
		CCSprite* informationSpriteClick = CCSprite::createWithSpriteFrameName(
				"CloseNormal.png");
		CCMenuItemSprite* infoGameItem = CCMenuItemSprite::create(
				informationSprite, informationSpriteClick, this,
				menu_selector(MainScreenLayer::menuStartGameCallback));
		// END NEW //////

		CCSprite* startGameSprite = CCSprite::createWithSpriteFrameName(
				"CloseNormal.png");
		CCSprite* startGameSpriteClick = CCSprite::createWithSpriteFrameName(
				"CloseNormal.png");
		// create a menu item (button) with the up/down sprites
		CCMenuItemSprite* startGameItem = CCMenuItemSprite::create(
				startGameSprite, startGameSpriteClick, this,
				menu_selector(MainScreenLayer::menuStartGameCallback));

		// create a menu to hold the buttons (remembering to NULL terminate the list)
		// NEW - we include the new info item
		CCMenu *menu = CCMenu::create(startGameItem, infoGameItem, NULL);
		// position the entire menu
		menu->setPosition(100, 100);
		// add it as a child (so it appears
		this->addChild(menu, 0);

		CCMenuItemSprite *btn = CCMenuItemSprite::create(
				CCSprite::createWithSpriteFrameName("logo.png"),
				NULL, this,
				menu_selector(MainScreenLayer::menuStartGameCallback));

		btn->setPosition(ccp(150, 150));
		CCMenu *pMenu = CCMenu::create(btn, NULL);
		this->addChild(pMenu, 0);

		this->_label = CCLabelTTF::create("abc", "Artial", 32);
		_label->retain();
		_label->setColor(ccc3(20, 0, 0));
		_label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(_label, 0);


		// 1. Add a menu item with "X" image, which is clicked to quit the program.

				// Create a "close" menu item with close icon, it's an auto release object.
				CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
					"CloseNormal.png",
					"CloseSelected.png",
					this,
					menu_selector(MainScreenLayer::endScreen));

				// Place the menu item bottom-right conner.
		        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

				pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
		                                    origin.y + pCloseItem->getContentSize().height/2));

				// Create a menu with the "close" menu item, it's an auto release object.
				CCMenu* pMenu2 = CCMenu::create(pCloseItem, NULL);
				pMenu2->setPosition(CCPointZero);

				// Add the menu to TestWorld layer as a child layer.
				this->addChild(pMenu2, 1);

		return true;
	} else {
		return false;
	}
}

void MainScreenLayer::menuStartGameCallback(CCObject* sender) {
	CCLOG("Hello!");
	CCScene *pScene = MainScreenScene::create();

	CCDirector *pDirector = CCDirector::sharedDirector();
	// run
	pDirector->runWithScene(pScene);
}

void MainScreenLayer::endScreen() {

	//WebOpNative::openLink("http://www.rostlab.org");
	/*
	 CCScene *pScene = DeeWorld::scene();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
	 */
}

MainScreenLayer::~MainScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
