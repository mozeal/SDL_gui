//
//  GUI_RadioButton.cpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_RadioButton.h"

GUI_RadioButton *GUI_RadioButton::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                   std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_RadioButton( parent, title, x, y, width, height, callbackFunction );
}

GUI_RadioButton::GUI_RadioButton(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction) :
GUI_CheckBox(parent, title, x, y, width, height, callbackFunction )
{
    iconView->setIcon(kIcon_solid_circle);
    checkView->corner = checkView->rectView.h / 2;
}

GUI_RadioButton::~GUI_RadioButton() {
    
}
