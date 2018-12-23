//
//  GUI_ImageView.cpp
//  GUI_ImageView
//
//  Created by Panutat Tejasen on 23/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_ImageView.h"

static GUI_ImageView *createImageView( GUI_View *parent, const char *title, int x, int y, int width, int height, const char *filename,
                                      std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_ImageView(parent, title, x, y, width, height, filename, userEventHandler );
}

GUI_ImageView::GUI_ImageView( GUI_View *parent, const char *title, int x, int y, int width, int height, const char *filename,
                             std::function<bool(SDL_Event* ev)>userEventHandler):
GUI_View( parent, title, x, y, width, height, userEventHandler)
{
    image.loadTexture(filename);
}

GUI_ImageView::~GUI_ImageView() {
    
}

