//
//  GUI_Menu.hpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_Menu_hpp
#define GUI_Menu_hpp

#include <stdio.h>
#include "GUI_Button.h"

class GUI_MenuItem : public GUI_View {
protected:
    bool selected;

public:
    static GUI_MenuItem *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                                std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_MenuItem(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                 std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_MenuItem();
    
    virtual void setSelected( bool c );
    virtual bool isSelected() { return selected; };
    
    virtual void postdraw();
    
    bool separator;
};

class GUI_Menu : public GUI_View {
protected:
    std::vector<GUI_MenuItem *>menuItems;
public:
    static GUI_Menu *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                            std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_Menu(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
             std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_Menu();
    
    virtual void add(GUI_MenuItem* child);
    virtual void remove(GUI_MenuItem* child);
    
    virtual void addSimpleMenu( const char *title, bool separator=false );
    
    GUI_MenuItem *selectedItem;

};

#endif /* GUI_Menu_hpp */
