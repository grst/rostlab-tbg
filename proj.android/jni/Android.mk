LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

APP_CFLAGS += -Wno-error=format-security

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/box2d/B2DebugDrawLayer.cpp \
                   ../../Classes/box2d/ContactListener.cpp \
                   ../../Classes/box2d/GLES-Render.cpp \
                   ../../Classes/ui_elements/AminoAcid.cpp \
                   ../../Classes/ui_elements/BoardAcid.cpp \
                   ../../Classes/ui_elements/CCBlade.cpp \
                   ../../Classes/ui_elements/PauseLayer.cpp \
                   ../../Classes/ui_elements/TouchTrailLayer.cpp \
                   ../../Classes/helper/DebugDraw.cpp  \
                   ../../Classes/helper/MatrixHelper.cpp  \
                   ../../Classes/helper/HelperFunctions.cpp  \
                   ../../Classes/helper/UIElements.cpp \
                   ../../Classes/helper/WebOpNative.cpp \
                   ../../Classes/screens/DeeWorldScene.cpp \
                   ../../Classes/screens/MainScreenScene.cpp \
                   ../../Classes/screens/SettingsScreenScene.cpp \
                   ../../Classes/screens/SplashScreenScene.cpp 
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../..//lib/Box2D
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes 

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions) \
$(call import-module,external/Box2D)
