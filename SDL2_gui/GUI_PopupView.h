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

class GUI_PopupView : public GUI_View {
protected:
    bool    autoHide;
public:
    static GUI_PopupView *create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                 std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    GUI_PopupView(GUI_View *parent, const char *title, int x, int y, int width, int height,
                  std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    virtual ~GUI_PopupView();
    
    virtual bool eventHandler(SDL_Event*ev);
    
    virtual void Center();
};

#endif /* GUI_PopupView_hpp */
