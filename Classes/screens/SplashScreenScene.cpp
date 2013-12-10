#include "SplashScreenScene.h"
#include "../helper/WebOpNative.h"
#include "MainScreenScene.h"
#include "../helper/HelperFunctions.h"

using namespace cocos2d;

CCScene* SplashScreenScene::create(bool start) {

	SplashScreenScene * scene = NULL;
	do {

		// 'scene' is an autorelease object
		scene = SplashScreenScene::create();
		CC_BREAK_IF(!scene);
		scene->_layer->start = start;
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
			"splashscreen-vert.png");

    HelperFunctions::fitBackground(pSpriteBackground);

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);


	// set a delay for two seconds
	this->runAction(
			CCSequence::create(CCDelayTime::create(2),
					CCCallFunc::create(this,
							callfunc_selector(SplashScreenLayer::startGame)),
					NULL));

}

void SplashScreenLayer::startGame() {
    //start Game
    CCScene *pScene = MainScreenScene::create();
    CCDirector::sharedDirector()->replaceScene(
    		CCTransitionMoveInT::create(1.0f, pScene));
}


SplashScreenLayer::~SplashScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
