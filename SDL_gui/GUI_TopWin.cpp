//
//  GUI_TopWin.cpp
//  OSX
//
//  Created by Panutat Tejasen on 10/16/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. Aldl rights reserved.
//

#include "GUI_TopWin.h"

GUI_TopWin *GUI_topWin = NULL;

GUI_TopWin::GUI_TopWin(const char* wm_title,GUI_Rect rect,Uint32 init_flag,void (*disp_cmd)(GUI_WinBase *)):
GUI_WinBase(NULL,NULL,rect.x,rect.y,rect.w,rect.h,cNop,disp_cmd)
{
    GUI_topWin = this;
    
    bgcol = cBackground;
}

void GUI_TopWin::draw() {
    GUI_WinBase::draw();
    
    drawChildren();
}

bool GUI_TopWin::handleEvents(SDL_Event *ev) {
    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            
            for (int i=lst_child;i>=0;--i) {      // last child first
                
                GUI_WinBase *wb=children[i]->hitTest(x, y,false); // no recursive
                if( wb ) {
                    if( wb != children[lst_child] ) {
                        if( wb->isMainWin ) {
                            remove_child( wb );
                            add_child( wb );
                        }
                    }
                    if( wb->handleEvents( ev ) )
                        return true;
                }
            }
            return false;
        }
        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            
            for (int i=lst_child;i>=0;--i) {      // last child first
                
                GUI_WinBase *wb=children[i]->hitTest(x, y,false); // no recursive
                if( wb ) {
                    if( wb != children[lst_child] ) {
                        if( wb->isMainWin ) {
                            //remove_child( wb );
                            //add_child( wb );
                        }
                    }
                    if( wb->handleEvents( ev ) )
                        return true;
                }
            }
            return false;
        }
        default: {


            for (int i=lst_child;i>=0;--i) {      // last child first
                GUI_WinBase *wb=children[i];
                if( wb ) {
                    if( wb->handleEvents( ev ) )
                        return true;
                }
            }
        }
    }
    return false;
}