#include "LevelEndScene.h"
#include "../helper/WebOpNative.h"
#include "MainScreenScene.h"
#include "../helper/HelperFunctions.h"
#include "../helper/LevelHelper.h"
#include "../ui_elements/cc-extensions/CCGestureRecognizer/CCSwipeGestureRecognizer.h"

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

	} while (0);

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
	CCSprite* pSpriteBackground = CCSprite::create("wood-grunge.jpg");
	HelperFunctions::fitBackground(pSpriteBackground);

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);

	/*

	 this->_label = CCLabelTTF::create("Level Ended", "Artial", 32);
	 _label->retain();
	 _label->setColor(ccc3(0, 0, 0));
	 _label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	 this->addChild(_label);

	 // add "LevelEnd" splash screen"
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


	// add swipe
	CCSwipeGestureRecognizer * swipe = CCSwipeGestureRecognizer::create();
	swipe->setTarget(this, callfuncO_selector(LevelEndLayer::didSwipe));
	swipe->setDirection(
			kSwipeGestureRecognizerDirectionRight
					| kSwipeGestureRecognizerDirectionLeft);
	swipe->setCancelsTouchesInView(true);
	this->addChild(swipe, 12);


	// set a delay for three seconds
	this->runAction(
			CCSequence::create(CCDelayTime::create(15),
					CCCallFunc::create(this,
							callfunc_selector(LevelEndLayer::endScreen)),
					NULL));

	return true;

}

void LevelEndLayer::didSwipe(CCObject* pSender) {
	CCSwipe * swipe =  (CCSwipe *) pSender;
	// recognize swipe to the left
	CCLog("got swipe event");
	if(swipe->direction == kSwipeGestureRecognizerDirectionLeft){
		CCScene * pScene1 = MainScreenScene::create();
				CCDirector::sharedDirector()->replaceScene(
						CCTransitionMoveInR::create(0.5f, pScene1));
	}
}

void LevelEndLayer::addLabels() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	std::string strLevel =
			static_cast<std::ostringstream*>(&(std::ostringstream() << level))->str();
	int scoreOld =
			cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
					("level_" + strLevel).c_str(), 0);

	if (score == scoreOld) {
		// new high score
		std::string seqLevel = "New Highscore";
			this->scoreLabel = CCLabelTTF::create(seqLevel.c_str(), "carrois", 18,
				CCSizeMake(winSize.width * 3 / 6, 30), kCCTextAlignmentRight,
				kCCVerticalTextAlignmentTop);
		scoreLabel->retain();
		scoreLabel->setColor(ccc3(0, 255, 0));
		scoreLabel->setPosition(
				ccp(winSize.width * 4 / 6, winSize.height * 1 / 4));
		this->addChild(scoreLabel);
	}

	// ask for stars


	int currentScore =
			cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
					("level_" + strLevel).c_str(), 0);

	int stars = LevelHelper::getStarsForLevel(level, currentScore);

	CCLog("stars: %i", stars);

	std::string starPath;
	switch (stars) {
	case 0:
		starPath = "stars-w3.png";
		break;
	case 1:
		starPath = "stars-w2.png";
		break;
	case 2:
		starPath = "stars-w1.png";
		break;
	case 3:
		starPath = "stars-w0.png";
		break;
	default:
		starPath = "stars-w3.png";
		break;
	}

	CCLog("path %s", starPath.c_str());

	CCSprite * spriteStar = CCSprite::create(starPath.c_str());

	// stars created

	spriteStar->setPosition(ccp(winSize.width * 4 /9 , winSize.height * 1 / 8));
	this->addChild(spriteStar);

	std::string strScore =
			static_cast<std::ostringstream*>(&(std::ostringstream() << score))->str();

	std::string seqLevel = "Your Score: " + strScore+ "/" + static_cast<std::ostringstream*>(&(std::ostringstream() << LevelHelper::getHighscoreForLevel(this->level)))->str();;
	this->scoreLabel = CCLabelTTF::create(seqLevel.c_str(), "carrois", 18,
			CCSizeMake(winSize.width * 3 / 6, 30), kCCTextAlignmentRight,
			kCCVerticalTextAlignmentTop);
	scoreLabel->retain();
	scoreLabel->setColor(ccc3(255, 255, 255));
	scoreLabel->setPosition(ccp(winSize.width * 4 / 6, winSize.height * 1 / 8));
	this->addChild(scoreLabel);

	// name
	this->ttfName = CCLabelTTF::create(
			LevelHelper::getNameForLevel(level).c_str(), "carrois", 20,
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

void LevelEndLayer::endScreen() {

	//WebOpNative::openLink("http://www.rostlab.org");
	/*
	 CCScene *pScene = DeeWorld::scene();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
	 */
	CCScene *pScene = MainScreenScene::create();
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionMoveInB::create(1.0, pScene));
}

LevelEndLayer::~LevelEndLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
