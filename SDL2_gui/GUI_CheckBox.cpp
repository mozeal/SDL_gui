//
//  GUI_CheckBox.cpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_CheckBox.h"
#include "GUI_Utils.h"
#include "GUI_shapes.h"


GUI_CheckBox *GUI_CheckBox::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                          std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_CheckBox( parent, title, x, y, width, height, callbackFunction );
}

GUI_CheckBox::GUI_CheckBox(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction) :
GUI_Button(parent, title, NULL, x, y, width, height, NULL ),
checked(false)
{
    clickable = false;
    capture_on_click = false;
    callback_on_mouse_up = true;
    
    checkBoxCallback = callbackFunction;
    
    int cSize = ((int)(textView->rectView.h/GUI_scale)) & ~1;
    
    checkView = GUI_View::create(this, "Check", 0, 0, cSize, cSize);
    checkView->setBorder( 1 );
    checkView->toBack();
    checkView->setCorner( 2 );
    checkView->setBackgroundColor(cClear);
    checkView->setMargin( 0, 6, 0, 0);
    checkView->setLayout(GUI_LAYOUT_ABSOLUTE);
    
    iconView = GUI_IconView::create(checkView, kIcon_solid_check, GUI_UIIconFontName.c_str(), GUI_UIIconFontSize, 0, 0, 0, 0);
    iconView->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_VCENTER);
    iconView->setBorder( 0 );
    iconView->setBackgroundColor( cClear );
    iconView->setColor( cBlack );
    iconView->setVisible(checked);

    
    setContentAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    setBackgroundColor( cClear );
    setBorder( 0 );
    setPadding( 5, 0, 5, 0 );
    updateLayout();
    
    setCheck(checked);
    
    this->setCallback([](GUI_View *v) {
        GUI_CheckBox *cb = (GUI_CheckBox *)v;
        
        cb->setCheck(!cb->isChecked());
    });
}

GUI_CheckBox::~GUI_CheckBox() {
    
}

void GUI_CheckBox::setCheck( bool c ) {
    checked = c;
    iconView->setVisible(checked);
    checkView->updateLayout();
    if( checkBoxCallback ) {
        checkBoxCallback(this);
    }
}

void GUI_CheckBox::setContentAlign( int a ) {
    GUI_View::setContentAlign(a);
    if( checkView ) {
        checkView->setAlign( a );
    }
    if( textView ) {
        textView->setAlign( a );
    }
}

void GUI_CheckBox::drawInteract() {
    if( showInteract && getInteract() ) {
        GUI_Rect *rect = GUI_MakeRect(checkView->topLeft.x, checkView->topLeft.y, checkView->rectView.w, checkView->rectView.h);
        
        GUI_FillRoundRect(rect->x, rect->y, rect->w, rect->h, checkView->getCorner(), cHightLightInteract);
    }
}
