//
//  GUI_Utils.cpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Utils.h"

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
cCornFlower = sdl_color(0x6495edff),
cHightLightSelected = sdl_color(0x338dfbff),
cTextSelected = sdl_color(0x9cc7e6ff); // Tooh - text selection


static GUI_LogLevel currentLogLevel = GUI_LOG_NOTICE;

void GUI_Log(const char * format, ...) {
#ifdef __ANDROID__
    char buffer[512];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    
    __android_log_print(ANDROID_LOG_VERBOSE, LOGNAME, buffer, 1);
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
