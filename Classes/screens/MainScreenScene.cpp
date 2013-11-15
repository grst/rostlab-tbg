#include "MainScreenScene.h"
#include "../helper/WebOpNative.h"
#include "SettingsScreenScene.h"
#include "SplashScreenScene.h"
#include "DeeWorldScene.h"
#include "LevelLoadingScene.h"
#include "../ui_elements/TouchTrailLayer.h"
#include "../ui_elements/ImpressumLayer.h"
#include "../ui_elements/AboutUsLayer.h"

using namespace cocos2d;

#define kFileStreak "streak.png"
#define TAG_IMPRESSUM_LAYER 789
#define TAG_ABOUTUS_LAYER 796

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

		// Place the menu item bottom-right corner.
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		CCArray * menuIcons = CCArray::create();

		int levels = 8;

		for (int i = 0; i < levels; i++) {

			CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
					"levelitem.png", "levelitem.png", this,
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

		// settings
		CCMenuItemImage *pSettingsItem = CCMenuItemImage::create("settings.png","settings.png", this,menu_selector(MainScreenLayer::changeScene));
		pSettingsItem->setPosition(
				ccp(winSize.width - pSettingsItem->getContentSize().width / 2,
						origin.y + winSize.height
								- pSettingsItem->getContentSize().height * 2));
		pSettingsItem->setTag(10);
		menuIcons->addObject(pSettingsItem);

		// about us
		CCMenuItemImage *pAboutUs = CCMenuItemImage::create("aboutus.png","aboutus.png", this,menu_selector(MainScreenLayer::changeScene));
		pAboutUs->setPosition(
				ccp( pAboutUs->getContentSize().width / 2 + winSize.width * 1/4,
						origin.y + winSize.height
								- pAboutUs->getContentSize().height * 2));
		pAboutUs->setTag(11);
		menuIcons->addObject(pAboutUs);

		// impressum
		CCMenuItemImage *pImpressum = CCMenuItemImage::create("impressum.png","impressum.png", this,menu_selector(MainScreenLayer::changeScene));
		pImpressum->setPosition(
				ccp( pImpressum->getContentSize().width / 2 + winSize.width * 2/4,
						origin.y + winSize.height
								- pImpressum->getContentSize().height * 2));
		pImpressum->setTag(12);
		menuIcons->addObject(pImpressum);

		// close app
		CCMenuItemImage *pCloseApp = CCMenuItemImage::create("closeapp.png","closeapp.png", this,menu_selector(MainScreenLayer::changeScene));
		pCloseApp->setPosition(
				ccp(  pCloseApp->getContentSize().width / 2 + winSize.width * 3/4,
						origin.y + winSize.height
								- pCloseApp->getContentSize().height * 2));
		pCloseApp->setTag(13);
		menuIcons->addObject(pCloseApp);

		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(CCPointZero);

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 1);

		SoundEffectHelper::playMainMenuBackgroundMusic();

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

	// disable lower layers
	if(isLayerOpen()){
		return;
	}

	SoundEffectHelper::playClickSound();

	CCScene *pScene1;

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();
	CCLOG("Changing to settings", tag);

	CCLayer * layer2;

	switch(tag){
	case 10:
		pScene1 = SettingsScreenScene::create();
		//transition to next scene for one sec
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pScene1));
		break;
	case 11:
		if(this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL){
				this->removeChildByTag(TAG_ABOUTUS_LAYER, true);
		}else{
			layer2 = AboutUsLayer::create();
			this->addChild(layer2, 10, TAG_ABOUTUS_LAYER);
		}
		break;
	case 12:
		if(this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL){
			this->removeChildByTag(TAG_IMPRESSUM_LAYER, true);
		}else{
			layer2 = ImpressumLayer::create();
			this->addChild(layer2, 10, TAG_IMPRESSUM_LAYER);
		}
		break;
	case 13:
		pScene1 = SplashScreenScene::create(false);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pScene1));
		break;
	}
}

void MainScreenLayer::menuStartGameCallback(CCObject* pSender) {

	// disable lower layers
	if(isLayerOpen()){
		return;
	}

	SoundEffectHelper::playClickSound();

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("Starting level %d", tag);

	// Crystal structure of human class II alcohol dehydrogenase (ADH4) in complex with NAD and Zn
	std::string seq= "";

	switch(tag){
	//TODO
	case 7:
		// just for short testing
		seq= "SMGTK"; break;
	default:
		seq= "SMGTKGKVIKCKAAIAWE";
	}

	CCScene *pScene = LevelLoadingScene::create(seq, tag);

	//SoundEffectHelper::stopBackgroundMusic();

	//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(1.0f, pScene));
}

void MainScreenLayer::keyBackClicked(void) {
	if(this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL){
		this->removeChildByTag(TAG_ABOUTUS_LAYER, true);
		return;
	}
	if(this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL){
		this->removeChildByTag(TAG_IMPRESSUM_LAYER, true);
		return;
	}
	CCScene *pScene = SplashScreenScene::create(false);
	//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(CCTransitionJumpZoom::create(1.0f, pScene));
}

bool MainScreenLayer::isLayerOpen(){
	if(this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL){
		return true;
	}
	if(this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL){
			return true;
		}
	return false;
}

void MainScreenLayer::keyMenuClicked(void) {

	CCScene *pScene = SettingsScreenScene::create();
	//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(CCTransitionCrossFade::create(1.0f, pScene));
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
