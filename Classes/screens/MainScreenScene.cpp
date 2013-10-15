#include "MainScreenScene.h"
#include "../helper/WebOpNative.h"
#include "SettingsScreenScene.h"
#include "DeeWorldScene.h"
#include "../ui_elements/TouchTrailLayer.h"

using namespace cocos2d;

#define kFileStreak "streak.png"

bool MainScreenScene::init() {
	if (CCScene::init()) {
		this->_layer = MainScreenLayer::create();
		this->_layer->retain();
		this->addChild(_layer);

		//TouchTrailLayer * layer2 = TouchTrailLayer::create();
		//this->addChild(layer2);

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
		this->setTouchEnabled(true);

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		initBackground();

		this->_label = CCLabelTTF::create("Select your level", "Artial", 32);
		_label->retain();
		_label->setColor(ccc3(220, 220, 220));
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

		CCMenuItemImage *pCloseItem = CCMenuItemImage::create("settings.png",
				"settings.png", this,
				menu_selector(MainScreenLayer::changeScene));

		// some stupid rectangular order of the menu
		pCloseItem->setPosition(
				ccp(winSize.width - pCloseItem->getContentSize().width / 2,
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

void MainScreenLayer::initBackground() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* pSpriteBackground = CCSprite::create("MainScreenBackground.jpg");

	// position the sprite on the center of the screen
	pSpriteBackground->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);
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

void MainScreenLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		CCBlade *blade = CCBlade::create(kFileStreak, 4, 50);
		_map[touch] = blade;
		addChild(blade);

		blade->setColor(ccc3(255, 0, 0));
		blade->setOpacity(100);
		blade->setDrainInterval(1.0 / 40);

		CCPoint point = convertTouchToNodeSpace(touch);
		blade->push(point);
	}
}

void MainScreenLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {
	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		if (_map.find(touch) == _map.end())
			continue;

		CCBlade *blade = _map[touch];
		CCPoint point = convertTouchToNodeSpace(touch);

		// TODO fix the blade position
		float tempHeight = CCDirector::sharedDirector()->getWinSize().height;
		CCPoint prevPoint = touch->getPreviousLocation();
		CCPoint prevPoint2 = ccp(prevPoint.x, tempHeight - prevPoint.y);
		CCPoint oldPlayerPoint = ccp(point.x, tempHeight - point.y);

		point = ccpAdd(ccpMult(point, 0.5f), ccpMult(prevPoint, 0.5f));
		blade->push(point);
	}
}

void MainScreenLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {
	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		if (_map.find(touch) == _map.end())
			continue;

		CCBlade *blade = _map[touch];
		blade->autoCleanup();
		_map.erase(touch);
	}
}

MainScreenLayer::~MainScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
