//
//  GUI_ScrollView.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 26/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_ScrollView.h"

GUI_ScrollView *GUI_ScrollView::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                       std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_ScrollView(parent, title, x, y, width, height, callbackFunction );
}

GUI_ScrollView::GUI_ScrollView(GUI_View *parent, const char *title, int x, int y, int width, int height,
               std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, x, y, width, height ) {
    
}

GUI_ScrollView::~GUI_ScrollView() {
    
}
