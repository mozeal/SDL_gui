LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_ROOT := $(LOCAL_PATH)/../../../SDL2

LOCAL_C_INCLUDES := $(SDL_ROOT)/include


# Add your application source files here...
LOCAL_SRC_FILES := main.cpp

LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_CFLAGS += -D__ANDROID__
LOCAL_CPPFLAGS += -fexceptions -std=c++11 -w
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -lz

include $(BUILD_SHARED_LIBRARY)
