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

#ifndef LevelEnd_H_
#define LevelEnd_H_

#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include "cocos2d.h"

class LevelEndLayer : public cocos2d::CCLayer
{
public:
	LevelEndLayer():_label(NULL) {};
    virtual ~LevelEndLayer();
    bool init();
    CREATE_FUNC(LevelEndLayer);

    void endScreen();
    void addLabels();
    CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);


    int score;
    int level;
private:
    cocos2d::CCLabelTTF* scoreLabel;
    cocos2d::CCLabelTTF* levelLabel;
};

class LevelEndScene : public cocos2d::CCScene
{
public:
	LevelEndScene():_layer(NULL) {};
    ~LevelEndScene();
    static CCScene* create(int score, int level);
    bool init();
    CREATE_FUNC(LevelEndScene);
  
    CC_SYNTHESIZE_READONLY(LevelEndLayer*, _layer, Layer);
};

#endif // _GAME_OVER_SCENE_H_
