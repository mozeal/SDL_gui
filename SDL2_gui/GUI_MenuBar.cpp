//
//  GUI_MenuBar.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 21/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_MenuBar.h"

GUI_MenuBarItem *GUI_MenuBarItem::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                   std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_MenuBarItem( parent, title, x, y, width, height, callbackFunction );
}

GUI_MenuBarItem::GUI_MenuBarItem(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) :
    GUI_View( parent, title, x, y, width, height )
{
    dragable = false;
    clickable = false;
    capture_on_click = false;
    focusable = true;
    showInteract = true;
    mouseReceive = true;
    callback_on_mouse_up = true;
    
    setBackgroundColor(cWhite);
    setAlign( GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT );
    
    setLayout(GUI_LAYOUT_HORIZONTAL);
    border = 0;
}

GUI_MenuBarItem::~GUI_MenuBarItem() {
    
}



GUI_MenuBar *GUI_MenuBar::create( GUI_View *parent, const char *title,
                                        std::function<void(GUI_View*)>callbackFunction) {
    return new GUI_MenuBar( parent, title, callbackFunction );
}

GUI_MenuBar::GUI_MenuBar(GUI_View *parent, const char *title,
                         std::function<void(GUI_View*)>callbackFunction ) :
    GUI_View( parent, title, 0, 0, -1, GUI_AppTopBarHeight )
{
    setLayout( GUI_LAYOUT_HORIZONTAL );
}

GUI_MenuBar::~GUI_MenuBar() {
    
}

void GUI_MenuBar::add(GUI_MenuBarItem* child) {
    if( child->parent ) {
        child->parent->remove_child( child );
    }
    add_child(child);
    
    menuItems.push_back(child);
    child->setCallback( [=](GUI_View *v) {
        //GUI_MenuBarItem *lit = (GUI_MenuBarItem *)v;
    });
}

void GUI_MenuBar::remove(GUI_MenuBarItem* child) {
    remove_child(child);
    for (std::vector<GUI_MenuBarItem *>::iterator it = menuItems.begin() ; it != menuItems.end(); ++it) {
        if( child == *it ) {
            menuItems.erase( it );
            break;
        }
    }
    child->setCallback(NULL);
}

void GUI_MenuBar::addSimpleMenu( const char *title ) {
    GUI_MenuBarItem *item1 = GUI_MenuBarItem::create( NULL, title, 0, 0, 0, 0 );
    item1->setPadding( 8, 10, 8, 10 );
    
    GUI_Label *lb1 = GUI_Label::create( item1, title, 0, 0, 0, 0 );
    lb1->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    lb1->setBackgroundColor(cClear);
    
    add( item1 );
    updateLayout();
}
