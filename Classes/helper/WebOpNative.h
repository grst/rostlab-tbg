/*
 * WebOpNative.h
 *
 *  Created on: Oct 4, 2013
 *      Author: "Sebastian Wilzbach <sebi@wilzbach.me>"
 */

//
//  WebOpNative.h
//  HelloJavascript
//
//  Created by Simon on 13/9/9.
//
//

#ifndef __HelloJavascript__WebOpNative__
#define __HelloJavascript__WebOpNative__

#include <iostream>
#include "cocos2d.h"
#include <string>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

using namespace cocos2d;

class WebOpNative {

public:
    static void openLink(const char* url);
private:
    static void openURLOS(const char* url);//usage for IOS
    static void openURLJNI(const char* url); // JNI - android
};

#endif /* defined(__HelloJavascript__WebOpNative__) */
