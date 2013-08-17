#include "DeeWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;


#define PTM_RATIO 32.0
		

using namespace cocos2d;

DeeWorld::~DeeWorld()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

DeeWorld::DeeWorld()
:_targets(NULL)
,_projectiles(NULL)
,_projectilesDestroyed(0)
{
}

CCScene* DeeWorld::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		DeeWorld *layer = DeeWorld::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool DeeWorld::init()
{

    if ( !CCLayer::init() ) return false;

	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(255,255,255,255) ) );

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////

		// 1. Add a menu item with "X" image, which is clicked to quit the program.

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(DeeWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);
        
		// Place the menu item bottom-right conner.
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to DeeWorld layer as a child layer.
		this->addChild(pMenu, 1);

		/////////////////////////////
		// 2. add your codes below...

		player = CCSprite::create("Player.png", CCRectMake(0, 0, 27, 40) );
        
		player->setPosition( ccp(origin.x + player->getContentSize().width/2,
                                 origin.y + visibleSize.height/2) );
        
        //create the box for the player (currently with rectangle)
        CreateBox2DBodyForSprite(player, 0, NULL);
        
		this->addChild(player);

		this->schedule( schedule_selector(DeeWorld::gameLogic), 1.0 );

		this->setTouchEnabled(true);

		_targets = new CCArray;
		_projectiles = new CCArray;

		// use updateGame instead of update, otherwise it will conflit with SelectorProtocol::update
		// see http://www.cocos2d-x.org/boards/6/topics/1478
		this->schedule( schedule_selector(DeeWorld::updateGame) );

		// disabled temporarly  (annoying!!)
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;
        
        
        /////////////////////////////
        // init Box2D
        b2Vec2 gravity = b2Vec2(0.0f, 0.0f); //no gravity
        _b2dWorld = new b2World(gravity);
        
        this->schedule(schedule_selector(DeeWorld::tick));
                
        
	} while (0);

	// Register the layer to touch dispatcher
	   CCDirector::sharedDirector() -> getTouchDispatcher() -> addStandardDelegate( this, 0 );

	return bRet;
}

void DeeWorld::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

// cpp with cocos2d-x
void DeeWorld::addTarget()
{
	CCSprite *target = CCSprite::create("Target.png", CCRectMake(0,0,27,40) );
    
	// Determine where to spawn the target along the Y axis
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	float minY = target->getContentSize().height/2;
	float maxY = winSize.height -  target->getContentSize().height/2;
	int rangeY = (int)(maxY - minY);
	// srand( TimGetTicks() );
	int actualY = ( rand() % rangeY ) + (int)minY;
    
   
	// Create the target slightly off-screen along the right edge,
	// and along a random position along the Y axis as calculated
	target->setPosition( 
		ccp(winSize.width + (target->getContentSize().width/2), 
            CCDirector::sharedDirector()->getVisibleOrigin().y + actualY) );
    
    //create the box for the player (currently with rectangle)
    CreateBox2DBodyForSprite(target, 0, NULL);

	this->addChild(target);

	// Determine speed of the target
	int minDuration = (int)2.0;
	int maxDuration = (int)4.0;
	int rangeDuration = maxDuration - minDuration;
	// srand( TimGetTicks() );
	int actualDuration = ( rand() % rangeDuration ) + minDuration;

	// Create the actions
	CCFiniteTimeAction* actionMove = CCMoveTo::create( (float)actualDuration,
                                            ccp(0 - target->getContentSize().width/2, actualY) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, 
                                            callfuncN_selector(DeeWorld::spriteMoveFinished));
    CCFiniteTimeAction* box2dDone = CCCallFuncN::create(this, callfuncN_selector(DeeWorld::spriteDone));
	target->runAction( CCSequence::create(actionMove, actionMoveDone, box2dDone) );

	// Add to targets array
	target->setTag(1);
	_targets->addObject(target);
}

void DeeWorld::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	

	if (sprite->getTag() == 1)  // target
	{
        //remove from target list
		_targets->removeObject(sprite);
        
        //////
        // box2d stuff! (remove objects from list)
        // Loop through all of the Box2D bodies in our Box2D world...
        // We're looking for the Box2D body corresponding to the sprite.
        b2Body *spriteBody = NULL;
        for(b2Body *b = _b2dWorld->GetBodyList(); b; b=b->GetNext())
        {
            // See if there's any user data attached to the Box2D body
            // There should be, since we set it in addBoxBodyForSprite
            if (b->GetUserData() != NULL) {
                
                // We know that the user data is a sprite since we set
                // it that way, so cast it...
                CCSprite *curSprite = (CCSprite *)b->GetUserData();
                
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

	}
	else if (sprite->getTag() == 2) // projectile
	{
        //box2d not implemented for projectiles
		_projectiles->removeObject(sprite);
	}
    
    //remove cocos2dx object
    this->removeChild(sprite, true);
}

void DeeWorld::gameLogic(float dt)
{
	this->addTarget();
}

// cpp with cocos2d-x
void DeeWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCSetIterator it = touches->begin();
	    CCTouch* touch;

	    CCPoint pt;
	    for( int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++ )
	    {
	        touch = (CCTouch*)(*it);
	        pt = touch->getLocationInView();
	        cocos2d::CCLog( "ccTouchesBegan id:%i %i,%in", touch->getID(), (int)pt.x, (int)pt.y );
	        it++;
	    }
}

// cpp with cocos2d-x
void DeeWorld::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCSetIterator it = touches->begin();
	    CCTouch* touch;

	    CCPoint pt;
	    for( int iTouchCount = 0; iTouchCount < touches->count(); iTouchCount++ )
	    {
	        touch = (CCTouch*)(*it);
	        pt = touch->getLocationInView();
	        cocos2d::CCLog( "ccTouchesBegan id:%i %i,%in", touch->getID(), (int)pt.x, (int)pt.y );
	        it++;

	        //world y
	        CCSize size = CCDirector::sharedDirector()->getWinSize();
	        float height =size.height;

	        //set player
	        player->setPosition( ccp(pt.x,
	                                         height - pt.y) );
	    }
}

// cpp with cocos2d-x
void DeeWorld::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	// Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

    //projectiles deactivated. 
    return;
    
	// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
	CCSprite *projectile = CCSprite::create("Projectile.png", CCRectMake(0, 0, 20, 20));
	projectile->setPosition( ccp(origin.x+20, origin.y+winSize.height/2) );

	// Determinie offset of location to projectile
	float offX = location.x - projectile->getPosition().x;
	float offY = location.y - projectile->getPosition().y;

	// Bail out if we are shooting down or backwards
	if (offX <= 0) return;

	// Ok to add now - we've double checked position
	this->addChild(projectile);

	// Determine where we wish to shoot the projectile to
	float realX = origin.x+winSize.width + (projectile->getContentSize().width/2);
	float ratio = offY / offX;
	float realY = (realX * ratio) + projectile->getPosition().y;
	CCPoint realDest = ccp(realX, realY);

	// Determine the length of how far we're shooting
	float offRealX = realX - projectile->getPosition().x;
	float offRealY = realY - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
	float velocity = 480/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;

	// Move projectile to actual endpoint
	projectile->runAction( CCSequence::create(
		CCMoveTo::create(realMoveDuration, realDest),
		CCCallFuncN::create(this,
                            callfuncN_selector(DeeWorld::spriteMoveFinished)),
        NULL) );

	// Add to projectiles array
	projectile->setTag(2);
	_projectiles->addObject(projectile);

	//disabled temporarly - working!
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

void DeeWorld::updateGame(float dt)
{
	CCArray *projectilesToDelete = new CCArray;
    CCObject* it = NULL;
    CCObject* jt = NULL;

	// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
    CCARRAY_FOREACH(_projectiles, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
			projectile->getPosition().x - (projectile->getContentSize().width/2),
			projectile->getPosition().y - (projectile->getContentSize().height/2),
			projectile->getContentSize().width,
			projectile->getContentSize().height);

		CCArray* targetsToDelete =new CCArray;

		// for (jt = _targets->begin(); jt != _targets->end(); jt++)
        CCARRAY_FOREACH(_targets, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake(
				target->getPosition().x - (target->getContentSize().width/2),
				target->getPosition().y - (target->getContentSize().height/2),
				target->getContentSize().width,
				target->getContentSize().height);

			// if (CCRect::CCRectIntersectsRect(projectileRect, targetRect))
            if (projectileRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(target);
			}
		}

		// for (jt = targetsToDelete->begin(); jt != targetsToDelete->end(); jt++)
        CCARRAY_FOREACH(targetsToDelete, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			_targets->removeObject(target);
			this->removeChild(target, true);

			_projectilesDestroyed++;
			if (_projectilesDestroyed >= 5)
			{
				// disabled for testing
				//GameOverScene *gameOverScene = GameOverScene::create();
				//gameOverScene->getLayer()->getLabel()->setString("You Win!");
				//CCDirector::sharedDirector()->replaceScene(gameOverScene);
			}
		}

		if (targetsToDelete->count() > 0)
		{
			projectilesToDelete->addObject(projectile);
		}
		targetsToDelete->release();
	}

	// for (it = projectilesToDelete->begin(); it != projectilesToDelete->end(); it++)
    CCARRAY_FOREACH(projectilesToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// This function send the vertice data to Box2D. Also, if you pass iNumVerts==0 and verts==NULL it simply creates a
// box round your sprite.
void DeeWorld::CreateBox2DBodyForSprite( cocos2d::CCSprite *sprite, int iNumVerts, b2Vec2 verts[] )
{
    if( _b2dWorld==NULL )
        return;
    
    CCPoint pos = sprite->getPosition();
	CCSize size = sprite->getContentSize();
    
    b2BodyDef spriteBodyDef;
    spriteBodyDef.type = b2_dynamicBody;
    spriteBodyDef.position.Set(pos.x/PTM_RATIO, pos.y/PTM_RATIO);
    spriteBodyDef.userData = sprite;
    b2Body *spriteBody = _b2dWorld->CreateBody(&spriteBodyDef);
    
    b2PolygonShape spriteShape;
    
    if( iNumVerts!=0 )
    {
        spriteShape.Set(verts, iNumVerts);
        b2FixtureDef spriteShapeDef;
        spriteShapeDef.shape = &spriteShape;
        spriteShapeDef.density = 10.0;
        spriteShapeDef.isSensor = true;
        
        spriteBody->CreateFixture(&spriteShapeDef);
    }
    else
    {
        // No Vertice supplied so just make a box round the sprite
        b2BodyDef spriteBodyDef;
        spriteBodyDef.type = b2_dynamicBody;
        spriteBodyDef.position.Set( pos.x/PTM_RATIO, pos.y/PTM_RATIO );
        spriteBodyDef.userData = sprite;
        b2Body *spriteBody = _b2dWorld->CreateBody( &spriteBodyDef );
        
        b2PolygonShape spriteShape;
        spriteShape.SetAsBox( size.width/PTM_RATIO/2, size.height/PTM_RATIO/2 );
        b2FixtureDef spriteShapeDef;
        spriteShapeDef.shape = &spriteShape;
        spriteShapeDef.density = 10.0;
        spriteShapeDef.isSensor = true;                     // isSensor true when you want to know when objects will collide without triggering a box2d collision response
        spriteBody->CreateFixture( &spriteShapeDef );
    }
}

void DeeWorld::spriteDone(CCNode* sender)
{
    // This selector is called from CCCallFuncN, and it passes the object the action is
    // run on as a parameter.  We know it's a sprite, so cast it as that!
    CCSprite *sprite = (CCSprite *)sender;
    
    // Loop through all of the Box2D bodies in our Box2D world...
    // We're looking for the Box2D body corresponding to the sprite.
    b2Body *spriteBody = NULL;
    for(b2Body *b = _b2dWorld->GetBodyList(); b; b=b->GetNext())
    {
        // See if there's any user data attached to the Box2D body
        // There should be, since we set it in addBoxBodyForSprite
        if (b->GetUserData() != NULL) {
            
            // We know that the user data is a sprite since we set
            // it that way, so cast it...
            CCSprite *curSprite = (CCSprite *)b->GetUserData();
            
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
    
//    // And of course we need to remove the Cocos2D sprite too.
//    m_spriteSheet->removeChild(sprite, true);
//    //m_spriteSheet->
}

void DeeWorld::tick(float delta) {
    _b2dWorld->Step(delta, 10, 10);
    // Loop through all of the Box2D bodies in our Box2D world..
    for(b2Body *b = _b2dWorld->GetBodyList(); b; b=b->GetNext())
    {
        // See if there's any user data attached to the Box2D body
        // There should be, since we set it in addBoxBodyForSprite
        if (b->GetUserData() != NULL)
        {
            // We know that the user data is a sprite since we set
            // it that way, so cast it...
            CCSprite *sprite = (CCSprite *)b->GetUserData();
            
            //TODO: sprite might already have been removed -> null pointer exception
            
            
            // Convert the Cocos2D position/rotation of the sprite to the Box2D position/rotation
            CCPoint Pos = sprite->getPosition();
            b2Vec2 b2Position = b2Vec2(Pos.x/PTM_RATIO,
                                           Pos.y/PTM_RATIO);
            float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(sprite->getRotation());
                
            // Update the Box2D position/rotation to match the Cocos2D position/rotation
            b->SetTransform(b2Position, b2Angle);
        }
    }
}

void DeeWorld::registerWithTouchDispatcher()
{
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
