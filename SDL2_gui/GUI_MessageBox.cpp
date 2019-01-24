//
//  GUI_MessageBox.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 24/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_MessageBox.h"
#include "GUI_Button.h"

GUI_MessageBox *GUI_MessageBox::create( GUI_View *parent, const char *title, const char *message, int x, int y, int width, int height,
                                       std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_MessageBox( parent, title, message, x, y, width, height, callbackFunction );
}

GUI_MessageBox::GUI_MessageBox(GUI_View *parent, const char *title, const char *message, int x, int y, int width, int height,
                               std::function<void(GUI_View*)>callbackFunction ) :
    GUI_DialogView(parent, title, x, y, width, height ) {
        setCallback( callbackFunction );
        
        auto text = GUI_Label::create( this->contentView, message );
        text->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
        text->setMargin(10, 10, 10, 10);
        
        auto ok = GUI_Button::create( this->contentView, "OK", 0, 0, 0, 80, 0 );
        ok->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
        ok->setMargin(10, 10, 10, 10);
        ok->setCallback([=](GUI_View *v) {
            GUI_View::closeQueue.push_back(this);
        });
}

GUI_MessageBox::~GUI_MessageBox() {
    
}
