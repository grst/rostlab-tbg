//
//  WebOpNative.cpp
//  HelloJavascript
//
//  Created by Simon on 13/9/9.
//   For questions look here
// http://digitalsynapsesblog.blogspot.de/2011/09/cocos2d-x-launching-url-on-android.html
// http://www.cocos2d-x.org/forums/20/topics/33218?r=35093
//

#include "WebOpNative.h"

void WebOpNative::openLink(const char *url)
{
    CCLog("into openLink");
    // iphone
#if ! CC_TARGET_PLATFORM && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
    openURLOS(url);
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    openURLJNI(url);
#endif
}

void WebOpNative::openURLOS(const char* url){
	//Create a URL object.
#if ! CC_TARGET_PLATFORM && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
	    NSString *url = [NSString stringWithUTF8String:willopenurl];
	    NSLog(@"will open url is, %@",url);

	     [[UIApplication sharedApplication] openURL:[NSURL URLWithString:url]];
#endif
}

// DO NOT FORGET to add "http://"
void WebOpNative::openURLJNI(const char* url)
{
	CCLog("Involking openFansPage JNI...");
	    JniMethodInfo t;

	    if (JniHelper::getStaticMethodInfo(t, "org/rostlab/tbg/rostlab_tbg"
	                                       ,"openURL"
	                                       ,"(Ljava/lang/String;)V"))
	    {
	        jstring str = t.env->NewStringUTF(url);
	        t.env->CallStaticVoidMethod(t.classID,t.methodID,str);
	    }
}
