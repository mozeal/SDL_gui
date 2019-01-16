//
//  GUI_RadioButton.hpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_RadioButton_hpp
#define GUI_RadioButton_hpp

#include <stdio.h>
#include "GUI_CheckBox.h"

class GUI_RadioButton : public GUI_CheckBox {
public:
    static GUI_RadioButton *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                                std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_RadioButton(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                 std::function<void(GUI_View*)>callbackFunction = NULL);
    virtual ~GUI_RadioButton();
};

#endif /* GUI_RadioButton_hpp */
