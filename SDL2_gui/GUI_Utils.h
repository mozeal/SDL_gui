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

const Uint32 GUI_EventID = SDL_USEREVENT;
const Uint32 GUI_EventUpdate = GUI_EventID + 1;
const Uint32 GUI_EventPaint  = GUI_EventID + 2;
const Uint32 GUI_EventUser = GUI_EventID + 300;

extern SDL_Renderer *GUI_renderer;

struct GUI_Rect :SDL_Rect {
    GUI_Rect();
    GUI_Rect(Sint16 x, Sint16 y, Uint16 dx, Uint16 dy);
    void set(Sint16 x, Sint16 y, Uint16 dx, Uint16 dy);
};
GUI_Rect *GUI_MakeRect(int x, int y, int w, int h);

struct GUI_Point {
    short x, y;
    GUI_Point();
    GUI_Point(short x, short y);
    void set(short x, short y);
    bool operator==(GUI_Point b);
    bool operator!=(GUI_Point b);
};

SDL_Color sdl_color(Uint32 c);

extern SDL_Color cClear;
extern SDL_Color cNop;
extern SDL_Color cWhite;
extern SDL_Color cBlack;
extern SDL_Color cGrey;
extern SDL_Color cRed;
extern SDL_Color cGreen;
extern SDL_Color cBlue;
extern SDL_Color cCyan;
extern SDL_Color cMagenta;
extern SDL_Color cYellow;
extern SDL_Color cDarkGrey;
extern SDL_Color cLightGrey;
extern SDL_Color cCornFlowerBlue;
extern SDL_Color cHightLightSelected;
extern SDL_Color cHightLightInteract;
extern SDL_Color cTextSelected;
extern SDL_Color cHightLightSelection;
extern SDL_Color cEmptyContent;

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

extern float GUI_scale;
extern float GUI_mouseScale;

#endif /* GUI_Utils_hpp */
