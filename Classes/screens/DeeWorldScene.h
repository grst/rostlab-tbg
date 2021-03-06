#ifndef __DEEWORLD_SCENE_H__
#define __DEEWORLD_SCENE_H__

#include <queue>
#include <iostream>
#include <string>
#include <ctype.h>
#include <cmath>

#include "cocos2d.h"

#include "Box2D/Box2D.h"
//for debug-draw
#include "../box2d/B2DebugDrawLayer.h"

#include "../box2d/GLES-Render.h"
#include "../ui_elements/AminoAcid.h"
#include "../ui_elements/BoardAcid.h"
#include "../ui_elements/cc-extensions/CCBlade.h"
#include "../box2d/ContactListener.h"
#include "../helper/AcidCounter.h"


class DeeWorld: public cocos2d::CCLayerColor, public b2ContactListener {
public:
	DeeWorld();
	~DeeWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
	// instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene(std::string sequence, int level);


	void setSequence(std::string seq);



	// implement the "static node()" method manually
	CREATE_FUNC (DeeWorld);

	void gameLogic(float dt);
    char getNextAminoAcid();
    char getCurrentAminoAcid();
    char popAcidFront();
    void gameEnd();
    
    int getEdge(CCSprite* wall);

	//Touch Handlers
	virtual void ccTouchesBegan(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are started.
	virtual void ccTouchesMoved(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are moved
	virtual void ccTouchesEnded(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are ended.
    
	// The back key clicked
	virtual void keyBackClicked();
	virtual void keyMenuClicked();

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

	cocos2d::CCLabelBMFont* _scoreNumber;


protected:
    cocos2d::CCArray *_targets;
	b2Body *player;
    //the walls around the screen
    b2Fixture *left, *top, *right, *bottom;

    void addTarget(char c);
	void addTarget();

private:
    //init functions
    void makeMenu();
    void initBox2D();
    void initWorld();
    void initPlayer();
    void initInfoUI();
    void initBackground();

    int getMinSpeed();
    int getMaxSpeed();
    int getMinAA();
    int getMaxAA();
    int getAAAddProb();
    int getOtherAAProp();

    bool gameEnded;

    void initListener();
    void onApplicationStatusChanged(CCObject* obj);

    
	// a selector callback
	void pauseLayerCallback(cocos2d::CCObject* pSender);
    void pauseAction(int i);
    void pauseGame();

	void manageCollision(b2Body* target);
    void scoreAminoAcid(AminoAcid* sTarget);
	void countdown();
	void countdownSoundCall();
	void showCountdown(int time);

	void updateInfoUI();
	bool isGamePaused();
	void resumeGame();
	bool isLayerOpen();

    int detectCorner();

    void playerRestore();

	int timer;
	double startTime;
	long lastAminoHitTime;
	std::string aminoSequence;
    std::vector<char> doneSequence;
    AcidCounter * acidCounter;
	bool isAminoAcidRemaining();
	bool pausedGame;
	bool startTimer;
	int level;
	int score;
	bool togglePlayer;
	bool blockedPlayer;


	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _timerLabel, timerLabel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _scoreLabel, scoreLabel);
	bool validTouch;
	int tempHeight;
    int AAcounter;

	std::map< cocos2d::CCTouch *, CCBlade * >  _map;

};

#endif  // __DEEWORLD_SCENE_H__
