//
//  HelperFunctions.cpp
//  tbg
//
//  Created by rootmac on 9/8/13.
//
//

#include "HelperFunctions.h"

USING_NS_CC;

//real modulo with positive result
int HelperFunctions::mod(int a, int b) {
	return ((a % b) + b) % b;
}

float HelperFunctions::randomValueBetween(float low, float high) {
	return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

/**
 * scale CCSprite to a fixed width and/or height
 * 
 * if width == 0, the sprite will be scale proportionally according to the height, 
 * if height == 0, vice versa
 */
void HelperFunctions::resizseSprite(cocos2d::CCSprite* sprite, float width, float height) {
    float contentHeight = sprite->getContentSize().height;
    float contentWidth = sprite->getContentSize().width;
    if(width == 0.0f && height == 0.0f) {
        return;
    } else if(width == 0.0f) {
        //scale proportionally according to height
        if(contentHeight == 0.0f) {
            return;
        }
        sprite->setScale(height/contentHeight);
    } else if(height == 0.0f) {
        //scale proportionally according to width
        if(contentWidth == 0.0f) {
            return;
        }
        sprite->setScale(width/contentWidth);
    } else {
        if(contentWidth == 0.0f || contentHeight == 0.0f){
            return;
        }
        sprite->setScaleX(width/contentWidth);
        sprite->setScaleY(height/contentHeight);
    }
}
//
///**
// * returns the scaled size of a sprite.
// */
//cocos2d::CCSize HelperFunctions::getCurrentSpriteSize(cocos2d::CCSprite* sprite) {
//    float width = sprite->getContentSize().width * sprite->getScaleX();
//    float height = sprite->getContentSize().height * sprite->getScaleY();
//    return CCSizeMake(width, height);
//}

/*
 * UNTESTED
 */
long HelperFunctions::nowInSeconds() {
	struct cocos2d::cc_timeval now;
	cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);
	//long retVal = (now.tv_sec + (double) (now.tv_usec / 1000000.0f));
	return ((long) now.tv_sec);
}

long HelperFunctions::nowInMilliSeconds() {
	struct cocos2d::cc_timeval now;
	cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);
	long retVal = (now.tv_sec * 1000) + (now.tv_usec / 1000);
	return retVal;
}

std::vector<std::string> HelperFunctions::split(const std::string &s,
		char delim) {
	std::vector < std::string > elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

void HelperFunctions::toggleMusic(){
	cocos2d::CCLog("toggle music");
	bool music = cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey(
						"music_enable", true);
	if (music) {
		cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("music_enable", false);
	}else{
		cocos2d::CCUserDefault::sharedUserDefault()->setBoolForKey("music_enable", true);
	}
}
