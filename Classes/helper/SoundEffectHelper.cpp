/*
 * SoundEffectHelper.cpp
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */



#include "SoundEffectHelper.h"

USING_NS_CC;
using namespace cocos2d;


bool SoundEffectHelper::isSoundEnabled(){
	CCLog("Music is: %s", cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey("music_enable", true) ? "on" :"off");
	if(!Globals::gSoundEnabled || (!	cocos2d::CCUserDefault::sharedUserDefault()->getBoolForKey(
			"music_enable", true))){
		return false;
	}else{
		return true;
	}
}

void SoundEffectHelper::playClickSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playLevelEndSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playLevelStartSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playGameEndSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playGameStartSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playTimerTickSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playNegativeCollisionSound(){
	if(isSoundEnabled()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playNeutralCollisionSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playPositiveCollisionSound(){
	if(isSoundEnabled()){
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
	}
}

void SoundEffectHelper::playLevelBackgroundMusic(int level){
	stopBackgroundMusic();
	if(isSoundEnabled()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
	}
}

// http://www.cocos2d-x.org/reference/native-cpp/V2.2/de/d8f/class_cocos_denshion_1_1_simple_audio_engine.html#a91013ef8ca9544db243e255161e15c1c
void SoundEffectHelper::playMainMenuBackgroundMusic(){
	stopBackgroundMusic();
	if(isSoundEnabled() && ! CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
	}
}

void SoundEffectHelper::playLevelLoadingBackgroundMusic(){
	stopBackgroundMusic();
	if(isSoundEnabled() && ! CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);
	}
}

void SoundEffectHelper::stopBackgroundMusic(){
	if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
	}
}

void SoundEffectHelper::stopAllMusic(){
	stopBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void SoundEffectHelper::pauseAllMusic(){
	if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void SoundEffectHelper::pauseBackgroundMusic(){
	if(CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()){
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
}

void SoundEffectHelper::resumeAllMusic(){
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
