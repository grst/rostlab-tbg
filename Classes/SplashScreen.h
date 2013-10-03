/*
 * SplashScreen.h
 *
 *  Created on: Oct 3, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef SPLASHSCREEN_H_
#define SPLASHSCREEN_H_


#include "cocos2d.h"

class SplashScreen : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

    void endScreen();

    // implement the "static node()" method manually
    CREATE_FUNC(SplashScreen);
};

#endif // __HELLOWORLD_SCENE_H__
