//
//  GUI_PopupView.hpp
//  GUI_App
//
//  Created by Panutat Tejasen on 23/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_PopupView_hpp
#define GUI_PopupView_hpp

#include <stdio.h>
#include "GUI_View.h"
#include "GUI_Label.h"
#include "GUI_TopBar.h"

class GUI_PopupView : public GUI_View {
protected:
    bool        autoHide;
    GUI_View    *activateView;
public:
    static GUI_PopupView *create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                 std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    GUI_PopupView(GUI_View *parent, const char *title, int x, int y, int width, int height,
                  std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    virtual ~GUI_PopupView();
    
    virtual bool eventHandler(SDL_Event*ev);
    
    virtual void Center();
    
    void setActivateView( GUI_View *v ) { activateView = v; }
};

class GUI_DialogView : public GUI_PopupView {
protected:
    GUI_TopBar *topBar;
    GUI_View *contentView;
    
public:
    static GUI_DialogView *create( GUI_View *parent, const char *title, int x, int y, int width, int height, int options=0 );
    GUI_DialogView(GUI_View *parent, const char *title, int x, int y, int width, int height, int options=0 );
    virtual ~GUI_DialogView();
};

#endif /* GUI_PopupView_hpp */
