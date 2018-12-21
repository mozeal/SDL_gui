//
//  GUI_View.hpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_View_hpp
#define GUI_View_hpp

#include <string>
#include <stdio.h>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include "GUI_Utils.h"

class GUI_View {
protected:
    std::function<bool(SDL_Event* ev)>user_events_handler;
    
    virtual void predraw();
    virtual void draw();
    virtual void postdraw();
public:
    static GUI_View *createView( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    
    GUI_View( GUI_View *parent, const char *title, int x, int y, int width, int height,
             std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    virtual bool eventHandler(SDL_Event*ev);

    std::string title;
    GUI_Rect rectView;   // relative to top window
    GUI_Point topLeft;  // relative to parent window
    
    GUI_View **children;
    GUI_View *parent;
};

#endif /* GUI_View_hpp */
