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

int GUI_Init( SDL_Window *window, SDL_Renderer *renderer );
void GUI_Run(std::function<bool(SDL_Event *ev)>handle_ev = NULL);
void GUI_Destroy();

#endif /* SDL_gui_hpp */
