#include "DeeWorldScene.h"
#include "SplashScreenScene.h"
#include "SimpleAudioEngine.h"
#include "../helper/PositionHelper.h"
#include "../helper/MatrixHelper.h"
#include "../helper/UIElements.h"
#include "MainScreenScene.h"
#include <iostream>
#include <string>

USING_NS_CC;

#define PTM_RATIO 32.0
#define NUMBER_START_TARGETS 8
#define INTRO_TIME_SECONDS 1
#define TOLERANCE_PLAYER -30
#define MS_TIME_PLAYER_BLOCKED 3000
#define PLAYER_IMAGE "Player.png"
#define kFileStreak "streak.png"

using namespace cocos2d;

DeeWorld::~DeeWorld() {
	if (_targets) {
		_targets->release();
		_targets = NULL;
	}

	if (_scoreLabel) {
		_scoreLabel->release();
		_scoreLabel = NULL;
	}

	CC_SAFE_RELEASE(_scoreNumber);

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

DeeWorld::DeeWorld() {

	//_targets(NULL);
}

/*
 * deprecated
 */
CCScene* DeeWorld::scene() {
	return DeeWorld::scene("ACCDDCCCAAA");
}

/*
 * pass the sequence on construction
 */
CCScene* DeeWorld::scene(std::string sequence) {
	CCScene * scene = NULL;
	do {
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		// 'layer' is an autorelease object
		DeeWorld *layer = DeeWorld::create();
		CC_BREAK_IF(!layer);

		layer->setSequence(sequence);

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

	cocos2d::CCLog("Hello World. App ist starting now");

	initBackground();
	makeMenu();
	initBox2D();

	////TAGS
	// 0 = player
	// 1 = target
	// 3 = wall
	initWorld();
	initPlayer();

	// load a user selected matrix
	std::string matrix =
			cocos2d::CCUserDefault::sharedUserDefault()->getStringForKey(
					"matrix", "BLOSUM62.txt");

	if (matrix.size() == 0) {
		matrix = "BLOSUM62.txt";
	}

	cocos2d::CCLog("Matrix %s loaded", matrix.c_str());

	//load the scoring matrix
	MatrixHelper::loadMatrix(matrix);

	// enable Android back button
	this->setKeypadEnabled(true);

	this->setTouchEnabled(true);

	initInfoUI();

	updateInfoUI();

	_targets = new CCArray;

	// disabled temporarily  (annoying!!)
	// CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(
			this, 10);

	CCLog("init successful");

	startTime = HelperFunctions::nowInSeconds();

	return true;
}

void DeeWorld::initBackground() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite* pSpriteBackground = CCSprite::create("DeeWorldBackground.jpg");

	// position the sprite on the center of the screen
	pSpriteBackground->setPosition(ccp(winSize.width / 2, winSize.height / 2));

	// add the sprite as a child to this layer
	this->addChild(pSpriteBackground, 0);
}

void DeeWorld::makeMenu() {

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCMenuItemImage *pSettings = CCMenuItemImage::create("settings.png",
			"settings.png", this, menu_selector(DeeWorld::pauseAction));

	// Place the menu item top-left corner.
	pSettings->setPosition(
			ccp(pSettings->getContentSize().width / 2,
					origin.y + winSize.height
							- pSettings->getContentSize().height * 2));

	//	pCloseItem->setTag(10);

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

void DeeWorld::menuCloseCallback(CCObject* pSender) {
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void DeeWorld::initBox2D() {
	b2Vec2 gravity = b2Vec2(0.0f, -0.0f); //no gravity
	_b2dWorld = new b2World(gravity);

	this->schedule(schedule_selector(DeeWorld::tick));

	//debug draw made easy with helper class
	this->addChild(B2DebugDrawLayer::create(_b2dWorld, PTM_RATIO), 9999);

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

	b2Body* groundBody = _b2dWorld->CreateBody(&groundBodyDef);
	b2PolygonShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;

    //wall definitions
	groundEdge.SetAsBox(visibleSize.width / PTM_RATIO, 20 / PTM_RATIO, b2Vec2(visibleSize.width / PTM_RATIO / 2, 0), 0);
	bottom = groundBody->CreateFixture(&boxShapeDef);
	groundEdge.SetAsBox(20/PTM_RATIO, visibleSize.height/PTM_RATIO, b2Vec2(0, visibleSize.height / PTM_RATIO /2), 0);
	left = groundBody->CreateFixture(&boxShapeDef);
	groundEdge.SetAsBox(visibleSize.width / PTM_RATIO, 20/PTM_RATIO, b2Vec2(visibleSize.width / PTM_RATIO / 2, visibleSize.height / PTM_RATIO), 0);
	top = groundBody->CreateFixture(&boxShapeDef);
	groundEdge.SetAsBox(20/PTM_RATIO, visibleSize.height/PTM_RATIO,
                        b2Vec2(visibleSize.width /PTM_RATIO, visibleSize.height / PTM_RATIO/2), 0);
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

	//int * value;
	//*value = 2;
	player->SetUserData(sPlayer);
	player->SetType(b2_kinematicBody);

	//z-order 3, tag=0
	this->addChild(sPlayer, 3, 0);
}

/**
 * initialize Information UI-Elements like score, timer, and amino-acid-code
 */
void DeeWorld::initInfoUI() {

	//reset AminoHit
	lastAminoHitTime = 0.0f;

	this->score = 0;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCLog("setting score");
	//score
	score = 0;
	// int -> str
	this->_scoreLabel = CCLabelTTF::create("0", "Helvetica",
			visibleSize.height * 1 / 8,
			CCSizeMake(60, visibleSize.height * 1 / 7), kCCTextAlignmentRight);
	this->_scoreLabel->retain();
	this->_scoreLabel->setPosition(
			ccp(visibleSize.width - 30, visibleSize.height + 10));
	this->_scoreLabel->setColor(ccc3(20, 20, 255));
	this->addChild(_scoreLabel);

	CCLog("setting timer");
	//timer
	this->timer = INTRO_TIME_SECONDS;

	this->_timerLabel = CCLabelTTF::create("", "Helvetica",
			visibleSize.height * 2 / 3, visibleSize, kCCTextAlignmentCenter);
	this->_timerLabel->retain();
	this->_timerLabel->setPosition(
			ccp(visibleSize.width / 2, visibleSize.height / 2));
	this->_timerLabel->setColor(ccc3(0, 0, 0));
	this->addChild(_timerLabel);

	CCActionInterval * tintTo = CCTintTo::create(timer, 20, 20, 255);
	this->_timerLabel->runAction(tintTo);

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
		//convert int to string
		string timeStr = static_cast<ostringstream*>(&(ostringstream()
				<< this->timer))->str();
		this->_timerLabel->setString(timeStr.c_str());
		this->timer = this->timer - 1;
		this->runAction(
				CCSequence::create(CCDelayTime::create(1),
						CCCallFunc::create(this,
								callfunc_selector(DeeWorld::countdown)),

						NULL));
	} else {
		CCLog("create targets");
		this->removeChild(this->_timerLabel);
		if (this->_timerLabel) {
			this->_timerLabel->release();
			this->_timerLabel = NULL;
		}
		this->createTargets();
	}
}

/**
 * adds the targets to the game. 
 * #targets specified in NUMBER_START_TARGETS constant
 */
void DeeWorld::createTargets() {
	int i;
	for (i = 0; i < NUMBER_START_TARGETS; i++) {
		this->addTarget();
	}
}

/**
 * adds a target to the game. 
 * creates the box2d body and the corresponding sprite
 * TODO: let the target drop into the screen from a random corner and start movement
 */
void DeeWorld::addTarget() {
	AminoAcid *sTarget = AminoAcid::create();
    HelperFunctions::resizseSprite(sTarget, 64, 0);
	//Place target in a randomly picked corner.
	int startX, startY;
	int corner = arc4random() % 4;
	//target-dimensions
	CCSize tSize = sTarget->getContentSize();
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	b2Vec2 impulse;
	switch (corner) {
	case 0:
		//left bottom
		startX = 0 - tSize.width;
		startY = 0 - tSize.height;
		impulse = b2Vec2(300, 300);
		break;
	case 1:
		//left top
		startY = winSize.height + tSize.height;
		startX = 0 - tSize.width;
		impulse = b2Vec2(300, -300);
		break;
	case 2:
		//right top
		startX = winSize.width + tSize.width;
		startY = winSize.height + tSize.height;
		impulse = b2Vec2(-300, -300);
		break;
	case 3:
		//right bottom
		startX = winSize.width + tSize.width;
		startY = 0 - tSize.height;
		impulse = b2Vec2(-300, 300);
		break;
	}

	// TODO temp fix to test scoring event
	startX = 200;
	startY = -200;
	CCLog("Start-Position:x=%i,y=%i", startX, startY);
	sTarget->setPosition(ccp(startX, startY));

	this->addChild(sTarget);

    //Temp: static polygon
    //TODO: put in MatrixHelperClass or read from file
    //row 1, col 1
    float scaleRatio = 1/PTM_RATIO/2;
    int num = 8;
    b2Vec2 verts[] = {
        b2Vec2(6.0f * scaleRatio, 42.0f * scaleRatio),
        b2Vec2(-24.0f * scaleRatio, 32.0f * scaleRatio),
        b2Vec2(-38.5f * scaleRatio, -15.2f * scaleRatio),
        b2Vec2(-29.5f * scaleRatio, -28.0f * scaleRatio),
        b2Vec2(4.0f * scaleRatio, -39.0f * scaleRatio),
        b2Vec2(39.7f * scaleRatio, -9.0f * scaleRatio),
        b2Vec2(38.5f * scaleRatio, 23.2f * scaleRatio),
        b2Vec2(19.7f * scaleRatio, 31.5f * scaleRatio)
    };
    
	//Move Target
	b2Body* target = CreateBox2DBodyForSprite(sTarget, num, verts);
	target->ApplyLinearImpulse(impulse, target->GetPosition());

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
		cocos2d::CCLog(
				"ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
				touch->getID(), (int) pt.x, (int) pt.y,
				(int) sPlayer->getPositionX(), (int) sPlayer - getPositionY());
		validTouch = false;
		return;
	}
	if (std::abs(y - sPlayer->getPositionY()) - TOLERANCE_PLAYER
			> sPlayer->getContentSize().height) {
		//non valid
		cocos2d::CCLog(
				"ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
				touch->getID(), (int) pt.x, (int) pt.y,
				(int) sPlayer->getPositionX(), (int) sPlayer - getPositionY());
		validTouch = false;
		return;
	}

	for (CCSetIterator it = touches->begin(); it != touches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		CCBlade *blade = CCBlade::create(kFileStreak, 4, 50);
		_map[touch] = blade;
		addChild(blade);

		blade->setColor(ccc3(255, 0, 0));
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
 * TODO: move the fancy 'movement track' to own method, better own class
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
	float y = tempHeight - pt.y;
	player->SetTransform(b2Vec2(pt.x / PTM_RATIO, y / PTM_RATIO), 0);

	/*	b2Vec2 playerPoint = player->GetPosition();

	 CCPoint oldPlayerPoint = ccp(pt.x, y);
	 //TODO real player position
	 CCPoint playerPointCC = ccp(pt.x-5, y-5);
	 // calling fancy movement track
	 UIElements::fancyMovement(oldPlayerPoint, playerPointCC, this); */

	for (CCSetIterator it = touches->begin(); it != touches->end(); it++) {
		CCTouch *touch = (CCTouch *) *it;
		if (_map.find(touch) == _map.end())
			continue;

		CCBlade *blade = _map[touch];
		CCPoint point = convertTouchToNodeSpace(touch);

		cocos2d::CCLog("TouchTrail x:%d, y:%d", int(point.x), int(point.y));

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

	return;
	//disabled temporarly - working!
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
			"pew-pew-lei.wav");
}

/**
 * this function is called when the back button is pressed. only useful for android?
 */
void DeeWorld::keyBackClicked(void) {
	pauseAction(NULL);
}

/**
 * contact listener for one-way walls
 */
void DeeWorld::BeginContact(b2Contact *contact) {
	CCLog("BeginContact");
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	this->collisionHandler2(fixtureA, fixtureB);
    //return;

	//check if one of the fixtures is the platform

	b2Fixture* platformFixture = NULL;
	b2Fixture* otherFixture = NULL;
	if (fixtureA == left || fixtureA == top || fixtureA == right
			|| fixtureA == bottom) {
		platformFixture = fixtureA;
		otherFixture = fixtureB;
	} else if (fixtureB == left || fixtureB == top || fixtureB == right
			|| fixtureB == bottom) {
		platformFixture = fixtureB;
		otherFixture = fixtureA;
	}

	if (!platformFixture)
		return;

	b2Body* platformBody = platformFixture->GetBody();
	b2Body* otherBody = otherFixture->GetBody();

	int numPoints = contact->GetManifold()->pointCount;
	b2WorldManifold worldManifold;
	contact->GetWorldManifold(&worldManifold);

	//check if contact points are moving in the wrong direction
	for (int i = 0; i < numPoints; i++) {
		b2Vec2 pointVelPlatform =
        platformBody->GetLinearVelocityFromWorldPoint( worldManifold.points[i] );
        b2Vec2 pointVelOther =
        otherBody->GetLinearVelocityFromWorldPoint( worldManifold.points[i] );
        b2Vec2 relativeVel = platformBody->GetLocalVector( pointVelOther - pointVelPlatform );
        
        //borderline case, moving only slightly out of platform
        b2Vec2 relativePoint = platformBody->GetLocalPoint( worldManifold.points[i] );
        float platformFaceY = 0.5f;//front of platform, from fixture definition :(
        
        float velocity = 0.0f;
        float point = 0.0f;
        //adapt velocity depending on with which wall the item collides
        if (platformFixture == left) {
			velocity = relativeVel.x;
            point = relativePoint.x;
		} else if (platformFixture == right) {
			velocity = -relativeVel.x;
            point = -relativePoint.x;
		} else if (platformFixture == top) {
			velocity = -relativeVel.y;
            point = -relativePoint.y;
		} else if (platformFixture == bottom) {
			velocity = relativeVel.y;
            point = relativePoint.y;
		}
        
		if ( velocity < -1 ) { //if moving down faster than 1 m/s, handle as before
            return;//point is moving into platform, leave contact solid and exit
        } else if ( velocity < 1 ) { //if moving slower than 1 m/s
            if ( point > 20/PTM_RATIO - 0.05 )
                return;//contact point is less than 5cm inside front face of platfrom
        } else {
            ;//moving up faster than 1 m/s
        }
    }

	//no points are moving downward, contact should not be solid
	contact->SetEnabled(false);
}

/**
 * TODO: implement game logic entirely, clean up
 * TODO: move everything that has something to do with the InfoUI to external Method, better external class.
 * TODO: integeate into BeginContact Handler or move parts to seperate method
 */
void DeeWorld::collisionHandler2(b2Fixture* fixtureA, b2Fixture* fixtureB) {
	// Loop through all of the box2d bodies that are currently colliding, that we have
	// gathered with our custom contact listener...
	std::vector<b2Body *> toDestroy; //list of targets that the player collided with

	// Get the box2d bodies for each object
	b2Body *bodyA = fixtureA->GetBody();
	b2Body *bodyB = fixtureB->GetBody();
	if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
		CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
		CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
		int iTagA = spriteA->getTag();
		int iTagB = spriteB->getTag();

		// Is sprite A a player and sprite B a target?  If so, push the target on a list to be destroyed...
		if (iTagA == 0 && iTagB == 1) {
			toDestroy.push_back(bodyB);
			CCLog("Collision: Player on Target");
			//this->manageCollision(acid);
		}
		// Is sprite A a target and sprite B a player?  If so, push the target on a list to be destroyed...
		else if (iTagA == 1 && iTagB == 0) {
			toDestroy.push_back(bodyA);
			CCLog("Collision: Target on Player");
			//this->manageCollision(acid);
		}
	}

	// Loop through all of the box2d bodies we want to destroy...
	std::vector<b2Body *>::iterator pos2;

	for (pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2) {
		b2Body *body = *pos2;

		// See if there's any user data attached to the Box2D body
		// There should be, since we set it in addBoxBodyForSprite
		if (body->GetUserData() != NULL) {

			// We know that the user data is a sprite since we set
			// it that way, so cast it...
			CCSprite *sprite = (CCSprite *) body->GetUserData();

			// Remove the sprite from the scene
			//[_spriteSheet removeChild:sprite cleanup:YES];

			//delete this sprite from the array after the sequence is finished
			//_b2dWorld->DestroyBody(body);
			CCLog("pos.x: %d", int(sprite->getPosition().x));
			//this->removeChild(sprite);

			//ATTETION we don't delete the sprite out of the array, this might cause memory leaks

			// block our player for 1.5s
			if (_code.size()
					> 0&& abs(HelperFunctions::nowInMilliSeconds() - lastAminoHitTime) > MS_TIME_PLAYER_BLOCKED) {
				lastAminoHitTime = HelperFunctions::nowInMilliSeconds();

				BoardAcid * acid = this->_code.front();

				char wantedAcid = acid->acid;
				char hitAcid = 'A';

				// trying to cast to AminoAcid
				AminoAcid * pScoredAcid = (AminoAcid *) sprite;
				if (pScoredAcid != NULL) {
					if (pScoredAcid->getType() != NULL) {
						hitAcid = pScoredAcid->getType();
					}
				}

				// show score
				int scoring = MatrixHelper::getScoreForAminoAcid(wantedAcid,
						hitAcid);
				// add to the current score
				this->score = scoring + this->score;

				std::string str = static_cast<ostringstream*>(&(ostringstream()
						<< scoring))->str();
				CCSize visibleSize =
						CCDirector::sharedDirector()->getVisibleSize();
				this->_scoreNumber = CCLabelTTF::create(str.c_str(),
						"Helvetica", 200, visibleSize, kCCTextAlignmentCenter);
				this->_scoreNumber->retain();
				this->_scoreNumber->setPosition(
						ccp(visibleSize.width / 2, visibleSize.height / 2));

				// generates a nice color according to the score
				this->_scoreNumber->setColor(
						UIElements::getColorForScore(scoring));

				this->addChild(_scoreNumber);

				CCActionInterval * tintToNumber;
				if (scoring > 0) {
					tintToNumber = CCTintTo::create(1.0, 0, 255, 0);
				} else if (scoring < 0) {
					tintToNumber = CCTintTo::create(1.0, 255, 0, 0);
				} else {
					tintToNumber = CCTintTo::create(1.5, 0, 0, 0);
				}

				this->_scoreNumber->runAction(tintToNumber);
				CCActionInterval * scaleTo = CCScaleTo::create(1.0, 0.01);
				this->_scoreNumber->runAction(scaleTo);

				cocos2d::CCLabelTTF* label = acid->_label;

				CCFiniteTimeAction* actionMove = CCMoveTo::create((float) 0.8,
						ccp(visibleSize.height, visibleSize.width));

				// Sebi: we have to add some dummy parameters otherwise it fails on Android
				CCSequence *readySequence = CCSequence::create(actionMove, NULL,
				NULL);
				label->runAction(readySequence);

				// TODO delete acid;
				_code.pop();

				UIElements::createNewAminoAcid(this);

			}

			//this->code.pMatrixHelper::getRandomAminoAcid();
			this->updateInfoUI();

		}

		// TODO destroy the body?
		// SEBI: not possible on Android -> error
		// Destroy the Box2D body as well
		//	_b2dWorld->DestroyBody(body);
	}

	//    // If we've destroyed anything, play an amusing and malicious sound effect!  ;]
	//    if (toDestroy.size() > 0)
	//    {
	//        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "cat_ouch.wav", false );
	//    }
}

void DeeWorld::EndContact(b2Contact* contact) {
	//reset the default state of the contact in case it comes back for more
	contact->SetEnabled(true);
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
	string temp =
			static_cast<ostringstream*>(&(ostringstream() << this->score))->str();
	this->_scoreLabel->setString(temp.c_str());
	this->_scoreLabel->draw();
	this->_scoreLabel->update(0.5);
}

/**
 * updates CCSprites to go along with the box2d bodies
 * method is invoked by scheduler every #n ms
 */
void DeeWorld::tick(float delta) {
	_b2dWorld->Step(delta, 10, 10);
	// Loop through all of the Box2D bodies in our Box2D world..
	for (b2Body *b = _b2dWorld->GetBodyList(); b; b = b->GetNext()) {
		// See if there's any user data attached to the Box2D body
		// There should be, since we set it in addBoxBodyForSprite

		if (b->GetUserData() != NULL) {
			// We know that the user data is a sprite since we set
			// it that way, so cast it...
			CCSprite *sprite = (CCSprite *) b->GetUserData();

			//update sprites to match the simulation
			sprite->setPosition(
					ccp(b->GetPosition().x * PTM_RATIO,
							b->GetPosition().y * PTM_RATIO));

			// TODO: do we really want to rotate the player?
			sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}

void DeeWorld::manageCollision(AminoAcid* aa) {
	//TODO

	CCLog("Collision with %c", aa->getType());

	return;
}

///**
// * Determines the edge-code by comparing the wall with which an object collided
// * to all the "walls".
// *
// * @param wall the CCSprite wall with which the object collided
// * @return 0=bottom, 1=left, 2=top, 3=right, -1 if wall is not a wall-sprite
// */
//int DeeWorld::getEdge(CCSprite* wall) {
//	if (wall == bottom) {
//		return 0;
//	} else if (wall == left) {
//		return 1;
//	} else if (wall == top) {
//		return 2;
//	} else if (wall == right) {
//		return 3;
//	} else {
//		return -1;
//	}
//}

// TODO blend layer with pause and pause the whole game
void DeeWorld::pauseAction(CCObject* pSender) {
	return;
	// TODO this creates an error on android
	CCScene *pScene = MainScreenScene::create();
	//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFade::create(1.0f, pScene));
}

/*
 * this function is called when all sequences have been processed
 */
void DeeWorld::gameEnd() {
	// TODO
	CCLog("GAME OVER");
}

char DeeWorld::getNextAminoAcid() {

	char end = '0';
	if (this->aminoSequence.size() > 0) {
		end = this->aminoSequence[0];
		this->aminoSequence = this->aminoSequence.erase(0, 1);
	}
	CCLog("Next Acid is %c", end);
	return end;
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
