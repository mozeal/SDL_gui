//
//  GUI_ImageView.hpp
//  GUI_ImageView
//
//  Created by Panutat Tejasen on 23/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_ImageView_hpp
#define GUI_ImageView_hpp

#include <string>
#include <stdio.h>
#include <functional>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "GUI_View.h"
#include "GUI_Utils.h"
#include "GUI_image.h"

class GUI_ImageView : public GUI_View {
protected:
    GUI_image image;
public:
    static GUI_ImageView *createImageView( GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0, const char *filename=NULL,
                                std::function<bool(SDL_Event* ev)>userEventHandler = NULL );
    GUI_ImageView(GUI_View *parent, const char *title, int x=0, int y=0, int width=0, int height=0, const char *filename=NULL,
                  std::function<bool(SDL_Event* ev)>userEventHandler = NULL);
    virtual ~GUI_ImageView();
};

#endif /* GUI_ImageView_hpp */
