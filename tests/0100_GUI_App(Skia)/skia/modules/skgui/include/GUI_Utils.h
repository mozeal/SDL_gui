//
//  GUI_Utils.hpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_Utils_hpp
#define GUI_Utils_hpp

#include <string>
#include <stdio.h>
#include <functional>
#include <SDL.h>
#include "GUI_Config.h"
#include "include/core/SkColor.h"
#include "include/core/SkPoint.h"
#include "include/core/SkRect.h"

class GUI_View;

const Uint32 GUI_EventID = SDL_USEREVENT;
const Uint32 GUI_EventUpdate = GUI_EventID + 1;
const Uint32 GUI_EventPaint  = GUI_EventID + 2;
const Uint32 GUI_WindowResized  = GUI_EventID + 3;
const Uint32 GUI_FontChanged  = GUI_EventID + 4;
const Uint32 GUI_UpdateSize  = GUI_EventID + 5;
const Uint32 GUI_EventUser = GUI_EventID + 300;

extern SDL_Renderer *GUI_renderer;

//struct GUI_Rect :SkRect {
struct GUI_Rect {
    int x, y, w, h;

    GUI_Rect();
    GUI_Rect(Sint16 x, Sint16 y, Uint16 dx, Uint16 dy);
    void set(Sint16 x, Sint16 y, Uint16 dx, Uint16 dy);
    void setX(int _x){
        x=_x;
    }
    void setY(int _y){
        y=_y;
    }
    void setW(int _w){
        w=_w;
    }
    void setH(int _h){
        h=_h;
    }
    void setWH(int _w, int _h){
        setW(_w);
        setH(_h);
    }

    SkRect toSkRect(){
        return SkRect::MakeXYWH(x, y, w, h);
    }

    GUI_Rect getIntersection(const GUI_Rect& rect) const{
        float x0 = std::max(x,rect.x);
        float x1 = std::min(x+w,rect.x+rect.w);

        float w = x1 - x0;
        if(w < 0.0f) return GUI_Rect(0,0,0,0); // short circuit if needed

        float y0 = std::max(y,rect.y);
        float y1 = std::min(y+h,rect.y+rect.h);

         float h = y1 - y0;
         if(h < 0.0f) return GUI_Rect(0,0,0,0);  // short circuit if needed

         return GUI_Rect(x0,y0,w,h);
    }


    int width(){ return w; }
    int height(){ return h; }
};
GUI_Rect *GUI_MakeRect(int x, int y, int w, int h);

//struct GUI_Point : SkPoint{
struct GUI_Point{
    int x, y;

    GUI_Point();
    GUI_Point(short _x, short _y);
    void set(short _x, short _y);
    bool operator==(GUI_Point b);
    bool operator!=(GUI_Point b);
};

SkColor sk_color(Uint32 c); // (a << 24) | (r << 16) | (g << 8) | (b << 0)

extern SkColor cClear;
extern SkColor cNop;
extern SkColor cWhite;
extern SkColor cBlack;
extern SkColor cGrey;
extern SkColor cRed;
extern SkColor cGreen;
extern SkColor cBlue;
extern SkColor cCyan;
extern SkColor cMagenta;
extern SkColor cYellow;
extern SkColor cDarkGrey;
extern SkColor cLightGrey;
extern SkColor cCornFlowerBlue;



typedef enum GUI_LOGLEVEL_ {
    GUI_LOG_VERBOSE = 0,
    GUI_LOG_NOTICE,
    GUI_LOG_WARNING,
    GUI_LOG_ERROR,
    GUI_LOG_FATAL_ERROR,
    GUI_LOG_SILENT
} GUI_LogLevel;

void GUI_SetLogLevel(GUI_LogLevel level);
GUI_LogLevel GUI_GetLogLevel();
void GUI_Log(const char * format, ...);
void GUI_Error(const char* fn, int result);

std::string GUI_GetCurrentPath();
std::string GUI_GetResourcePath(const std::string &subDir = "");
SDL_Texture* GUI_LoadTexture(const std::string &filename, SDL_Renderer *ren);
void GUI_SetMouseCapture( GUI_View *v );
GUI_View * GUI_GetMouseCapture();
int GUI_GetStatusBarHeight();

extern float GUI_scale;
extern float GUI_mouseScale;
extern int GUI_windowWidth;
extern int GUI_windowHeight;

#ifdef __IPHONEOS__
extern "C" int getiOSStatusBarHeight();
extern "C" bool isIPAD();
extern "C" char *getDeviceModel();
extern "C" char *getOrientation();
extern "C" bool hasNotch();
extern "C" int *getContentSaftyMargin();
#endif

#ifdef __EMSCRIPTEN__
inline bool SDL_IsTablet() { return true; };
#endif

SDL_Color GUI_TranslateColor(Uint32 int_color);

enum ScreenShotType
{
    ePng,
    eJpg,
    eRaw
};

void GUI_MakeRendererScreenshot(SDL_Renderer* renderer, std::string fileName, ScreenShotType type = ePng);

void GUI_SetViewport(GUI_Rect vp);
void GUI_SetClipRect(GUI_Rect rect);
void GUI_GetViewport(GUI_Rect &vp);
void GUI_GetClipRect(GUI_Rect &rect);


#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

#endif /* GUI_Utils_hpp */
