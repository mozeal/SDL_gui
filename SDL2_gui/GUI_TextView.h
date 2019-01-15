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

class GUI_TextView : public GUI_ImageView {
protected:
    TTF_Font *font;
public:
    static GUI_TextView *create( GUI_View *parent, const char *title, const char *fontname, int fontsize, int x=0, int y=0, int width=0, int height=0,
                                 std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    GUI_TextView(GUI_View *parent, const char *title, const char *fontname, int fontsize, int x=0, int y=0, int width=0, int height=0,
                  std::function<bool(SDL_Event* ev)>userEventHandler = NULL);
    virtual ~GUI_TextView();
    virtual void setTextColor( SDL_Color c );
    virtual void setTitle( std::string t ) {
        title = std::string(t);
        updateContent();
    }
    virtual void updateContent();
};

#endif /* GUI_TextView_hpp */
