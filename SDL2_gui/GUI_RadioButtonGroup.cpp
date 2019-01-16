//
//  GUI_RadioButtonGroup.cpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_RadioButtonGroup.h"

GUI_RadioButtonGroup *GUI_RadioButtonGroup::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                    std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_RadioButtonGroup( parent, title, x, y, width, height, callbackFunction );
}

GUI_RadioButtonGroup::GUI_RadioButtonGroup(GUI_View *parent, const char *title, int x, int y, int width, int height,
                                           std::function<void(GUI_View*)>callbackFunction ) :
GUI_View(parent, title, x, y, width, height),
selectedRadioButton(NULL)
{
    setCallback( callbackFunction );
    border = 0;
}

GUI_RadioButtonGroup::~GUI_RadioButtonGroup() {
    
}

void GUI_RadioButtonGroup::add(GUI_RadioButton* child) {
    if( child->parent ) {
        child->parent->remove_child( child );
    }
    add_child(child);
    radioButtonChildren.push_back(child);
    child->setCallback( [=](GUI_View *v) {
        GUI_RadioButton *rb = (GUI_RadioButton *)v;
        for (std::vector<GUI_RadioButton *>::iterator it = radioButtonChildren.begin() ; it != radioButtonChildren.end(); ++it) {
            GUI_RadioButton *c = *it;
            if( rb == c ) {
                c->setCheck(true);
                this->selectedRadioButton = c;
                if( this->callback ) {
                    this->callback(this);
                }
            }
            else {
                c->setCheck(false);
            }
        }
    });
}

void GUI_RadioButtonGroup::remove(GUI_RadioButton* child) {
    remove_child(child);
    for (std::vector<GUI_RadioButton *>::iterator it = radioButtonChildren.begin() ; it != radioButtonChildren.end(); ++it) {
        if( child == *it ) {
            radioButtonChildren.erase( it );
            break;
        }
    }
    child->setCallback(NULL);
}
