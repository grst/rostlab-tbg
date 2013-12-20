#include "DeeWorldScene.h"
#include "LevelEndScene.h"
#include "SplashScreenScene.h"
#include "SimpleAudioEngine.h"
#include "../helper/MatrixHelper.h"
#include "../helper/UIElements.h"
#include "MainScreenScene.h"
#include "../ui_elements/PauseLayer.h"
#include "../helper/SoundEffectHelper.h"
#include "../helper/LevelHelper.h"

USING_NS_CC;

#define PTM_RATIO 32.0
#define INTRO_TIME_SECONDS 3
#define TOLERANCE_PLAYER -30
#define PLAYER_IMAGE "Player.png"
#define BLADE_FILE "streak.png"
#define TAG_PAUSE_LAYER 567 // arbitrary tag for pause menu - DO NOT MODIFY
#define DEBUG_DRAW 0 // 1 enable debug draw, 0 disable

using namespace cocos2d;

DeeWorld::~DeeWorld() {
	if (_scoreLabel) {
		_scoreLabel->release();
		_scoreLabel = NULL;
	}

	// TODO add release
	//CC_SAFE_RELEASE(_scoreNumber);

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

DeeWorld::DeeWorld() {

}

/*
 * pass the sequence on construction
 */
CCScene* DeeWorld::scene(std::string sequence, int level) {
	CCScene * scene = NULL;
	do {

		// TODO Those aren't autorelease objects

		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		// 'layer' is an autorelease object
		DeeWorld *layer = DeeWorld::create();
		CC_BREAK_IF(!layer);

		layer->setSequence(sequence);
		layer->level = level;

		// add layer as a child to scene
		scene->addChild(layer);

	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool DeeWorld::init() {

	if (!CCLayer::init()) {
		return false;
	}
	if (!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {
		return false;
	}

	pausedGame = false;
	gameEnded = false;
	cocos2d::CCLog("Hello World. App ist starting now");
	AAcounter = 0;
    acidCounter = new AcidCounter();

	initBackground();
	initBox2D();

	////TAGS
	// 0 = player
	// 1 = target
	// 3 = wall
	initWorld();
	initPlayer();


	// enable Android back button
	this->setKeypadEnabled(true);

	this->setTouchEnabled(true);

	initInfoUI();
	initListener();

	updateInfoUI();

	SoundEffectHelper::playLevelBackgroundMusic(level);

	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(
			this, 10);

	CCLog("init successful");

	startTime = HelperFunctions::nowInSeconds();

	return true;
}

void DeeWorld::initBackground() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* pSpriteBackground = CCSprite::create("wood.jpg");

	HelperFunctions::fitBackground(pSpriteBackground);

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);
}

//Listen event.
void DeeWorld::initListener() {
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
			callfuncO_selector(DeeWorld::onApplicationStatusChanged),
			"APP_STATUS_CHANGED", NULL);
}

//Handling event
void DeeWorld::onApplicationStatusChanged(CCObject* obj) {
	CCBool* b = (CCBool*) obj;
	if (b->getValue()) {
		CCLog("app resumed");
	//	CCMessageBox("Application resumed", "Debug by us");
		pauseAction(2);
	} else {
		CCLog("app went into background");
		if (!pausedGame ){
			pauseAction(1);
		}
	}

}

void DeeWorld::makeMenu() {

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCMenuItemImage *pSettings = CCMenuItemImage::create("white/settings.png",
			"white/settings.png", this,
			menu_selector(DeeWorld::pauseLayerCallback));
	pSettings->setScale(.7f);

	// Place the menu item top-left corner.
	pSettings->setPosition(ccp(20, winSize.height - 20));

	if (!pSettings) {
		return;
	}

	// Create a menu with the "close" menu item, it's an auto release object.
	CCMenu* pMenu = CCMenu::create(pSettings, NULL);
	pMenu->setPosition(CCPointZero);
	if (!pMenu) {
		return;
	}

	// Add the menu to DeeWorld layer as a child layer.
	this->addChild(pMenu, 1);
}

void DeeWorld::initBox2D() {
	b2Vec2 gravity = b2Vec2(0.0f, -0.0f); //no gravity
	_b2dWorld = new b2World(gravity);

	this->schedule(schedule_selector(DeeWorld::tick));

	//debug draw made easy with helper class
	if (DEBUG_DRAW > 0) {
		this->addChild(B2DebugDrawLayer::create(_b2dWorld, PTM_RATIO), 9999);
	}

	// Create contact listener
	_b2dWorld->SetContactListener(this);
}

/**
 * creates the walls around the screen
 */
void DeeWorld::initWorld() {
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	// Create edges around the entire screen
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	CCSprite * emptySprite = new CCSprite(); //only to keep tag.
	emptySprite->setTag(3);
	groundBodyDef.userData = emptySprite;

	b2Body* groundBody = _b2dWorld->CreateBody(&groundBodyDef);
	b2PolygonShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;
	int wallThickness = 40;
	int wallOffset = wallThickness * 1;

	//wall definitions
	groundEdge.SetAsBox(visibleSize.width / PTM_RATIO,
			wallThickness / PTM_RATIO,
			b2Vec2(visibleSize.width / PTM_RATIO / 2, -wallOffset / PTM_RATIO),
			0);
	bottom = groundBody->CreateFixture(&boxShapeDef);
	groundEdge.SetAsBox(wallThickness / PTM_RATIO,
			visibleSize.height / PTM_RATIO,
			b2Vec2(-wallOffset / PTM_RATIO, visibleSize.height / PTM_RATIO / 2),
			0);
	left = groundBody->CreateFixture(&boxShapeDef);
	groundEdge.SetAsBox(visibleSize.width / PTM_RATIO,
			wallThickness / PTM_RATIO,
			b2Vec2(visibleSize.width / PTM_RATIO / 2,
					(visibleSize.height + wallOffset) / PTM_RATIO), 0);
	top = groundBody->CreateFixture(&boxShapeDef);
	groundEdge.SetAsBox(wallThickness / PTM_RATIO,
			visibleSize.height / PTM_RATIO,
			b2Vec2((visibleSize.width + wallOffset) / PTM_RATIO,
					visibleSize.height / PTM_RATIO / 2), 0);
	right = groundBody->CreateFixture(&boxShapeDef);

}

/**
 * initialiye Plazer box2d body and corresponding CCSprite
 */
void DeeWorld::initPlayer() {

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	//player
	CCSprite *sPlayer = CCSprite::create(PLAYER_IMAGE);
	sPlayer->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));
	HelperFunctions::resizseSprite(sPlayer, 32, 0);

	float x = 1 / PTM_RATIO / 8;
	float y = 1 / PTM_RATIO / 8;

	int num = 8;
	b2Vec2 vertices[] = { b2Vec2(-126.0f * x, -34.0f * y), b2Vec2(-66.0f * x,
			-114.0f * y), b2Vec2(34.0f * x, -124.0f * y), b2Vec2(112.0f * x,
			-65.0f * y), b2Vec2(127.0f * x, 33.0f * y), b2Vec2(65.0f * x,
			113.0f * y), b2Vec2(-30.0f * x, 127.0f * y), b2Vec2(-116.0f * x,
			67.0f * y) };

	//create the box for the player

	player = CreateBox2DBodyForSprite(sPlayer, num, vertices);
	//only rectangle around the player (for testing)
	// player = CreateBox2DBodyForSprite(sPlayer, 0, NULL);

	player->SetUserData(sPlayer);
	player->SetType(b2_kinematicBody);

	//z-order 3, tag=0
	this->addChild(sPlayer, 3, 0);
}

/**
 * initialize Information UI-Elements like score, timer, and amino-acid-code
 * TODO: also move to UI-Elements-Class (?)
 */
void DeeWorld::initInfoUI() {
	float cornerScale = .7f;
	int num = 5;
	b2Vec2 verts_right[] = { b2Vec2(-107.5f * cornerScale / PTM_RATIO,
			45.5f * cornerScale / PTM_RATIO), b2Vec2(
			-106.5f * cornerScale / PTM_RATIO, 1.5f * cornerScale / PTM_RATIO),
			b2Vec2(-87.5f * cornerScale / PTM_RATIO,
					-20.5f * cornerScale / PTM_RATIO), b2Vec2(
					130.5f * cornerScale / PTM_RATIO,
					-19.5f * cornerScale / PTM_RATIO), b2Vec2(
					130.5f * cornerScale / PTM_RATIO,
					45.5f * cornerScale / PTM_RATIO) };

	b2Vec2 verts_left[] = { b2Vec2(-130.5f * cornerScale / PTM_RATIO,
			45.5f * cornerScale / PTM_RATIO), b2Vec2(
			-129.5f * cornerScale / PTM_RATIO,
			-18.5f * cornerScale / PTM_RATIO), b2Vec2(
			86.5f * cornerScale / PTM_RATIO, -20.5f * cornerScale / PTM_RATIO),
			b2Vec2(108.5f * cornerScale / PTM_RATIO,
					-1.5f * cornerScale / PTM_RATIO), b2Vec2(
					108.5f * cornerScale / PTM_RATIO,
					46.5f * cornerScale / PTM_RATIO) };

	CCSize layerSize = this->getContentSize();

	CCSprite * cornerLeft = CCSprite::create("corner-left.png");
	CCSprite * cornerRight = CCSprite::create("corner-right.png");

	cornerLeft->setScale(cornerScale);
	cornerRight->setScale(cornerScale);

	cornerLeft->setPosition(
			ccp(cornerLeft->getContentSize().width * cornerScale / 2,
					layerSize.height
							- cornerLeft->getContentSize().height * cornerScale
									/ 2));
	cornerRight->setPosition(
			ccp(
					layerSize.width
							- cornerRight->getContentSize().width * cornerScale
									/ 2,
					layerSize.height
							- cornerLeft->getContentSize().height * cornerScale
									/ 2));
	cornerLeft->setTag(3);
	cornerRight->setTag(3);
	b2Body * clbody = CreateBox2DBodyForSprite(cornerLeft, num, verts_left);
	b2Body * crbody = CreateBox2DBodyForSprite(cornerRight, num, verts_right);
	clbody->SetType(b2_staticBody);
	crbody->SetType(b2_staticBody);

	//reset AminoHit
	lastAminoHitTime = 0.0f;

	score = 0;

	CCLog("setting score");
	//score
	// int -> str
	this->_scoreLabel = CCLabelTTF::create("Score: 0", "Helvetica", 24,
			CCSizeMake(150, 30), kCCTextAlignmentRight);
	this->_scoreLabel->retain();
	this->_scoreLabel->setPosition(
			ccp(layerSize.width - 90, layerSize.height - 20));
	this->_scoreLabel->setColor(ccc3(255, 255, 255));

	this->addChild(cornerRight);
	this->addChild(cornerLeft);
	this->addChild(_scoreLabel);

	makeMenu();

	CCLog("setting timer");
	//timer
	pauseGame();
	startTimer =true;
	showCountdown(INTRO_TIME_SECONDS);
	UIElements::loadCache(this);
}

void DeeWorld::showCountdown(int time) {
	timer = time;

	CCSize layerSize = this->getContentSize();
	this->_timerLabel = CCLabelTTF::create("", "Helvetica",
			layerSize.height * 2 / 3, layerSize, kCCTextAlignmentCenter);
	this->_timerLabel->retain();
	this->_timerLabel->setPosition(
			ccp(layerSize.width / 2, layerSize.height / 2));
	this->_timerLabel->setColor(ccc3(0, 0, 0));
	this->addChild(_timerLabel);

	CCActionInterval * tintTo = CCTintTo::create(timer, 20, 25, 255);
	this->_timerLabel->runAction(tintTo);
	pauseGame();
	this->countdown();
}

/**
 * countdown at the beginning of each level
 */
void DeeWorld::countdown() {
	if (timer == 1) {
		CCActionInterval * fadeOut = CCFadeOut::create(1.0);
		this->_timerLabel->runAction(fadeOut);

	}
	if (this->timer >= 1) {
		SoundEffectHelper::playTimerTickSound();

		//convert int to string
		string timeStr = static_cast<ostringstream*>(&(ostringstream()
				<< this->timer))->str();
		this->_timerLabel->setString(timeStr.c_str());
		this->timer = this->timer - 1;
		if (!startTimer) {
			this->runAction(
					CCSequence::create(CCDelayTime::create(1),
							CCCallFunc::create(this,
									callfunc_selector(DeeWorld::countdown)),

							NULL));
		} else {
			this->runAction(
					CCSequence::create(CCDelayTime::create(2),
							CCCallFunc::create(this,
									callfunc_selector(DeeWorld::countdown)),

							NULL));
			startTimer = false;
		}
	} else {
		CCLog("create targets");
		this->removeChild(this->_timerLabel);
		if (this->_timerLabel) {
			this->_timerLabel->release();
			this->_timerLabel = NULL;
		}
		resumeGame();
	}
}

/**
 * detects, depending on the player's position,
 * in which corner the next amino acid should appear.
 */
int DeeWorld::detectCorner() {
	//split board into 4 areas:
	// A1 | B2
	// -----
	// C0 | D3
	// A and C overlap partly, B and D do likewise
	// if the player is found in one of the areas,
	// the amino acid must not appear in that area
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	float areaWidth = (visibleSize.width / 2)
			- ((CCSprite *) player->GetUserData())->getContentSize().width;
	float areaHeight = (visibleSize.height / 2) * 1.2;
	CCSprite* sPlayer = (CCSprite *) player->GetUserData();
	CCPoint playerPosition = sPlayer->getPosition();
	vector<int> validCorners;

	//player in C
	if (!(playerPosition.x < areaWidth && playerPosition.y < areaHeight)) {
		validCorners.push_back(0);
	}
	//player in A
	if (!(playerPosition.x < areaWidth
			&& playerPosition.y > visibleSize.height - areaHeight)) {
		validCorners.push_back(1);
	}
	//player in B
	if (!(playerPosition.x > visibleSize.width - areaWidth
			&& playerPosition.y > visibleSize.height - areaHeight)) {
		validCorners.push_back(2);
	}
	//player in C
	if (!(playerPosition.x > visibleSize.width - areaWidth
			&& playerPosition.y < areaHeight)) {
		validCorners.push_back(3);
	}

	int length = validCorners.size();
	return validCorners.at(arc4random() % length);
}

/**
 * adds a random target to the game
 */
void DeeWorld::addTarget() {
    char targetToAdd = getCurrentAminoAcid();
    CCLog("Failig AA %c || test:A = %c", targetToAdd, 'A');
    char c;
    if(acidCounter->hasAcid(targetToAdd) ||
       HelperFunctions::randomValueBetween(0, this->getOtherAAProp()) > 1) {
        c = MatrixHelper::getRandomAminoAcid();
    } else {
    	CCLog("Did not Found AA in MatrixHelper");
        c = targetToAdd;
    }
    addTarget(c);
}

/**
 * adds a target to the game
 * creates the box2d body and the corresponding sprite
 * TODO: let the target drop into the screen from a random corner and start movement
 */
void DeeWorld::addTarget(char c) {
	AminoAcid *sTarget = AminoAcid::create(c);
	CCLog("Adding Target called: %c ", +sTarget->getType());
	sTarget->setScale(MatrixHelper::getRelativeScaleFactor(sTarget->getType()));
	//Place target in a randomly picked corner.
	int startX, startY;
	int corner = detectCorner();
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	b2Vec2 impulse;
	int minDeg = 10;
	int maxDeg = 80;
	//position of the amino acids:
	//inside the board, but directly at the border!
	switch (corner) {
	case 0:
		//left bottom
		startX = 0;
		startY = 0;
		break;
	case 1:
		//left top
		startY = winSize.height - 60; //due to corner
		startX = 0;
		break;
	case 2:
		//right top
		startX = winSize.width;
		startY = winSize.height - 60;
		break;
	case 3:
		//right bottom
		startX = winSize.width;
		startY = 0;
		break;
	}

	CCLog("Start-Position:x=%i,y=%i", startX, startY);
	sTarget->setPosition(ccp(startX, startY));

	AAcounter++;
	this->addChild(sTarget);
    acidCounter->incCounter(c);

	//Temp: static polygon
	//TODO: put in MatrixHelperClass or read from file
	//row 1, col 1

	b2Vec2 *verts = MatrixHelper::getVerticeData(sTarget->getType(),
			MatrixHelper::getRelativeScaleFactor(sTarget->getType()) / PTM_RATIO
					/ 4);
	int n = MatrixHelper::getVerticeNum(sTarget->getType());
	//Move Target
	//random direction, random velocity.
	minDeg -= 90 * corner;
	maxDeg -= 90 * corner;
	float deg = HelperFunctions::randomValueBetween(minDeg, maxDeg);
	float scale = HelperFunctions::randomValueBetween(getMinSpeed(),
			getMaxSpeed());
	b2Vec2 vel = b2Vec2(sin(CC_DEGREES_TO_RADIANS(deg)),
			cos(CC_DEGREES_TO_RADIANS(deg)));
	vel.operator*=(scale);
	b2Body* target = CreateBox2DBodyForSprite(sTarget, n, verts);
	target->SetLinearVelocity(vel);
}

/**
 * callback for touch-start event. 
 * necessary for movement of the player. 
 * checks whether the touch is valid (=on the player) or not. 
 */
void DeeWorld::ccTouchesBegan(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {
	validTouch = true;
	tempHeight = CCDirector::sharedDirector()->getWinSize().height;

	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*) *it;
	CCPoint pt = touch->getLocationInView();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCSprite* sPlayer = (CCSprite*) player->GetUserData();
	float height = size.height;
	float y = height - pt.y;

	if (std::abs(pt.x - sPlayer->getPositionX()) - TOLERANCE_PLAYER
			> sPlayer->getContentSize().width) {
		//non valid
//		cocos2d::CCLog(
//				"ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
//				touch->getID(), (int) pt.x, (int) pt.y,
//				(int) sPlayer->getPositionX(), (int) sPlayer - getPositionY());
		validTouch = false;
		return;
	}
	if (std::abs(y - sPlayer->getPositionY()) - TOLERANCE_PLAYER
			> sPlayer->getContentSize().height) {
		//non valid
//		cocos2d::CCLog(
//				"ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
//				touch->getID(), (int) pt.x, (int) pt.y,
//				(int) sPlayer->getPositionX(), (int) sPlayer - getPositionY());
		validTouch = false;
		return;
	}

	for (CCSetIterator it = touches->begin(); it != touches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		CCBlade *blade = CCBlade::create(BLADE_FILE, 4, 50);
		_map[touch] = blade;
		addChild(blade);

		// TODO set the color of the blade
		blade->setColor(ccc3(100, 255, 0));
		blade->setOpacity(255);
		blade->setDrainInterval(1.0 / 40);
		blade->setZOrder(20);

		CCPoint point = convertTouchToNodeSpace(touch);
		blade->push(point);
	}
}

/**
 * callback for touch-move event
 * makes the player follow the finger. 
 */
void DeeWorld::ccTouchesMoved(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {

	if (validTouch == false) {
		return;
	}

	CCSetIterator it = touches->begin();
	CCTouch* touch;
	for (int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++) {
		touch = (CCTouch*) (*it);
		it++;
	}

	CCPoint pt = touch->getLocationInView();
    if(pt.y < 50 && (pt.x < 167 || pt.x > 313)) {
        pt.y = 50;
    }
	player->SetTransform(b2Vec2(pt.x / PTM_RATIO, (tempHeight - pt.y) / PTM_RATIO), 0);

	for (CCSetIterator it = touches->begin(); it != touches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		if (_map.find(touch) == _map.end())
			continue;

		CCBlade *blade = _map[touch];
		CCPoint point = convertTouchToNodeSpace(touch);

		//	cocos2d::CCLog("TouchTrail x:%d, y:%d", int(point.x), int(point.y));

		//point = ccpAdd(ccpMult(point, 0.5f), ccpMult(touch->getPreviousLocation(), 0.5f));

		blade->push(point);
	}

	return;
}

/**
 * callback for touch-end event
 * currently doesn't do anything. Can be used in future to play a sound or whatever.
 */
void DeeWorld::ccTouchesEnded(cocos2d::CCSet* pTouches,
		cocos2d::CCEvent* event) {

	for (CCSetIterator it = pTouches->begin(); it != pTouches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		if (_map.find(touch) == _map.end())
			continue;

		CCBlade *blade = _map[touch];
		blade->autoCleanup();
		_map.erase(touch);
	}
}

/**
 * this function is called when the back button is pressed. only useful for android?
 */
void DeeWorld::keyBackClicked(void) {
	pauseAction(0);
}

void DeeWorld::keyMenuClicked(void) {
	pauseAction(0);
}

/**
 * contact listener for collision detection
 */
/**
 * TODO: implement game logic entirely, clean up
 * TODO: move everything that has something to do with the InfoUI to external Method, better external class.
 * TODO: integeate into BeginContact Handler or move parts to seperate method
 */
void DeeWorld::BeginContact(b2Contact *contact) {
	if (!isGamePaused()) {
		//CCLog("BeginContact");
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		AminoAcid* toRemove = NULL;

		b2Body* target;

		// Get the box2d bodies for each object
		b2Body *bodyA = fixtureA->GetBody();
		b2Body *bodyB = fixtureB->GetBody();
		if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
			CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
			CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
			int iTagA = spriteA->getTag();
			int iTagB = spriteB->getTag();

			// Is sprite A a player and sprite B a target?
			if (iTagA == 0 && iTagB == 1) {
				target = bodyB;
				CCLog("Collision: Player on Target");
				this->manageCollision(target);
			}
			// Is sprite A a target and sprite B a player?
			else if (iTagA == 1 && iTagB == 0) {
				target = bodyA;
				CCLog("Collision: Target on Player");
				this->manageCollision(target);
			}
			// Is sprite A a target and sprite B a wall?
			else if (iTagA == 1 && iTagB == 3) {
				CCLog("Collision: Wall on Target");
				toRemove = (AminoAcid*) spriteA;
			}
			// Is sprite A a wall and sprite B a target?
			else if (iTagA == 3 && iTagB == 1) {
				CCLog("Collision: Target on Wall");
				toRemove = (AminoAcid*) spriteB;
			}

			// if a target collides with a wall, we want to remove it with a certain probability
			if (toRemove != NULL) {
				if (HelperFunctions::nowInMilliSeconds() - toRemove->getCreatedTime() > LevelHelper::getTimeToLive(level)) {
					toRemove->flagForDelete();
				}
			}
		}
	}
}

/**
 * create a box2d body for a given sprite.
 *
 * This function send the vertice data to Box2D. Also, if you pass iNumVerts==0 and verts==NULL it simply creates a
 * box round your sprite.
 *
 * Please note, that box2d has an 8-vertice-limit!
 *
 * @param CCSprite sprite the sprite
 * @param int iNumVerts #vertices, if 0 it will create a rectangluar box around the sprite
 * @param b2Vec2 verts[] the vertice-data, if NULL it will create a rectangular box around the sprite
 * @return b2Body the created box2d-body
 */
b2Body* DeeWorld::CreateBox2DBodyForSprite(cocos2d::CCSprite *sprite,
		int iNumVerts, b2Vec2 verts[]) {

	if (_b2dWorld == NULL) {
		return NULL;
	}
	if (iNumVerts > 8) {
		CCLog("Number of vertices exceeds 8!");
	}

	CCPoint pos = sprite->getPosition();
	CCSize size = sprite->getContentSize();

	b2BodyDef spriteBodyDef;

	spriteBodyDef.type = b2_dynamicBody;
	spriteBodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);
	spriteBodyDef.userData = sprite;
	spriteBodyDef.angularDamping = 0.9f; //slow down fast rotations quickly
	b2Body *spriteBody = _b2dWorld->CreateBody(&spriteBodyDef);

	b2PolygonShape spriteShape;
	b2FixtureDef spriteShapeDef;
	spriteShapeDef.shape = &spriteShape;
	spriteShapeDef.density = 10.0;
	spriteShapeDef.isSensor = false;
	spriteShapeDef.restitution = 1.0f;

	if (iNumVerts != 0) {
		//Apparently it crashes somewhere here
		spriteShape.Set(verts, iNumVerts);
		spriteBody->CreateFixture(&spriteShapeDef);
	} else {
		spriteShape.SetAsBox(size.width / PTM_RATIO / 2,
				size.height / PTM_RATIO / 2);
		spriteBody->CreateFixture(&spriteShapeDef);
	}
	return spriteBody;
}

/**
 * updates the InfoUI-Elements.
 * set new score, set new amino-acid-code
 */
void DeeWorld::updateInfoUI() {
	//update score
	std::ostringstream out;
	out << "Score: " << score;
	this->_scoreLabel->setString(out.str().c_str());
	this->_scoreLabel->draw();
	this->_scoreLabel->update(0.5);
}

/**
 * updates CCSprites to go along with the box2d bodies
 * removes flagged sprites
 * updates velocity of moving items. 
 * method is invoked by scheduler every #n ms
 */
void DeeWorld::tick(float delta) {

	//  TODO what happens if AA is deleted / remove
	if (!isGamePaused()) {

		for (b2Body *b = _b2dWorld->GetBodyList(); b; b = b->GetNext()) {
			if (b->GetUserData() != NULL) {
				// We know that the user data is a sprite since we set
				// it that way, so cast it...
				CCSprite *sprite = (CCSprite *) b->GetUserData();

				//amino-Acid-specific-actions
				AminoAcid* aSprite = dynamic_cast<AminoAcid*>(sprite);
				if (aSprite != 0) {
					if (aSprite->isFlaggedForDelete()) {
						std::string stro = "DELETE ";
						stro.append(std::string(1, aSprite->getType()));
						CCLog("%s", stro.c_str());
						AAcounter--;
						b->SetUserData(NULL);
						b = NULL;
						//delete b;
						this->removeChild(aSprite, true);
						_b2dWorld->DestroyBody(b);
						continue;
					}
				}
			}
		}

		//CCLog("Starting tickTack");
		_b2dWorld->Step(delta, 10, 10);
		//CCLog("Stopping tickTack");

		// Loop through all of the Box2D bodies in our Box2D world..
		for (b2Body *b = _b2dWorld->GetBodyList(); b; b = b->GetNext()) {
			// See if there's any user data attached to the Box2D body
			// There should be, since we set it in addBoxBodyForSprite

			if (b->GetUserData() != NULL) {
				// We know that the user data is a sprite since we set
				// it that way, so cast it...
				CCSprite *sprite = (CCSprite *) b->GetUserData();

				//amino-Acid-specific-actions
				AminoAcid* aSprite = dynamic_cast<AminoAcid*>(sprite);
				if (aSprite != 0) {
					if (aSprite->isFlaggedForDelete()) {
                        acidCounter->decCounter(aSprite->getType());
						AAcounter--;
						_b2dWorld->DestroyBody(b);
						this->removeChild(aSprite, true);
						continue;
					}


					//update velocity to be within MIN_SPEED and MAX_SPEED
					//TODO: accelearte gradually (?)
					// --> tutorial www.iforce2d.net/b2dtut/constant-speed
					b2Vec2 vel = b->GetLinearVelocity();
					//CCLog("Speed: %f, x: %f, y: %f", vel.Length(), vel.x, vel.y);
					if (vel.Length() > getMaxSpeed()
							|| vel.Length() < getMinSpeed()) {
						float desiredVelocity =
								HelperFunctions::randomValueBetween(
										getMinSpeed(), getMaxSpeed());
						vel.Normalize();
						//  CCLog("Normalized: %f, x: %f, y: %f", vel.Length(), vel.x, vel.y);
						vel.operator*=(desiredVelocity);
						// CCLog("New Speed: %f, x: %f, y: %f", vel.Length(), vel.x, vel.y);
						b->SetLinearVelocity(vel);
					}
				}

				//update sprites to match the simulation
				sprite->setPosition(
						ccp(b->GetPosition().x * PTM_RATIO,
								b->GetPosition().y * PTM_RATIO));

				// TODO: do we really want to rotate the player?
				sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
			}
		}
        
		//if (isAminoAcidRemaining()) {
		//add aminoAcids, if neccessary
		while (AAcounter < this->getMinAA()) {
			CCLog("adding AAs to min (1)");
			this->addTarget();
		}
		if (AAcounter < this->getMaxAA()) {
			if (HelperFunctions::randomValueBetween(0, this->getAAAddProb())
					< 1) {
				CCLog("adding AAs to max (2)");
				this->addTarget();
			}
		}
		//}
	}
}

/**
 * Manages the collision of the b2Body target (the Amino-Acid)
 * with the player. 
 * - remove amino-acid
 * - update counter
 * - update score
 * - effects
 */
void DeeWorld::manageCollision(b2Body* target) {
	if (target->GetUserData() == NULL) {
		return;
	}
	AminoAcid* aTarget = (AminoAcid*) target->GetUserData();

	CCLog("Collision with %c", aTarget->getType());

	scoreAminoAcid(aTarget);

	//animate our own player
	CCSprite* playerSprite = (CCSprite*) this->player->GetUserData();
	//  if(!blockedPlayer){
	if (false) {
		CCActionInterval * rotateBy = CCFlipY3D::create(3.0);

		playerSprite->runAction(rotateBy);
		blockedPlayer = true;
		this->runAction(
				CCSequence::create(CCDelayTime::create(4),
						CCCallFunc::create(this,
								callfunc_selector(DeeWorld::playerRestore)),
						NULL));
	}

	CCActionInterval * tintTo = CCTintTo::create(2.0, 122, 156, 163);
	playerSprite->runAction(tintTo);

	playerSprite->stopAllActions();
	HelperFunctions::resizseSprite(playerSprite, 64, 64);
	CCActionInterval * scalteTo = CCScaleBy::create(.3, 2.0);
	CCActionInterval * scaleBack = CCScaleBy::create(.3, 0.5);
	playerSprite->runAction(scalteTo);
	playerSprite->runAction(scaleBack);

	//remove amino acid
	aTarget->flagForDelete();

	this->updateInfoUI();
}

// TODO: remove or use
void DeeWorld::playerRestore() {
	return;
	CCSprite* playerSprite = (CCSprite*) this->player->GetUserData();
	CCActionInterval * rotateBy = CCFlipX3D::create(2.0);
	playerSprite->runAction(rotateBy);
	blockedPlayer = false;
}

/**
 * computes the score of the collision of the player
 * with the AminoAcid sTarget. 
 * 
 * The Method updates the value of the score, updates the InfoUI
 * and displays a nice  * animation with the score 
 * of the current collision.
 */
void DeeWorld::scoreAminoAcid(AminoAcid* sTarget) {

	if (this->_code.size() != 0 && !gameEnded) {
		//scoring-event
		BoardAcid * acid = this->_code.front();

		char hitAcid = sTarget->getType();
		char wantedAcid = acid->acid;

		// show score
		int scoring = MatrixHelper::getScoreForAminoAcid(wantedAcid, hitAcid);
		// add to the current score
		score = scoring + score;

		UIElements::displayScoreEffect(this, scoring);
		UIElements::runDestroyAcidEffect(acid->_label);

		// play a nice sound
		if (SoundEffectHelper::isSoundEnabled()) {
			if (scoring > 0) {
				SoundEffectHelper::playPositiveCollisionSound();
			} else if (scoring == 0) {
				SoundEffectHelper::playNeutralCollisionSound();
			} else {
				SoundEffectHelper::playNegativeCollisionSound();
			}
		}

		// TODO release acid;
		_code.pop();

		UIElements::createNewAminoAcid(this);
	}
}

void DeeWorld::pauseLayerCallback(CCObject* pSender) {
	if (!isLayerOpen()) {
		pauseAction(1);
	}
}

// 0: toggle
// 1: on
// 2: off
void DeeWorld::pauseAction(int i) {
	if (timer == 0) {

		if (pausedGame && i == 1) {
			cocos2d::CCMessageBox("You can't pause that", "Pause error");
		}
		if (!pausedGame && i == 2) {
			cocos2d::CCMessageBox("You can't resume that", "Resume error");
		}

		if (!pausedGame) {
			pauseGame();
			PauseLayer *layer = PauseLayer::create();
			this->addChild(layer, 10, TAG_PAUSE_LAYER);

		} else {
			if (this->getChildByTag(TAG_PAUSE_LAYER) != NULL) {
				this->removeChildByTag(TAG_PAUSE_LAYER, true);
			}
			showCountdown(3);
		}
	}
}

void DeeWorld::pauseGame() {
	if (!pausedGame) {
		CCLog("pause game");
		SoundEffectHelper::pauseAllMusic();
		CCLog("finished pausing music");
		pausedGame = true;
		this->setTouchEnabled(false);
		/*
		 * this executes a exception on android
		 CCArray* childs = this->getChildren();
		 CCObject* child;
		 CCLog("calling pause action on all sprites");
		 CCARRAY_FOREACH(childs, child)
		 {
		 CCSprite *sprite = (CCSprite *)child;
		 sprite->pauseSchedulerAndActions();
		 }
		 CCLog("finished calling pause action on all sprites");
		 */
	}
}

void DeeWorld::resumeGame() {

	pausedGame = false;
	SoundEffectHelper::resumeAllMusic();
	//this->setTouchEnabled(true);

	CCArray* childs = this->getChildren();
	CCObject* child;
	CCARRAY_FOREACH(childs, child) {
		CCSprite *sprite = (CCSprite *) child;
		sprite->resumeSchedulerAndActions();
	}
	SoundEffectHelper::playLevelStartSound();
}

/*
 * this function is called when all sequences have been processed
 */
void DeeWorld::gameEnd() {
	gameEnded = true;
	SoundEffectHelper::stopBackgroundMusic();
	CCLog("LEVEL END");

	std::string strLevel =
			static_cast<std::ostringstream*>(&(std::ostringstream() << level))->str();
	int scoreOld =
			cocos2d::CCUserDefault::sharedUserDefault()->getIntegerForKey(
					("level_" + strLevel).c_str(), 0);

	if (scoreOld < score || scoreOld == 0) {
		CCLog("New Highscore");
		cocos2d::CCUserDefault::sharedUserDefault()->setIntegerForKey(
				("level_" + strLevel).c_str(), score);
	}

	CCScene *pScene = LevelEndScene::create(score, level);
	//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionMoveInB::create(1.0f, pScene));
}

/**
 * get the fourth amino Acid of the sequence in order to add it to the "pipeline"
 */
char DeeWorld::getNextAminoAcid() {
	return popAcidFront();
}

char DeeWorld::getCurrentAminoAcid() {
    char end = '0';
    for(int i=0; i++; i <3){
    	CCLog("Current: %c", doneSequence[i]);
    }
	if (this->doneSequence.size() > 2) {
        return this->doneSequence.at(this->doneSequence.size() - 3);
    }
    return end;
}

/**
 * removes and returns the first acid from the aminoSequence
 */
char DeeWorld::popAcidFront() {
    char front = '0';
	if (this->aminoSequence.size() > 0) {
		front = this->aminoSequence[0];
        this->aminoSequence = this->aminoSequence.erase(0, 1);
        this->doneSequence.push_back(front);
	}
	return front;
}

bool DeeWorld::isAminoAcidRemaining() {
	if (this->aminoSequence.size() > 3) {
		return true;
	}
	return false;
}

void DeeWorld::setSequence(std::string seq) {
	// copy string
	char * c = new char[seq.size() + 1];
	std::copy(seq.begin(), seq.end(), c);
	c[seq.size()] = '\0';
	this->aminoSequence = c;
	CCLog("Transmitted AcidSeq %s", aminoSequence.c_str());
	for (int i = 0; i < 3; i++) {
		UIElements::createNewAminoAcid(this);
	}
}

bool DeeWorld::isGamePaused() {
	bool ret = this->pausedGame || CCDirector::sharedDirector()->isPaused()
			|| gameEnded;
	return ret;
}

bool DeeWorld::isLayerOpen() {
	if (this->getChildByTag(TAG_PAUSE_LAYER) != NULL) {
		return true;
	}
	return false;
}

int DeeWorld::getMinSpeed() {
	// TODO: add some code to speed up during game
	return LevelHelper::getMinSpeed(level);
}

int DeeWorld::getMaxSpeed() {
	// TODO: add some code to speed up during game
	return LevelHelper::getMaxSpeed(level);
}

int DeeWorld::getMinAA() {
	// TODO: add some code to speed up during game
	return LevelHelper::getMinAA(level);
}

int DeeWorld::getMaxAA() {
	// TODO: add some code to speed up during game
	return LevelHelper::getMaxAA(level);
}

int DeeWorld::getOtherAAProp() {
	// TODO: add some code to speed up during game
	return LevelHelper::getOtherAAProp(level);
}

int DeeWorld::getAAAddProb() {
	// TODO: add some code to speed up during game
	return LevelHelper::getAAAddProb(level);
}

