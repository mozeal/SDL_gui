//
//  GUI_Utils.cpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifdef __MACOSX__
#include <SDL_ttf.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#endif
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

#define LOGNAME "SDL_gui"

GUI_Rect::GUI_Rect() {
    x = y = w = h = 0;
}

GUI_Rect::GUI_Rect(Sint16 _x, Sint16 _y, Uint16 _dx, Uint16 _dy) {
    x = _x; y = _y; w = _dx; h = _dy;
}

void GUI_Rect::set(Sint16 _x, Sint16 _y, Uint16 _dx, Uint16 _dy) {
    x = _x; y = _y; w = _dx; h = _dy;
}

GUI_Rect *GUI_MakeRect(int x, int y, int w, int h) { // creates pointer to Rect. Each source file gets its own instance.
    static GUI_Rect rp_rect;
    rp_rect.set(x, y, w, h);
    return &rp_rect;
}

GUI_Point::GUI_Point(short x1, short y1) :x(x1), y(y1) {
}

GUI_Point::GUI_Point() : x(0), y(0) {
}

void GUI_Point::set(short x1, short y1) {
    x = x1; y = y1;
}

bool GUI_Point::operator==(GUI_Point b) {
    return x == b.x && y == b.y;
}

bool GUI_Point::operator!=(GUI_Point b) {
    return x != b.x || y != b.y;
}

SDL_Color sdl_color(Uint32 c) {
    SDL_Color sdl_c;
    sdl_c.r = c >> 24 & 0xff;
    sdl_c.g = c >> 16 & 0xff;
    sdl_c.b = c >> 8 & 0xff;
    sdl_c.a = c & 0xff;
    return sdl_c;
}

SDL_Color
cClear = {0, 0, 0, 0},
cNop = {0, 0, 0, 1},
cWhite = sdl_color(0xffffffff),
cBlack = sdl_color(0xff),
cGrey = sdl_color(0xc0c0c0ff),
cRed = sdl_color(0xff0000ff),
cGreen = sdl_color(0x00ff00ff),
cBlue = sdl_color(0xffff),
cCyan = sdl_color(0xffffff),
cMagenta = sdl_color(0xff00ffff),
cYellow = sdl_color(0xffff00ff),
cDarkGrey = sdl_color(0x707070ff),
cLightGrey = sdl_color(0xe0e0e0ff),
cCornFlowerBlue = sdl_color(0x6495edff);


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
    std::string imagePath;

    if (filename[0] != '/')
        imagePath = GUI_GetResourcePath() + filename;
    else
        imagePath = "" + filename;

    SDL_Texture *tex = IMG_LoadTexture(ren, imagePath.c_str());

    if (tex == NULL) {
        GUI_Log("Load image %s failed.", filename.c_str());
        imagePath = GUI_GetCurrentPath() + PATH_SEPARATOR "data" + PATH_SEPARATOR + filename;
        tex = IMG_LoadTexture(ren, imagePath.c_str());
    }

    return tex;
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
        IMG_SavePNG(surface, fileName.c_str());
        break;
    case eJpg:
        IMG_SaveJPG(surface, fileName.c_str(), 100);
        break;
    case eRaw:
        std::vector<char> lVector;
        lVector.resize(surface->pitch * h);
        for(int i(0); i < surface->pitch * h; ++i)
            lVector[i] = ((char*) surface->pixels)[i];
        std::fstream s(fileName, s.binary | s.trunc | s.in | s.out);
        if(!s.is_open())
            SDL_LogError( 0, "Open file failed");
        s.write(lVector.data(), lVector.size());
        break;
    }

    SDL_FreeSurface(surface);
}
