#include "DeeWorldScene.h"
#include "GameOverScene.h"
#include "SplashScreen.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define PTM_RATIO 32.0
#define NUMBER_START_TARGETS 20

using namespace cocos2d;

DeeWorld::~DeeWorld() {
	if (_targets) {
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles) {
		_projectiles->release();
		_projectiles = NULL;
	}

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

DeeWorld::DeeWorld() :
		_targets(NULL), _projectiles(NULL), _projectilesDestroyed(0) {
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

		pCloseItem->setPosition(
				ccp(
						origin.x + visibleSize.width
								- pCloseItem->getContentSize().width / 2,
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

		player = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40));

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

	} while (0);

	// Register the layer to touch dispatcher
	// Sebi: Android fails with the dispatcher ..
	// -- CCDirector::sharedDirector() -> getTouchDispatcher() -> addStandardDelegate( this, 0 );

	return bRet;
}

void DeeWorld::loadGame() {
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	player->setPosition(ccp(visibleSize.width / 2, visibleSize.height / 2));

	// begin to create the targets in one  second
	this->runAction(
			CCSequence::create(CCDelayTime::create(1),
					CCCallFunc::create(this,
							callfunc_selector(DeeWorld::createTargets)),
					NULL));

	_targetsAlive = NUMBER_START_TARGETS;
	_targets = new CCArray;
	_projectiles = new CCArray;
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

	CCSprite *target = CCSprite::create("Target.png", CCRectMake(0, 0, 27, 38));
	//create the box for the player (currently with rectangle)

	this->addChild(target);

	// Add to targets array
	target->setTag(1);
	_targets->addObject(target);

	// Determine where to spawn the target along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	float minY = target->getContentSize().height / 2;
	float maxY = winSize.height - target->getContentSize().height / 2;
	int rangeY = (int) (maxY - minY);
	// srand( TimGetTicks() );
	int actualY = (rand() % rangeY) + (int) minY;

	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated

	// TODO set the target somewhere
	int ranEdge = rand() % 4;
	int startX = 0;
	int startY = 0;
	int endX = target->getContentSize().width;
	int endY = target->getContentSize().height;

	switch (ranEdge) {

	// right
	case 0:
		startX = winSize.width + (target->getContentSize().width / 2);
		startY = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
		endX = 0 - target->getContentSize().width / 2;
		endY = actualY;
		break;
		// lower
	case 1:
		endY = winSize.width + (target->getContentSize().width / 2);
		startX = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
		startY = 0 - target->getContentSize().width / 2;
		endX = actualY;
		break;

		// left
	case 2:
		endX = winSize.width + (target->getContentSize().width / 2);
		startY = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
		startX = 0 - target->getContentSize().width / 2;
		endY = actualY;
		break;
		// upper
	case 3:
		startY = winSize.width + (target->getContentSize().width / 2);
		startX = CCDirector::sharedDirector()->getVisibleOrigin().y + actualY;
		endY = 0 - target->getContentSize().width / 2;
		endX = actualY;
		break;
	}

	target->setPosition(ccp(startX, startY));

	TBGTarget *tbg = new TBGTarget();
	tbg->setSprite(target);
	tbg->velX = rand() % 4 + 1;
	tbg->velY = rand() % 4 + 1;
	this->moveTarget(tbg);

}

void DeeWorld::moveTarget(TBGTarget* tbg) {

	CCSprite *target = tbg->getSprite();
	CreateBox2DBodyForSprite(target, 0, NULL);

	// Determine where to spawn the target along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();

	int edge = 5;
	int corner = 0;

	//recognize the edge
	CCPoint pos = target->getPosition();
	CCSize tarSize = target->getContentSize();

	int sensi = 5;
	int x = int(pos.x);
	int y = int(pos.y);
	//left edge
	if (x <= sensi) {
		edge = 2;
	}
	// right
	if (x >= winSize.width - sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 0;
	}

	//upper edge
	if (y >= winSize.height - sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 3;

	}

	// lower edge
	if (y <= sensi) {
		// look for corners
		if (edge < 5) {
			corner = 1;
		}
		edge = 2;
	}

	int endX = 0;
	int endY = 0;

	CCLog("edge type %d", edge);

	int randomX = winSize.width
			- int(rand()) % int((winSize.width - tarSize.width / 2));
	int randomY = winSize.height
			- int(rand()) % int((winSize.height - tarSize.height));

	if (edge > 4) {
		edge = rand() % 4;
	}

	switch (edge) {

	// right
	case 0:
		switch (rand() % 3) {
		case 0:
			endX = randomX / 2;
			endY = 0;
			break;
		case 1:
			endX = randomX / 2;
			endY = winSize.height;
			break;
		case 2:
			endX = 0;
			endY = randomY;
			break;
		}
		break;

		// lower
	case 1:
		switch (rand() % 4) {
		case 0:
		case 1:
			endX = randomX;
			endY = winSize.height;
			break;
		case 2:
			endX = tarSize.width / 2;
			endY = winSize.height - randomY / 2;
			break;
		case 3:
			endX = winSize.width;
			endY = winSize.height - randomY / 2;
			break;
		}

		break;

		// left
	case 2:

		switch (rand() % 3) {
		case 0:
			endX = winSize.width - randomX / 2;
			endY = 0;
			break;
		case 1:
			endX = winSize.width - randomX / 2;
			endY = winSize.height;
			break;
		case 2:
			endX = winSize.width;
			endY = randomY;
			break;
		}
		break;

		// upper
	case 3:
		switch (rand() % 4) {
		case 0:
		case 1:
			endX = randomX;
			endY = 0;
			break;
		case 2:
			endX = tarSize.width / 2;
			endY = randomY / 2;
			break;
		case 3:
			endX = winSize.width;
			endY = randomY / 2;
			break;
		}

		break;
	}

	/*
	 } else {

	 // test if we have a corner
	 if (corner == 1) {
	 // just swap
	 //endX = winSize.width - pos.x + 50 - rand() % 100;
	 //endY = winSize.height - pos.y + 50 - rand() % 100;

	 endX = winSize.width / 2;
	 endY = winSize.height / 2;

	 CCLog("corner recognized");

	 tbg->velX = -tbg->velX;
	 tbg->velY = -tbg->velY;

	 } else {

	 edge = (edge + 2) % 4;
	 CCPoint point;

	 switch (edge) {

	 // right
	 case 0:

	 // we have to mirror it again
	 if (tbg->velY < 0) {
	 tbg->velY = -tbg->velY;
	 }
	 tbg->velX = -tbg->velX;

	 point = this->mirrorVertical(int(winSize.width),
	 int(winSize.height), int(tarSize.width),
	 int(tarSize.height), int(tbg->velX), int(tbg->velY),
	 winSize.width, int(pos.y));
	 endX = winSize.width - point.x;
	 endY = point.y;
	 break;

	 // lower
	 case 1:
	 //check velocity for security
	 if (tbg->velY < 0) {
	 tbg->velY = -tbg->velY;
	 }

	 tbg->velX = -tbg->velX;

	 point = this->mirrorHorizontal(int(winSize.width),
	 int(winSize.height), int(tarSize.width),
	 int(tarSize.height), int(tbg->velX), int(tbg->velY),
	 int(pos.x), int(pos.y));
	 endX = point.x;
	 endY = point.y;
	 break;

	 // left
	 case 2:

	 if (tbg->velY < 0) {
	 tbg->velY = -tbg->velY;
	 }

	 point = this->mirrorVertical(int(winSize.width),
	 int(winSize.height), int(tarSize.width),
	 int(tarSize.height), int(tbg->velX), int(tbg->velY),
	 int(pos.x), int(pos.y));
	 endX = int( point.x) % int(winSize.width);
	 endY = point.y;
	 break;

	 // upper
	 case 3:
	 // we have to mirror it again
	 if (tbg->velY < 0) {
	 tbg->velY = -tbg->velY;
	 }

	 tbg->velX = -tbg->velX;

	 point = this->mirrorHorizontal(int(winSize.width),
	 int(winSize.height), int(tarSize.width),
	 int(tarSize.height), int(tbg->velX), int(tbg->velY),
	 int(pos.x), int(tarSize.height));
	 endX =  point.x;
	 endY = winSize.width - point.y;
	 break;
	 }

	 // simulate the wall and mirror velocity
	 //normal edges: right, left
	 if (edge % 2 == 1) {
	 tbg->velX = -tbg->velX;
	 } else {
	 tbg->velY = -tbg->velY;
	 }
	 }

	 }
	 */

	CCLog("endX: %d, endY: %d", endX, endY);

	if (endX < 0) {
		endX = 0;
	}

	if (endY < target->getContentSize().height) {
		endY = target->getContentSize().height;
	}

	if (endX > winSize.width) {
		endX = winSize.width;
	}
	if (endY > winSize.height) {
		endY = winSize.height;
	}

//target->setPosition(ccp(startX, startY));

// Determine speed of the target
	int minDuration = (int) 2.0;
	int maxDuration = (int) 4.0;
	int rangeDuration = maxDuration - minDuration;
// srand( TimGetTicks() );
	int actualDuration = (rand() % rangeDuration) + minDuration;

// Create the actions
	CCFiniteTimeAction* actionMove = CCMoveTo::create((float) actualDuration,
			ccp(endX, endY));

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
	target->runAction(readySequence);

}
/*
 cocos2d::CCPoint DeeWorld::mirrorHorizontal(int maxX, int maxY, int oWidth,
 int oHeight, int velX, int velY, int x, int y) {
 int endX, endY;
 CCLog("HZ x: %d, y: %d, velX: %d, velY: %d", x, y, velX, velY);

 if (velY > 0) {
 int possibleX = (maxY-0) * velX / 1.0 * velY;
 if (velX < 0) {
 possibleX = -possibleX;
 }
 // look if we hit upper edge
 if (oWidth < possibleX < maxX) {
 CCLog("horizontal edge");
 endX = possibleX;
 endY = maxY;
 } else {
 //right edge
 if (velX > 0) {
 CCLog("right edge");
 endX = maxX;
 endY = (velY * 1.0 / velX) * (maxX - x);
 } else {
 //left
 CCLog("left edge");
 endX = 0;
 endY = -(velY * 1.0 / velX) * x;
 }
 }
 } else {
 endX = 50;
 endY = 50;
 }

 CCPoint point;
 point.setPoint(endX, endY);

 return point;
 }

 cocos2d::CCPoint DeeWorld::mirrorVertical(int maxX, int maxY, int oWidth,
 int oHeight, int velX, int velY, int x, int y) {
 int endX, endY;

 CCLog("VK x: %d, y: %d, velX: %d, velY: %d", x, y, velX, velY);

 if (velY > 0) {

 int possibleX = (maxY - x) * velX / velY;

 if (possibleX < maxX) {
 endX = possibleX;
 endY = maxY;
 } else {
 endX = maxX;
 endY = maxX * velY / velX + x;
 }

 } else {
 int minY = 0;
 int possibleX = (minY - x) * velX / velY;

 if (possibleX < maxX) {
 endX = possibleX;
 endY = minY;
 } else {
 endX = maxX;
 endY = maxX * velY / velX + x;
 }
 }

 endX = maxX - rand() % 200;
 endY = maxY;

 CCPoint point;
 point.setPoint(endX, endY);
 return point;
 }
 */

void DeeWorld::spriteMoveFinished(CCNode* sender, void* tbg_void) {

	CCSprite *sprite = (CCSprite *) sender;

	TBGTarget *tbg = (TBGTarget*) tbg_void;

	if (sprite->getTag() == 1)  // target
			{
//remove from target list - NO keep the target
//_targets->removeObject(sprite);

//////
// box2d stuff! (remove objects from list)
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
			_b2dWorld->DestroyBody(spriteBody);
		}

//diabled for testing
//GameOverScene *gameOverScene = GameOverScene::create();
//gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
//CCDirector::sharedDirector()->replaceScene(gameOverScene);

	} else if (sprite->getTag() == 2) // projectile
			{
//box2d not implemented for projectiles
		_projectiles->removeObject(sprite);
	}

//remove cocos2dx object - NO KEEP IT

	this->moveTarget(tbg);
//this->removeChild(sprite, true);
}

void DeeWorld::gameLogic(float dt) {
// do some crazy stuff here
}

// cpp with cocos2d-x
void DeeWorld::ccTouchesBegan(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {
	CCSetIterator it = touches->begin();
	CCTouch* touch;

	CCPoint pt;
	for (int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++) {
		touch = (CCTouch*) (*it);
		pt = touch->getLocationInView();
		cocos2d::CCLog("ccTouchesBegan id:%i %i,%in", touch->getID(),
				(int) pt.x, (int) pt.y);
		it++;
	}
}

// cpp with cocos2d-x
void DeeWorld::ccTouchesMoved(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {
	CCSetIterator it = touches->begin();
	CCTouch* touch;

	CCPoint pt;
	for (int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++) {
		touch = (CCTouch*) (*it);
		pt = touch->getLocationInView();
//cocos2d::CCLog( "ccTouchesBegan id:%i %i,%in", touch->getID(), (int)pt.x, (int)pt.y );
		it++;

//world y
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		float height = size.height;

//set player
		player->setPosition(ccp(pt.x, height - pt.y));
	}
}

// cpp with cocos2d-x
void DeeWorld::ccTouchesEnded(cocos2d::CCSet* touches,
		cocos2d::CCEvent* event) {
// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*) (touches->anyObject());
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

//projectiles deactivated.
	return;

// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite *projectile = CCSprite::create("Projectile.png",
			CCRectMake(0, 0, 20, 20));
	projectile->setPosition(ccp(origin.x + 20, origin.y + winSize.height / 2));

// Determinie offset of location to projectile
	float offX = location.x - projectile->getPosition().x;
	float offY = location.y - projectile->getPosition().y;

// Bail out if we are shooting down or backwards
	if (offX <= 0)
		return;

// Ok to add now - we've double checked position
	this->addChild(projectile);

// Determine where we wish to shoot the projectile to
	float realX = origin.x + winSize.width
			+ (projectile->getContentSize().width / 2);
	float ratio = offY / offX;
	float realY = (realX * ratio) + projectile->getPosition().y;
	CCPoint realDest = ccp(realX, realY);

// Determine the length of how far we're shooting
	float offRealX = realX - projectile->getPosition().x;
	float offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
	float velocity = 480 / 1; // 480pixels/1sec
	float realMoveDuration = length / velocity;

// Move projectile to actual endpoint
	projectile->runAction(
			CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),
					CCCallFuncN::create(this,
							callfuncN_selector(DeeWorld::spriteMoveFinished)),
					NULL));

// Add to projectiles array
	projectile->setTag(2);
	_projectiles->addObject(projectile);

//disabled temporarly - working!
//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

void DeeWorld::updateGame(float dt) {
	CCArray *projectilesToDelete = new CCArray;
	CCObject* it = NULL;
	CCObject* jt = NULL;

// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
	CCARRAY_FOREACH(_projectiles, it) {
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
				projectile->getPosition().x
						- (projectile->getContentSize().width / 2),
				projectile->getPosition().y
						- (projectile->getContentSize().height / 2),
				projectile->getContentSize().width,
				projectile->getContentSize().height);

		CCArray* targetsToDelete = new CCArray;

// for (jt = _targets->begin(); jt != _targets->end(); jt++)
		CCARRAY_FOREACH(_targets, jt) {
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake(
					target->getPosition().x
							- (target->getContentSize().width / 2),
					target->getPosition().y
							- (target->getContentSize().height / 2),
					target->getContentSize().width,
					target->getContentSize().height);

			// if (CCRect::CCRectIntersectsRect(projectileRect, targetRect))
			if (projectileRect.intersectsRect(targetRect)) {
				targetsToDelete->addObject(target);
			}
		}

// for (jt = targetsToDelete->begin(); jt != targetsToDelete->end(); jt++)
		CCARRAY_FOREACH(targetsToDelete, jt) {
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			_targets->removeObject(target);
			this->removeChild(target, true);

			_projectilesDestroyed++;
			if (_projectilesDestroyed >= 5) {
				// disabled for testing
				//GameOverScene *gameOverScene = GameOverScene::create();
				//gameOverScene->getLayer()->getLabel()->setString("You Win!");
				//CCDirector::sharedDirector()->replaceScene(gameOverScene);
			}
		}

		if (targetsToDelete->count() > 0) {
			projectilesToDelete->addObject(projectile);
		}
		targetsToDelete->release();
	}

// for (it = projectilesToDelete->begin(); it != projectilesToDelete->end(); it++)
	CCARRAY_FOREACH(projectilesToDelete, it) {
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// This function send the vertice data to Box2D. Also, if you pass iNumVerts==0 and verts==NULL it simply creates a
// box round your sprite.
void DeeWorld::CreateBox2DBodyForSprite(cocos2d::CCSprite *sprite,
		int iNumVerts, b2Vec2 verts[]) {
	if (_b2dWorld == NULL)
		return;

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
//    //m_spriteSheet->
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

			//TODO: sprite might already have been removed -> null pointer exception

			// Convert the Cocos2D position/rotation of the sprite to the Box2D position/rotation
			CCPoint Pos = sprite->getPosition();
			b2Vec2 b2Position = b2Vec2(Pos.x / PTM_RATIO, Pos.y / PTM_RATIO);
			float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(sprite->getRotation());

			// Update the Box2D position/rotation to match the Cocos2D position/rotation
			b->SetTransform(b2Position, b2Angle);
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

			// Is sprite A a player and sprite B a target?  If so, push the target on a list to be destroyed...
			if (iTagA == 0 && iTagB == 1) {
				toDestroy.push_back(bodyB);
				CCLog("Collision");
			}
			// Is sprite A a target and sprite B a player?  If so, push the target on a list to be destroyed...
			else if (iTagA == 1 && iTagB == 0) {
				toDestroy.push_back(bodyA);
				CCLog("Collision");
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
			this->removeChild(sprite, true);

			//ATTETION we don't delete the sprite out of the array, this might cause memory leaks

			_targetsAlive = _targetsAlive - 1;
			CCLOG("remaining % d", _targetsAlive);
			if (_targetsAlive <= 0) {
				// restart game
				CCLabelTTF* pLabel = CCLabelTTF::create("you win", "Thonburi",
						34);
				// ask director the window size
				CCSize size = CCDirector::sharedDirector()->getWinSize();
				// position the label on the center of the screen
				pLabel->setPosition(ccp(size.width / 2, size.height / 2 - 60));

				// add the label as a child to this layer
				this->addChild(pLabel, 1);

				CCScene *pScene = SplashScreen::scene("you win");
				//transition to next scene for one sec
				CCDirector::sharedDirector()->replaceScene(
						CCTransitionFade::create(1.0f, pScene));
			}

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
