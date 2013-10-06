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
#include "box2d/B2DebugDrawLayer.h"
#include "box2d/GLES-Render.h"
#include "TBGTarget.h"
#include "BoardAcid.h"


#include "box2d/ContactListener.h"

#include "SimpleAudioEngine.h"

typedef struct
 {
     CCPoint pt1;
     CCPoint pt2;
     float r;
     float g;
     float b;
} DebugLine;

class DeeWorld: public cocos2d::CCLayerColor {
public:
	DeeWorld();
	~DeeWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
	// instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	//virtual void draw(void);

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC (DeeWorld);

	void loadGame();

	void moveTarget(TBGTarget* target);
	void createTargets();

	void spriteMoveFinished(cocos2d::CCNode* sender, void* tbg);

	void gameLogic(float dt);

	void updateGame(float dt);

	void registerWithTouchDispatcher();

	//Touch Handlers
	virtual void ccTouchesBegan(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are started.
	virtual void ccTouchesMoved(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are moved
	virtual void ccTouchesEnded(cocos2d::CCSet * touches, cocos2d::CCEvent * event); // When touches are ended.
    
    //box2d
    b2World *_b2dWorld;
    void CreateBox2DBodyForSprite(cocos2d::CCSprite *sprite, int iNumVerts, b2Vec2 verts[] );
    void tick(float delta);
    void spriteDone(CCNode* sender);
    CContactListener *_contactListener;
    GLESDebugDraw *_debugDraw;
    

protected:
    cocos2d::CCArray *_targets;
	cocos2d::CCSprite *player;    
	std::string aminoAcidSeq;
	int _targetsAlive;

	void addTarget();

private:
	void manageCollision(TBGTarget* acid);
	void countdown();
	void updateView();
	void createNewAminoAcid(char c);
	int score;
	int timer;
	std::queue<BoardAcid*> _code;
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _timerLabel, timerLabel);
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _scoreLabel, scoreLabel);
	bool validTouch;
	int tempHeight;
	std::queue<BoardAcid*> movementLines;
	 std::vector<DebugLine>* m_lines;
};

#endif  // __DEEWORLD_SCENE_H__
