//
//  SDL_gui.hpp
//  TestSDLimage
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef SDL_gui_hpp
#define SDL_gui_hpp

#include <SDL.h>
#include <stdio.h>
#include <iostream>
#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#include <SDL_syswm.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "GUI_shapes.h"
#include "SDL2_gfxPrimitives.h"

int GUI_Init( SDL_Window *window, SDL_Renderer *renderer );
void GUI_Run(std::function<bool(SDL_Event *ev)>handle_ev = NULL);
void GUI_Destroy();

extern SDL_Renderer *GUI_renderer;
extern SDL_Window *GUI_window;

extern int GUI_physicalWindowWidth;
extern int GUI_physicalWindowHeight;

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern float GUI_scale;

typedef enum GUI_LOGLEVEL_ {
    GUI_LOG_VERBOSE = 0,
    GUI_LOG_NOTICE,
    GUI_LOG_WARNING,
    GUI_LOG_ERROR,
    GUI_LOG_FATAL_ERROR,
    GUI_LOG_SILENT
} GUI_LogLevel;

void GUI_Error(const char* fn, int result);
void GUI_SetLogLevel(GUI_LogLevel level);
GUI_LogLevel GUI_GetLogLevel();

const Uint32 GUI_EventID = SDL_USEREVENT;
const Uint32 GUI_EventPaint = GUI_EventID + 1;
const Uint32 GUI_EventUser = GUI_EventID + 300;

SDL_Color sdl_color(Uint32 c);

extern SDL_Color
    cClear,
    cNop,
    cWhite,
    cBlack,
    cGrey,
    cRed,
    cGreen,
    cBlue,
    cCyan,
    cMagenta,
    cYellow,
    cDarkGrey,
    cLightGrey,
    cCornFlower,
    cHightLightSelected,
    cTextSelected;

#endif /* SDL_gui_hpp */
