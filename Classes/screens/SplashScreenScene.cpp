#include "SplashScreenScene.h"
#include "../helper/WebOpNative.h"
#include "MainScreenScene.h"

using namespace cocos2d;

CCScene* SplashScreenScene::create(bool start) {

	SplashScreenScene * scene = NULL;
	do {

		// 'scene' is an autorelease object
		scene = SplashScreenScene::create();
		CC_BREAK_IF(!scene);
		scene->_layer->start = start;
		scene->_layer->addLabels();
	} while (0);

	return scene;
}

bool SplashScreenScene::init() {
	if (CCScene::init()) {
		this->_layer = SplashScreenLayer::create();
		this->_layer->retain();
		this->addChild(_layer);

		return true;
	} else {
	}
}

SplashScreenScene::~SplashScreenScene() {
	if (_layer) {
		_layer->release();
		_layer = NULL;
	}
}

bool SplashScreenLayer::init() {
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// add "SplashScreen" splash screen"
	CCSprite* pSpriteBackground = CCSprite::create(
			"NotificationScreenBackground.jpg");

	// position the sprite on the center of the screen
	pSpriteBackground->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);

	// add "SplashScreen" splash screen"
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

	// set a delay for three seconds
	this->runAction(
			CCSequence::create(CCDelayTime::create(2),
					CCCallFunc::create(this,
							callfunc_selector(SplashScreenLayer::endScreen)),
					NULL));

}

void SplashScreenLayer::addLabels() {
	return;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	std::string seq = "";
	if (start) {
		SoundEffectHelper::playGameStartSound();
		seq = "Starting game. Good Luck!";
	} else {
		SoundEffectHelper::playGameEndSound();
		seq = "Thank you for playing with us";
	}

	this->_label = CCLabelTTF::create(seq.c_str(), "Arial", 32);
	_label->retain();
	_label->setColor(ccc3(0, 0, 0));
	_label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	this->addChild(_label);
}

void SplashScreenLayer::endScreen() {

	//WebOpNative::openLink("http://www.rostlab.org");
	/*
	 CCScene *pScene = DeeWorld::scene();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
	 */
	if (start) {
		//start Game
		CCScene *pScene = MainScreenScene::create();
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFlipX::create(0.7f, pScene));
	} else {
		// end Game
		CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
}

SplashScreenLayer::~SplashScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
