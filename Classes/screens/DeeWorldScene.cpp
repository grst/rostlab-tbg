#include "DeeWorldScene.h"
#include "SplashScreenScene.h"
#include "SimpleAudioEngine.h"
#include "../helper/PositionHelper.h"
#include "../helper/MatrixHelper.h"
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
    
	if (!CCLayer::init()) {
        return false;
    }
    if(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {
        return false;
    }
        
    makeMenu();
    initBox2D();
    
    ////TAGS
    // 0 = player
    // 1 = target
    // 2 = projectile (not in use any more)
    // 3 = wall
    initWorld();
    initPlayer();
    
    //load the scoring matrix
    MatrixHelper::loadMatrix("BLOSUM62.txt");
    
    this->setTouchEnabled(true);
    
    this->loadGame();
    
    
    // disabled temporarily  (annoying!!)
    // CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
        
    
    // Register the layer to touch dispatcher
    // Sebi: Android fails with the dispatcher ..
    // -- CCDirector::sharedDirector() -> getTouchDispatcher() -> addStandardDelegate( this, 0 );
    
	CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(
                                                                            this, 10);
    
	CCLog("init successful");
    
	return true;
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
    b2Vec2 gravity = b2Vec2(0.0f, -0.0f); //no gravity
    _b2dWorld = new b2World(gravity);
    
    this->schedule(schedule_selector(DeeWorld::tick));
    
//    /////////////////////// Box2dTests ////////////////////////////
//    //step for physics simulation
//    float32 timeStep = 1/20.0;      //the length of time passed to simulate (seconds)
//    int32 velocityIterations = 8;   //how strongly to correct velocity
//    int32 positionIterations = 3;   //how strongly to correct position
//    
//
//    b2BodyDef myBodyDef;
//    myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
//    myBodyDef.position.Set(200, 200); //a little to the left
//    
//    b2Body* dynamicBody1 = _b2dWorld->CreateBody(&myBodyDef);
//    b2CircleShape circleShape;
//    circleShape.m_p.Set(0, 0); //position, relative to body position
//    circleShape.m_radius = 1; //radius
//    
//    b2FixtureDef myFixtureDef;
//    myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
//    dynamicBody1->CreateFixture(&myFixtureDef); //add a fixture to the body
//    
//    _b2dWorld->Step( timeStep, velocityIterations, positionIterations);
//    
//    /////////////////// END TESTS //////////////////////////////////////
    
    //debug draw made easy with helper class
    this->addChild(B2DebugDrawLayer::create(_b2dWorld, PTM_RATIO), 9999);
    
    // Create contact listener
    _contactListener = new CContactListener();
    _b2dWorld->SetContactListener(_contactListener);
}

void DeeWorld::initWorld() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    // Create edges around the entire screen
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
    
	b2Body *groundBody = _b2dWorld->CreateBody(&groundBodyDef);
	b2EdgeShape groundEdge;
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &groundEdge;
    
	//wall definitions
	groundEdge.Set(b2Vec2(0,0), b2Vec2(visibleSize.width/PTM_RATIO, 0));
	groundBody->CreateFixture(&boxShapeDef);
    groundEdge.Set(b2Vec2(0,0), b2Vec2(0,visibleSize.height/PTM_RATIO));
    groundBody->CreateFixture(&boxShapeDef);
    groundEdge.Set(b2Vec2(0, visibleSize.height/PTM_RATIO),
                   b2Vec2(visibleSize.width/PTM_RATIO, visibleSize.height/PTM_RATIO));
    groundBody->CreateFixture(&boxShapeDef);
    groundEdge.Set(b2Vec2(visibleSize.width/PTM_RATIO, visibleSize.height/PTM_RATIO),
                   b2Vec2(visibleSize.width/PTM_RATIO, 0));
    groundBody->CreateFixture(&boxShapeDef);
}

void DeeWorld::initPlayer() {
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //player
    CCSprite *sPlayer = CCSprite::create(PLAYER_IMAGE, CCRectMake(0, 0, 27, 40));
    sPlayer->setZOrder(3);
    sPlayer->setPosition(
                         ccp(visibleSize.width / 2, sPlayer->getContentSize().height));
    
    //create the box for the player (currently with rectangle)
    player = CreateBox2DBodyForSprite(sPlayer, 0, NULL);
    
    //z-order 3, tag=0
    this->addChild(sPlayer, 3, 0);
    
    this->schedule(schedule_selector(DeeWorld::gameLogic), 1.0);

}

void DeeWorld::loadGame() {
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
	AminoAcid *sTarget = AminoAcid::create();
    
	//Place target in a randomly picked corner.
	int startX, startY;
	int corner = arc4random() % 4;
	//target-dimensions
	CCSize tSize = sTarget->getContentSize();
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
	sTarget->setPosition(ccp(startX, startY));
    
	this->addChild(sTarget);
	
    
	b2Body* target = CreateBox2DBodyForSprite(sTarget, 0, NULL);
    target->ApplyLinearImpulse(b2Vec2(300, 300), target->GetPosition());
    
    //_targets->addObject(target);
	//this->moveTarget(target, corner);
    
}

void DeeWorld::moveTarget(AminoAcid* target, int edge) {
    CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

    CCPoint point = PositionHelper::calculateNewPos(target, winSize, edge);
    
    CCLog("endX: %d, endY: %d", int(point.x), int(point.y));
    
    // Determine speed of the target
    int minDuration = (int) 2.0;
    int maxDuration = (int) 4.0;
    int rangeDuration = maxDuration - minDuration;
    
    int actualDuration = (rand() % rangeDuration) + minDuration;
    
    // Create the actions
    //TODO: what are all these actions about?
    CCFiniteTimeAction* actionMove = CCMoveTo::create(
                                                      (float) actualDuration, point);
    
    //TODO: the void-parameter is unnecessary!?
    CCFiniteTimeAction* actionMoveDone = CCCallFuncND::create(this,
                                                              callfuncND_selector(DeeWorld::spriteMoveFinished), (void*) target);
    CCFiniteTimeAction* box2dDone = CCCallFuncN::create(this,
                                                        callfuncN_selector(DeeWorld::spriteDone));
    
    //TODO: unused variables
    CCFadeIn *fadeInReadyText = CCFadeIn::create(1.0f);
    CCDelayTime *readyDelay = CCDelayTime::create(0.5f);
    CCFadeOut *fadeOutReadyText = CCFadeOut::create(1.0f);
    
    // Sebi: we have to add some dummy parameters otherwise it fails on Android
    CCSequence *readySequence = CCSequence::create(actionMove,
                                                   actionMoveDone, box2dDone, NULL, NULL);
    target->runAction(readySequence);
}



void DeeWorld::spriteMoveFinished(CCNode* sender, void* tbg_void) {
    
	CCSprite *sprite = (CCSprite *) sender;

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
	tempHeight = CCDirector::sharedDirector()->getWinSize().height;
    
	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*) *it;
	CCPoint pt = touch->getLocationInView();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* sPlayer = (CCSprite*) player->GetUserData();
	float height = size.height;
	float y = height - pt.y;
	int tolerance = 10;
    
	if (std::abs(pt.x - sPlayer->getPositionX()) - tolerance
        > sPlayer->getContentSize().width) {
		//non valid
		cocos2d::CCLog(
                       "ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
                       touch->getID(), (int) pt.x, (int) pt.y,
                       (int) sPlayer->getPositionX(), (int) sPlayer - getPositionY());
		validTouch = false;
	}
	if (std::abs(y - sPlayer->getPositionY()) - tolerance
        > sPlayer->getContentSize().height) {
		//non valid
		cocos2d::CCLog(
                       "ccTouchesCanceled id:%i x:%i,y:%in -- player x:%i, y:%in",
                       touch->getID(), (int) pt.x, (int) pt.y,
                       (int) sPlayer->getPositionX(), (int) sPlayer - getPositionY());
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
    
	b2Vec2 playerPoint = player->GetPosition();
    
	CCPoint pt = touch->getLocationInView();
	float y = tempHeight - pt.y;
	player->SetTransform(b2Vec2(pt.x /PTM_RATIO, y /PTM_RATIO), 0);
	//player->draw();
	//player->update(0.01);
    
	CCDrawNode *node = CCDrawNode::create();
    
//    // Movement line disabled for developing purposes
//    {
//	ccColor4F color;
//	switch (arc4random() % 4) {
//        case 0:
//            color = ccc4FFromccc4B(ccc4(255, 0, 0, 255));
//            break;
//        case 1:
//            color = ccc4FFromccc4B(ccc4(200, 0, 0, 255));
//            break;
//        case 2:
//            color = ccc4FFromccc4B(ccc4(160, 0, 0, 255));
//            break;
//        case 3:
//            color = ccc4FFromccc4B(ccc4(100, 0, 0, 255));
//            break;
//        case 4:
//            color = ccc4FFromccc4B(
//                                   ccc4(arc4random() % 256, arc4random() % 256, arc4random() % 256,
//                                        255));
//            break;
//	}
//    
//	// TODO color is not working correct (same red!) - ideally we make a nice gradient
//	CCLog("r: %f, g, :%f ,b: %f", color.r, color.g, color.b);
//    
//	CCPoint points[4];
//	points[0] = ccp(0, 0);
//	points[1] = ccp(0, std::abs(pt.x - playerPoint.x));
//	points[2] = ccp(std::abs(pt.x - playerPoint.x),
//                    std::abs(y - playerPoint.y));
//	points[3] = ccp(std::abs(y - playerPoint.y), 0);
//    
//	node->drawPolygon(points, 4, color, 0, ccc4FFromccc4B(ccc4(0, 0, 0, 255)));
//	node->setPosition(playerPoint.x, playerPoint.y);
//	node->draw();
//    
//	if (movementLines.size() >= 40) {
//		CCDrawNode * nodeDel = movementLines.front();
//		movementLines.pop();
//		this->removeChild(nodeDel);
//		//nodeDel->release();
//		//CC_SAFE_RELEASE(nodeDel);
//	}
//	movementLines.push(node);
//    
//	this->addChild(node, 1);
//    
//	CCActionInterval * fadeOut = CCFadeOut::create(1.0);
//	CCActionInterval * move = CCMoveTo::create(2.0, ccp(-10, -10));
//	//	CCActionInterval * tilt = CC
//	node->runAction(move);
//    }
    
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

//////////////////////////////////////////////////////////////////////////////////////////////////////
// This function send the vertice data to Box2D. Also, if you pass iNumVerts==0 and verts==NULL it simply creates a
// box round your sprite

b2Body* DeeWorld::CreateBox2DBodyForSprite(cocos2d::CCSprite *sprite,
                                        int iNumVerts, b2Vec2 verts[]) {
	if (_b2dWorld == NULL) {
		return NULL;
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
		spriteShape.Set(verts, iNumVerts);
		spriteBody->CreateFixture(&spriteShapeDef);
	} else {
		spriteShape.SetAsBox(size.width / PTM_RATIO / 2,
                             size.height / PTM_RATIO / 2);
		spriteBody->CreateFixture(&spriteShapeDef);
	}
    return spriteBody;
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
            
            //update sprites to match the simulation
            sprite->setPosition(ccp(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }
	}
    
    //return;
    
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
//            // Is sprite A a target and sprite B a wall
//            else if(iTagA == 1 && iTagB == 3){
//                CCLog("Collision: Target on Wall");
//                int edge = getEdge(spriteB);
//                moveTarget(dynamic_cast<AminoAcid*>(spriteA), edge);
//            } else if (iTagA == 3 && iTagB == 1) {
//                CCLog("Collision: Wall on Target");
//                int edge = getEdge(spriteA);
//                moveTarget(dynamic_cast<AminoAcid*>(spriteB), edge);
//            }
            
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

void DeeWorld::manageCollision(AminoAcid* aa) {
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
