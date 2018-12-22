//
//  GUI_View.hpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_View_hpp
#define GUI_View_hpp

#include <string>
#include <stdio.h>
#include <functional>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "GUI_Utils.h"

#define GUI_ALIGN_LEFT        0
#define GUI_ALIGN_CENTER      2
#define GUI_ALIGN_RIGHT       4
#define GUI_ALIGN_TOP         0
#define GUI_ALIGN_VCENTER     16
#define GUI_ALIGN_BOTTOM      32
#define GUI_ALIGN_ABSOLUTE    64

#define GUI_LAYOUT_ABSOLUTE   0
#define GUI_LAYOUT_HORIZONTAL 2
#define GUI_LAYOUT_VERTICAL   4

class GUI_View {
protected:
    std::function<bool(SDL_Event* ev)>user_events_handler;
    
    virtual void predraw();
    virtual void draw();
    virtual void postdraw();
    
    bool    _hidden;
    bool    _disable;
    bool    _uiready;
    bool    _focus;
    bool    _dragging;
    
    int     ox, oy, ow, oh;
public:
    static GUI_View *createView( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    
    GUI_View( GUI_View *parent, const char *title, int x, int y, int width, int height,
             std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    virtual ~GUI_View();
    
    virtual bool eventHandler(SDL_Event*ev);

    std::string title;
    GUI_Point topLeft;      // relative to parent window
    GUI_Rect rectView;      // relative to top window
    GUI_Rect rectClip;      // clip rec
    
    std::vector<GUI_View *>children;
    GUI_View *parent;
    
    SDL_Color backgroundColor;
    SDL_Color borderColor;
    int corner;
    int border;
    bool dragable;
    bool click_through;
    bool click_to_top;
    
    int layout;
    int align;
    
    int padding[4];
    int margin[4];
    
    virtual void setPadding(int p0, int p1, int p2, int p3);
    virtual void setMargin(int p0, int p1, int p2, int p3);
    
    void updateLayout();
    
    virtual void add_child(GUI_View* child);
    virtual void remove_child(GUI_View* child);
    virtual void remove_all_children();
    
    virtual void toTop();
    virtual void toBack();

    virtual void clear(GUI_Rect *rect = 0);
    
    virtual GUI_View *hitTest(int x, int y, bool bRecursive = true);
    
    virtual void show() {
        _hidden = false; _uiready = false;
    };
    
    virtual void hide() {
        _hidden = true; _uiready = false;
    };
    
    virtual void enable() {
        _disable = false;
    };
    
    virtual void disable() {
        _disable = true;
        _focus = false;
        //textSelectionCancel(); 
    };
    
    virtual void setFocus() {
        _focus = true;
    };

    virtual void killFocus() {
        _focus = false;
    };

    virtual bool isEnable() {
        return !_disable;
    };
    
    virtual bool isVisible() {
        return !_hidden;
    };
    
    virtual bool isFocus() {
        return _focus;
    };
    
    virtual bool isUIReady() {
        return _uiready;
    };
};

#endif /* GUI_View_hpp */
