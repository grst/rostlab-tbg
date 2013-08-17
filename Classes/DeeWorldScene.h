#ifndef __DEEWORLD_SCENE_H__
#define __DEEWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D.h"
//for debug-draw
//#include "GLES-Render.h"
#include "ContactListener.h"

#include "SimpleAudioEngine.h"

class DeeWorld: public cocos2d::CCLayerColor {
public:
	DeeWorld();
	~DeeWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
	// instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC (DeeWorld);

	void spriteMoveFinished(cocos2d::CCNode* sender);

	void gameLogic(float dt);

	void updateGame(float dt);

	void registerWithTouchDispatcher();

	//Touch Handlers
	virtual void ccTouchesBegan(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are started.
	virtual void ccTouchesMoved(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are moved
	virtual void ccTouchesEnded(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are ended.
    
    //box2d
    b2World *_b2dWorld;
    void CreateBox2DBodyForSprite( cocos2d::CCSprite *sprite, int iNumVerts, b2Vec2 verts[] );
    void tick(float delta);
    void spriteDone(CCNode* sender);
    CContactListener *_contactListener;
    //cocos2d::extension::GLESDebugDraw *_debugDraw;
    

protected:
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
	cocos2d::CCSprite *player;    
	int _projectilesDestroyed;

	void addTarget();

};

#endif  // __DEEWORLD_SCENE_H__
