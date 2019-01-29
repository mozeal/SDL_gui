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
#include <list>
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
    std::function<void(GUI_View*)>callback;
    
    virtual void predraw();
    virtual void draw();
    virtual void postdraw();
    virtual void drawInteract();
    virtual void drawFocus();
    
    static GUI_View *lastInteractView;
    static GUI_View *lastFocusView;

    bool    _visible;
    bool    _enable;
    bool    _focus;
    bool    _interact;
    bool    _selected;
    

    bool    _dragging;

    int     _layout;
    int     _align;
    int     _contentAlign;
    
    int     _corner;
    int     _border;


    int     _padding[4];
    int     _margin[4];
    
    SDL_Color _backgroundColor;
    SDL_Color _textColor;
    SDL_Color _borderColor;

    GUI_Point lastMousePoint;
    
    int     ox, oy, ow, oh;
    
    Uint32 touchTime, touchHoldTime;
    
    void move_topLeft(int dx, int dy);
    void move_rectView(int dx, int dy);  // move win
    
    int     moveOriginX;
    int     moveOriginY;
    int     moveTargetX;
    int     moveTargetY;
    int     moveDuration;
    int     moveTimeStart;
    bool    isMoving;
    
    
public:
    static GUI_View *create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    
    GUI_View( GUI_View *parent, const char *title, int x, int y, int width, int height,
             std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    virtual ~GUI_View();
    
    virtual bool eventHandler(SDL_Event*ev);
    
    void setUserEventHandler( std::function<bool(SDL_Event* ev)>userEventHandler );

    std::string title;
    GUI_Point topLeft;      // relative to parent window
    GUI_Rect rectView;      // relative to top window
    GUI_Rect rectClip;      // clip rec
    
    std::vector<GUI_View *>children;
    GUI_View *parent;
    
    static std::vector<GUI_View *>closeQueue;

    int focusBorder;
    bool in_scroll_bed;
    bool focusable;
    bool clickable;
    bool capture_on_click;
    bool showInteract;
    bool mouseReceive;
    bool click_to_top;
    bool focus_need_input;
    bool callback_on_mouse_down;
    bool callback_on_mouse_up;
    bool callback_on_drag;
    bool propagate_sibling_on_mouseup_outside;
    bool dragable;
    bool drag_limit;
    bool drag_outside_parent;
    
    int dragMinX;
    int dragMaxX;
    int dragMinY;
    int dragMaxY;

    bool isMouseCapturing;
    
    int getWidth() { return rectView.w / GUI_scale; };
    int getHeight() { return rectView.h / GUI_scale; };

    virtual void setBackgroundColor( SDL_Color c ) { _backgroundColor = c; };
    virtual SDL_Color getBackgroundColor() { return _backgroundColor; };

    virtual void setTextColor( SDL_Color c ) { _textColor = c; };
    virtual SDL_Color getTextColor() { return _textColor; };

    virtual void setBorderColor( SDL_Color c ) { _borderColor = c; };
    virtual SDL_Color getBorderColor() { return _borderColor; };
    
    virtual void setPadding(int p0, int p1, int p2, int p3);
    virtual void setMargin(int p0, int p1, int p2, int p3);

    virtual void setContentAlign( int a ) { _contentAlign = a; };
    virtual int  getContentAlign() { return _contentAlign; };

    virtual void setAlign( int a );
    virtual int  getAlign() { return _align; };
    
    virtual void setLayout( int l ) { _layout = l; };
    virtual int  getLayout() { return _layout; };
    
    virtual void updateSize();
    virtual void updateLayout();
    
    virtual void add_child(GUI_View* child);
    virtual void remove_child(GUI_View* child);
    virtual void delete_all_children();
    
    virtual bool toTop();
    virtual bool toBack();
    
    virtual void close();
    
    virtual void updateContent() {
        
    }
    
    virtual void update();
    
    virtual void move( int dx, int dy, int time=0 );
    
    virtual void setAbsolutePosition( int x, int y, int time=0 );
    virtual GUI_Point getAbsolutePosition();

    virtual void clear(GUI_Rect *rect = 0);
    
    virtual GUI_View *hitTest(int x, int y, bool bRecursive = true);
    
    virtual void show() {
        _visible = true;
        //GUI_Log( "Show %s\n", title.c_str() );
    };
    
    virtual void hide() {
        //GUI_Log( "Hide %s (h)\n", title.c_str() );
        _visible = false;
        if( _focus )
            killFocus();
    };
    
    virtual void enable() {
        _enable = true;
    };
    
    virtual void disable() {
        _enable = false;
        if( _focus )
            killFocus();
    };
    
    virtual void setCorner( int c );
    virtual void setBorder( int b );
    virtual int getCorner() { return _corner; };
    virtual int getBorder() { return _border; };
    
    virtual void setFocus();

    virtual void killFocus();

    virtual bool isEnable() {
        return _enable;
    };
    
    virtual void setEnable(bool e) {
        _enable = e;
    };
    
    virtual bool isVisible() {
        return _visible;
    };
    
    virtual void setVisible(bool v) {
        _visible = v;
    };
    
    virtual void setSelected(bool s) {
        _selected = s;
    };
    
    virtual bool isSelected() {
        return _selected;
    }
    
    virtual void setInteract(bool i);
    
    virtual int getInteract() {
        return _interact;
    }
    
    virtual bool isFocus() {
        return _focus;
    };
    
    virtual void setTitle( std::string t ) {
        title = std::string(t);
        updateContent();
    }
    
    virtual void setCallback( std::function<void(GUI_View*)>cb ) {
        callback = cb;
    }
};

#endif /* GUI_View_hpp */
