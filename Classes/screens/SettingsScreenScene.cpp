#include "SettingsScreenScene.h"
#include "../helper/WebOpNative.h"
#include "MainScreenScene.h"

using namespace cocos2d;

bool SettingsScreenScene::init() {
	if (CCScene::init()) {
		CCLog("settings: trying to create Layer");
		this->_layer = SettingsScreenLayer::create();
		CCLog("settings: layer created");
		this->_layer->retain();
		CCLog("settings: adding layer");
		this->addChild(_layer);
		CCLog("settings: layer addded");

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

		CCLog("settings: loading background");
		// add "SettingsScreen" splash screen"
		CCSprite* pSpriteBackground = CCSprite::create("NotificationScreenBackground.jpg");

		// position the sprite on the center of the screen
		pSpriteBackground->setPosition(
				ccp(winSize.width / 2, winSize.height / 2));

		// add the sprite as a child to this layer
		this->addChild(pSpriteBackground, 0);

		this->_label = CCLabelTTF::create("", "Arial", 32);
		_label->retain();
		_label->setColor(ccc3(0, 0, 0));
		_label->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(_label);

		//info
		CCLog("settings: loading info");
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

		CCLog("settings: playing music");
		SoundEffectHelper::playMainMenuBackgroundMusic();

		// Add the menu to TestWorld layer as a child layer.
		this->addChild(levelMenu, 1);

		showCurrentMatrix();

		return true;
	} else {
		return false;
	}

}

void SettingsScreenLayer::keyBackClicked(void) {
	SoundEffectHelper::playClickSound();
	endScreen();
}

void SettingsScreenLayer::showCurrentMatrix(){
	int matrixInt =
				(cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
						"matrixInt", 0)) % 8;
	std::string matrix = "BLOSUM62.txt";

	switch (matrixInt) {
		case 0:
			matrix = "BLOSUM62";
			break;
		case 1:
			matrix = "BLOSUM80";
			break;
		case 2:
			matrix = "PAM60";
			break;
		case 3:
			matrix = "PAM80";
			break;
		case 4:
			matrix = "PAM100";
			break;
		case 5:
			matrix = "PAM120";
			break;
		case 6:
			matrix = "PAM160";
			break;
		case 7:
			matrix = "PAM250";
			break;
	}

	std::string strMatrix = "Sel. Matrix: " + matrix;
	CCLabelTTF* pauseLabel = CCLabelTTF::create(strMatrix.c_str(), "Arial", 24);


	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	pauseLabel->setPosition(ccp(pauseLabel->getContentSize().width / 2,winSize.height - pauseLabel->getContentSize().height/3));

	// add this to the layer
	this->addChild(pauseLabel, 1);
}

void SettingsScreenLayer::changeScene(CCObject* pSender) {

	SoundEffectHelper::playClickSound();

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();



	int matrixInt =
			(cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
					"matrixInt", 0) + 1) % 8;
	std::string matrix = "BLOSUM62.txt";

	switch (matrixInt) {
		case 0:
			matrix = "BLOSUM62.txt";
			break;
		case 1:
			matrix = "BLOSUM80.txt";
			break;
		case 2:
			matrix = "PAM60.txt";
			break;
		case 3:
			matrix = "PAM80.txt";
			break;
		case 4:
			matrix = "PAM100.txt";
			break;
		case 5:
			matrix = "PAM120.txt";
			break;
		case 6:
			matrix = "PAM160.txt";
			break;
		case 7:
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
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0f, pScene));
}

SettingsScreenLayer::~SettingsScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
