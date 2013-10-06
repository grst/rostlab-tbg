LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_CFLAGS += -Wno-error=format-security

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/box2d/B2DebugDrawLayer.cpp \
                   ../../Classes/DeeWorldScene.cpp \
                   ../../Classes/TBGTarget.cpp  \
                   ../../Classes/BoardAcid.cpp  \
                   ../../Classes/SplashScreenScene.cpp \
                   ../../Classes/box2d/ContactListener.cpp \
                   ../../Classes/box2d/GLES-Render.cpp  \
                   ../../Classes/helper/PositionHelper.cpp  \
                   ../../Classes/helper/MatrixHelper.cpp  \
                   ../../Classes/helper/WebOpNative.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../..//lib/Box2D
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes 

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external/Box2D)
