//
//  GUI_TopBar.hpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_TopBar_hpp
#define GUI_TopBar_hpp

#include <stdio.h>
#include "GUI_View.h"
#include "GUI_Label.h"

class GUI_TopBar : public GUI_View {
protected:
    GUI_Label *titleView;
public:
    static GUI_TopBar *create( GUI_View *parent, const char *title,
                                std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_TopBar(GUI_View *parent, const char *title,
                 std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_TopBar();
    GUI_View *contentView;
};

#endif /* GUI_TopBar_hpp */
