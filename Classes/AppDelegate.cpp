#include "AppDelegate.h"
#include "ui_elements/TouchTrailLayer.h"
#include "screens/DeeWorldScene.h"
#include "screens/MainScreenScene.h"
#include "screens/SplashScreenScene.h"

USING_NS_CC;


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCSize designSize = CCSizeMake(480, 320);
    std::vector<std::string> searchPaths;
    
    if (screenSize.height > 320)
    {
        searchPaths.push_back("hd");
        searchPaths.push_back("hd/acids");
        searchPaths.push_back("hd/buttons");
        searchPaths.push_back("hd/backgrounds");
        pDirector->setContentScaleFactor(640.0f/designSize.height);
    }
    else
    {
        searchPaths.push_back("sd");
        searchPaths.push_back("sd/acids");
        searchPaths.push_back("sd/buttons");
        searchPaths.push_back("hd/backgrounds");
        pDirector->setContentScaleFactor(320.0f/designSize.height);
    }
    
    searchPaths.push_back("matrices");
    searchPaths.push_back("music");

    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionFixedHeight);

    // turn on display FPS
   // pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = SplashScreenScene::create(true);

    //Sebi: Change here the start screen

    // for dev reasons use normal screen
   // CCScene *pScene = DeeWorld::scene();

    //GS: changed back to DeeWorld for dev purposes. 
   //CCScene *pScene = MainScreenScene::create();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCLOG("Application in background");
	CCDirector::sharedDirector()->stopAnimation();
    CCDirector::sharedDirector()->pause();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("APP_STATUS_CHANGED", CCBool::create(false));
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCLOG("Application there again");
    CCDirector::sharedDirector()->startAnimation();
    CCDirector::sharedDirector()->resume();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("APP_STATUS_CHANGED", CCBool::create(true));
}
