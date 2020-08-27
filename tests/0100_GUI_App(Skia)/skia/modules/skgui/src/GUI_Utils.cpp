//
//  GUI_Utils.cpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

//#include <SDL_ttf.h>
//#include <SDL_image.h>
#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
    #include <direct.h>  // for _chdir(), _getcwd()
    #define GetCurrentDir _getcwd
    #define    chdir _chdir // pom
#else
    #include <unistd.h>
    #include <dirent.h> // for DIR, dirent, opendir()
    #include <pwd.h>
    #define GetCurrentDir getcwd
#endif
#ifdef __ANDROID__
#include <android/log.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#endif
#include <fstream>
#include <iostream>
#include <vector>
#include "GUI_Utils.h"
#include "GUI_Config.h"
#include "GUI_App.h"
#include "include/core/SkCanvas.h"

#define LOGNAME "sk_gui"

GUI_Rect::GUI_Rect() {
//    setXYWH(0,0,0,0);
    set(0,0,0,0);
}

GUI_Rect::GUI_Rect(Sint16 _x, Sint16 _y, Uint16 _dx, Uint16 _dy) {
//    setXYWH(_x, _y, _dx, _dy);
    set(_x, _y, _dx, _dy);
}

void GUI_Rect::set(Sint16 _x, Sint16 _y, Uint16 _dx, Uint16 _dy) {
//    setXYWH(_x, _y, _dx, _dy);
    x = _x;
    y = _y;
    w = _dx;
    h = _dy;
}

GUI_Rect *GUI_MakeRect(int x, int y, int w, int h) { // creates pointer to Rect. Each source file gets its own instance.
    static GUI_Rect rp_rect;
    rp_rect.set(x, y, w, h);
    return &rp_rect;
}

GUI_Point::GUI_Point(short x1, short y1){
//    SkPoint::set(x1, y1);
    set(x1, y1);
}

GUI_Point::GUI_Point(){
//    SkPoint::set(0, 0);
    set(0, 0);
}

void GUI_Point::set(short _x1, short _y1) {
//    SkPoint::set(x1, y1);
    x = _x1;
    y = _y1;
}

bool GUI_Point::operator==(GUI_Point b) {
    return x == b.x && y == b.y;
//    return equals(b.x(), b.y());
}

bool GUI_Point::operator!=(GUI_Point b) {
    return x!=b.x || y!=b.y;
//    return !equals(b.x(), b.y());
}

SkColor sk_color(Uint32 c) {
    return c;
}

SkColor
cClear = SK_ColorTRANSPARENT,
cNop = SkColorSetARGB(1, 0, 0, 0),
cWhite = SK_ColorWHITE,
cBlack = SK_ColorBLACK,
cGrey = SK_ColorGRAY,
cRed = SK_ColorRED,
cGreen = SK_ColorGREEN,
cBlue = SK_ColorBLUE,
cCyan = SK_ColorCYAN,
cMagenta = SK_ColorMAGENTA,
cYellow = SK_ColorYELLOW,
cDarkGrey = SK_ColorDKGRAY,
cLightGrey = SK_ColorLTGRAY,
cCornFlowerBlue = sk_color(0xff6495ed);


static GUI_LogLevel currentLogLevel = GUI_LOG_NOTICE;

void GUI_Log(const char * format, ...) {
#ifdef __ANDROID__
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    //__android_log_print(ANDROID_LOG_VERBOSE, LOGNAME, buffer, 1);
    SDL_Log( "%s", buffer );
#else
    if (GUI_GetLogLevel() <= GUI_LOG_NOTICE) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
#endif
}

//--------------------------------------------------
void GUI_SetLogLevel(GUI_LogLevel level) {
    currentLogLevel = level;
}

//--------------------------------------------------
GUI_LogLevel GUI_GetLogLevel() {
    return currentLogLevel;
}

std::string GUI_GetCurrentPath() {
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        return NULL;
    }

    return std::string(cCurrentPath);
}

std::string GUI_GetResourcePath(const std::string &subDir) {
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif
    static std::string baseRes;

    baseRes = "";
#if defined (__ANDROID__) || (__MACOSX__)
    baseRes = baseRes + "data" + PATH_SEP;
#else

    if (baseRes.empty()) {
        char *basePath = SDL_GetBasePath();

        if (basePath) {
            baseRes = basePath;
            SDL_free(basePath);
        } else {
            std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
            return "";
        }

        //We replace the last bin/ with res/ to get the the resource path
        size_t pos = baseRes.rfind("bin");
        baseRes = baseRes.substr(0, pos) + "data" + PATH_SEP;
    }
#endif
    return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

SDL_Texture* GUI_LoadTexture(const std::string &filename, SDL_Renderer *ren) {
//    std::string imagePath;
//
//    if (filename[0] != '/')
//        imagePath = GUI_GetResourcePath() + filename;
//    else
//        imagePath = "" + filename;
//
//    SDL_Texture *tex = IMG_LoadTexture(ren, imagePath.c_str());
//
//    if (tex == NULL) {
//        GUI_Log("Load image %s failed.", filename.c_str());
//        imagePath = GUI_GetCurrentPath() + PATH_SEPARATOR "data" + PATH_SEPARATOR + filename;
//        tex = IMG_LoadTexture(ren, imagePath.c_str());
//    }
//
//    return tex;
    return NULL;
}

SDL_Color GUI_TranslateColor(Uint32 int_color) //Change from an "int color" to an SDL_Color
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        SDL_Color color={(unsigned char)((int_color & 0x00ff0000) >> 16), (unsigned char)((int_color & 0x0000ff00) >> 8), (unsigned char)(int_color & 0x000000ff), 0};
    #else
        SDL_Color color={(unsigned char)(int_color & 0x000000ff), (unsigned char)((int_color & 0x0000ff00) >> 8), (unsigned char)((int_color & 0x00ff0000) >> 16), 0};
    #endif
    return color;
}

void GUI_MakeRendererScreenshot(SDL_Renderer* renderer, std::string fileName, ScreenShotType type)
{
    int w(0), h(0);
    int result(0);
    result = SDL_GetRendererOutputSize(renderer, &w, &h);
    if(result) SDL_LogError(0, SDL_GetError());

    SDL_Surface* surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(GUI_renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    switch(type)
    {
    case ePng:
//        IMG_SavePNG(surface, fileName.c_str());
        break;
    case eJpg:
//        IMG_SaveJPG(surface, fileName.c_str(), 100);
        break;
    case eRaw:
        std::vector<char> lVector;
        lVector.resize(surface->pitch * h);
        for(int i(0); i < surface->pitch * h; ++i)
            lVector[i] = ((char*) surface->pixels)[i];
        std::fstream s(fileName, std::fstream::ios_base::binary | std::fstream::ios_base::trunc | std::fstream::ios_base::in | std::fstream::ios_base::out);
        if(!s.is_open())
            SDL_LogError( 0, "Open file failed");
        s.write(lVector.data(), lVector.size());
        break;
    }

    SDL_FreeSurface(surface);
}

GUI_Rect __viewport, __cliprect;

void GUI_SetViewport(GUI_Rect vp){
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    canvas->resetMatrix();
    canvas->translate(vp.x, vp.y);
    __viewport.set(vp.x, vp.y, vp.w, vp.h);
}

void GUI_GetViewport(GUI_Rect & vp){
    vp.set(__viewport.x, __viewport.y, __viewport.w, __viewport.h);
}

void GUI_SetClipRect(GUI_Rect rect){
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    canvas->restoreToCount(0);
    canvas->save();
    canvas->clipRect(rect.toSkRect(), SkClipOp::kIntersect);
    __cliprect.set(rect.x, rect.y, rect.w, rect.h);
}

void GUI_GetClipRect(GUI_Rect & rect){
    rect.set(__cliprect.x, __cliprect.y, __cliprect.w, __cliprect.h);
}
