//
//  GUI_ComboBox.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 25/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_ComboBox.h"
#include "GUI_View.h"
#include "GUI_App.h"

GUI_ComboBox *GUI_ComboBox::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                            std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_ComboBox( parent, title, x, y, width, height, callbackFunction );
}

GUI_ComboBox::GUI_ComboBox(GUI_View *parent, const char *title, int x, int y, int width, int height,
             std::function<void(GUI_View*)>callbackFunction ) :
    GUI_View(parent, title, x, y, width, height ),
    editText(NULL),
    editable(false),
    button(NULL),
    cb_callback(NULL),
    popup(NULL)
{
    cb_callback = callbackFunction;
    
    corner = 3;
    
    setLayout( GUI_LAYOUT_HORIZONTAL );
    
    editText = GUI_EditText::create( this, " ", 0, 0, -1, 0 );
    editText->setAlign( GUI_ALIGN_TOP | GUI_ALIGN_LEFT );
    editText->border = 0;
    editText->focusBorder = 0;
    editText->setMargin( 0, 3, 0, 0 );
    editText->mouseReceive = false;
    

    
    button = GUI_Button::create( this, NULL, kIcon_solid_caret_down, 0, 0, editText->getHeight()-4, editText->getHeight()-4 );
    button->setAlign( GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT );
    button->setMargin( 2, 2, 2, 2 );
    button->border = 0;
    button->focusBorder = 0;
    button->setCallback([=](GUI_View *v) {
        GUI_Button *bt = (GUI_Button *)v;
        
        if( this->popup->isVisible() ) {
            this->popup->hide();
        }
        else {
            GUI_Point posn = this->getAbsolutePosition();
            posn.x += this->getWidth() - popup->getWidth();
            posn.y += this->getHeight();
            popup->setAbsolutePosition( posn.x, posn.y );
            
            this->popup->show();
        }
    });
    
    updateLayout();
    
    popup = GUI_PopupMenu::create( GUI_App::getInstance()->topView, "CBPopup", 0, 0, 180, 0 );
    popup->hide();
    popup->setActivateView( button );
    popup->setCallback([=](GUI_View *v) {
        GUI_PopupMenu *pm = (GUI_PopupMenu *)v;
        GUI_MenuItem *it = pm->selectedItem;
        
        selectedItem = pm->selectedItem;
        pm->hide();
        GUI_SetMouseCapture(NULL);
        
        editText->setTitle( it->title );
        if( this->callback ) {
            this->callback(this);
        }
    });

    
   
}

GUI_ComboBox::~GUI_ComboBox() {
    
}

void GUI_ComboBox::addSimpleItem( const char *title, bool separator ) {
    if( popup ) {
        popup->addSimpleMenu( title, separator );
    }
}
