LOCAL_PATH := $(call my-dir)

SDL_ROOT := $(LOCAL_PATH)/../../../../SDL
SDL_IMAGE_ROOT := $(LOCAL_PATH)/../../../../SDL_image
SDL_NET_ROOT := $(LOCAL_PATH)/../../../../SDL_net
SDL_TTF_ROOT := $(LOCAL_PATH)/../../../../SDL_ttf
SDL_GUI_ROOT := $(LOCAL_PATH)/../../../../SDL_gui

include $(CLEAR_VARS)

LOCAL_MODULE := main

ifeq ($(OS), Windows_NT)
	GUI_PATH := $(LOCAL_PATH)/../../../../../../SDL_gui/SDL_gui
	GUI_SRC_PATH :=  ../../../../../../SDL_gui/SDL_gui
	SDL_ANDROID_MAIN_PATH :=  ../../../../../../SDL/src/main/android
else
	GUI_PATH := $(SDL_GUI_ROOT)/SDL_gui
	GUI_SRC_PATH := $(GUI_PATH)
	SDL_ANDROID_MAIN_PATH := $(SDL_ROOT)/src/main/android
endif

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image
LOCAL_CFLAGS += -D__ANDROID__ 
LOCAL_CPPFLAGS += -fexceptions -std=c++11
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -landroid
LOCAL_C_INCLUDES := $(SDL_ROOT)/include $(SDL_IMAGE_ROOT)

# Add your application source files here...
LOCAL_SRC_FILES := main.cpp jni_glue.cpp

include $(BUILD_SHARED_LIBRARY)
