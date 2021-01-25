//
//  GUI_ImageView.cpp
//  GUI_ImageView
//
//  Created by Panutat Tejasen on 23/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_ImageView.h"
#include "GUI_Utils.h"
#include "skgui.h"

GUI_ImageView *GUI_ImageView::create( GUI_View *parent, const char *title, const char *filename, int x, int y, int width, int height,
                                      std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_ImageView(parent, title, filename, x, y, width, height, userEventHandler );
}

GUI_ImageView::GUI_ImageView( GUI_View *parent, const char *title, const char *filename, int x, int y, int width, int height,
                             std::function<bool(SDL_Event* ev)>userEventHandler):
GUI_View( parent, title, x, y, width, height, userEventHandler),
colorMod(cWhite),
contentScrollPosnX(0),
contentScrollPosnY(0)
{
    if( filename ) {
        image.loadImage(filename);
        updateLayout();
    }
}

GUI_ImageView::~GUI_ImageView() {
    
}

void GUI_ImageView::setColor( SkColor color ) {
    colorMod = color;
}



void GUI_ImageView::draw() {
//    if (image._texture) {
    if (image.image) {
        GUI_Rect bounds = image.bounds;
        int alpha = SkColorGetA(colorMod);
        if (alpha > 0) {
//            SDL_SetTextureColorMod(image._texture, colorMod.r, colorMod.g, colorMod.b);
        }
        if( _contentAlign & GUI_ALIGN_RIGHT ) {
            bounds.setX(rectView.width() - bounds.width() - _padding[1] * GUI_scale);
        }
        else if( _contentAlign & GUI_ALIGN_CENTER ) {
            bounds.setX((rectView.width() - bounds.width()) / 2);
        }
        else {
            bounds.setX((_padding[3] * GUI_scale) + contentScrollPosnX);
        }
        
        if( _contentAlign & GUI_ALIGN_BOTTOM ) {
            bounds.setY(rectView.height() - bounds.height() - _padding[2] * GUI_scale);
        }
        else if( _contentAlign & GUI_ALIGN_VCENTER ) {
            bounds.setY((rectView.height() - bounds.height()) / 2);
        }
        else {
            bounds.setY(_padding[0] * GUI_scale);
        }


//        SDL_RenderCopy(GUI_renderer, image._texture, NULL, &bounds);
        
        auto canvas = GUI_App::getInstance()->getCanvas();
        SkPaint p;
        p.setStyle(SkPaint::kFill_Style);
        p.setColor(colorMod);
        canvas->drawImage(image.image, bounds.x, bounds.y, &p);
    }
}

void GUI_ImageView::updateSize() {
    if( ow == 0 ) {
        rectView.w = (image.bounds.width() + (_padding[1] + _padding[3]) * GUI_scale);
    }
    if( oh == 0 ) {
        rectView.h = (image.bounds.height() + (_padding[0] + _padding[2]) * GUI_scale);
    }
}

