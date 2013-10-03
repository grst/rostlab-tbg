#include "SplashScreen.h"
#include "DeeWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* SplashScreen::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    SplashScreen *layer = SplashScreen::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SplashScreen::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("bioinformatics", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height /2 - 60) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "SplashScreen" splash screen"
    CCSprite* pSpriteBackground = CCSprite::create("background.png");

    // position the sprite on the center of the screen
    pSpriteBackground->setPosition( ccp(size.width/2, size.height/2  ) );

    // add the sprite as a child to this layer
    this->addChild(pSpriteBackground, 0);

    // add "SplashScreen" splash screen"
    CCSprite* pSpriteLogo = CCSprite::create("logo.png");

     // position the sprite on the center of the screen
    pSpriteLogo->setPosition( ccp(size.width/2, size.height/2 + 40) );

    float scale = size.width;
    CCSize logoSize = pSpriteLogo->getContentSize();

    //scale it proportionally to 80% of the screen
    float scaleFactor = size.width / logoSize.width * 0.8;

    pSpriteLogo->setScaleX(scaleFactor);
    pSpriteLogo->setScaleY(scaleFactor);

    // add the sprite as a child to this layer
    this->addChild(pSpriteLogo, 0);


    // set a delay for two seconds
    this->runAction( CCSequence::create(
                                    CCDelayTime::create(2),
                                    CCCallFunc::create(this,
                                    callfunc_selector(SplashScreen::endScreen)),
                                    NULL));

    return true;
}

void SplashScreen::endScreen()
{
	CCScene *pScene = DeeWorld::scene();
	//transition to next scene for one sec
	CCDirector::sharedDirector()->replaceScene( CCTransitionFade::create(1.0f,pScene) );
}

