//
//  GUI_ImageView.cpp
//  GUI_ImageView
//
//  Created by Panutat Tejasen on 23/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_ImageView.h"
#include "GUI_Utils.h"

GUI_ImageView *GUI_ImageView::create( GUI_View *parent, const char *title, const char *filename, int x, int y, int width, int height,
                                      std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_ImageView(parent, title, filename, x, y, width, height, userEventHandler );
}

GUI_ImageView::GUI_ImageView( GUI_View *parent, const char *title, const char *filename, int x, int y, int width, int height,
                             std::function<bool(SDL_Event* ev)>userEventHandler):
GUI_View( parent, title, x, y, width, height, userEventHandler),
colorMod(cWhite)
{
    image.loadTexture(filename);
}

GUI_ImageView::~GUI_ImageView() {
    
}

void GUI_ImageView::draw() {
    if (image._texture) {
        GUI_Rect bounds = image.bounds;
        if (colorMod.a != 0) {
            SDL_SetTextureColorMod(image._texture, colorMod.r, colorMod.g, colorMod.b);
        }
        
        SDL_RenderCopy(GUI_renderer, image._texture, NULL, &bounds);
    }
}
