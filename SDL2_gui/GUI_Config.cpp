//
//  GUI_Config.cpp
//  iOS
//
//  Created by Panutat Tejasen on 29/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Config.h"
#include "GUI_Utils.h"
#include "SDL_gui.h"

static const int GUI_AppTopBarHeight = 40;
static const int GUI_UITextFontSize = 15;
static const int GUI_UIIconFontSize = 15;
static const int GUI_AppMenuWidth = 200;

std::string GUI_GetUITextFontName() {
    return std::string("Kanit-Light.ttf");
}

int GUI_GetUITextFontSize() {
    int ww, hh;

    if( GUI_windowWidth < GUI_windowHeight ) {
        ww = GUI_physicalWindowWidth;
        hh = GUI_physicalWindowHeight;
    }
    else {
        ww = GUI_physicalWindowHeight;
        hh = GUI_physicalWindowWidth;
    }
    if( ww < 1024 || hh < 1920 ) {
        if( !SDL_IsTablet() )
            return GUI_UITextFontSize * 1.8;
    }

    if( GUI_scale >= 3 ) {
        return 18;
    }
    else {
        return GUI_UITextFontSize;
    }
}

std::string GUI_GetUIIconFontName() {
    return "fa-solid-900.ttf";
}

int GUI_GetUIIconFontSize() {
    int ww, hh;

    if( GUI_windowWidth < GUI_windowHeight ) {
        ww = GUI_physicalWindowWidth;
        hh = GUI_physicalWindowHeight;
    }
    else {
        ww = GUI_physicalWindowHeight;
        hh = GUI_physicalWindowWidth;
    }
    if( ww < 1024 || hh < 1920 ) {
        if( !SDL_IsTablet() )
            return GUI_UIIconFontSize * 1.8;
    }

    if( GUI_scale >= 3 ) {
        return 18;
    }
    else {
        return GUI_UIIconFontSize;
    }
}

int GUI_GetAppTopBarHeight() {
    int ww, hh;

    if( GUI_windowWidth < GUI_windowHeight ) {
        ww = GUI_physicalWindowWidth;
        hh = GUI_physicalWindowHeight;
    }
    else {
        ww = GUI_physicalWindowHeight;
        hh = GUI_physicalWindowWidth;
    }
    if( ww < 1024 || hh < 1920 ) {
        if( !SDL_IsTablet() )
            return GUI_AppTopBarHeight * 1.8;
    }
    return GUI_AppTopBarHeight;
}

int GUI_GetAppMenuWidth() {
    int ww, hh;

    if( GUI_windowWidth < GUI_windowHeight ) {
        ww = GUI_physicalWindowWidth;
        hh = GUI_physicalWindowHeight;
    }
    else {
        ww = GUI_physicalWindowHeight;
        hh = GUI_physicalWindowWidth;
    }
    if( ww < 1024 || hh < 1920 ) {
        if( !SDL_IsTablet() )
            return GUI_AppMenuWidth * 1.8;
    }
    return GUI_AppMenuWidth;
}
