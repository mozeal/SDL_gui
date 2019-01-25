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
    GUI_View(parent, title, x, y, width, height, userEventHandler ),
    activateView(NULL)
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
    int destX;
    int destY;
    if( parent ) {
        destX = (parent->rectView.w - rectView.w) / 2;
        destY = (parent->rectView.h - rectView.h) / 2;
    }
    else {
        destX = (GUI_windowWidth - rectView.w) / 2;
        destY = (GUI_windowHeight - rectView.h) / 2;
    }
    destX /= GUI_scale;
    destY /= GUI_scale;
    
    move( destX - topLeft.x / GUI_scale, destY - topLeft.y / GUI_scale );
}

bool GUI_PopupView::eventHandler(SDL_Event*event) {
    switch (event->type) {
            
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
        {
            SDL_TouchFingerEvent e = event->tfinger;
            
            int x = (int)(e.x*GUI_windowWidth*GUI_mouseScale);
            int y = (int)(e.y*GUI_windowHeight*GUI_mouseScale);
            
            if( activateView ) {
                if( activateView->hitTest(x, y, false) ) {
                    return true;
                }
            }
            if( isVisible() ) {
                
                if( !hitTest(x, y, false) ) {
                    if( autoHide ) {
                        GUI_SetMouseCapture( NULL );
                        hide();
                    }
                }
            }
            return GUI_View::eventHandler(event);
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
            SDL_MouseButtonEvent e = event->button;
            
            int x = (int)(e.x*GUI_mouseScale);
            int y = (int)(e.y*GUI_mouseScale);
            
            if( activateView ) {
                if( activateView->hitTest(x, y, false) ) {
                    return true;
                }
            }
            if( isVisible() ) {

                if( !hitTest(x, y, false) ) {
                    if( autoHide ) {
                        GUI_SetMouseCapture( NULL );
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

GUI_DialogView *GUI_DialogView::create( GUI_View *parent, const char *title, int x, int y, int width, int height, int options ) {
    return new GUI_DialogView( parent, title, x, y, width, height, options );
}

GUI_DialogView::GUI_DialogView(GUI_View *parent, const char *title, int x, int y, int width, int height, int options ) :
    GUI_PopupView(parent, title, x, y, width, height ),
    topBar(NULL),
    contentView(NULL)
{
    setLayout(GUI_LAYOUT_VERTICAL);
    topBar = GUI_TopBar::create(this, title );
    topBar->mouseReceive = false;
    
    contentView = GUI_View::create( this, "Content", 0, 0, -1, 0 );
    contentView->setBackgroundColor(cWhite);
    contentView->mouseReceive = true;
    contentView->setLayout(GUI_LAYOUT_VERTICAL);
    contentView->setPadding( 20, 10, 10, 10 );
    
    this->updateLayout();
}

GUI_DialogView::~GUI_DialogView() {
    
}
