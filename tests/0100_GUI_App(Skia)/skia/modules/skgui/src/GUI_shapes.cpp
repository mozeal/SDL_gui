//
//  GUI_shapes.cpp
//  GUI_shapes
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "skgui.h"
#include "GUI_shapes.h"
#include <SDL.h>
#include <fstream>
#include "include/effects/SkGradientShader.h"

static SDL_Texture *drawCircle[24] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
static SDL_Texture *fillCircle[24] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };


void GUI_FillRect(int x, int y, int w, int h, SkColor col) {
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    SkRect r;
    r.setXYWH(x, y, w, h);
    SkPaint p;
    p.setStyle(SkPaint::kFill_Style);
    p.setColor(col);
    canvas->drawRect(r, p);
}

void GUI_DrawRect(int x, int y, int w, int h, SkColor col) {
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    SkRect r;
    r.setXYWH(x, y, w, h);
    SkPaint p;
    p.setStyle(SkPaint::kStroke_Style);
    p.setColor(col);
    canvas->drawRect(r, p);
}

SDL_Texture *getDrawTexture( int radius ) {
//#if defined(__WIN32__) || defined(__MACOSX__) || defined(__IPHONEOS__) || defined(__ANDROID__)
//    return NULL;
//#else
//
//    // Only __EMSCRIPTEN__ and __RPI__
//
//    if( radius <= 1 || radius >= 24 )
//        return NULL;
//    if( drawCircle[radius] ) {
//        return drawCircle[radius];
//    }
//
//    SDL_Texture *tex = SDL_CreateTexture(GUI_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius*2+1, radius*2+1);
//    drawCircle[radius] = tex;
//    SDL_SetRenderTarget(GUI_renderer, drawCircle[radius]);
//    SDL_SetTextureBlendMode(drawCircle[radius], SDL_BLENDMODE_BLEND);
//    SDL_SetRenderDrawColor(GUI_renderer, 0, 0, 0, 0);
//    SDL_RenderClear(GUI_renderer);
//
//    int result = circleRGBA(GUI_renderer, radius, radius, radius, cWhite.r, cWhite.g, cWhite.b, cWhite.a);
//    if( result )
//        return NULL;
//
//    SDL_SetRenderTarget(GUI_renderer, NULL);
//
//    return drawCircle[radius];
//#endif
}

SDL_Texture *getFillTexture( int radius ) {
//#if defined(__WIN32__) || defined(__MACOSX__) || defined(__IPHONEOS__) || defined(__ANDROID__)
//    return NULL;
//#else
//
//    // Only __EMSCRIPTEN__ and __RPI__
//
//    if( radius <= 1 || radius >= 24 )
//        return NULL;
//    if( fillCircle[radius] ) {
//        return fillCircle[radius];
//    }
//
//    SDL_Texture *tex = SDL_CreateTexture(GUI_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius*2+1, radius*2+1);
//    fillCircle[radius] = tex;
//    SDL_SetRenderTarget(GUI_renderer, fillCircle[radius]);
//    SDL_SetTextureBlendMode(fillCircle[radius], SDL_BLENDMODE_BLEND);
//    SDL_SetRenderDrawColor(GUI_renderer, 0, 0, 0, 0);
//    SDL_RenderClear(GUI_renderer);
//
//    int result = filledCircleRGBA(GUI_renderer, radius, radius, radius, cWhite.r, cWhite.g, cWhite.b, cWhite.a);
//
//    SDL_SetRenderTarget(GUI_renderer, NULL);
//
//    if( result )
//        return NULL;
//    return fillCircle[radius];
//#endif
}

void GUI_FillCircle(int cx, int cy, int radius, SkColor col) {
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    SkPaint p;
    p.setStyle(SkPaint::kFill_Style);
    p.setColor(col);
    canvas->drawCircle(cx, cy, radius, p);
}

void GUI_DrawCircle(int cx, int cy, int radius, SkColor col) {
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    SkPaint p;
    p.setStyle(SkPaint::kStroke_Style);
    p.setColor(col);
    canvas->drawCircle(cx, cy, radius, p);
}

void GUI_DrawRoundRect(int x, int y, int w, int h, int radius, SkColor col) {
    if( radius == w /2 ) {
        return GUI_DrawCircle( x+radius, y+radius, radius, col);
    }
    if ((radius * 2) > w)
        radius = w / 2;
    if ((radius * 2) > h)
        radius = h / 2;

    if( radius <= 2 ) {
        GUI_DrawRect(x, y, w, h, col );
    }
    else{
        SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
        SkRect r;
        r.setXYWH(x, y, w, h);
        SkPaint p;
        p.setStyle(SkPaint::kStroke_Style);
        p.setColor(col);
        canvas->drawRoundRect(r, radius, radius, p);
    }
}

void GUI_FillRoundRect(int x, int y, int w, int h, int radius, SkColor col) {
    if ((radius * 2) > w)
        radius = w / 2;
    if ((radius * 2) > h)
        radius = h / 2;

    if( radius <= 2 ) {
        GUI_FillRect(x, y, w, h, col );
    }
    else{
        SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
        SkRect r;
        r.setXYWH(x, y, w, h);
        SkPaint p;
        p.setStyle(SkPaint::kFill_Style);
        p.setColor(col);
        canvas->drawRoundRect(r, radius, radius, p);
    }
}

void filledCircleRGBAPixels(int radius, SkColor& circleColor, std::vector<Uint32>& pixels, Uint32& transparent)
{
//    SDL_Color backgroundColor; backgroundColor.r = 0; backgroundColor.g = 0; backgroundColor.b = 0; backgroundColor.a = 0;
//
//    SDL_Texture *tex(getFillTexture(radius));
//    if(!tex) SDL_LogError(0, SDL_GetError());
//
//    int result(0);
//    result = SDL_SetRenderTarget(GUI_renderer, tex);
//    if(result) SDL_LogError(0, SDL_GetError());
//
//    Uint32 format(0);
//    result = SDL_QueryTexture(tex, &format, nullptr, nullptr, nullptr);
//    if(result) SDL_LogError(0, SDL_GetError());
//    SDL_PixelFormat* mappingFormat(SDL_AllocFormat(format));
//    if(!mappingFormat) SDL_LogError(0, SDL_GetError());
//    transparent = SDL_MapRGBA(mappingFormat, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
//
//    Uint32 pitch((radius * 2 + 1) * sizeof (Uint32));
//    SDL_Rect rect; rect.x = 0; rect.y = 0; rect.w = radius * 2 + 1; rect.h = radius * 2 + 1;
//    pixels.resize(pitch/4 * (radius * 2 + 1));
//    result = SDL_RenderReadPixels(GUI_renderer, &rect, format, pixels.data(), pitch);
//    if(result) SDL_LogError(0, SDL_GetError());
//
//    result = SDL_SetRenderTarget(GUI_renderer, NULL);
//    if(result) SDL_LogError(0, SDL_GetError());
}

static sk_sp<SkShader> setgradHor(const SkRect& r, SkColor c0, SkColor c1) {
    SkColor colors[] = { c0, c1 };
    SkPoint pts[] = { { r.fLeft, r.fTop }, { r.fRight, r.fTop } };
    return SkGradientShader::MakeLinear(pts, colors, nullptr, 2, SkTileMode::kClamp);
}

static sk_sp<SkShader> setgradVert(const SkRect& r, SkColor c0, SkColor c1) {
    SkColor colors[] = { c0, c1 };
    SkPoint pts[] = { { r.fLeft, r.fTop }, { r.fLeft, r.fBottom } };
    return SkGradientShader::MakeLinear(pts, colors, nullptr, 2, SkTileMode::kClamp);
}

void GUI_FillRectWithGradientHorizontal(SkRect rect, SkColor c1, SkColor c2, int r){
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    SkPaint p;
    p.setStyle(SkPaint::kFill_Style);
    p.setShader(setgradHor(rect, c1, c2));
    canvas->drawRoundRect(rect, r, r, p);
}

void GUI_FillRectWithGradientVertical(SkRect rect, SkColor c1, SkColor c2, int r){
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    SkPaint p;
    p.setStyle(SkPaint::kFill_Style);
    p.setShader(setgradVert(rect, c1, c2));
    canvas->drawRoundRect(rect, r, r, p);
}

void GUI_FillRectWithGradientHorizontal(GUI_Rect* rect, SkColor c1, SkColor c2, int radius)
{
    SkRect r;
    r.setXYWH(rect->x, rect->y, rect->width(), rect->height());
    GUI_FillRectWithGradientHorizontal(r, c1, c2, radius);
}

void GUI_FillRectWithGradientVertical(GUI_Rect* rect, SkColor c1, SkColor c2, int radius)
{
    SkRect r;
    r.setXYWH(rect->x, rect->y, rect->width(), rect->height());
    GUI_FillRectWithGradientHorizontal(r, c1, c2, radius);
}

void GUI_FillRectWithTexture(GUI_Rect* rect, SDL_Texture* texture, int radius)
{
//    int result(0);
//    int w(0), h(0);
//    result = SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
//    if(result) SDL_LogError(0, SDL_GetError());
//
//    Uint32 transparent(0);
//    SkColor circleColor(cBlack);
//    std::vector<Uint32> pixels;
//
//    if(radius)
//        filledCircleRGBAPixels(radius, circleColor, pixels, transparent);
//#ifdef DEBUG
//                int lBottomIndex(0);
//#endif
//    SDL_Rect src; src.x = 0; src.y = 0; src.w = w; src.h = h;
//    SDL_Rect dest; dest.x = 0; dest.y = 0; dest.w = src.w; dest.h = src.h;
//    bool partial(false);
//    for(int y(0); y < rect->h; y += h)
//    {
//        dest.y = y;
//        for(int x(0); x < rect->w - 1;)
//        {
//            if(!radius)
//            {
//                dest.x = x;
//                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
//                x += w;
//                continue;
//            }
//
//            while(x < radius)
//            {
//                src.x = x; src.y = 0; src.w = 1; src.h = h;
//                dest.x = x; dest.y = y; dest.w = 1; dest.h = h;
//                if(y < radius)
//                    while(pixels[src.y * (radius * 2 + 1) + x] == transparent)
//                    {
//                        ++dest.y;
//                        --dest.h;
//                        ++src.y;
//                        --src.h;
//                    }
//                if(((y + dest.y + dest.h) >= (rect->h - radius)))
//                {
//                    if((y + dest.y + dest.h) >= (rect->h))
//                    {
//                        dest.h -= (y + dest.y + dest.h) - (rect->h);
//                        src.h = dest.h;
//                    }
//#ifdef DEBUG
//                    lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + x;
//#endif
//
//                    while(pixels[(radius + radius - (rect->h - (y + + dest.y + dest.h))) * (radius * 2 + 1) + x] == transparent)
////                    while(pixels[(radius /*<- this because we check below part of pixel cirle*/ + y + src.h - (rect->h - radius)) * (radius * 2 + 1) + x] == transparent)
//                    {
//                        --dest.h;
//                        --src.h;
//                        if(!src.h)
//                            break;
//                    }
//#ifdef DEBUG
//                    lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + x;
//#endif
//                }
//                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
//                ++x;
//                if(!partial) partial = true;
//            }
//
//            if(partial)
//                { src.x = radius; src.y = 0; src.w = w - radius; src.h = h; }
//            else
//                { src.x = 0; src.y = 0; src.h = h; src.w = w; }
//            dest.x = x; dest.y = y; dest.w = w; dest.h = h;
//
//            if(partial) partial = false;
//
//            if(x + src.w >= rect->w - radius)
//            {
//                partial = true;
//                src.w -= (x + src.w - (rect->w - radius));
//                dest.w = src.w;
//                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
//                x += src.w;
//
//                src.x += src.w;
//                src.w = w;
//                if(src.x + 1 >= src.w)
//                    src.x = 0;
//                dest.x = x;
//                dest.w = src.w;
//            }
//            else
//            {
//                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
//                x += src.w;
//                src.x = 0; src.y = 0; src.w = w; src.h = h;
//                dest.x = x; dest.y = y; dest.w = w; dest.h = h;
//            }
//
//            if(partial) partial = false;
//
//            while(((rect->w - (x + 1)) <= radius) && ((rect->w - (x + 1)) > 0))
//            {
//                src.y = 0; src.w = 1; src.h = h;
//                dest.w = 1; dest.h = h;
//                if(dest.y < radius)
//                    while(pixels[dest.y * (radius * 2 + 1) + radius + radius - (rect->w - (x + 1))] == transparent)
//                    {
//                        ++dest.y;
//                        --dest.h;
//                        ++src.y;
//                        --src.h;
//                    }
//                if(((y + dest.y + dest.h) >= (rect->h - radius)))
//                {
//                    if((y + dest.y + dest.h) >= (rect->h))
//                    {
//                        dest.h -= (y + dest.y + dest.h) - (rect->h);
//                        src.h = dest.h;
//                    }
//#ifdef DEBUG
//                    lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + radius + radius + 1 - (rect->w - (x + 1)) - 1;
//#endif
//                    while(pixels[(radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + radius + radius + 1 - (rect->w - (x + 1)) - 1] == transparent)
//                    {
//#ifdef DEBUG
//                        lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + radius + radius + 1 - (rect->w - (x + 1)) - 1;
//#endif
//                        --dest.h;
//                        --src.h;
//                        if(!dest.h)
//                            break;
//                    }
//                }
//
//                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
//                ++dest.x;
//                dest.y = y;
//                dest.h = h;
//                ++src.x;
//                src.y = y;
//                src.h = h;
//
//                ++x;
//            }
//        }
//    }
}

void GUI_DrawHLine( int x1, int x2, int y, SkColor color ) {
    GUI_DrawLine(x1, y, x2, y, color);
}

void GUI_DrawVLine( int x, int y1, int y2, SkColor color ) {
    GUI_DrawLine(x, y1, x, y2, color);
}

void GUI_DrawLine(int x1, int y1, int x2, int y2, SkColor color){
    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
    SkPaint paint;
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setColor(color);
    canvas->drawLine(x1, y1, x2, y2, paint);
}

//void GUI_DrawPath(const SkPath & path,const SkPaint & paint){
//    SkCanvas * canvas = GUI_App::getInstance()->getCanvas();
//    canvas->drawPath(path, paint);
//}
