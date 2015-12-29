LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := /Users/Jim/___Phoenix/sdl/SDL
COMMON_PATH := /users/Jim/___Phoenix/sdl/MyExperiment/_common

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(COMMON_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	$(COMMON_PATH)/common.cpp main.cpp

LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_CFLAGS += -D__ANDROID__ 
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

APP_STL := gnustl_static
LOCAL_C_INCLUDES += ${ANDROID_NDK}/sources/cxx-stl/gnu-libstdc++/4.8/include

include $(BUILD_SHARED_LIBRARY)
