#include "LevelLoadingScene.h"
#include "../helper/WebOpNative.h"
#include "../helper/LevelHelper.h"
#include "MainScreenScene.h"
#include "DeeWorldScene.h"
#include "../ui_elements/cc-extensions/CCGestureRecognizer/CCSwipeGestureRecognizer.h"

using namespace cocos2d;

CCScene* LevelLoadingScene::create(std::string seq, int level) {

	LevelLoadingScene * scene = NULL;
	do {

		// 'scene' is an autorelease object
		scene = LevelLoadingScene::create();
		CC_BREAK_IF(!scene);
		char * c = new char[seq.size() + 1];
		std::copy(seq.begin(), seq.end(), c);
		c[seq.size()] = '\0';
		scene->_layer->seq = c;
		scene->_layer->level = level;
		scene->_layer->addLabels();

	} while (0);

	return scene;
}

bool LevelLoadingScene::init() {
	if (CCScene::init()) {
		this->_layer = LevelLoadingLayer::create();
		this->_layer->retain();
		this->addChild(_layer);

		return true;
	} else {
		return false;
	}
}

LevelLoadingScene::~LevelLoadingScene() {
	if (_layer) {
		_layer->release();
		_layer = NULL;
	}
}

bool LevelLoadingLayer::init() {
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}

	// enable buttons
	this->setKeypadEnabled(true);

	SoundEffectHelper::playLevelLoadingBackgroundMusic();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// add "LevelLoading" splash screen"
	CCSprite* pSpriteBackground = CCSprite::create("wood-grunge.jpg");
	HelperFunctions::fitBackground(pSpriteBackground);

	HelperFunctions::fitBackground(pSpriteBackground);

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);

	CCMenuItemImage *pCloseApp = CCMenuItemImage::create("white/exit.png",
			"white/exit.png", this,
			menu_selector(LevelLoadingLayer::changeScene));
	pCloseApp->setPosition(0, 0);
	pCloseApp->setTag(1);
	pCloseApp->setScale(0.7);

	CCMenuItemImage *pStartButton = CCMenuItemImage::create("start-button.png",
			"start-button.png", this,
			menu_selector(LevelLoadingLayer::changeScene));
	pStartButton->setPosition(0, 0);
	pStartButton->setTag(2);
	float scale = 0.45;
    pStartButton->setScale(scale);

	// Create a menu with our menu items
	cocos2d::CCMenu* backMenu = CCMenu::createWithItem(pCloseApp);
	backMenu->setPosition(ccp(20, winSize.height - 25));
	backMenu->alignItemsHorizontally();
	this->addChild(backMenu, 11);
	cocos2d::CCMenu* startMenu = CCMenu::createWithItem(pStartButton);
	// align in the middle
	startMenu->setPosition(
			(ccp(winSize.width / 2 - pStartButton->getContentSize().width / 4 * scale,
					winSize.height * 1 / 8)));
	startMenu->alignItemsHorizontally();
	this->addChild(startMenu, 10);

	// add swipe
	CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
	swipe->setTarget(this, callfuncO_selector(LevelLoadingLayer::didSwipe));
	swipe->setDirection(
			kSwipeGestureRecognizerDirectionRight
					| kSwipeGestureRecognizerDirectionLeft);
	swipe->setCancelsTouchesInView(true);
	this->addChild(swipe, 12);

	/*
	 CCString labelText = "You will fight for ";
	 this->_label = CCLabelTTF::create(labelText.getCString(), "Artial", 12);
	 _label->retain();
	 _label->setColor(ccc3(0, 0, 0));
	 _label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	 this->addChild(_label);


	 // add "LevelLoading" splash screen"
	 CCSprite* pSpriteLogo = CCSprite::create("logo.png");

	 // position the sprite on the center of the screen
	 pSpriteLogo->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 40));

	 float scale = winSize.width;
	 CCSize logoSize = pSpriteLogo->getContentSize();

	 //scale it proportionally to 80% of the screen
	 float scaleFactor = winSize.width / logoSize.width * 0.8;

	 pSpriteLogo->setScaleX(scaleFactor);
	 pSpriteLogo->setScaleY(scaleFactor);

	 // add the sprite as a child to this layer
	 this->addChild(pSpriteLogo, 0);
	 */

	// set a delay for three seconds
	/*
	 this->runAction(
	 CCSequence::create(CCDelayTime::create(7),
	 CCCallFunc::create(this,
	 callfunc_selector(LevelLoadingLayer::endScreen)),
	 NULL));
	 */

	return true;

}



void LevelLoadingLayer::didSwipe(CCObject* pSender) {
	CCSwipe * swipe =  (CCSwipe *) pSender;
	// recognize swipe to the left
	CCLog("got swipe event");
	if(swipe->direction == kSwipeGestureRecognizerDirectionLeft){
		CCScene * pScene1 = DeeWorld::scene(seq, level);
				CCDirector::sharedDirector()->replaceScene(
						CCTransitionMoveInR::create(0.2f, pScene1));
	}
}

void LevelLoadingLayer::changeScene(CCObject* pSender) {

	SoundEffectHelper::playClickSound();

	CCScene *pScene1;

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();
	CCLOG("Levelloading: Changing to scene ", tag);

	switch (tag) {
	case 1:
		pScene1 = MainScreenScene::create();
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene1));
		break;
	case 2:
		pScene1 = DeeWorld::scene(seq, level);
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene1));
		break;
	}

}

void LevelLoadingLayer::addLabels() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	std::string strLevel =
			static_cast<std::ostringstream*>(&(std::ostringstream() << level))->str();
	int score = cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
			("level_" + strLevel).c_str(), 0);

	std::string strScore =
			static_cast<std::ostringstream*>(&(std::ostringstream() << score))->str();

	std::string seqLevel = "Highscore: " + strScore + "/"
			+ static_cast<std::ostringstream*>(&(std::ostringstream()
					<< LevelHelper::getHighscoreForLevel(this->level)))->str();
	this->scoreLabel = CCLabelTTF::create(seqLevel.c_str(), "Artial", 18,
			CCSizeMake(winSize.width * 3 / 6, 30), kCCTextAlignmentRight,
			kCCVerticalTextAlignmentTop);
	scoreLabel->retain();
	scoreLabel->setColor(ccc3(255, 255, 255));
	scoreLabel->setPosition(ccp(winSize.width * 4 / 6, winSize.height * 1 / 8));
	this->addChild(scoreLabel);


	// name
	this->ttfName = CCLabelTTF::create(
			LevelHelper::getNameForLevel(level).c_str(), "carrois-sc", 20,
			CCSizeMake(winSize.width * 3 / 6, 30), kCCTextAlignmentRight,
			kCCVerticalTextAlignmentTop);
	ttfName->retain();
	ttfName->setColor(ccc3(255, 255, 255));
	ttfName->setPosition(ccp(winSize.width * 4 / 6, winSize.height * 7 / 8));
	this->addChild(ttfName);

	// desc
	std::string sDesc = LevelHelper::getDescriptionForLevel(level);
	this->ttfDesc = CCLabelTTF::create(sDesc.c_str(), "carrois", 12,
			CCSizeMake(winSize.width * 3 / 6, winSize.height * 1 / 2),
			kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
	ttfDesc->retain();
	ttfDesc->setColor(ccc3(255, 255, 255));
	ttfDesc->setPosition(ccp(winSize.width * 4 / 6, winSize.height * 3 / 5));
	this->addChild(ttfDesc);

	// code
	std::string seqCode = LevelHelper::getCodeForLevel(level);
	this->ttfCode = CCLabelTTF::create(seqCode.c_str(), "carrois", 20,
			CCSizeMake(80, 30), kCCTextAlignmentCenter,
			kCCVerticalTextAlignmentCenter);
	ttfCode->retain();
	ttfCode->setColor(ccc3(255, 255, 255));
	ttfCode->setPosition(ccp(winSize.width * 1 / 6, winSize.height * 1 / 8));
	this->addChild(ttfCode);

	// add Protein
	this->pProtein = CCSprite::create(
			LevelHelper::getPathForLevel(level).c_str());

	//scale it proportionally to 30% of the screen
	float scale = 0.3;
	CCSize logoSize = pProtein->getContentSize();
	HelperFunctions::resizseSprite(pProtein, winSize.width * scale, 0.0);
	pProtein->setPosition(
			ccp(logoSize.width * scale + winSize.width * 1 / 12,
					winSize.height * 2 / 3));
	this->addChild(pProtein, 0);

}

void LevelLoadingLayer::keyMenuClicked(){
	keyBackClicked();
}

void LevelLoadingLayer::keyBackClicked(){
	CCLog("LevelLoading: keyback clicked.");
	CCScene * pScene1 = MainScreenScene::create();
	CCDirector::sharedDirector()->replaceScene(
					CCTransitionFade::create(1.0f, pScene1));
}

void LevelLoadingLayer::endScreen() {

	SoundEffectHelper::stopBackgroundMusic();
	CCScene *pScene = DeeWorld::scene(seq, level);
	//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(1.0f, pScene));
}

LevelLoadingLayer::~LevelLoadingLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
