//
//  GUI_Config.cpp
//  iOS
//
//  Created by Panutat Tejasen on 29/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Config.h"
#include "GUI_Utils.h"

std::string GUI_GetUITextFontName() {
    return "Kanit-Light.ttf";
}

int GUI_GetUITextFontSize() {
    if( GUI_scale >= 3 ) {
        return 18;
    }
    else {
        return 15;
    }
}

std::string GUI_GetUIIconFontName() {
    return "fa-solid-900.ttf";
}

int GUI_GetUIIconFontSize() {
    if( GUI_scale >= 3 ) {
        return 18;
    }
    else {
        return 15;
    }
}
