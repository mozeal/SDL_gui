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
#if defined(__IPHONEOS__)
    GUI_View( parent, title, 0, 0, -1, GUI_AppTopBarHeight+getiOSStatusBarHeight() ),
#else
    GUI_View( parent, title, 0, 0, -1, GUI_AppTopBarHeight ),
#endif
titleView(NULL)
{
#if defined(__IPHONEOS__)
    setPadding( getiOSStatusBarHeight()/GUI_scale, 0, 0, 0 );
#endif
    setCallback(callbackFunction);
    setBackgroundColor(GUI_AppTopBarColor);
    setLayout( GUI_LAYOUT_VERTICAL );
    setBorder( 0 );
    setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    
    contentView = GUI_View::create(this, "TopBarContent", 0, 0, -1, GUI_AppTopBarHeight);
    contentView->setAlign(GUI_ALIGN_LEFT|GUI_ALIGN_BOTTOM);
    contentView->setBorder( 0 );
    contentView->setBackgroundColor( cClear );
    
    if( title ) {
        titleView = GUI_Label::create(contentView, title, 0, 0, 0, 0 );
        titleView->setBackgroundColor( cClear );
        titleView->setTextColor(cWhite);
        titleView->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_VCENTER );
    }
}

GUI_TopBar::~GUI_TopBar() {
    
}
