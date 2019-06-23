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

	virtual void enable();
	virtual void disable();
	virtual void setEnable(bool e);
	SDL_Color labelTextColor;
	GUI_Label* label;
};

enum MenuAnimationMode
{
    eFromLeft,
    eFromRight,
    eFromBottom,
    eFromTop
};

class GUI_Menu : public GUI_View {
public:
    std::vector<GUI_MenuItem *>menuItems;
protected:
    GUI_View    *activateView;
    MenuAnimationMode menuAnimationMode = eFromLeft;
    int hiddenX = 0, hiddenY = 0;
    int visibleX = 0, visibleY = 0;
public:
    static GUI_Menu *create( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
                            std::function<void(GUI_View*)>callbackFunction = nullptr, MenuAnimationMode mode = eFromLeft );
    GUI_Menu(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0,
             std::function<void(GUI_View*)>callbackFunction = nullptr, MenuAnimationMode mode = eFromLeft );
    virtual ~GUI_Menu();

    virtual bool eventHandler(SDL_Event*ev);

    virtual void add(GUI_MenuItem* child);
    virtual void remove(GUI_MenuItem* child);

    virtual GUI_MenuItem *addSimpleMenu( const char *title, bool separator=false );

    GUI_MenuItem *selectedItem = nullptr;

    bool isOpen;

    MenuAnimationMode getMenuAnimationMode();
    void setMenuAnimationMode(MenuAnimationMode mode);

    virtual void close( int duration=0 );
    virtual void open( int duration=0 );

    virtual void predraw();

    void setActivateView( GUI_View *v ) { activateView = v; };
};

#endif /* GUI_Menu_hpp */
