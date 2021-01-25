//
//  GUI_StatusBar.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_StatusBar.h"

GUI_StatusBar *GUI_StatusBar::create( GUI_View *parent, const char *title,
                               std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_StatusBar( parent, title, callbackFunction );
}

GUI_StatusBar::GUI_StatusBar(GUI_View *parent, const char *title,
                       std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, 0, 0, -1, GUI_AppStatusBarHeight )
{
    setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    setCallback(callbackFunction);
    setBackgroundColor(cLightGrey);
    setLayout( GUI_LAYOUT_HORIZONTAL );
    setBorder( 0 );

    fpsView = GUI_FPSView::create(this, 0, 0, 0, 0 );
    fpsView->setAlign( GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT );
    fpsView->setMargin(0,10, 0, 0);
}

GUI_StatusBar::~GUI_StatusBar() {
    
}
