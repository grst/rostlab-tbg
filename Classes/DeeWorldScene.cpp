#include "DeeWorldScene.h"
#include "SplashScreenScene.h"
#include "SimpleAudioEngine.h"
#include "TBGTarget.h"
#include "BoardAcid.h"
#include "helper/PositionHelper.h"
#include "helper/MatrixHelper.h"
#include "helper/AminoAcid.h"
#include "helper/DebugDraw.h"
#include <iostream>
#include <string>

USING_NS_CC;

#define PTM_RATIO 32.0
#define NUMBER_START_TARGETS 4
#define INTRO_TIME_SECONDS 2
#define PLAYER_IMAGE "Player.png"

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

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

DeeWorld::DeeWorld() {

	m_lines = new std::vector<DebugLine>();
	//_targets(NULL);
}

CCScene* DeeWorld::scene() {
	CCScene * scene = NULL;
	do {
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(!scene);

		// 'layer' is an autorelease object
		DeeWorld *layer = DeeWorld::create();
		CC_BREAK_IF(!layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool DeeWorld::init() {

	if (!CCLayer::init())
		return false;

	bool bRet = false;
	do {
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)));

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
				"CloseSelected.png", this,
				menu_selector(DeeWorld::menuCloseCallback));
		CC_BREAK_IF(!pCloseItem);

		// Place the menu item bottom-right conner.
		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		/*	pCloseItem->setPosition(
		 ccp(
		 origin.x + visibleSize.width
		 - pCloseItem->getContentSize().width / 2,
		 origin.y + pCloseItem->getContentSize().height / 2)); */

		pCloseItem->setPosition(
				ccp(

				pCloseItem->getContentSize().width / 2,
						origin.y + pCloseItem->getContentSize().height / 2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(!pMenu);

		// Add the menu to DeeWorld layer as a child layer.
		this->addChild(pMenu, 1);

		/////////////////////////////
		// init Box2D
		b2Vec2 gravity = b2Vec2(0.0f, 0.0f); //no gravity
		_b2dWorld = new b2World(gravity);

		this->schedule(schedule_selector(DeeWorld::tick));

		//debug draw made easy with helper class
		this->addChild(B2DebugDrawLayer::create(_b2dWorld, PTM_RATIO), 9999);

		// Create contact listener
		_contactListener = new CContactListener();
		_b2dWorld->SetContactListener(_contactListener);

		/////////////////////////////
		// 2. add your codes below...

		player = CCSprite::create(PLAYER_IMAGE, CCRectMake(0, 0, 27, 40));
		player->setZOrder(3);

		//create the box for the player (currently with rectangle)
		CreateBox2DBodyForSprite(player, 0, NULL);

		this->addChild(player, 0, 0); // tag 0 for player, 1 target, 2 projectile

		this->schedule(schedule_selector(DeeWorld::gameLogic), 1.0);

		this->setTouchEnabled(true);

		this->loadGame();

		// use updateGame instead of update, otherwise it will conflict with SelectorProtocol::update
		// see http://www.cocos2d-x.org/boards/6/topics/1478
		this->schedule(schedule_selector(DeeWorld::updateGame));

		// disabled temporarily  (annoying!!)
		// CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;

		//testing drawing

		 m_debugDraw = DebugDraw::create();
		scene->addChild(debugDraw);

		m_debugDraw->appendLine(ccp(0, 0), ccp(100, 100));

	} while (0);

// Register the layer to touch dispatcher
// Sebi: Android fails with the dispatcher ..
// -- CCDirector::sharedDirector() -> getTouchDispatcher() -> addStandardDelegate( this, 0 );

	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(
			this, 10);

	return bRet;
}

void DeeWorld::loadGame() {
	this->score = 0;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	player->setPosition(
			ccp(visibleSize.width / 2, player->getContentSize().height));

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

//code
	int i;
	for (i = 0; i < 3; i++) {
		this->createNewAminoAcid(MatrixHelper::getRandomAminoAcid());
	}

//timer
	this->timer = INTRO_TIME_SECONDS;

	this->_timerLabel = CCLabelTTF::create("", "Helvetica",
			visibleSize.height * 2 / 3,
			CCSizeMake(visibleSize.width, visibleSize.height),
			kCCTextAlignmentCenter);
	this->_timerLabel->retain();
	this->_timerLabel->setPosition(
			ccp(visibleSize.width / 2, visibleSize.height / 2));
	this->_timerLabel->setColor(ccc3(0, 0, 0));
	this->addChild(_timerLabel);

	CCActionInterval * tintTo = CCTintTo::create(timer, 20, 20, 255);
	this->_timerLabel->runAction(tintTo);

	this->countdown();

	updateView();

	_targetsAlive = NUMBER_START_TARGETS;
	_targets = new CCArray;
}

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

void DeeWorld::createTargets() {
	int i;
	for (i = 0; i < NUMBER_START_TARGETS; i++) {
		this->addTarget();
	}
}

void DeeWorld::menuCloseCallback(CCObject* pSender) {
// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

// cpp with cocos2d-x
void DeeWorld::addTarget() {

	TBGTarget* tbg = new TBGTarget();

	CCLog("ImagePath: %s", MatrixHelper::getImagePathForAcid(tbg->acidType));

	AminoAcid *target = AminoAcid::create(tbg->acidType);
	//target->setZOrder(10);
	tbg->setSprite(target);
	target->setZOrder(2);

    //Place target in a randomly picked corner.
    int startX, startY;
    int corner = arc4random() % 4;
    //target-dimensions
    CCSize tSize = target->getContentSize();
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    corner = 0;
    switch(corner) {
        case 0:
            //left bottom
            startX = 0 - tSize.width;
            startY = 0 - tSize.height;
            break;
        case 1:
            //left top
            startY = winSize.height + tSize.height;
            startX = 0 - tSize.width;
            break;
        case 2:
            //right top
            startX = winSize.width + tSize.width;
            startY = winSize.height + tSize.height;
            break;
        case 3:
            //right bottom
            startX = winSize.width + tSize.width;
            startY = 0 - tSize.height;
            break;
    }
    
//	// Determine where to spawn the target along the Y axis
//	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
//	float minY = target->getContentSize().height / 2;
//	float maxY = winSize.height - target->getContentSize().height / 2;
//	int rangeY = (int) (maxY - minY);
//	// srand( TimGetTicks() );
//	int actualY = (arc4random() % rangeY) + (int) minY;
//
//	// Create the target slightly off-screen along the right edge,
//	// and along a random position along the Y axis as calculated
//
//	// TODO set the target somewhere
//	int startX = 0;
//	int startY = 0;
//
//	switch (ranEdge) {
//
//	// right
//	case 0:
//		startX = winSize.width + (target->getContentSize().width / 2);
//		startY = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
//		break;
//// lower
//	case 1:
//		startX = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
//		startY = 0 - target->getContentSize().width / 2;
//		break;
//
//// left
//	case 2:
//		startY = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
//		startX = 0 - target->getContentSize().width / 2;
//		break;
//// upper
//	case 3:
//		startY = winSize.width + (target->getContentSize().width / 2);
//		startX = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
//		break;
//	}

    CCLog("Start-Position:x=%i,y=%i", startX, startY);
	target->setPosition(ccp(startX, startY));

	this->addChild(target);

// Add to targets array
	target->setTag(1);
	_targets->addObject(tbg);

	CreateBox2DBodyForSprite(target, 0, NULL);
	this->moveTarget(tbg);

}

void DeeWorld::moveTarget(TBGTarget* tbg) {

	CCSprite *target = tbg->getSprite();
    AminoAcid *aa = dynamic_cast<AminoAcid*>(target);
    if(aa != 0) {
        CCLog("this is an aa object");
        // Determine where to spawn the target along the Y axis
        CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    
        
        CCPoint point = PositionHelper::calculateNewPos(aa, winSize);
        
        CCLog("endX: %d, endY: %d", int(point.x), int(point.y));
        
//        // be secure we have a valid end result
//        if (point.x < 0) {
//            point.x = 0;
//        }
//        
//        if (point.y < target->getContentSize().height) {
//            point.y = target->getContentSize().height;
//        }
//        
//        if (point.x > winSize.width) {
//            point.x = winSize.width;
//        }
//        if (point.y > winSize.height) {
//            point.y = winSize.height;
//        }
        
        // Determine speed of the target
        int minDuration = (int) 2.0;
        int maxDuration = (int) 4.0;
        int rangeDuration = maxDuration - minDuration;
        
        int actualDuration = (rand() % rangeDuration) + minDuration;
        
        // Create the actions
        CCFiniteTimeAction* actionMove = CCMoveTo::create((float) actualDuration,
                                                          point);
        
        CCFiniteTimeAction* actionMoveDone = CCCallFuncND::create(this,
                                                                  callfuncND_selector(DeeWorld::spriteMoveFinished), (void*) tbg);
        CCFiniteTimeAction* box2dDone = CCCallFuncN::create(this,
                                                            callfuncN_selector(DeeWorld::spriteDone));
        
        CCFadeIn *fadeInReadyText = CCFadeIn::create(1.0f);
        CCDelayTime *readyDelay = CCDelayTime::create(0.5f);
        CCFadeOut *fadeOutReadyText = CCFadeOut::create(1.0f);
        
        // Sebi: we have to add some dummy parameters otherwise it fails on Android
        CCSequence *readySequence = CCSequence::create(actionMove, actionMoveDone,
                                                       box2dDone, NULL, NULL);
        aa->runAction(readySequence);
    }
    
	

}

void DeeWorld::spriteMoveFinished(CCNode* sender, void* tbg_void) {

	CCSprite *sprite = (CCSprite *) sender;

	TBGTarget *tbg = (TBGTarget*) tbg_void;

	if (sprite->getTag() == 1)  // target
			{
//////
// box2d stuff!
// Loop through all of the Box2D bodies in our Box2D world...
// We're looking for the Box2D body corresponding to the sprite.
		b2Body *spriteBody = NULL;
		for (b2Body *b = _b2dWorld->GetBodyList(); b; b = b->GetNext()) {
			// See if there's any user data attached to the Box2D body
			// There should be, since we set it in addBoxBodyForSprite
			if (b->GetUserData() != NULL) {

				// We know that the user data is a sprite since we set
				// it that way, so cast it...
				CCSprite *curSprite = (CCSprite *) b->GetUserData();

				// If the sprite for this body is the same as our current
				// sprite, we've found the Box2D body we're looking for!
				if (sprite == curSprite) {
					spriteBody = b;
					break;
				}
			}
		}

// If we found the body, we want to destroy it since the cat is offscreen now.
		if (spriteBody != NULL) {
			//	_b2dWorld->DestroyBody(spriteBody);
		}

		this->moveTarget(tbg);
	}
}


// cpp with cocos2d-x
void DeeWorld::ccTouchesBegan(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {
	validTouch = true;
	int touchNumber = 0;
	tempHeight = CCDirector::sharedDirector()->getWinSize().height;

	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*) *it;
	CCPoint pt = touch->getLocationInView();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	float height = size.height;
	float y = height - pt.y;
	int tolerance = 10;

	if (std::abs(pt.x - player->getPositionX()) - tolerance
			> player->getContentSize().width) {
		//non valid
		cocos2d::CCLog(
				"ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
				touch->getID(), (int) pt.x, (int) pt.y,
				(int) player->getPositionX(), (int) player - getPositionY());
		validTouch = false;
	}
	if (std::abs(y - player->getPositionY()) - tolerance
			> player->getContentSize().height) {
		//non valid
		cocos2d::CCLog(
				"ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
				touch->getID(), (int) pt.x, (int) pt.y,
				(int) player->getPositionX(), (int) player - getPositionY());
		validTouch = false;
	}
}

// cpp with cocos2d-x
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
	player->setPosition(ccp(pt.x, tempHeight - pt.y));
	player->draw();
	player->update(0.01);

	return;
	/*
	 CCSetIterator it = touches->begin();
	 CCTouch* touch;

	 CCPoint pt;
	 for (int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++) {
	 touch = (CCTouch*) (*it);
	 pt = touch->getLocationInView();

	 it++;

	 //world y
	 CCSize size = CCDirector::sharedDirector()->getWinSize();
	 float height = size.height;

	 float y = tempHeight - pt.y;

	 //CCLog("x: %f, y: %f", pt.x, pt.y);

	 cocos2d::CCLog("size: %d", touches->count());

	 //set player
	 player->setPosition(ccp(pt.x, y));
	 }
	 */
}

// cpp with cocos2d-x
void DeeWorld::ccTouchesEnded(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {
	return;
//disabled temporarly - working!
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
			"pew-pew-lei.wav");
}

void DeeWorld::updateGame(float dt) {

	return;
	CCArray *targetsToDelete = new CCArray;
	CCObject* jt = NULL;

// for (jt = _targets->begin(); jt != _targets->end(); jt++)
	CCARRAY_FOREACH(_targets, jt) {
		CCSprite *target = dynamic_cast<TBGTarget*>(jt)->getSprite();
		CCRect targetRect = CCRectMake(
				target->getPosition().x - (target->getContentSize().width / 2),
				target->getPosition().y - (target->getContentSize().height / 2),
				target->getContentSize().width,
				target->getContentSize().height);

// if (CCRect::CCRectIntersectsRect(projectileRect, targetRect))
//if (projectileRect.intersectsRect(targetRect)) {
//		targetsToDelete->addObject(target);
//	}
	}

	CCARRAY_FOREACH(targetsToDelete, jt) {
		CCSprite *target = dynamic_cast<CCSprite*>(jt);
		_targets->removeObject(target);
		this->removeChild(target, true);
	}

	targetsToDelete->release();

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// This function send the vertice data to Box2D. Also, if you pass iNumVerts==0 and verts==NULL it simply creates a
// box round your sprite

void DeeWorld::CreateBox2DBodyForSprite(cocos2d::CCSprite *sprite,
		int iNumVerts, b2Vec2 verts[]) {
	if (_b2dWorld == NULL) {
		return;
	}

	CCPoint pos = sprite->getPosition();
	CCSize size = sprite->getContentSize();

	b2BodyDef spriteBodyDef;

	spriteBodyDef.type = b2_dynamicBody;
	spriteBodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);

	spriteBodyDef.userData = sprite;
	b2Body *spriteBody = _b2dWorld->CreateBody(&spriteBodyDef);

	b2PolygonShape spriteShape;

	if (iNumVerts != 0) {
		spriteShape.Set(verts, iNumVerts);
		b2FixtureDef spriteShapeDef;
		spriteShapeDef.shape = &spriteShape;
		spriteShapeDef.density = 10.0;
		spriteShapeDef.isSensor = true;

		spriteBody->CreateFixture(&spriteShapeDef);
	} else {
// No Vertice supplied so just make a box round the sprite
		b2BodyDef spriteBodyDef;
		spriteBodyDef.type = b2_dynamicBody;
		spriteBodyDef.position.Set(pos.x / PTM_RATIO, pos.y / PTM_RATIO);
		spriteBodyDef.userData = sprite;
		b2Body *spriteBody = _b2dWorld->CreateBody(&spriteBodyDef);

		b2PolygonShape spriteShape;
		spriteShape.SetAsBox(size.width / PTM_RATIO / 2,
				size.height / PTM_RATIO / 2);
		b2FixtureDef spriteShapeDef;
		spriteShapeDef.shape = &spriteShape;
		spriteShapeDef.density = 10.0;
		spriteShapeDef.isSensor = true; // isSensor true when you want to know when objects will collide without triggering a box2d collision response
		spriteBody->CreateFixture(&spriteShapeDef);
	}

}

void DeeWorld::spriteDone(CCNode* sender) {

// This selector is called from CCCallFuncN, and it passes the object the action is
// run on as a parameter.  We know it's a sprite, so cast it as that!
	CCSprite *sprite = (CCSprite *) sender;

// Loop through all of the Box2D bodies in our Box2D world...
// We're looking for the Box2D body corresponding to the sprite.
	b2Body *spriteBody = NULL;
	for (b2Body *b = _b2dWorld->GetBodyList(); b; b = b->GetNext()) {
// See if there's any user data attached to the Box2D body
// There should be, since we set it in addBoxBodyForSprite
		if (b->GetUserData() != NULL) {

			// We know that the user data is a sprite since we set
			// it that way, so cast it...
			CCSprite *curSprite = (CCSprite *) b->GetUserData();

			// If the sprite for this body is the same as our current
			// sprite, we've found the Box2D body we're looking for!
			if (sprite == curSprite) {
				spriteBody = b;
				break;
			}
		}
	}

// If we found the body, we want to destroy it since the cat is offscreen now.
	if (spriteBody != NULL) {
//	_b2dWorld->DestroyBody(spriteBody);
	}

//    // And of course we need to remove the Cocos2D sprite too.
//    m_spriteSheet->removeChild(sprite, true);
}

void DeeWorld::updateView() {

//update score
	string temp =
			static_cast<ostringstream*>(&(ostringstream() << this->score))->str();
	this->_scoreLabel->setString(temp.c_str());
	this->_scoreLabel->draw();
	this->_scoreLabel->update(0.5);

//update code
//this->_codeLabel->setString(code.c_str());
}

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

			// sprite might already have been removed -> null pointer exception
			if (sprite != NULL) {

				// Convert the Cocos2D position/rotation of the sprite to the Box2D position/rotation
				CCPoint Pos = sprite->getPosition();
				b2Vec2 b2Position = b2Vec2(Pos.x / PTM_RATIO,
						Pos.y / PTM_RATIO);
				float32 b2Angle = -1
						* CC_DEGREES_TO_RADIANS(sprite->getRotation());

				// Update the Box2D position/rotation to match the Cocos2D position/rotation
				b->SetTransform(b2Position, b2Angle);
			}
		}
	}

// Loop through all of the box2d bodies that are currently colliding, that we have
// gathered with our custom contact listener...
	std::vector<b2Body *> toDestroy; //list of targets that the player collided with
	std::vector<ContactData>::iterator pos;
	for (pos = _contactListener->_contacts.begin();
			pos != _contactListener->_contacts.end(); ++pos) {
		ContactData contact = *pos;
// Get the box2d bodies for each object
		b2Body *bodyA = contact.fixtureA->GetBody();
		b2Body *bodyB = contact.fixtureB->GetBody();
		if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
			CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
			CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
			int iTagA = spriteA->getTag();
			int iTagB = spriteB->getTag();

			TBGTarget * acid = NULL;

			// Is sprite A a player and sprite B a target?  If so, push the target on a list to be destroyed...
			if (iTagA == 0 && iTagB == 1) {
				toDestroy.push_back(bodyB);
				//this->manageCollision(acid);
			}
			// Is sprite A a target and sprite B a player?  If so, push the target on a list to be destroyed...
			else if (iTagA == 1 && iTagB == 0) {
				toDestroy.push_back(bodyA);
				CCLog("Collision");
				//this->manageCollision(acid);
			}

		}
	}

// Loop through all of the box2d bodies we wnat to destroy...
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

			_targetsAlive = _targetsAlive - 1;
			CCLOG("remaining % d", _targetsAlive);

			this->score = score + 1;

			if (_code.size() > 0) {
				BoardAcid * acid = this->_code.front();

				cocos2d::CCLabelTTF* label = acid->_label;
				//label->setString("y");

				// Create the actions
				CCSize visibleSize =
						CCDirector::sharedDirector()->getVisibleSize();

				CCFiniteTimeAction* actionMove = CCMoveTo::create((float) 0.8,
						ccp(visibleSize.height, visibleSize.width));

				// Sebi: we have to add some dummy parameters otherwise it fails on Android
				CCSequence *readySequence = CCSequence::create(actionMove, NULL,
				NULL);
				label->runAction(readySequence);

				//delete acid;
				//break;
				_code.pop();

				createNewAminoAcid(MatrixHelper::getRandomAminoAcid());
			}

			//this->code.pMatrixHelper::getRandomAminoAcid();
			this->updateView();
			/*	if (_targetsAlive <= 0) {

			 SplashScreenScene *splashScene = SplashScreenScene::create();
			 splashScene->getLayer()->getLabel()->setString("You Lose :[");

			 //transition to next scene for one sec
			 CCDirector::sharedDirector()->replaceScene(
			 CCTransitionFade::create(1.0f, splashScene));
			 }
			 */

		}

// Destroy the Box2D body as well
		_b2dWorld->DestroyBody(body);
	}

//    // If we've destroyed anything, play an amusing and malicious sound effect!  ;]
//    if (toDestroy.size() > 0)
//    {
//        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect( "cat_ouch.wav", false );
//    }
}

void DeeWorld::registerWithTouchDispatcher() {
// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(
			this, 0);

}

void DeeWorld::gameLogic(float dt) {
// do some crazy stuff here
}

void DeeWorld::createNewAminoAcid(char c) {

	BoardAcid * acid = new BoardAcid();
	acid->setAcid(c);

	char tt[] = "t";
	tt[0] = c;

	std::string str = string(tt);

// Create the actions
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	CCLabelTTF * label = CCLabelTTF::create(str.c_str(), "Helvetica",
			visibleSize.height * 1 / 10,
			CCSizeMake(20, visibleSize.height * 1 / 7), kCCTextAlignmentRight);
	acid->_label = label;

	acid->_label->setPosition(
			ccp(visibleSize.width - 15, visibleSize.height * 1 / 6));
	acid->_label->setColor(ccc3(20, 20, 255));
	this->addChild(acid->_label);

// move all elements a bit
	std::queue<BoardAcid*> tmpQueue;
	while (!_code.empty()) {
		BoardAcid* el = _code.front();
		tmpQueue.push(el);

		CCFiniteTimeAction* actionMove = CCMoveTo::create((float) 0.8,
				ccp(el->_label->getPositionX() - 20,
						el->_label->getPositionY()));

		// Sebi: we have to add some dummy parameters otherwise it fails on Android
		CCSequence *readySequence = CCSequence::create(actionMove, NULL,
		NULL);
		el->_label->runAction(readySequence);

		_code.pop();
	}

	_code = tmpQueue;

	_code.push(acid);

}

void DeeWorld::manageCollision(TBGTarget* tbg) {
//TODO
	return;
}
