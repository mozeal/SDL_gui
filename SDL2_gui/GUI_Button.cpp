//
//  GUI_Button.cpp
//  GUI_Button
//
//  Created by Panutat Tejasen on 13/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Button.h"
#include "GUI_shapes.h"

GUI_Button *GUI_Button::create( GUI_View *parent, const char *title, uint16_t unicode, int x, int y, int width, int height,
                          std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_Button( parent, title, unicode, x, y, width, height, callbackFunction );
}

GUI_Button::GUI_Button(GUI_View *parent, const char *title, uint16_t unicode, int x, int y, int width, int height,
                       std::function<void(GUI_View*)>callbackFunction) :
GUI_Label(parent, title, unicode, x, y, width, height, NULL )
{
    dragable = false;
    clickable = true;
    focusable = true;
    showInteract = true;
    mouseReceive = true;
    
    setBackgroundColor(cWhite);
    callback = callbackFunction;
    
    border = 1;
    focusBorder = 8;
    corner = 4;
    if( title ) {
        setPadding( 5, 12, 5, 12 );
    }
    else {
        setPadding( 5, 10, 5, 10 );
    }
    updateLayout();
}

GUI_Button::~GUI_Button() {
    
}

void GUI_Button::setEnable(bool e) {
    _enable = e;
    if( _enable ) {
        textView->setTextColor(cBlack);
        iconView->setTextColor(cBlack);
        setBorderColor(cBlack);
    } else {
        textView->setTextColor(cGrey);
        iconView->setTextColor(cGrey);
        setBorderColor(cGrey);
    }
};

