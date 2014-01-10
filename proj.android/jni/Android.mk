LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Globals.cpp \
                   ../../Classes/box2d/B2DebugDrawLayer.cpp \
                   ../../Classes/box2d/ContactListener.cpp \
                   ../../Classes/box2d/GLES-Render.cpp \
                   ../../Classes/ui_elements/AminoAcid.cpp \
                   ../../Classes/ui_elements/AboutUsLayer.cpp \
                   ../../Classes/ui_elements/BoardAcid.cpp \
                   ../../Classes/ui_elements/ImpressumLayer.cpp \
                   ../../Classes/ui_elements/PauseLayer.cpp \
                   ../../Classes/ui_elements/cc-extensions/TouchTrailLayer.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCBlade.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCSlider.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCRadioMenu.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCGestureRecognizer/CCGestureRecognizer.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCGestureRecognizer/CCLongPressGestureRecognizer.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCGestureRecognizer/CCPanGestureRecognizer.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCGestureRecognizer/CCPinchGestureRecognizer.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCGestureRecognizer/CCSwipeGestureRecognizer.cpp \
                   ../../Classes/ui_elements/cc-extensions/CCGestureRecognizer/CCTapGestureRecognizer.cpp \
                   ../../Classes/ui_elements/SlidingMenu.cpp \
                   ../../Classes/helper/AcidCounter.cpp  \
                   ../../Classes/helper/CCHttpRequest.cpp  \
                   ../../Classes/helper/DebugDraw.cpp  \
                   ../../Classes/helper/MatrixHelper.cpp  \
                   ../../Classes/helper/HelperFunctions.cpp  \
                   ../../Classes/helper/LevelHelper.cpp \
                   ../../Classes/helper/SoundEffectHelper.cpp  \
                   ../../Classes/helper/UIElements.cpp \
                   ../../Classes/helper/WebOpNative.cpp \
                   ../../Classes/screens/DeeWorldScene.cpp \
                   ../../Classes/screens/LevelLoadingScene.cpp \
                   ../../Classes/screens/LevelEndScene.cpp \
                   ../../Classes/screens/MainScreenScene.cpp \
                   ../../Classes/screens/SettingsScreenScene.cpp \
                   ../../Classes/screens/SplashScreenScene.cpp 
                  
   
               
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
