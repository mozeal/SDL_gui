//
//  GUI_View.cpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_View.h"
#include "GUI_Utils.h"

extern int GUI_physicalWindowWidth;
extern int GUI_physicalWindowHeight;

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern float GUI_scale;

GUI_View *GUI_View::createView( GUI_View *parent, const char *title, int x, int y, int width, int height,
                               std::function<bool(SDL_Event* ev)>userEventHandler) {
    GUI_View *view = new GUI_View( parent, title, x, y, width, height, userEventHandler );
    return view;
}

GUI_View::GUI_View( GUI_View *p, const char *t, int x, int y, int w, int h,
                   std::function<bool(SDL_Event* ev)>userEventHandler):
parent(p),
children(0),
topLeft(x * GUI_scale, y * GUI_scale),
rectView(x * GUI_scale, y * GUI_scale, w == -1 ? w : w * GUI_scale, h == -1 ? h : h * GUI_scale)
{
    if (t) {
        title = std::string(t);
    }
}

bool GUI_View::eventHandler(SDL_Event*event) {
    if (user_events_handler) {
        if (user_events_handler(event))
            return true;;
    }
    switch( event->type ) {
        case GUI_EventPaint:
            GUI_Log( "Paint" );
            break;
        default:
            break;
    }
    
    return false;
}

void GUI_View::predraw() {
    
}

void GUI_View::draw() {
    
}

void GUI_View::postdraw() {
    
}
