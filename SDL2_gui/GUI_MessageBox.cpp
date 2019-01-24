//
//  GUI_MessageBox.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 24/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_MessageBox.h"
#include "GUI_Button.h"
#include "GUI_EditText.h"

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
            if( callback ) {
                callback(this);
            }
            this->close();
        });
}

GUI_MessageBox::~GUI_MessageBox() {
    
}

// ----------------------------------------------------------------------------------------------------------

GUI_OptionBox *GUI_OptionBox::create( GUI_View *parent, const char *title, const char *message, int x, int y, int width, int height,
                                       std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_OptionBox( parent, title, message, x, y, width, height, callbackFunction );
}

GUI_OptionBox::GUI_OptionBox(GUI_View *parent, const char *title, const char *message, int x, int y, int width, int height,
                               std::function<void(GUI_View*)>callbackFunction ) :
GUI_DialogView(parent, title, x, y, width, height ),
result(0)
{
    setCallback( callbackFunction );
    
    auto text = GUI_Label::create( this->contentView, message );
    text->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
    text->setMargin(10, 10, 10, 10);
    
    auto btns = GUI_View::create( this->contentView, "btns", 0, 0, -1, 0 );
    btns->setLayout( GUI_LAYOUT_HORIZONTAL );
    btns->border = 0;
    
    auto yes = GUI_Button::create( btns, "Yes", 0, 0, 0, 70, 0 );
    yes->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
    yes->setMargin(10, 10, 10, 10);
    yes->setCallback([=](GUI_View *v) {
        this->result = 1;
        if( callback ) {
            callback(this);
        }
        this->close();
    });

    auto no = GUI_Button::create( btns, "No", 0, 0, 0, 70, 0 );
    no->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
    no->setMargin(10, 0, 10, 0);
    no->setCallback([=](GUI_View *v) {
        this->result = 2;
        if( callback ) {
            callback(this);
        }
        this->close();
    });
    
    auto cancel = GUI_Button::create( btns, "Cancel", 0, 0, 0, 70, 0 );
    cancel->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
    cancel->setMargin(10, 10, 10, 10);
    cancel->setCallback([=](GUI_View *v) {
        this->result = 0;
        if( callback ) {
            callback(this);
        }
        this->close();
    });

}

GUI_OptionBox::~GUI_OptionBox() {
    
}


// ----------------------------------------------------------------------------------------------------------

GUI_TextInputBox *GUI_TextInputBox::create( GUI_View *parent, const char *title, const char *message, int x, int y, int width, int height,
                                     std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_TextInputBox( parent, title, message, x, y, width, height, callbackFunction );
}

GUI_TextInputBox::GUI_TextInputBox(GUI_View *parent, const char *title, const char *message, int x, int y, int width, int height,
                             std::function<void(GUI_View*)>callbackFunction ) :
GUI_DialogView(parent, title, x, y, width, height )
{
    setCallback( callbackFunction );
    
    auto ins = GUI_View::create( this->contentView, "btns", 0, 0, -1, 0 );
    ins->setLayout( GUI_LAYOUT_VERTICAL );
    ins->border = 0;
    
    auto lb = GUI_Label::create( ins, message, 0, 0, -1, 0 );
    lb->setAlign(GUI_ALIGN_LEFT|GUI_ALIGN_TOP);
    lb->setMargin(10, 10, 0, 10);
    
    textBox = GUI_EditText::create( ins, "", 0, 0, -1, 0 );
    textBox->setAlign(GUI_ALIGN_LEFT|GUI_ALIGN_TOP);
    textBox->setMargin(0, 10, 10, 10);
    textBox->setFocus();
    
    ins->updateLayout();
    
    auto btns = GUI_View::create( this->contentView, "btns", 0, 0, -1, 0 );
    btns->setLayout( GUI_LAYOUT_HORIZONTAL );
    btns->border = 0;
    
    auto ok = GUI_Button::create( btns, "OK", 0, 0, 0, 80, 0 );
    ok->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
    ok->setMargin(10, 10, 10, 10);
    ok->setCallback([=](GUI_View *v) {
        this->result = textBox->title;
        if( callback ) {
            callback(this);
        }
        this->close();
    });
    
    auto cancel = GUI_Button::create( btns, "Cancel", 0, 0, 0, 70, 0 );
    cancel->setAlign(GUI_ALIGN_CENTER|GUI_ALIGN_TOP);
    cancel->setMargin(10, 10, 10, 10);
    cancel->setCallback([=](GUI_View *v) {
        this->result = "";
        if( callback ) {
            callback(this);
        }
        this->close();
    });
    
}

GUI_TextInputBox::~GUI_TextInputBox() {
    
}
