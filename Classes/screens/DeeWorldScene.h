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
#include "../ui_elements/CCBlade.h"
#include "../box2d/ContactListener.h"

#include "SimpleAudioEngine.h"


class DeeWorld: public cocos2d::CCLayerColor, public b2ContactListener {
public:
	DeeWorld();
	~DeeWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
	// instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene(std::string sequence);


	void setSequence(std::string seq);

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC (DeeWorld);

	void gameLogic(float dt);
    char getNextAminoAcid();
    void gameEnd();
    
    int getEdge(CCSprite* wall);

	//Touch Handlers
	virtual void ccTouchesBegan(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are started.
	virtual void ccTouchesMoved(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are moved
	virtual void ccTouchesEnded(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are ended.
    
	// The back key clicked
	virtual void keyBackClicked();

    //collision detected
    void BeginContact(b2Contact* contact);

    //box2d
    b2World *_b2dWorld;
    b2Body* CreateBox2DBodyForSprite(cocos2d::CCSprite *sprite, int iNumVerts, b2Vec2 verts[]);
    void tick(float delta);
    GLESDebugDraw *_debugDraw;
    
    // delete this property
	std::queue<CCDrawNode*> movementLines;


	std::queue<BoardAcid*> _code;

	//sound
	void startBackgroundSound();
	void stopBackgroundSound();



protected:
    cocos2d::CCArray *_targets;
	b2Body *player;
    //the walls around the screen
    b2Fixture *left, *top, *right, *bottom;


	void addTarget();

private:
    //init functions
    void makeMenu();
    void initBox2D();
    void initWorld();
    void initPlayer();
    void initInfoUI();
    void initBackground();

    
    void pauseAction(CCObject* pSender);

	void manageCollision(b2Body* target);
    void scoreAminoAcid(AminoAcid* sTarget);
	void countdown();
	void updateInfoUI();
	bool isGamePaused();
	bool isSoundEnabled();
	void resumeGame();

    int detectCorner();

	int score;
	int timer;
	double startTime;
	long lastAminoHitTime;
	std::string aminoSequence;
	bool pausedGame;


	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _timerLabel, timerLabel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _scoreLabel, scoreLabel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _scoreNumber, scoreNumber);
	bool validTouch;
	int tempHeight;
    int AAcounter;

	std::map< cocos2d::CCTouch *, CCBlade * >  _map;

};

#endif  // __DEEWORLD_SCENE_H__
