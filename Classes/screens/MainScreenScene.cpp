#include "MainScreenScene.h"
#include "../helper/WebOpNative.h"
#include "../helper/LevelHelper.h"
#include "SettingsScreenScene.h"
#include "SplashScreenScene.h"
#include "DeeWorldScene.h"
#include "LevelLoadingScene.h"
#include "../ui_elements/cc-extensions/TouchTrailLayer.h"
#include "../ui_elements/ImpressumLayer.h"
#include "../ui_elements/AboutUsLayer.h"

using namespace cocos2d;

#define kFileStreak "streak.png"
#define TAG_IMPRESSUM_LAYER 789
#define TAG_ABOUTUS_LAYER 796
#define MENU_ITEM_SCALE .7f

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

	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}

	// enable Android back button
	//this->setKeypadEnabled(true);
	//this->setTouchEnabled(true);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	initBackground();
//
//	this->_label = CCLabelTTF::create("Select your level", "Arial", 24);
//	_label->retain();
//	_label->setColor(ccc3(255, 255, 255));
//	_label->setPosition(ccp(winSize.width / 2, winSize.height - 30));
//	this->addChild(_label, 0);


	// Place the menu item bottom-right corner.
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCArray * menuIcons = CCArray::create();
    CCArray * levelIcons = CCArray::create();

	int levels = 10;

	for (int i = 0; i < levels; i++) {

		CCMenuItemImage *levelItem = CCMenuItemImage::create("white/levelitem.png",
				"levelitem.png", this,
				menu_selector(MainScreenLayer::menuStartGameCallback));


		levelItem->setTag(i);

		levelIcons->addObject(levelItem);
	}

	// settings
	CCMenuItemImage *pSettingsItem = CCMenuItemImage::create("white/settings.png",
			"white/settings.png", this, menu_selector(MainScreenLayer::changeScene));
    pSettingsItem->setScale(MENU_ITEM_SCALE);
    pSettingsItem->setTag(10);
	menuIcons->addObject(pSettingsItem);

	// about us
	CCMenuItemImage *pAboutUs = CCMenuItemImage::create("white/aboutus.png",
			"white/aboutus.png", this, menu_selector(MainScreenLayer::changeScene));
    pAboutUs->setScale(MENU_ITEM_SCALE);
	pAboutUs->setTag(11);
	menuIcons->addObject(pAboutUs);

	// impressum
	CCMenuItemImage *pImpressum = CCMenuItemImage::create("white/impressum.png",
			"white/impressum.png", this, menu_selector(MainScreenLayer::changeScene));
    pImpressum->setScale(MENU_ITEM_SCALE);
	pImpressum->setTag(12);
	menuIcons->addObject(pImpressum);

	// close app
	CCMenuItemImage *pCloseApp = CCMenuItemImage::create("white/closeapp.png",
			"white/closeapp.png", this, menu_selector(MainScreenLayer::changeScene));
    pCloseApp->setScale(MENU_ITEM_SCALE);
	pCloseApp->setTag(13);
	menuIcons->addObject(pCloseApp);

	// Create a menu with our menu items
	mainMenu = CCMenu::createWithArray(menuIcons);
	mainMenu->setPosition(ccp(100, winSize.height - 25));
    mainMenu->alignItemsHorizontallyWithPadding(15);
    
	levelMenu = CCMenu::createWithArray(levelIcons);
	//levelMenu->setPosition(ccp(0, 50));
    levelMenu->alignItemsInColumns(5, 5);

	// Add the menu to TestWorld layer as a child layer.
	this->addChild(mainMenu, 1);
    this->addChild(levelMenu, 1);


	SoundEffectHelper::playMainMenuBackgroundMusic();

	return true;
}

void MainScreenLayer::initBackground() {
	

	CCSprite* pSpriteBackground = CCSprite::create("wood.jpg");
    HelperFunctions::fitBackground(pSpriteBackground);

// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);
}

void MainScreenLayer::changeScene(CCObject* pSender) {

// disable lower layers
	if (isLayerOpen()) {
		return;
	}

	SoundEffectHelper::playClickSound();

	CCScene *pScene1;

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();
	CCLOG("Changing to settings", tag);

	CCLayer * layer2;

	switch (tag) {
	case 10:
		pScene1 = SettingsScreenScene::create();
		//transition to next scene for one sec
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene1));
		break;
	case 11:
		if (this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL) {
			this->removeChildByTag(TAG_ABOUTUS_LAYER, true);
		} else {
			layer2 = AboutUsLayer::create();
			this->addChild(layer2, 10, TAG_ABOUTUS_LAYER);
		}
		break;
	case 12:
		if (this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL) {
			this->removeChildByTag(TAG_IMPRESSUM_LAYER, true);
		} else {
			layer2 = ImpressumLayer::create();
			this->addChild(layer2, 10, TAG_IMPRESSUM_LAYER);
		}
		break;
	case 13:
		pScene1 = SplashScreenScene::create(false);
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene1));
		break;
	}
}

void MainScreenLayer::menuStartGameCallback(CCObject* pSender) {

// disable lower layers
	if (isLayerOpen()) {
		return;
	}

	SoundEffectHelper::playClickSound();

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("Starting level %d", tag);

	std::string seq = LevelHelper::getSequenceForLevel(tag);

	CCScene *pScene = LevelLoadingScene::create(seq, tag);

//SoundEffectHelper::stopBackgroundMusic();

//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionMoveInB::create(0.7f, pScene));
}

void MainScreenLayer::keyBackClicked(void) {
	if (this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL) {
		this->removeChildByTag(TAG_ABOUTUS_LAYER, true);
		return;
	}
	if (this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL) {
		this->removeChildByTag(TAG_IMPRESSUM_LAYER, true);
		return;
	}
	CCScene *pScene = SplashScreenScene::create(false);
//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionJumpZoom::create(1.0f, pScene));
}

bool MainScreenLayer::isLayerOpen() {
	if (this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL) {
		return true;
	}
	if (this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL) {
		return true;
	}
	return false;
}

void MainScreenLayer::keyMenuClicked(void) {

	CCScene *pScene = SettingsScreenScene::create();
//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionCrossFade::create(1.0f, pScene));
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
