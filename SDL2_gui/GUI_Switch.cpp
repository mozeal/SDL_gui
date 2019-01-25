//
//  GUI_Switch.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 25/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Switch.h"

GUI_Switch *GUI_Switch::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                       std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_Switch( parent, title, x, y, width, height, callbackFunction );
}

GUI_Switch::GUI_Switch(GUI_View *parent, const char *title, int x, int y, int width, int height,
                               std::function<void(GUI_View*)>callbackFunction ) :
    GUI_View(parent, title, x, y, width, height ),
    indicator(NULL),
    status(0),
    sw_callback(NULL)
{
    sw_callback = callbackFunction;
    
    callback_on_mouse_up = true;
    mouseReceive = true;
    setBackgroundColor( GUI_AppTopBarColor );
    
    border = 0;
    
    int w = width;
    int h = width;
    if( w == 0 || h == 0 ) {
        w = 24;
        h = 24;
    }
    
    indicator = GUI_View::create( this, "Ind", 0, 0, w, h );
    indicator->corner = h / 2;
    
    auto cover = GUI_View::create( this, "cover", 0, 0, w*1.8, h );
    cover->setBackgroundColor( cClear );
    cover->border = 1;
    cover->corner = h / 2;
    cover->mouseReceive = false;
    
    corner = h/2;
    
    setCallback([=](GUI_View *v) {
        GUI_Switch *sw = (GUI_Switch *)v;

        status = !status;
        if( sw_callback ) {
            sw_callback(this);
        }
    });
}

GUI_Switch::~GUI_Switch() {
    
}

void GUI_Switch::update() {
    if( status ) {
        indicator->setAlign( GUI_ALIGN_TOP | GUI_ALIGN_RIGHT );
        setBackgroundColor( GUI_AppTopBarColor );
    }
    else {
        indicator->setAlign( GUI_ALIGN_TOP | GUI_ALIGN_LEFT );
        setBackgroundColor( cGrey );
    }
}

