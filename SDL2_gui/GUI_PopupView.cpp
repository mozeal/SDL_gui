//
//  GUI_PopupView.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 23/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_PopupView.h"

GUI_PopupView *GUI_PopupView::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                     std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_PopupView( parent, title, x, y, width, height, userEventHandler );
}

GUI_PopupView::GUI_PopupView(GUI_View *parent, const char *title, int x, int y, int width, int height,
                             std::function<bool(SDL_Event* ev)>userEventHandler ) :
    GUI_View(parent, title, x, y, width, height, userEventHandler )
{
    mouseReceive = true;
    setInteract( true );
    dragable = true;
    
    capture_on_click = true;
    autoHide = true;
    setAlign( GUI_ALIGN_ABSOLUTE );
}

GUI_PopupView::~GUI_PopupView() {
    
}

void GUI_PopupView::Center() {
    if( parent ) {
        topLeft.x = (parent->rectView.w - rectView.w) / 2;
        topLeft.y = (parent->rectView.h - rectView.h) / 2;
        rectView.x = parent->rectView.x + topLeft.x;
        rectView.y = parent->rectView.y + topLeft.y;
    }
    else {
        topLeft.x = (GUI_windowWidth - rectView.w) / 2;
        topLeft.y = (GUI_windowHeight - rectView.h) / 2;
        rectView.x = 0 + topLeft.x;
        rectView.y = 0 + topLeft.y;
    }
}

bool GUI_PopupView::eventHandler(SDL_Event*event) {
    switch (event->type) {
            
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
        {
            if( isVisible() ) {
                SDL_TouchFingerEvent e = event->tfinger;
                
                int x = (int)(e.x*GUI_windowWidth*GUI_mouseScale);
                int y = (int)(e.y*GUI_windowHeight*GUI_mouseScale);
                if( !hitTest(x, y, false) ) {
                    if( autoHide ) {
                        hide();
                    }
                }
            }
            return GUI_View::eventHandler(event);
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            
            if( isVisible() ) {
                SDL_MouseButtonEvent e = event->button;
                
                int x = (int)(e.x*GUI_mouseScale);
                int y = (int)(e.y*GUI_mouseScale);
                if( !hitTest(x, y, false) ) {
                    if( autoHide ) {
                        hide();
                    }
                }
            }
            return GUI_View::eventHandler(event);
        }
            
        default:
        {
            return GUI_View::eventHandler(event);
        }
    }
    
    return false;
}
