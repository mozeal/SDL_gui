//
//  GUI_List.hpp
//  GUI_CheckBox
//
//  Created by Panutat Tejasen on 16/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_List_hpp
#define GUI_List_hpp

#include <stdio.h>
#include "GUI_Button.h"
#include "GUI_ScrollView.h"

class GUI_ListItem : public GUI_View {
protected:
    bool selected;
public:
    static GUI_ListItem *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                            std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_ListItem(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
             std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_ListItem();
    
    virtual void setSelected( bool c );
    virtual bool isSelected() { return selected; };
    
    virtual void postdraw();
};

class GUI_List : public GUI_View {
protected:
    std::vector<GUI_ListItem *>listItems;
    GUI_ScrollView *scrollView;
public:
    static GUI_List *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                                        std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_List(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                         std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_List();
    
    virtual void add(GUI_ListItem* child);
    virtual void remove(GUI_ListItem* child);
    
    virtual void addSimpleItem( const char *title );
    
    bool multiSelect;
    
    GUI_ListItem *selectedItem;
    std::vector<GUI_ListItem *>selectedItems;
};
#endif /* GUI_List_hpp */
