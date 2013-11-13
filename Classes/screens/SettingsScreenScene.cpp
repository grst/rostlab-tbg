#include "SettingsScreenScene.h"
#include "../helper/WebOpNative.h"
#include "MainScreenScene.h"

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

		// enable Android back button
		this->setKeypadEnabled(true);

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

		//info

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		CCArray * menuIcons = CCArray::create();

		CCMenuItemImage *pCloseItem = CCMenuItemImage::create("settings.png",
				"settings.png", this,
				menu_selector(SettingsScreenLayer::changeScene));

		// some stupid rectangular order of the menu
		pCloseItem->setPosition(
				ccp(winSize.width / 2,
						origin.y + winSize.height / 2));
		// maybe we should use the dedicated align method for this?

		pCloseItem->setTag(10);

		menuIcons->addObject(pCloseItem);

		// Create a menu with our menu items
		levelMenu = CCMenu::createWithArray(menuIcons);
		levelMenu->setPosition(CCPointZero);

		SoundEffectHelper::playMainMenuBackgroundMusic();

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 1);

		return true;
	} else {
		return false;
	}

}

void SettingsScreenLayer::keyBackClicked(void) {
	SoundEffectHelper::playClickSound();
	endScreen();
}

void SettingsScreenLayer::changeScene(CCObject* pSender) {

	SoundEffectHelper::playClickSound();

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();



	int matrixInt =
			(cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
					"matrixInt", 0) + 1) % 3;
	std::string matrix = "BLOSUM62.txt";

	switch (matrixInt) {
	case 0:
		matrix = "BLOSUM62.txt";
		break;
	case 1:
		matrix = "PAM100.txt";
		break;
	case 2:
		matrix = "PAM250.txt";
		break;
	}

	CCLOG("Changing matrix to %s", matrix.c_str());

	cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey("matrixInt",
			matrixInt);

	//toggle same settings
	cocos2d::CCUserDefault::sharedUserDefault()->setStringForKey("matrix",
			matrix);

	endScreen();
}

void SettingsScreenLayer::endScreen() {

	CCScene *pScene = MainScreenScene::create();
		//transition to next scene for one sec
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene));
}

SettingsScreenLayer::~SettingsScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
