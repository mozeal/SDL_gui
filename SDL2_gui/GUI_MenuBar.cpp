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
    GUI_View( parent, title, x, y, width, height ),
popupMenu(NULL)
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
    setBorder( 0 );
}

GUI_MenuBarItem::~GUI_MenuBarItem() {
    
}

void GUI_MenuBarItem::setPopupMenu( GUI_PopupMenu *menu ) {
    popupMenu = menu;
}

GUI_PopupMenu *GUI_MenuBarItem::addPopupMenu( GUI_View *parentView ) {
    setPopupMenu( GUI_PopupMenu::create(parentView, "PopupMenu1", 0, 0, 160, 0) );
    popupMenu->hide();
    popupMenu->setActivateView(this);
    
    return popupMenu;
}

// -------------------------------------------------------------------------------------------


GUI_PopupMenu *GUI_PopupMenu::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_PopupMenu( parent, title, x, y, width, height, callbackFunction );
}

GUI_PopupMenu::GUI_PopupMenu(GUI_View *parent, const char *title, int x, int y, int width, int height,
                   std::function<void(GUI_View*)>callbackFunction ) :
    GUI_PopupView( parent, title, x, y, width, height ),
    scrollView(NULL)
{
    // setCallback( callbackFunction );
    dragable = false;
    
    setBackgroundColor(cEmptyContent);
    setLayout(GUI_LAYOUT_VERTICAL);
    
    scrollView = GUI_ScrollView::create( this, "Scroll", 0, 0, -1, -1 );
}

GUI_PopupMenu::~GUI_PopupMenu() {
    
}

void GUI_PopupMenu::add(GUI_MenuItem* child) {
    if( child->parent ) {
        child->parent->remove_child( child );
    }
    scrollView->scrollContent->add_child(child);
    scrollView->updateLayout();
    if( oh == 0 ) {
        rectView.h = scrollView->scrollContent->rectView.h;
    }
    child->in_scroll_bed = true;
    
    menuItems.push_back(child);
    child->setCallback( [=](GUI_View *v) {
        GUI_MenuItem *lit = (GUI_MenuItem *)v;
        for (std::vector<GUI_MenuItem *>::iterator it = menuItems.begin() ; it != menuItems.end(); ++it) {
            GUI_MenuItem *c = *it;
            if( lit == c && c->isEnable()) {
                c->setSelected(true);
                //GUI_Log( "%s\n", c->title.c_str());
                this->selectedItem = c;
                if( this->callback ) {
                    this->callback(this);
                }
                GUI_SetMouseCapture(NULL);
            }
            else {
                c->setSelected(false);
            }
        }
    });
}

void GUI_PopupMenu::remove(GUI_MenuItem* child) {
    scrollView->scrollContent->remove_child(child);
    child->in_scroll_bed = false;
    for (std::vector<GUI_MenuItem *>::iterator it = menuItems.begin() ; it != menuItems.end(); ++it) {
        if( child == *it ) {
            menuItems.erase( it );
            break;
        }
    }
    child->setCallback(NULL);
}

GUI_MenuItem* GUI_PopupMenu::addSimpleMenu( const char *title, bool separator ) {
    GUI_MenuItem *item1 = GUI_MenuItem::create( NULL, title, 0, 0, -1, 0 );
    item1->setPadding( 8, 10, 8, 10 );
    item1->separator = separator;
    
    item1->label = GUI_Label::create( item1, title, 0, 0, -1, 0 );
	item1->label->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
	item1->label->setBackgroundColor(cClear);
	item1->labelTextColor = cBlack;
	item1->label->setTextColor(item1->labelTextColor);
    
    add( item1 );
    updateLayout();

    return item1;
}

// --------------------------------------------------------------------------------------------

GUI_MenuBar *GUI_MenuBar::create( GUI_View *parent, const char *title,
                                        std::function<void(GUI_View*)>callbackFunction) {
    return new GUI_MenuBar( parent, title, callbackFunction );
}

GUI_MenuBar::GUI_MenuBar(GUI_View *parent, const char *title,
                         std::function<void(GUI_View*)>callbackFunction ) :
    GUI_View( parent, title, 0, 0, -1, GUI_GetAppTopBarHeight() ),
selectedItem(NULL)
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
        GUI_MenuBarItem *item = (GUI_MenuBarItem *)v;
        GUI_Log( "%s\n", item->title.c_str() );
        if( item->getPopupMenu() ) {
            GUI_Point posn = item->getAbsolutePosition();
            item->getPopupMenu()->setAbsolutePosition( posn.x, posn.y + getHeight() );
            if( item->getPopupMenu()->isVisible() ) {
                item->getPopupMenu()->hide();
            }
            else {
				if (item->user_callback) {
					item->user_callback(item->getPopupMenu());
				}

				item->getPopupMenu()->show();
                GUI_SetMouseCapture(item->getPopupMenu());
            }
        }
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

GUI_MenuBarItem * GUI_MenuBar::addSimpleMenu( const char *title ) {
    GUI_MenuBarItem *item1 = GUI_MenuBarItem::create( NULL, title, 0, 0, 0, 0 );
    item1->setPadding( 8, 10, 8, 10 );
    item1->setBackgroundColor(cWhite);
    
    GUI_Label *lb1 = GUI_Label::create( item1, title, 0, 0, 0, 0 );
    lb1->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    lb1->setBackgroundColor(cClear);
    
    add( item1 );
    updateLayout();
    
    return item1;
}

GUI_MenuBarItem * GUI_MenuBar::addPopupMenu( const char *title, GUI_View *parentView, std::function<void(GUI_View*)>ucb) {
    GUI_MenuBarItem *item = addSimpleMenu(title);
    GUI_PopupMenu *popupMenu = item->addPopupMenu( parentView );
	item->user_callback = ucb;
    popupMenu->setCallback([=](GUI_View *v) {
        GUI_PopupMenu *pm = (GUI_PopupMenu *)v;
        GUI_MenuItem *it = pm->selectedItem;
        it->setSelected(false);
        selectedItem = pm->selectedItem;
        pm->selectedItem = NULL;
        
        pm->hide();

        if( this->callback ) {
            this->callback(this);
        }
    });
    
    return item;
}
