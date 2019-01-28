//
//  GUI_RadioButton.cpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_RadioButton.h"
#include "GUI_Utils.h"
#include "GUI_shapes.h"

GUI_RadioButton *GUI_RadioButton::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                   std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_RadioButton( parent, title, x, y, width, height, callbackFunction );
}

GUI_RadioButton::GUI_RadioButton(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction) :
GUI_CheckBox(parent, title, x, y, width, height, callbackFunction )
{
    iconView->setIcon(kIcon_solid_circle);
    int cSize = ((int)(textView->rectView.h)) & ~1;
    checkView->setCorner( cSize / 2 );
}

GUI_RadioButton::~GUI_RadioButton() {
    
}

void GUI_RadioButton::drawInteract() {
    if( showInteract && getInteract() ) {
        GUI_Rect *rect = GUI_MakeRect(checkView->topLeft.x, checkView->topLeft.y, checkView->rectView.w, checkView->rectView.h);
        
        //GUI_FillRoundRect(rect->x, rect->y, rect->w, rect->h, checkView->corner, cHightLightInteract);
        int r = rect->w / 2;
        GUI_FillCircle(rect->x+r, rect->y+r, r, cHightLightInteract);
    }
}
