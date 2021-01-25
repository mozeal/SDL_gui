//
//  GUI_StatusBar.hpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_StatusBar_hpp
#define GUI_StatusBar_hpp

#include <stdio.h>
#include "GUI_View.h"
#include "GUI_Label.h"

class GUI_StatusBar : public GUI_View {
protected:

public:
    static GUI_StatusBar *create( GUI_View *parent, const char *title,
                              std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_StatusBar(GUI_View *parent, const char *title,
               std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_StatusBar();
    
    GUI_FPSView *fpsView;
};

#endif /* GUI_StatusBar_hpp */
