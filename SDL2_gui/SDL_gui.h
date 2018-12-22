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
#include "GUI_Utils.h"
#include "GUI_View.h"
#include "SDL2_gfxPrimitives.h"

int GUI_Init( SDL_Window *window, SDL_Renderer *renderer );
void GUI_Run(std::function<bool(SDL_Event *ev)>handle_ev = NULL);
void GUI_Destroy();

extern SDL_Renderer *GUI_renderer;
extern SDL_Window *GUI_window;
extern GUI_View *GUI_topView;
extern GUI_View * GUI_mouseCapturedView;

extern int GUI_physicalWindowWidth;
extern int GUI_physicalWindowHeight;

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern float GUI_scale;
extern float GUI_mouseScale;

extern void GUI_updateScaleParameters();

GUI_View *GUI_createTopView(const char* t, int x, int y, int w, int h,
                            std::function<bool(SDL_Event* ev)>userEventHandler = NULL);

#endif /* SDL_gui_hpp */
