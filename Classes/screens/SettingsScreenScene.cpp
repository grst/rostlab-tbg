#include "SettingsScreenScene.h"
#include "helper/WebOpNative.h"

using namespace cocos2d;

bool SettingsScreenScene::init() {
	if (CCScene::init()) {
		this->_layer = SettingsScreenLayer::create();
		this->_layer->retain();
		this->addChild(_layer);

		return true;
	} else {
		return false;
	}
}

SettingsScreenScene::~SettingsScreenScene() {
	if (_layer) {
		_layer->release();
		_layer = NULL;
	}
}

bool SettingsScreenLayer::init() {
	if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {

		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

		// add "SettingsScreen" splash screen"
		CCSprite* pSpriteBackground = CCSprite::create("background.png");

		// position the sprite on the center of the screen
		pSpriteBackground->setPosition(
				ccp(winSize.width / 2, winSize.height / 2));

		// add the sprite as a child to this layer
		this->addChild(pSpriteBackground, 0);

		this->_label = CCLabelTTF::create("", "Artial", 32);
		_label->retain();
		_label->setColor(ccc3(0, 0, 0));
		_label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(_label);

		// add "SettingsScreen" splash screen"
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

		// set a delay for two seconds
		this->runAction(
				CCSequence::create(CCDelayTime::create(2),
						CCCallFunc::create(this,
								callfunc_selector(
										SettingsScreenLayer::endScreen)),
						NULL));

		return true;
	} else {
		return false;
	}
}

void SettingsScreenLayer::endScreen() {

	//WebOpNative::openLink("http://www.rostlab.org");
	/*
	 CCScene *pScene = DeeWorld::scene();
	 //transition to next scene for one sec
	 CCDirector::sharedDirector()->replaceScene(
	 CCTransitionFade::create(1.0f, pScene));
	 */
}

SettingsScreenLayer::~SettingsScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
