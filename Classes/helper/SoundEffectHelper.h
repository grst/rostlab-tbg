/*
 * SoundEffectHelper.h
 *
 *  Created on: Nov 13, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

#ifndef SOUNDEFFECTHELPER_H_
#define SOUNDEFFECTHELPER_H_

#include "cocos2d.h"
#include "../Globals.h"

#include "SimpleAudioEngine.h"

class SoundEffectHelper {
public:
	static void playClickSound();
	static void playLevelEndSound();
	static void playLevelStartSound();
	static void playTimerTickSound();
	static void playNegativeCollisionSound();
	static void playNeutralCollisionSound();
	static void playPositiveCollisionSound();

	static void stopBackgroundMusic();
	static void pauseBackgroundMusic();
	static void stopAllMusic();
	static void pauseAllMusic();
	static void resumeAllMusic();


	static void playLevelBackgroundMusic(int level);
	static void playMainMenuBackgroundMusic();
	static void playLevelLoadingBackgroundMusic();
	static bool isSoundEnabled();
};

#endif /* SOUNDEFFECTHELPER_H_ */
