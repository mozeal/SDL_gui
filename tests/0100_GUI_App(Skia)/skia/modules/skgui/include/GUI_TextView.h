//
//  GUI_TextView.hpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_TextView_hpp
#define GUI_TextView_hpp

#include <stdio.h>
#include "GUI_ImageView.h"
#include "include/core/SkFont.h"

class GUI_TextView : public GUI_ImageView {
protected:
    SkFont font;
    std::string _fontName;
    int _fontSize;
public:
    static GUI_TextView *create( GUI_View *parent, const char *title, const char *fontname, int fontsize, int x=0, int y=0, int width=0, int height=0,
                                 std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    GUI_TextView(GUI_View *parent, const char *title, const char *fontname, int fontsize, int x=0, int y=0, int width=0, int height=0,
                  std::function<bool(SDL_Event* ev)>userEventHandler = NULL);
    virtual ~GUI_TextView();
    virtual void setTextColor( SkColor c );
    virtual void setTitle( std::string t ) {
        title = std::string(t);
        updateContent();
    }
    virtual void updateContent();
    virtual void updateSize();
    bool forceEmptyText;
    
    virtual bool eventHandler(SDL_Event*event);
};

class GUI_FPSView : public GUI_TextView {
public:
    static GUI_FPSView *create( GUI_View *parent, int x=0, int y=0, int width=0, int height=0 );
    GUI_FPSView(GUI_View *parent, int x=0, int y=0, int width=0, int height=0);
    virtual ~GUI_FPSView();
    
    virtual void update();
    
    int frame_count;
    Uint32 timer_start;
};

#endif /* GUI_TextView_hpp */
