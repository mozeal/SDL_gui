//
//  GUI_ComboBox.hpp
//  GUI_App
//
//  Created by Panutat Tejasen on 25/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_ComboBox_hpp
#define GUI_ComboBox_hpp

#include <stdio.h>
#include "GUI_View.h"
#include "GUI_EditText.h"
#include "GUI_Button.h"
#include "GUI_PopupView.h"
#include "GUI_Menu.h"
#include "GUI_MenuBar.h"
#include "GUI_ScrollView.h"

class GUI_ComboBox : public GUI_View {
protected:
    GUI_EditText *editText;
    GUI_Button *button;
    GUI_PopupMenu *popup;
    std::function<void(GUI_View*)>cb_callback;
public:
    static GUI_ComboBox *create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                              std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_ComboBox(GUI_View *parent, const char *title, int x, int y, int width, int height,
               std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_ComboBox();
    
    bool editable;
    GUI_MenuItem *selectedItem;
    
    virtual void addSimpleItem( const char *title, bool separator=false );
};

#endif /* GUI_ComboBox_hpp */
