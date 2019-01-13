//
//  GUI_Label.cpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 13/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Label.h"

GUI_Label *GUI_Label::create( GUI_View *parent, const char *title, uint16_t unicode, int x, int y, int width, int height,
                             std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_Label(parent, title, unicode, x, y, width, height, userEventHandler );
}

GUI_Label::GUI_Label(GUI_View *parent, const char *title, uint16_t unicode, int x, int y, int width, int height,
                     std::function<bool(SDL_Event* ev)>userEventHandler) :
GUI_View( parent, title, x, y, width, height, userEventHandler)
{
    backgroundColor = cWhite;
    border = 0;
    dragable = false;
    layout = GUI_LAYOUT_HORIZONTAL;
    setMargin( 0, 0, 0, 0 );
    setPadding( 5, 5, 5, 5 );
    
    iconView = GUI_IconView::create(this, unicode, GUI_UIIconFontName.c_str(), GUI_UIIconFontSize);
    iconView->align = GUI_ALIGN_CENTER | GUI_ALIGN_VCENTER;
    iconView->border = 0;
    iconView->backgroundColor = cClear;
    iconView->setColor( cBlack );
    iconView->setMargin( 0, 3, 0, 0 );

    textView = GUI_TextView::create(this, title, GUI_UITextFontName.c_str(), GUI_UITextFontSize);
    textView->align = GUI_ALIGN_CENTER | GUI_ALIGN_VCENTER;
    textView->border = 0;
    textView->backgroundColor = cClear;
    textView->setColor( cBlack );
    textView->setMargin( 0, 0, 0, 3 );
}

GUI_Label::~GUI_Label() {
    
}
