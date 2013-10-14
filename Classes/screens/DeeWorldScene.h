#ifndef __DEEWORLD_SCENE_H__
#define __DEEWORLD_SCENE_H__

#include <queue>
#include <iostream>
#include <string>
#include <ctype.h>
#include <cmath>

#include "cocos2d.h"

#include "Box2D.h"
//for debug-draw
#include "../box2d/B2DebugDrawLayer.h"

#include "../box2d/GLES-Render.h"
#include "../ui_elements/AminoAcid.h"
#include "../ui_elements/BoardAcid.h"
#include "../box2d/ContactListener.h"

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

	void createTargets();

	void gameLogic(float dt);
    
    int getEdge(CCSprite* wall);

	//Touch Handlers
	virtual void ccTouchesBegan(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are started.
	virtual void ccTouchesMoved(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are moved
	virtual void ccTouchesEnded(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are ended.
    
	// The back key clicked
	virtual void keyBackClicked();


    //box2d
    b2World *_b2dWorld;
    b2Body* CreateBox2DBodyForSprite(cocos2d::CCSprite *sprite, int iNumVerts, b2Vec2 verts[] );
    void tick(float delta);
    CContactListener *_contactListener;
    GLESDebugDraw *_debugDraw;
    
	std::queue<CCDrawNode*> movementLines;
	std::queue<BoardAcid*> _code;


protected:
    cocos2d::CCArray *_targets;
	b2Body *player;
	std::string aminoAcidSeq;

	void addTarget();

private:
    //init functions
    void makeMenu();
    void initBox2D();
    void initWorld();
    void initPlayer();
    void initInfoUI();
    
	void manageCollision(AminoAcid* acid);
	void countdown();
	void updateInfoUI();
	int score;
	int timer;

	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _timerLabel, timerLabel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _scoreLabel, scoreLabel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _scoreNumber, scoreNumber);
	bool validTouch;
	int tempHeight;

    CCSprite *bottom, *left, *top, *right; //Walls
};

#endif  // __DEEWORLD_SCENE_H__
