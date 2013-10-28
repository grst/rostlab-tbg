//
//  HelperFunctions.h
//  tbg
//
//  Created by rootmac on 9/8/13.
//
//

#ifndef __tbg__HelperFunctions__
#define __tbg__HelperFunctions__

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include "math.h"
#include "cocos2d.h"

class HelperFunctions {
public:
    static int mod(int a, int b);
    static float randomValueBetween(float low, float high);
    static std::vector<std::string> split(const std::string &s, char delim);
    static long nowInSeconds();
    static long nowInMilliSeconds();
    static void resizseSprite(cocos2d::CCSprite* sprite, float width, float height);
//    static cocos2d::CCSize getCurrentSpriteSize(cocos2d::CCSprite* sprite);
};
#endif /* defined(__tbg__HelperFunctions__) */
