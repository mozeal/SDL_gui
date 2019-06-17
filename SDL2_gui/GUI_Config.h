//
//  GUI_Config.h
//  GUI_TextView
//
//  Created by Panutat Tejasen on 13/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_Config_h
#define GUI_Config_h

#include <string>
#include <SDL2/SDL.h>

//const std::string GUI_UIIconFontName = "fa-solid-900.ttf";

const int GUI_UIDefaultHeight = 22;

const int GUI_AppStatusBarHeight = 32;

const int GUI_AppMenuCollapseTime = 120;

std::string GUI_GetUITextFontName();
std::string GUI_GetUIIconFontName();
int GUI_GetUITextFontSize();
int GUI_GetUIIconFontSize();
int GUI_GetAppTopBarHeight();
int GUI_GetAppMenuWidth();

SDL_Color sdl_color(Uint32 c);

const SDL_Color cHightLightSelected     = { 0x33, 0x8d, 0xfb, 0xff };
const SDL_Color cHightLightInteract     = { 0x60, 0x60, 0x60, 0x18 };
const SDL_Color cTextSelected           = { 0x9c, 0xc7, 0xe6, 0xff }; // Tooh - text selection
const SDL_Color cHightLightSelection    = { 0x64, 0x95, 0xed, 0xC0 };
const SDL_Color cEmptyContent           = { 0xF0, 0xF0, 0xF0, 0xFF }; //sdl_color(0xf0f0f0ff);

const SDL_Color GUI_AppTopBarColor      = { 0x64, 0x95, 0xed, 0xff };


#endif /* GUI_Config_h */
