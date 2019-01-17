//
//  GUI_EditText.hpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 17/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_EditText_hpp
#define GUI_EditText_hpp

#include <stdio.h>
#include "GUI_TextView.h"

class GUI_EditText : public GUI_TextView {
protected:
    virtual void draw();
    int textEditIndex;
public:
    static GUI_EditText *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                                std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_EditText(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                 std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_EditText();
    
    virtual bool eventHandler(SDL_Event*event);
};

#endif /* GUI_EditText_hpp */
