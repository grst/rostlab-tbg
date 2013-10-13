#include "MainScreenScene.h"
#include "helper/WebOpNative.h"

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
		CCSprite* informationSprite = CCSprite::createWithSpriteFrameName("CloseNormal.png");
		CCSprite* informationSpriteClick = CCSprite::createWithSpriteFrameName("CloseNormal.png");
		CCMenuItemSprite* infoGameItem = CCMenuItemSprite::create(informationSprite, informationSpriteClick, this, menu_selector(MainScreenLayer::menuStartGameCallback));
		// END NEW //////

		CCSprite* startGameSprite = CCSprite::createWithSpriteFrameName("CloseNormal.png");
		CCSprite* startGameSpriteClick = CCSprite::createWithSpriteFrameName("CloseNormal.png");
		// create a menu item (button) with the up/down sprites
		CCMenuItemSprite* startGameItem = CCMenuItemSprite::create(startGameSprite, startGameSpriteClick, this, menu_selector(MainScreenLayer::menuStartGameCallback));

		// create a menu to hold the buttons (remembering to NULL terminate the list)
		// NEW - we include the new info item
		CCMenu *menu = CCMenu::create(startGameItem, infoGameItem, NULL);
		// position the entire menu
		menu->setPosition(100,100);
		// add it as a child (so it appears
		this->addChild(menu);



		CCMenuItemSprite *btn = CCMenuItemSprite::create(
		        CCSprite::createWithSpriteFrameName("stripes.png"),
		        NULL,
		        this,
		        menu_selector(MainScreenLayer::menuStartGameCallback));

		btn->setPosition(ccp(150,150));
		CCMenu *pMenu = CCMenu::create(btn, NULL);
		this->addChild(pMenu, 1);


		this->_label = CCLabelTTF::create("", "Artial", 32);
				_label->retain();
				_label->setColor(ccc3(0, 0, 0));
				_label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
				this->addChild(_label);

				// add "MainScreen" splash screen"
				CCSprite* pSpriteLogo = CCSprite::create("logo.png");

				// position the sprite on the center of the screen
				pSpriteLogo->setPosition(
						ccp(winSize.width / 2, winSize.height / 2 + 40));

				float scale = winSize.width;
				CCSize logoSize = pSpriteLogo->getContentSize();

				//scale it proportionally to 80% of the screen
				float scaleFactor = winSize.width / logoSize.width * 0.8;

				pSpriteLogo->setScaleX(scaleFactor);
				pSpriteLogo->setScaleY(scaleFactor);

				// add the sprite as a child to this layer
				this->addChild(pSpriteLogo, 0);



		return true;
	} else {
		return false;
	}
}

void MainScreenLayer::menuStartGameCallback(CCObject* sender)
{
    CCLOG("Hello!");
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
