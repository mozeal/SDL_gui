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
#include <SDL.h>
#include "include/core/SkColor.h"

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

SkColor sk_color(Uint32 c);

const SkColor cHightLightSelected     = SkColorSetARGB( 0xff, 0x33, 0x8d, 0xfb );
const SkColor cHightLightInteract     = SkColorSetARGB( 0x18, 0x60, 0x60, 0x60 );
const SkColor cTextSelected           = SkColorSetARGB( 0xff, 0x9c, 0xc7, 0xe6 ); // Tooh - text selection
const SkColor cHightLightSelection    = SkColorSetARGB( 0xC0, 0x64, 0x95, 0xed );
const SkColor cEmptyContent           = SkColorSetARGB( 0xFF, 0xF0, 0xF0, 0xF0 ); //sdl_color(0xf0f0f0ff);

const SkColor GUI_AppTopBarColor      = SkColorSetARGB( 0xff, 0x64, 0x95, 0xed );


#endif /* GUI_Config_h */
