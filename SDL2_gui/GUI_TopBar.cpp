//
//  GUI_TopBar.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_TopBar.h"

GUI_TopBar *GUI_TopBar::create( GUI_View *parent, const char *title,
                               std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_TopBar( parent, title, callbackFunction );
}

GUI_TopBar::GUI_TopBar(GUI_View *parent, const char *title,
                       std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, 0, 0, -1, GUI_AppTopBarHeight ),
titleView(NULL)
{
    setCallback(callbackFunction);
    setBackgroundColor(cCornFlowerBlue);
    setLayout( GUI_LAYOUT_HORIZONTAL );
    border = 0;
    setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    
    if( title ) {
        titleView = GUI_Label::create(this, title, 0, 0, 0, 0 );
        titleView->setBackgroundColor( cClear );
        titleView->setTextColor(cWhite);
        titleView->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_VCENTER );
    }
}

GUI_TopBar::~GUI_TopBar() {
    
}
