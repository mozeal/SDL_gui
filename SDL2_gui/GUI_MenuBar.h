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
#include "GUI_PopupView.h"
#include "GUI_ScrollView.h"

class GUI_PopupMenu : public GUI_PopupView {
protected:
    GUI_ScrollView *scrollView;
public:
    static GUI_PopupMenu *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                            std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_PopupMenu(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
             std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_PopupMenu();
    
    virtual void add(GUI_MenuItem* child);
    virtual void remove(GUI_MenuItem* child);
    
    virtual void addSimpleMenu( const char *title, bool separator=false );
    
    GUI_MenuItem *selectedItem;
	std::vector<GUI_MenuItem *>menuItems;
};

class GUI_MenuBarItem : public GUI_View {
protected:
    GUI_PopupMenu *popupMenu;
public:
    static GUI_MenuBarItem *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                                std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_MenuBarItem(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                 std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_MenuBarItem();
    
    void setPopupMenu( GUI_PopupMenu *menu );
    GUI_PopupMenu *getPopupMenu() { return popupMenu; };
    
    GUI_PopupMenu *addPopupMenu(GUI_View *parent);
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
    
    GUI_MenuBarItem * addSimpleMenu( const char *title );
    GUI_MenuBarItem * addPopupMenu( const char *title, GUI_View *parentView, std::function<void(GUI_View*)>ucb = NULL);
    GUI_MenuItem *selectedItem;
};



#endif /* GUI_MenuBar_hpp */
