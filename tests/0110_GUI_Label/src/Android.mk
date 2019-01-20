LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_GUI_ROOT := $(LOCAL_PATH)/../../../SDL2_gui
SDL_GFX_ROOT := $(LOCAL_PATH)/../../../SDL2_gfx

LOCAL_C_INCLUDES := $(SDL_GUI_ROOT) $(SDL_GFX_ROOT)

# Add your application source files here...
LOCAL_SRC_FILES := main.cpp

LOCAL_SRC_FILES += $(SDL_GFX_ROOT)/SDL2_gfxPrimitives.c \
                   $(SDL_GFX_ROOT)/SDL2_rotozoom.c \
                   $(SDL_GFX_ROOT)/SDL2_framerate.c \
                   $(SDL_GFX_ROOT)/SDL2_imageFilter.c

LOCAL_SRC_FILES += $(SDL_GUI_ROOT)/SDL_gui.cpp \
                   $(SDL_GUI_ROOT)/GUI_shapes.cpp \
                   $(SDL_GUI_ROOT)/GUI_image.cpp \
                   $(SDL_GUI_ROOT)/GUI_Utils.cpp \
                   $(SDL_GUI_ROOT)/GUI_View.cpp \
                   $(SDL_GUI_ROOT)/GUI_ImageView.cpp \
                   $(SDL_GUI_ROOT)/GUI_TextView.cpp \
                   $(SDL_GUI_ROOT)/GUI_Fonts.cpp \
                   $(SDL_GUI_ROOT)/GUI_IconView.cpp \
                   $(SDL_GUI_ROOT)/GUI_Label.cpp

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf
LOCAL_CFLAGS += -D__ANDROID__
LOCAL_CPPFLAGS += -fexceptions -std=c++11
LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
