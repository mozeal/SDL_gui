//
//  GUI_CheckBox.hpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_CheckBox_hpp
#define GUI_CheckBox_hpp

#include <stdio.h>
#include "GUI_Button.h"

class GUI_CheckBox : public GUI_Button {
    GUI_View *checkView;
    bool checked;
    std::function<void(GUI_View*)>checkBoxCallback;
public:
    static GUI_CheckBox *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                              std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_CheckBox(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
               std::function<void(GUI_View*)>callbackFunction = NULL);
    virtual ~GUI_CheckBox();
    
    virtual void drawInteract();
    virtual void setContentAlign( int a );
    
    virtual void setCheck( bool c );
    virtual bool isChecked() { return checked; };
    
};

#endif /* GUI_CheckBox_hpp */
