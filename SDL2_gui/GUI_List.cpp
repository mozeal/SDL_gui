//
//  GUI_List.cpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_List.h"
#include "GUI_shapes.h"

GUI_ListItem *GUI_ListItem::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                            std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_ListItem( parent, title, x, y, width, height, callbackFunction );
}

GUI_ListItem::GUI_ListItem(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, x, y, width, height )
{
    dragable = false;
    //clickable = true;
    clickable = false;
    capture_on_click = false;
    callback_on_mouse_up = true;
    
    focusable = true;
    showInteract = true;
    mouseReceive = true;
    
    setBackgroundColor(cWhite);
    showInteract = true;
    
    setLayout(GUI_LAYOUT_HORIZONTAL);
    border = 0;
}

GUI_ListItem::~GUI_ListItem() {
    
}

void GUI_ListItem::setSelected( bool s ) {
    selected = s;
    if( selected ) {
        setBackgroundColor( cHightLightSelection );
    }
    else {
        setBackgroundColor( cWhite );
    }
}

void GUI_ListItem::postdraw() {
    GUI_View::postdraw();
    
    GUI_DrawHLine( 0, rectView.w, rectView.h-1, cBlack );
}

// ------------------------------------------------------------------------------------------------

GUI_List *GUI_List::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                   std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_List( parent, title, x, y, width, height, callbackFunction );
}

GUI_List::GUI_List(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, x, y, width, height ),
scrollView(NULL)
{
    setCallback( callbackFunction );
 
    setPadding(5, 0, 5, 0);
    setBackgroundColor(cEmptyContent);
    setLayout(GUI_LAYOUT_VERTICAL);
    border = 1;
    corner = 4;
    
    scrollView = GUI_ScrollView::create( this, "Scroll", 0, 0, -1, -1 );

}

GUI_List::~GUI_List() {
    
}

void GUI_List::add(GUI_ListItem* child) {
    if( child->parent ) {
        child->parent->remove_child( child );
    }
    scrollView->scrollContent->add_child(child);
    child->in_scroll_bed = true;
    
    listItems.push_back(child);
    child->setCallback( [=](GUI_View *v) {
        GUI_ListItem *lit = (GUI_ListItem *)v;
        for (std::vector<GUI_ListItem *>::iterator it = listItems.begin() ; it != listItems.end(); ++it) {
            GUI_ListItem *c = *it;
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

void GUI_List::remove(GUI_ListItem* child) {
    scrollView->scrollContent->remove_child(child);
    child->in_scroll_bed = false;
    for (std::vector<GUI_ListItem *>::iterator it = listItems.begin() ; it != listItems.end(); ++it) {
        if( child == *it ) {
            listItems.erase( it );
            break;
        }
    }
    child->setCallback(NULL);
}

void GUI_List::addSimpleItem( const char *title ) {
    GUI_ListItem *item1 = GUI_ListItem::create( NULL, title, 0, 0, -1, 0 );
    item1->setPadding( 5, 10, 5, 10 );

    
    GUI_Label *lb1 = GUI_Label::create( item1, title, 0, 0, -1, 0 );
    lb1->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    lb1->setBackgroundColor(cClear);
    
    add( item1 );
    updateLayout();
}
