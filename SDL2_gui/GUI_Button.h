//
//  GUI_Button.hpp
//  GUI_Button
//
//  Created by Panutat Tejasen on 13/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_Button_hpp
#define GUI_Button_hpp

#include <stdio.h>
#include "GUI_Label.h"

class GUI_Button : public GUI_Label {
protected:
    bool isDown;
    Uint32 touchTime, touchHoldTime;
public:
    static GUI_Button *create( GUI_View *parent, const char *title, uint16_t unicode=0, int x=0, int y=0, int width=0, int height=0,
                                std::function<void(GUI_Button*)>callbackFunction = NULL );
    GUI_Button(GUI_View *parent, const char *title, uint16_t unicode=0, int x=0, int y=0, int width=0, int height=0,
               std::function<void(GUI_Button*)>callbackFunction = NULL);
    virtual ~GUI_Button();
    
    std::function<void(GUI_Button*)>callback;
    
    virtual bool eventHandler(SDL_Event*ev);
};


#endif /* GUI_Button_hpp */
