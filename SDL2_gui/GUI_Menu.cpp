//
//  GUI_Menu.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Menu.h"
#include "GUI_shapes.h"

GUI_MenuItem *GUI_MenuItem::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                   std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_MenuItem( parent, title, x, y, width, height, callbackFunction );
}

GUI_MenuItem::GUI_MenuItem(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, x, y, width, height ),
separator(true)
{
    dragable = false;
    clickable = true;
    focusable = true;
    showInteract = true;
    mouseReceive = true;
    
    setBackgroundColor(cWhite);
    
    setLayout(GUI_LAYOUT_HORIZONTAL);
    border = 0;
}

GUI_MenuItem::~GUI_MenuItem() {
    
}

void GUI_MenuItem::setSelected( bool s ) {
    selected = s;
    if( selected ) {
        setBackgroundColor( cHightLightSelection );
    }
    else {
        setBackgroundColor( cWhite );
    }
}

void GUI_MenuItem::postdraw() {
    GUI_View::postdraw();
    
    if( separator ) {
        GUI_DrawHLine( 0, rectView.w, rectView.h-1, cBlack );
    }
}

// ------------------------------------------------------------------------------------------------

GUI_Menu *GUI_Menu::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_Menu( parent, title, x, y, width, height, callbackFunction );
}

GUI_Menu::GUI_Menu(GUI_View *parent, const char *title, int x, int y, int width, int height,
                   std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, x, y, width, height )
{
    setCallback( callbackFunction );
    
    setBackgroundColor(cEmptyContent);
    setLayout(GUI_LAYOUT_VERTICAL);
}

GUI_Menu::~GUI_Menu() {
    
}

void GUI_Menu::add(GUI_MenuItem* child) {
    if( child->parent ) {
        child->parent->remove_child( child );
    }
    add_child(child);
    
    menuItems.push_back(child);
    child->setCallback( [=](GUI_View *v) {
        GUI_MenuItem *lit = (GUI_MenuItem *)v;
        for (std::vector<GUI_MenuItem *>::iterator it = menuItems.begin() ; it != menuItems.end(); ++it) {
            GUI_MenuItem *c = *it;
            if( lit == c ) {
                c->setSelected(true);
                GUI_Log( "%s\n", c->title.c_str());
                this->selectedItem = c;
                if( this->callback ) {
                    this->callback(this);
                }
            }
            else {
                c->setSelected(false);
            }
        }
    });
}

void GUI_Menu::remove(GUI_MenuItem* child) {
    remove_child(child);
    for (std::vector<GUI_MenuItem *>::iterator it = menuItems.begin() ; it != menuItems.end(); ++it) {
        if( child == *it ) {
            menuItems.erase( it );
            break;
        }
    }
    child->setCallback(NULL);
}

void GUI_Menu::addSimpleMenu( const char *title, bool separator ) {
    GUI_MenuItem *item1 = GUI_MenuItem::create( NULL, title, 0, 0, -1, 0 );
    item1->setPadding( 8, 10, 8, 10 );
    item1->separator = separator;
    
    GUI_Label *lb1 = GUI_Label::create( item1, title, 0, 0, -1, 0 );
    lb1->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    lb1->setBackgroundColor(cClear);
    
    add( item1 );
    updateLayout();
}
