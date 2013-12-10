#include "LevelLoadingScene.h"
#include "../helper/WebOpNative.h"
#include "../helper/LevelHelper.h"
#include "MainScreenScene.h"
#include "DeeWorldScene.h"

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

	SoundEffectHelper::playLevelLoadingBackgroundMusic();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// add "LevelLoading" splash screen"
		CCSprite* pSpriteBackground = CCSprite::create("wood-grunge.jpg");

	// position the sprite on the center of the screen
	pSpriteBackground->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);

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
	this->runAction(
			CCSequence::create(CCDelayTime::create(3),
					CCCallFunc::create(this,
							callfunc_selector(LevelLoadingLayer::endScreen)),
					NULL));

	return true;

}

void LevelLoadingLayer::addLabels() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	int score = 0;
	std::string strScore =
			static_cast<std::ostringstream*>(&(std::ostringstream() << score))->str();

	std::string seqLevel = "Your Score: " + strScore;
	this->levelLabel = CCLabelTTF::create(seqLevel.c_str(), "Artial", 18);
	levelLabel->retain();
	levelLabel->setColor(ccc3(255, 255, 255));
	levelLabel->setPosition(ccp(winSize.width * 5 / 6, winSize.height * 1 / 6));
	this->addChild(levelLabel);

	// add Protein
	CCSprite* pPercentage = CCSprite::create("loading-bar-bg.png");

	//scale it proportionally to 30% of the screen
	float scalePer = 0.3;
	CCSize logoPercentageSize = pPercentage->getContentSize();
	HelperFunctions::resizseSprite(pPercentage, winSize.width * scalePer, 0.0);
	pPercentage->setPosition(
			ccp(winSize.width - logoPercentageSize.width * scalePer - winSize.width * 5 / 6,
					winSize.height * 2 / 3));
	this->addChild(pPercentage, 0);

	// name
	this->seqLabel = CCLabelTTF::create(
			LevelHelper::getNameForLevel(level).c_str(), "Artial", 20,
			CCSizeMake(60, 30), kCCTextAlignmentCenter,
			kCCVerticalTextAlignmentTop);
	seqLabel->retain();
	seqLabel->setColor(ccc3(255, 255, 255));
	seqLabel->setPosition(ccp(winSize.width * 4 / 6, winSize.height * 5 / 6));
	this->addChild(seqLabel);

	// desc
	std::string sDesc = LevelHelper::getDescriptionForLevel(level);
	this->seqLabel = CCLabelTTF::create(sDesc.c_str(), "Artial", 12,
			CCSizeMake(winSize.width * 3/6, winSize.height * 1 / 2),
			kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
	seqLabel->retain();
	seqLabel->setColor(ccc3(255, 255, 255));
	seqLabel->setPosition(ccp(winSize.width * 4 /6, winSize.height * 2 / 4));
	this->addChild(seqLabel);

	// code
	std::string seqCode = LevelHelper::getCodeForLevel(level);
	this->seqLabel = CCLabelTTF::create(seqCode.c_str(), "Artial", 20,
			CCSizeMake(80, 30), kCCTextAlignmentCenter,
			kCCVerticalTextAlignmentCenter);
	seqLabel->retain();
	seqLabel->setColor(ccc3(255, 255, 255));
	seqLabel->setPosition(ccp(winSize.width * 1 / 6, winSize.height * 1 / 6));
	this->addChild(seqLabel);

	// add Protein
	CCSprite* pProtein = CCSprite::create(
			LevelHelper::getPathForLevel(level).c_str());

	//scale it proportionally to 30% of the screen
	float scale = 0.3;
	CCSize logoSize = pProtein->getContentSize();
	HelperFunctions::resizseSprite(pProtein, winSize.width * scale, 0.0);
	pProtein->setPosition(ccp(logoSize.width * scale + winSize.width * 1/12, winSize.height * 2 / 3));
	this->addChild(pProtein, 0);

}

void LevelLoadingLayer::endScreen() {

	SoundEffectHelper::stopBackgroundMusic();
	CCScene *pScene = DeeWorld::scene(seq, level);
	//transition to next scene for one sec
	//CCDirector::sharedDirector()->replaceScene(
	//		CCTransitionMoveInB::create(2.0f, pScene));
}

LevelLoadingLayer::~LevelLoadingLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
