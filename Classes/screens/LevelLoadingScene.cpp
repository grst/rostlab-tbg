#include "LevelLoadingScene.h"
#include "../helper/WebOpNative.h"
#include "../helper/LevelHelper.h"
#include "MainScreenScene.h"
#include "DeeWorldScene.h"

using namespace cocos2d;

CCScene* LevelLoadingScene::create(std::string seq,  int level) {

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

	}while(0);

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
	if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {

		SoundEffectHelper::playLevelLoadingBackgroundMusic();
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// add "LevelLoading" splash screen"
		CCSprite* pSpriteBackground = CCSprite::create("NotificationScreenBackground.jpg");

		// position the sprite on the center of the screen
		pSpriteBackground->setPosition(
				ccp(winSize.width / 2, winSize.height / 2));

		// add the sprite as a child to this layer
		this->addChild(pSpriteBackground, 0);

		CCString labelText = "You will fight for ";
		this->_label = CCLabelTTF::create(labelText.getCString(), "Artial", 12);
		_label->retain();
		_label->setColor(ccc3(0, 0, 0));
		_label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(_label);

		// add "LevelLoading" splash screen"
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
										LevelLoadingLayer::endScreen)),
						NULL));



		return true;
	} else {
		return false;
	}
}

void LevelLoadingLayer::addLabels() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	std::string strLevel = static_cast<std::ostringstream*>(&(std::ostringstream() << level))->str();
	std::string seqLevel= "Level: " + strLevel;
	this->levelLabel = CCLabelTTF::create(seqLevel.c_str(), "Artial", 18);
	levelLabel->retain();
	levelLabel->setColor(ccc3(0, 0, 0));
	levelLabel->setPosition(ccp(winSize.width / 2 , winSize.height / 2 - 80));
	this->addChild(levelLabel);

	std::string seqSeq = "Sequenz: "+ seq;
	this->seqLabel = CCLabelTTF::create(seqSeq.c_str(), "Artial", 14);
	seqLabel->retain();
	seqLabel->setColor(ccc3(0, 0, 0));
	seqLabel->setPosition(ccp(winSize.width / 2 , winSize.height / 2 -40));
	this->addChild(seqLabel);

	std::string sDesc = "Description: "+ LevelHelper::getDescriptionForLevel(level);
	this->seqLabel = CCLabelTTF::create(sDesc.c_str(), "Artial", 14);
	seqLabel->retain();
	seqLabel->setColor(ccc3(0, 0, 0));
	seqLabel->setPosition(ccp(winSize.width / 2 , winSize.height / 2 + 20));
	this->addChild(seqLabel);
}

void LevelLoadingLayer::endScreen() {

	 SoundEffectHelper::stopBackgroundMusic();
	 CCScene *pScene = DeeWorld::scene(seq, level);
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene( CCTransitionMoveInB::create(2.0f, pScene));
}

LevelLoadingLayer::~LevelLoadingLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
