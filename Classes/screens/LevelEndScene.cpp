#include "LevelEndScene.h"
#include "../helper/WebOpNative.h"
#include "MainScreenScene.h"
#include "../helper/HelperFunctions.h"

using namespace cocos2d;

CCScene* LevelEndScene::create(int score, int level) {

	LevelEndScene * scene = NULL;
	do {

		// 'scene' is an autorelease object
		scene = LevelEndScene::create();
		CC_BREAK_IF(!scene);
		scene->_layer->score = score;
		scene->_layer->level = level;
		scene->_layer->addLabels();

	}while(0);

	return scene;
}

bool LevelEndScene::init() {
	if (CCScene::init()) {
			this->_layer = LevelEndLayer::create();
			this->_layer->retain();
			this->addChild(_layer);

			return true;
		} else {
			return false;
		}
}

LevelEndScene::~LevelEndScene() {
	if (_layer) {
		_layer->release();
		_layer = NULL;
	}
}

bool LevelEndLayer::init() {
	//////////////////////////////
	    // 1. super init first
	    if (!CCLayer::init()) {
	        return false;
	    }

		SoundEffectHelper::playLevelEndSound();
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// add "LevelEnd" splash screen"
		CCSprite* pSpriteBackground = CCSprite::create("wood.jpg");

    HelperFunctions::fitBackground(pSpriteBackground);

		// add the sprite as a child to this layer
		this->addChild(pSpriteBackground, 0);

		this->_label = CCLabelTTF::create("Level Ended", "Artial", 32);
		_label->retain();
		_label->setColor(ccc3(0, 0, 0));
		_label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(_label);

		// add "LevelEnd" splash screen"
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

		// set a delay for three seconds
		this->runAction(
				CCSequence::create(CCDelayTime::create(3),
						CCCallFunc::create(this,
								callfunc_selector(
										LevelEndLayer::endScreen)),
						NULL));



		return true;

}

void LevelEndLayer::addLabels(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	std::string strLevel = static_cast<std::ostringstream*>(&(std::ostringstream() << level))->str();
	std::string seqLevel= "Level:" + strLevel;
	this->levelLabel = CCLabelTTF::create(seqLevel.c_str(), "Artial", 32);
	levelLabel->retain();
	levelLabel->setColor(ccc3(0, 0, 0));
	levelLabel->setPosition(ccp(winSize.width / 2 , winSize.height / 2 - 40));
	this->addChild(levelLabel);

	std::string strScore = static_cast<std::ostringstream*>(&(std::ostringstream() << score))->str();
	std::string seqScore = "Score:"+ strScore;
	this->scoreLabel = CCLabelTTF::create(seqScore.c_str(), "Artial", 32);
	scoreLabel->retain();
	scoreLabel->setColor(ccc3(0, 0, 0));
	scoreLabel->setPosition(ccp(winSize.width / 2 , winSize.height / 2 -80));
	this->addChild(scoreLabel);
}

void LevelEndLayer::endScreen() {

	//WebOpNative::openLink("http://www.rostlab.org");
	/*
	 CCScene *pScene = DeeWorld::scene();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
	 */
	CCScene *pScene = MainScreenScene::create();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFlipX::create(2.0f, pScene));
}

LevelEndLayer::~LevelEndLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
