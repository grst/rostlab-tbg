/****************************************************************************
 Copyright (c) 2010-2011 cocos2d-x.org
 Copyright (c) 2010      Ray Wenderlich
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef SPLASHSCREEN_H_
#define SPLASHSCREEN_H_

#include "cocos2d.h"

class SplashScreenLayer : public cocos2d::CCLayerColor
{
public:
	SplashScreenLayer():_label(NULL) {};
    virtual ~SplashScreenLayer();
    bool init();
    CREATE_FUNC(SplashScreenLayer);

    void endScreen();

    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
};

class SplashScreenScene : public cocos2d::CCScene
{
public:
	SplashScreenScene():_layer(NULL) {};
    ~SplashScreenScene();
    bool init();
    CREATE_FUNC(SplashScreenScene);
  
    CC_SYNTHESIZE_READONLY(SplashScreenLayer*, _layer, Layer);
};

#endif // _GAME_OVER_SCENE_H_
