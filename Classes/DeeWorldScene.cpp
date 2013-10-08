#include "DeeWorldScene.h"
#include "SplashScreenScene.h"
#include "SimpleAudioEngine.h"
#include "TBGTarget.h"
#include "BoardAcid.h"
#include "helper/PositionHelper.h"
#include "helper/MatrixHelper.h"
#include <iostream>
#include <string>

USING_NS_CC;

#define PTM_RATIO 32.0
#define NUMBER_START_TARGETS 1
#define INTRO_TIME_SECONDS 1
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
    
	CC_SAFE_RELEASE(_scoreNumber);
    
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

DeeWorld::DeeWorld() {
    
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
		//super init
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)));
        
        makeMenu();
        initBox2D();
        
		////TAGS
		// 0 = player
		// 1 = target
		// 2 = projectile (not in use any more)
		// 3 = wall
        initWorld();
		
        
		//player
		player = CCSprite::create(PLAYER_IMAGE, CCRectMake(0, 0, 27, 40));
		player->setZOrder(3);
        
		//create the box for the player (currently with rectangle)
		CreateBox2DBodyForSprite(player, 0, NULL);
        
		//z-order 3, tag=0
		this->addChild(player, 3, 0);
        
		this->schedule(schedule_selector(DeeWorld::gameLogic), 1.0);
        
		//load the scoring matrix
		MatrixHelper::loadMatrix("BLOSUM62.txt");
        
		this->setTouchEnabled(true);
        
		CCLog("before loading game");
		this->loadGame();
        
		CCLog("after loading game");
        
		// use updateGame instead of update, otherwise it will conflict with SelectorProtocol::update
		// see http://www.cocos2d-x.org/boards/6/topics/1478
		this->schedule(schedule_selector(DeeWorld::updateGame));
        
		// disabled temporarily  (annoying!!)
		// CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
        
		bRet = true;
        
	} while (0);
    
    // Register the layer to touch dispatcher
    // Sebi: Android fails with the dispatcher ..
    // -- CCDirector::sharedDirector() -> getTouchDispatcher() -> addStandardDelegate( this, 0 );
    
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(
                                                                            this, 10);
    
	CCLog("init successful");
    
	return bRet;
}

void DeeWorld::makeMenu() {
    //close-item
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
                                                          "CloseSelected.png", this,
                                                          menu_selector(DeeWorld::menuCloseCallback));
    if (!pCloseItem) {
        return;
    }
    // Place the menu item bottom-left conner.
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    pCloseItem->setPosition(
                            ccp(
                                
                                pCloseItem->getContentSize().width / 2,
                                origin.y + pCloseItem->getContentSize().height / 2));
    
    // Create a menu with the "close" menu item, it's an auto release object.
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    if(!pMenu) {
        return;
    }
    
    // Add the menu to DeeWorld layer as a child layer.
    this->addChild(pMenu, 1);

}
void DeeWorld::menuCloseCallback(CCObject* pSender) {
    // "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void DeeWorld::initBox2D() {
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f); //no gravity
    _b2dWorld = new b2World(gravity);
    
    this->schedule(schedule_selector(DeeWorld::tick));
    
    //debug draw made easy with helper class
    this->addChild(B2DebugDrawLayer::create(_b2dWorld, PTM_RATIO), 9999);
    
    // Create contact listener
    _contactListener = new CContactListener();
    _b2dWorld->SetContactListener(_contactListener);
}

void DeeWorld::initWorld() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //walls limiting the screen
    bottom = CCSprite::create();
    left = CCSprite::create();
    top = CCSprite::create();
    right = CCSprite::create();
    
    bottom->setColor(ccc3(0, 0, 0));
    bottom->setTextureRect(CCRectMake(0, 0, visibleSize.width, 20));
    bottom->setPosition(ccp(visibleSize.width / 2, 0));
    
    left->setColor(ccc3(0, 0, 0));
    left->setTextureRect(CCRectMake(0, 0, 20, visibleSize.height));
    left->setPosition(ccp(0, visibleSize.height / 2));
    
    top->setColor(ccc3(0, 0, 0));
    top->setTextureRect(CCRectMake(0, 0, visibleSize.width, 20));
    top->setPosition(ccp(visibleSize.width / 2, visibleSize.height));
    
    right->setColor(ccc3(0, 0, 0));
    right->setTextureRect(CCRectMake(0, 0, 20, visibleSize.height));
    right->setPosition(ccp(visibleSize.width, visibleSize.height / 2));
    
    CreateBox2DBodyForSprite(bottom, 0, NULL);
    CreateBox2DBodyForSprite(left, 0, NULL);
    CreateBox2DBodyForSprite(top, 0, NULL);
    CreateBox2DBodyForSprite(right, 0, NULL);
    
    this->addChild(bottom, 3, 3);
    this->addChild(left, 3, 3);
    this->addChild(top, 3, 3);
    this->addChild(right, 3, 3);
}

void DeeWorld::loadGame() {
	this->score = 0;
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	player->setPosition(
                        ccp(visibleSize.width / 2, player->getContentSize().height));
    
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
    
	CCLog("setting code");
    //code
	int i;
	for (i = 0; i < 3; i++) {
		this->createNewAminoAcid(MatrixHelper::getRandomAminoAcid());
	}
    
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
    
	updateView();
    
	_targets = new CCArray;
    
}

//countdown at the beginning of a level
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



// cpp with cocos2d-x
void DeeWorld::addTarget() {
    
	TBGTarget* tbgTarget = new TBGTarget();
    
	CCLog("ImagePath: %s", MatrixHelper::getImagePathForAcid(tbgTarget->getType()));
    
	CCSprite *target = tbgTarget->getSprite();
    
	//Place target in a randomly picked corner.
	int startX, startY;
	int corner = arc4random() % 4;
	//target-dimensions
	CCSize tSize = target->getContentSize();
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	corner = 0;
	switch (corner) {
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
    
	// TODO temp fix to test scoring event
	startX = 100;
	startY = 100;
	CCLog("Start-Position:x=%i,y=%i", startX, startY);
	target->setPosition(ccp(startX, startY));
    
	this->addChild(target);
	_targets->addObject(tbgTarget);
    
	CreateBox2DBodyForSprite(target, 0, NULL);
	this->moveTarget(tbgTarget, corner);
    
}

void DeeWorld::moveTarget(TBGTarget* tbgTarget, int edge) {
    
	CCSprite *target = tbgTarget->getSprite();
    
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

    CCPoint point = PositionHelper::calculateNewPos(tbgTarget, winSize, edge);
    
    CCLog("endX: %d, endY: %d", int(point.x), int(point.y));
    
    // Determine speed of the target
    int minDuration = (int) 2.0;
    int maxDuration = (int) 4.0;
    int rangeDuration = maxDuration - minDuration;
    
    int actualDuration = (rand() % rangeDuration) + minDuration;
    
    // Create the actions
    CCFiniteTimeAction* actionMove = CCMoveTo::create(
                                                      (float) actualDuration, point);
    
    CCFiniteTimeAction* actionMoveDone = CCCallFuncND::create(this,
                                                              callfuncND_selector(DeeWorld::spriteMoveFinished), (void*) tbgTarget);
    CCFiniteTimeAction* box2dDone = CCCallFuncN::create(this,
                                                        callfuncN_selector(DeeWorld::spriteDone));
    
    CCFadeIn *fadeInReadyText = CCFadeIn::create(1.0f);
    CCDelayTime *readyDelay = CCDelayTime::create(0.5f);
    CCFadeOut *fadeOutReadyText = CCFadeOut::create(1.0f);
    
    // Sebi: we have to add some dummy parameters otherwise it fails on Android
    CCSequence *readySequence = CCSequence::create(actionMove,
                                                   actionMoveDone, box2dDone, NULL, NULL);
    tbgTarget->getSprite()->runAction(readySequence);
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
        
        //should not be necessarey any more, since there will always be a collision with the walls
		//this->moveTarget(tbg);
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
    
	CCPoint playerPoint = player->getPosition();
    
	CCPoint pt = touch->getLocationInView();
	float y = tempHeight - pt.y;
	player->setPosition(ccp(pt.x, y));
	player->draw();
	player->update(0.01);
    
	CCDrawNode *node = CCDrawNode::create();
    
	ccColor4F color;
	switch (arc4random() % 4) {
        case 0:
            color = ccc4FFromccc4B(ccc4(255, 0, 0, 255));
            break;
        case 1:
            color = ccc4FFromccc4B(ccc4(200, 0, 0, 255));
            break;
        case 2:
            color = ccc4FFromccc4B(ccc4(160, 0, 0, 255));
            break;
        case 3:
            color = ccc4FFromccc4B(ccc4(100, 0, 0, 255));
            break;
        case 4:
            color = ccc4FFromccc4B(
                                   ccc4(arc4random() % 256, arc4random() % 256, arc4random() % 256,
                                        255));
            break;
	}
    
	// TODO color is not working correct (same red!) - ideally we make a nice gradient
	CCLog("r: %f, g, :%f ,b: %f", color.r, color.g, color.b);
    
	int thickness = 5;
	CCPoint points[4];
	points[0] = ccp(0, 0);
	points[1] = ccp(0, std::abs(pt.x - playerPoint.x));
	points[2] = ccp(std::abs(pt.x - playerPoint.x),
                    std::abs(y - playerPoint.y));
	points[3] = ccp(std::abs(y - playerPoint.y), 0);
    
	node->drawPolygon(points, 4, color, 0, ccc4FFromccc4B(ccc4(0, 0, 0, 255)));
	node->setPosition(playerPoint.x, playerPoint.y);
	node->draw();
    
	if (movementLines.size() >= 40) {
		CCDrawNode * nodeDel = movementLines.front();
		movementLines.pop();
		this->removeChild(nodeDel);
		//nodeDel->release();
		//CC_SAFE_RELEASE(nodeDel);
	}
	movementLines.push(node);
    
	this->addChild(node, 1);
    
	CCActionInterval * fadeOut = CCFadeOut::create(1.0);
	CCActionInterval * move = CCMoveTo::create(2.0, ccp(-10, -10));
	//	CCActionInterval * tilt = CC
	node->runAction(move);
    
	return;
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
                CCLog("Collision: Player on Target");
				//this->manageCollision(acid);
			}
			// Is sprite A a target and sprite B a player?  If so, push the target on a list to be destroyed...
			else if (iTagA == 1 && iTagB == 0) {
				toDestroy.push_back(bodyA);
				CCLog("Collision: Target on Player");
				//this->manageCollision(acid);
			}
            // Is sprite A a target and sprite B a wall
            else if(iTagA == 1 && iTagB == 3){
                CCLog("Collision: Target on Wall");
                int edge = getEdge(spriteB);
            } else if (iTagA == 3 && iTagB == 1) {
                CCLog("Collision: Wall on Target");
                int edge = getEdge(spriteA);
                //moveTarget(spriteB, edge);
            }
            
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
            
			// TODO only one scoring every five seconds - block it otherwise?
            
			if (_code.size() > 0) {
				BoardAcid * acid = this->_code.front();
                
				char wantedAcid = acid->acid;
				// TODO change this
				char playerAcid = 'A';
                
				// show score
				int scoring = MatrixHelper::getScoreForAminoAcid(wantedAcid, 'A');
                
				this->score = scoring;
                
				std::string str = static_cast<ostringstream*>(&(ostringstream()
                                                                << scoring))->str();
				CCSize visibleSize =
                CCDirector::sharedDirector()->getVisibleSize();
				this->_scoreNumber = CCLabelTTF::create(str.c_str(),
                                                        "Helvetica", 200, visibleSize, kCCTextAlignmentCenter);
				this->_scoreNumber->retain();
				this->_scoreNumber->setPosition(
                                                ccp(visibleSize.width / 2, visibleSize.height / 2));
				this->_scoreNumber->setColor(ccc3(0, 255, 0));
				this->addChild(_scoreNumber);
                
				CCActionInterval * tintToNumber = CCTintTo::create(1.0, 255, 20,
                                                                   50);
				this->_scoreNumber->runAction(tintToNumber);
				CCActionInterval * scaleTo = CCScaleTo::create(1.0, 0.01);
				this->_scoreNumber->runAction(scaleTo);
                
				cocos2d::CCLabelTTF* label = acid->_label;
				//label->setString("y");
                
                
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

//add Aminoacid to "Pipeline"
// TODO  this function appears to crash sometimes on Android
void DeeWorld::createNewAminoAcid(char c) {
    
	BoardAcid * acid = new BoardAcid();
	acid->setAcid(c);
    
	char tt[] = "t";
	tt[0] = c;
    
	std::string str = string(tt);
    
	CCLog("adding Acid %c", c);
    
    // Create the actions
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
	CCLabelTTF * label = CCLabelTTF::create(str.c_str(), "Helvetica", 30,
                                            CCSizeMake(30, visibleSize.height * 1 / 6), kCCTextAlignmentRight);
	acid->_label = label;
    
	acid->_label->setPosition(
                              ccp(visibleSize.width - 20, visibleSize.height * 1 / 7));
	acid->_label->setColor(ccc3(20, 20, 255));
	this->addChild(acid->_label);
    
	CCLog("moving AAs");
    
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

/**
 * Determines the edge-code by comparing the wall with which an object collided
 * to all the "walls". 
 * 
 * @param wall the CCSprite wall with which the object collided
 * @return 0=bottom, 1=left, 2=top, 3=right, -1 if wall is not a wall-sprite
 */
int DeeWorld::getEdge(CCSprite* wall) {
    if(wall == bottom) {
        return 0;
    } else if (wall == left) {
        return 1;
    } else if (wall == top ) {
        return 2;
    } else if (wall == right) {
        return 3;
    } else {
        return -1;
    }
}
