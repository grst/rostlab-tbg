#include "MainScreenScene.h"
#include "../helper/WebOpNative.h"
#include "../helper/LevelHelper.h"
#include "../helper/CCHttpRequest.h"
#include "../helper/MatrixHelper.h"
#include "../helper/SoundEffectHelper.h"
#include "SettingsScreenScene.h"
#include "SplashScreenScene.h"
#include "DeeWorldScene.h"
#include "LevelLoadingScene.h"
#include "../ui_elements/cc-extensions/TouchTrailLayer.h"
#include "../ui_elements/ImpressumLayer.h"
#include "../ui_elements/AboutUsLayer.h"
#include "../ui_elements/cc-extensions/CCGestureRecognizer/CCSwipeGestureRecognizer.h"

using namespace cocos2d;

#define kFileStreak "streak.png"
#define TAG_IMPRESSUM_LAYER 789
#define TAG_ABOUTUS_LAYER 796
#define MENU_ITEM_SCALE .7f

bool MainScreenScene::init() {
	if (CCScene::init()) {

		// load a user selected matrix - default is BLOSUM62.txt
		std::string matrix =
				cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey(
						"matrix", "BLOSUM62.txt");

		if (matrix.size() == 0) {
			matrix = "BLOSUM62.txt";
		}

		cocos2d::CCLog("Matrix %s loaded", matrix.c_str());

		//load the scoring matrix
		MatrixHelper::loadMatrix(matrix);

		this->_layer = MainScreenLayer::create();
		this->_layer->retain();
		this->addChild(_layer);

		//TouchTrailLayer * layer2 = TouchTrailLayer::create();
		//this->addChild(layer2);

		return true;
	} else {
		return false;
	}
}

MainScreenScene::~MainScreenScene() {
	if (_layer) {
		_layer->release();
		_layer = NULL;
	}
}

bool MainScreenLayer::init() {

	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}

	// enable buttons
	this->setKeypadEnabled(true);

	//this->setTouchEnabled(true);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	initBackground();

//
//	this->_label = CCLabelTTF::create("Select your level", "Arial", 24);
//	_label->retain();
//	_label->setColor(ccc3(255, 255, 255));
//	_label->setPosition(ccp(winSize.width / 2, winSize.height - 30));
//	this->addChild(_label, 0);

	// Place the menu item bottom-right corner.
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCArray * menuIcons = CCArray::create();
	CCArray * levelIcons = CCArray::create();

	int levels = 10;

	for (int i = 0; i < levels; i++) {

		std: string path = LevelHelper::getPathForLevel(i);

		CCMenuItemImage *levelItem = CCMenuItemImage::create("transparent.png",
				"transparent.png", this,
				menu_selector(MainScreenLayer::menuStartGameCallback));

		/*
		 CCTexture2D *tex = new CCTexture2D;
		 tex->initWithData(NULL, kCCTexture2DPixelFormat_RGB5A1, 200, 200, CCSize(200,200));
		 CCSprite* spr = CCSprite::createWithTexture(tex);
		 CCSprite* selspr =CCSprite::createWithTexture(tex);
		 CCSprite* dis = CCSprite::createWithTexture(tex);
		 */

		/*
		 CCMenuItemSprite * levelItem = CCMenuItemSprite::create(this, spr,
		 selspr, dis,
		 menu_selector(MainScreenLayer::menuStartGameCallback));
		 */

		levelItem->setTag(i);

		levelItem->setContentSize(CCSize(100, 100));

		//	levelItem->setContentSize(CCSize(100,100));

		// levelItem->setScale(0.5);

		// add AA
		CCSprite * spriteAA = CCSprite::create(path.c_str());
		float scale = 0.6;
		spriteAA->setScale(scale);

		spriteAA->setPosition(
				ccp(levelItem->getContentSize().height / 2 ,
						levelItem->getContentSize().width / 2));

		//levelItem->setContentSize(spriteAA->getContentSize());
		levelItem->addChild(spriteAA);
		CCLog("Width: %f, Height: %f", spriteAA->getContentSize().height,
				spriteAA->getContentSize().width);

		CCLabelTTF* strLevelPID = CCLabelTTF::create(
				LevelHelper::getCodeForLevel(i).c_str(), "carrois", 14,
				CCSize(levelItem->getContentSize().width, 20),
				kCCTextAlignmentCenter);

		strLevelPID->setPosition(
				ccp(levelItem->getContentSize().width / 2, -65));
		levelItem->addChild(strLevelPID);

		// ask for stars

		std::string strLevel =
				static_cast<std::ostringstream*>(&(std::ostringstream() << i))->str();
		int currentScore =
				cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
						("level_" + strLevel).c_str(), 0);

		int stars = LevelHelper::getStarsForLevel(i, currentScore);

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

		spriteStar->setPosition(
				ccp(levelItem->getContentSize().width / 2, -40));
		levelItem->addChild(spriteStar);

		levelIcons->addObject(levelItem);
	}

	/*
	 // settings
	 CCMenuItemImage *pSettingsItem = CCMenuItemImage::create(
	 "white/settings.png", "white/settings.png", this,
	 menu_selector(MainScreenLayer::changeScene));
	 pSettingsItem->setScale(MENU_ITEM_SCALE);
	 pSettingsItem->setTag(10);
	 menuIcons->addObject(pSettingsItem);
	 */

	// about us
	CCMenuItemImage *pAboutUs = CCMenuItemImage::create("white/howto.png",
			"white/howto.png", this,
			menu_selector(MainScreenLayer::changeScene));
	pAboutUs->setScale(MENU_ITEM_SCALE);
	pAboutUs->setTag(11);
	menuIcons->addObject(pAboutUs);

	//music
	menuIcons->addObject(
			SoundEffectHelper::getVolumeMenu(15,
					menu_selector(MainScreenLayer::changeScene), this, MENU_ITEM_SCALE, "white"));

	// impressum
	CCMenuItemImage *pImpressum = CCMenuItemImage::create("white/aboutus.png",
			"white/aboutus.png", this,
			menu_selector(MainScreenLayer::changeScene));
	pImpressum->setScale(MENU_ITEM_SCALE);
	pImpressum->setTag(12);
	menuIcons->addObject(pImpressum);

	// close app
	CCMenuItemImage *pCloseApp = CCMenuItemImage::create("white/exit.png",
			"white/exit.png", this,
			menu_selector(MainScreenLayer::changeScene));
	pCloseApp->setScale(MENU_ITEM_SCALE);
	pCloseApp->setTag(13);
	menuIcons->addObject(pCloseApp);

	// Create a menu with our menu items
	mainMenu = CCMenu::createWithArray(menuIcons);
	mainMenu->setPosition(ccp(115, winSize.height - 25));
	mainMenu->alignItemsHorizontallyWithPadding(15);

	/*
	 levelMenu = CCMenu::createWithArray(levelIcons);
	 levelMenu->setPosition(
	 ccp(winSize.width / 2,
	 (winSize.height / 2 - winSize.height * 1 / 9)));
	 levelMenu->alignItemsInColumns(5, 5);
	 */

	int row = 1;
	int col = 3;
	// padding
	CCPoint p = ccp(150, 100);
	CCSize windowSize =
			CCDirector::sharedDirector()->getOpenGLView()->getDesignResolutionSize();

	//some trick to center menu
	float w = 0;
	float h = 0;
	float eWidth = (col - 1) * (w + p.x);
	float eHeight = (row - 1) * (h + p.y);

	CCPoint menuPosition = ccp(windowSize.width / 2.0f - eWidth / 2.0f,
			windowSize.height / 2.0f - eHeight / 2.0f);

	// create Menu for icons
	sliderMenu = SlidingMenuGrid::menuWithArray(levelIcons, col, row,
			menuPosition, p);

	sliderMenu->setAnchorPoint(ccp(0.5, 3 / 9));

	//    sliderMenu->setPosition(100,100);

	// Add the menu to TestWorld layer as a child layer.
	this->addChild(mainMenu, 1);
	this->addChild(sliderMenu, 1);

	SoundEffectHelper::playMainMenuBackgroundMusic();

	// is first start  -> display help screen
	if (HelperFunctions::isFirstStart()) {
		CCLayer* layer2 = AboutUsLayer::create();
		this->sliderMenu->freeze();
		this->addChild(layer2, 10, TAG_ABOUTUS_LAYER);
	}

	// oke for the beginning we track every app start :-)
	//CCHttpRequest *requestor = CCHttpRequest::sharedHttpRequest();

	//std::string url = "http://sprunge.us/AfPU";
	//std::string postData = "key=val";

	//requestor->addGetTask(url, this,			callfuncND_selector(MainScreenLayer::onHttpRequestCompleted));

	//requestor->addPostTask(url, postData, this,
	//		callfuncND_selector(HelloWorld::onHttpRequestCompleted));

	/*	std::vector < std::string > downloads;
	 downloads.push_back("http://www.baidu.com/index.html");
	 requestor->addDownloadTask(downloads, this,
	 callfuncND_selector(HelloWorld::onHttpRequestCompleted));
	 */

	return true;
}

void MainScreenLayer::initBackground() {

	CCSprite* pSpriteBackground = CCSprite::create("wood.jpg");
	HelperFunctions::fitBackground(pSpriteBackground);

// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);
}

void MainScreenLayer::onHttpRequestCompleted(cocos2d::CCObject *pSender,
		void *data) {
	HttpResponsePacket *response = (HttpResponsePacket *) data;

	if (response->request->reqType == kHttpRequestGet) {
		if (response->succeed) {
			CCLog("Get Request Completed!");
			CCLog("Content: %s", response->responseData.c_str());
		} else {
			CCLog("Get Errorcode: " + response->responseCode);
			CCLog("Get Error: %s", response->responseData.c_str());
		}
	} else if (response->request->reqType == kHttpRequestPost) {
		if (response->succeed) {
			CCLog("Post Request Completed!");
			CCLog("Content: %s", response->responseData.c_str());
		} else {
			CCLog("Post Error: %s", response->responseData.c_str());
		}
	} else if (response->request->reqType == kHttpRequestDownloadFile) {
		if (response->succeed) {
			CCLog("Download Request Completed! Downloaded:");

			std::vector<std::string>::iterator iter;
			for (iter = response->request->files.begin();
					iter != response->request->files.end(); ++iter) {
				std::string url = *iter;
				CCLog("%s", url.c_str());
			}
		} else {
			CCLog("Download Error: %s", response->responseData.c_str());
		}
	}
}

void MainScreenLayer::changeScene(CCObject* pSender) {

// disable lower layers
	if (isLayerOpen()) {
		return;
	}

	SoundEffectHelper::playClickSound();

	CCScene *pScene1;

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();
	CCLOG("Changing to settings %d", tag);

	CCLayer * layer2;

	switch (tag) {
	case 10:
		pScene1 = SettingsScreenScene::create();
		//transition to next scene for one sec
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene1));
		break;
	case 11:
		/*
		 pScene1 = PauseLayerScene::create();
		 //transition to next scene for one sec
		 CCDirector::sharedDirector()->replaceScene(
		 CCTransitionFade::create(1.0f, pScene1));
		 */

		if (this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL) {
			this->removeChildByTag(TAG_ABOUTUS_LAYER, true);
			this->sliderMenu->unfreeze();
		} else {
			layer2 = AboutUsLayer::create();
			this->addChild(layer2, 10, TAG_ABOUTUS_LAYER);
			this->sliderMenu->freeze();
		}
		break;

	case 12:
		if (this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL) {
			this->removeChildByTag(TAG_IMPRESSUM_LAYER, true);
			this->sliderMenu->unfreeze();
		} else {
			layer2 = ImpressumLayer::create();
			this->addChild(layer2, 10, TAG_IMPRESSUM_LAYER);
			this->sliderMenu->freeze();
		}
		break;
	case 13:
		pScene1 = SplashScreenScene::create(false);
		CCDirector::sharedDirector()->replaceScene(
				CCTransitionFade::create(1.0f, pScene1));
		break;

		// music
	case 15:
		// toggle music
		HelperFunctions::toggleMusic();
		if(SoundEffectHelper::isSoundEnabled()){
			SoundEffectHelper::playMainMenuBackgroundMusic();
			SoundEffectHelper::playClickSound();
		}else{
			SoundEffectHelper::stopAllMusic();
		}
		break;

	}
}

void MainScreenLayer::menuStartGameCallback(CCObject* pSender) {

// disable lower layers
	if (isLayerOpen()) {
		return;
	}

	SoundEffectHelper::playClickSound();

	CCMenuItem* pMenuItem = (CCMenuItem *) (pSender);
	int tag = (int) pMenuItem->getTag();

	CCLOG("Starting level %d", tag);

	std::string seq = LevelHelper::getSequenceForLevel(tag);

	CCScene *pScene = LevelLoadingScene::create(seq, tag);

//SoundEffectHelper::stopBackgroundMusic();

	CCFiniteTimeAction* actionMove = CCFadeOut::create(0.4);

	// Sebi: we have to add some dummy parameters otherwise it fails on Android
	CCSequence *readySequence = CCSequence::create(actionMove, NULL, NULL);

	pMenuItem->runAction(readySequence);

//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(0.7f, pScene));

}

void MainScreenLayer::keyBackClicked(void) {
	CCLog("key back clicked.");
	if (this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL) {
		this->removeChildByTag(TAG_ABOUTUS_LAYER, true);
		this->sliderMenu->unfreeze();
		return;
	}
	if (this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL) {
		this->removeChildByTag(TAG_IMPRESSUM_LAYER, true);
		this->sliderMenu->unfreeze();
		return;
	}
	CCScene *pScene = SplashScreenScene::create(false);
//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionJumpZoom::create(1.0f, pScene));
}

bool MainScreenLayer::isLayerOpen() {
	if (this->getChildByTag(TAG_ABOUTUS_LAYER) != NULL) {
		return true;
	}
	if (this->getChildByTag(TAG_IMPRESSUM_LAYER) != NULL) {
		return true;
	}
	return false;
}

void MainScreenLayer::keyMenuClicked(void) {

	keyBackClicked();
	/*
	CCScene *pScene = SettingsScreenScene::create();
//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionCrossFade::create(1.0f, pScene));
	*/
}

void MainScreenLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		CCBlade *blade = CCBlade::create(kFileStreak, 4, 50);
		_map[touch] = blade;
		addChild(blade);

		blade->setColor(ccc3(255, 0, 0));
		blade->setOpacity(100);
		blade->setDrainInterval(1.0 / 40);

		CCPoint point = convertTouchToNodeSpace(touch);
		blade->push(point);
	}
}

void MainScreenLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {
	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		if (_map.find(touch) == _map.end())
			continue;

		CCBlade *blade = _map[touch];
		CCPoint point = convertTouchToNodeSpace(touch);

		// TODO fix the blade position
		float tempHeight = CCDirector::sharedDirector()->getWinSize().height;
		CCPoint prevPoint = touch->getPreviousLocation();
		CCPoint prevPoint2 = ccp(prevPoint.x, tempHeight - prevPoint.y);
		CCPoint oldPlayerPoint = ccp(point.x, tempHeight - point.y);

		point = ccpAdd(ccpMult(point, 0.5f), ccpMult(prevPoint, 0.5f));
		blade->push(point);
	}
}

void MainScreenLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {
	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		if (_map.find(touch) == _map.end())
			continue;

		CCBlade *blade = _map[touch];
		blade->autoCleanup();
		_map.erase(touch);
	}
}

MainScreenLayer::~MainScreenLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
