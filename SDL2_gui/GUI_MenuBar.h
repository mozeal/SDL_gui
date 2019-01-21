//
//  GUI_MenuBar.hpp
//  GUI_App
//
//  Created by Panutat Tejasen on 21/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_MenuBar_hpp
#define GUI_MenuBar_hpp

#include <stdio.h>

#include <stdio.h>
#include "GUI_View.h"
#include "GUI_Label.h"
#include "GUI_Menu.h"

class GUI_MenuBarItem : public GUI_View {
protected:

public:
    static GUI_MenuBarItem *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                                std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_MenuBarItem(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                 std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_MenuBarItem();
};

class GUI_MenuBar : public GUI_View {
protected:
    std::vector<GUI_MenuBarItem *>menuItems;
public:
    static GUI_MenuBar *create( GUI_View *parent, const char *title,
                              std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_MenuBar(GUI_View *parent, const char *title,
               std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_MenuBar();
    
    virtual void add(GUI_MenuBarItem* child);
    virtual void remove(GUI_MenuBarItem* child);
    
    virtual void addSimpleMenu( const char *title );
};



#endif /* GUI_MenuBar_hpp */
