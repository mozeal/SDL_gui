//
//  GUI_Switch.hpp
//  GUI_App
//
//  Created by Panutat Tejasen on 25/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_Switch_hpp
#define GUI_Switch_hpp

#include <stdio.h>

#include <stdio.h>
#include "GUI_View.h"

class GUI_Switch : public GUI_View {
protected:
    GUI_View *indicator;
    std::function<void(GUI_View*)>sw_callback;
public:
    static GUI_Switch *create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                 std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_Switch(GUI_View *parent, const char *title, int x, int y, int width, int height,
                  std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_Switch();
    
    virtual void update();

    int status;
};

#endif /* GUI_Switch_hpp */
