//
//  GUI_shapes.cpp
//  GUI_shapes
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "SDL_gui.h"
#include "GUI_shapes.h"
#include <SDL.h>
#include <fstream>

static SDL_Texture *drawCircle[24] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
static SDL_Texture *fillCircle[24] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

void GUI_FillRect(SDL_Renderer * renderer, int x, int y, int w, int h, SDL_Color col) {
    SDL_Rect rect = {x, y, w, h};

    int result = 0;
    result |= SDL_SetRenderDrawBlendMode(renderer, (col.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    result |= SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    result |= SDL_RenderFillRect(renderer, &rect);

    if (result)
        GUI_Error("GUI_FillRect", result);
}

void GUI_FillRect(int x, int y, int w, int h, SDL_Color col) {
    GUI_FillRect(GUI_renderer, x, y, w, h, col);
}

void GUI_DrawRect(SDL_Renderer * renderer, int x, int y, int w, int h, SDL_Color col) {
    SDL_Rect rect = {x, y, w, h};

    int result = 0;
    result |= SDL_SetRenderDrawBlendMode(renderer, (col.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
    result |= SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    result |= SDL_RenderDrawRect(renderer, &rect);

    if (result)
        GUI_Error("GUI_DrawRect", result);
}

void GUI_DrawRect(int x, int y, int w, int h, SDL_Color col) {
    GUI_DrawRect(GUI_renderer, x, y, w, h, col);
}

SDL_Texture *getDrawTexture( int radius ) {
#if defined(__WIN32__) || defined(__MACOSX__) || defined(__IPHONEOS__) || defined(__ANDROID__)
    return NULL;
#else

    // Only __EMSCRIPTEN__ and __RPI__

    if( radius <= 1 || radius >= 24 )
        return NULL;
    if( drawCircle[radius] ) {
        return drawCircle[radius];
    }

    SDL_Texture *tex = SDL_CreateTexture(GUI_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius*2+1, radius*2+1);
    drawCircle[radius] = tex;
    SDL_SetRenderTarget(GUI_renderer, drawCircle[radius]);
    SDL_SetTextureBlendMode(drawCircle[radius], SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GUI_renderer, 0, 0, 0, 0);
    SDL_RenderClear(GUI_renderer);

    int result = circleRGBA(GUI_renderer, radius, radius, radius, cWhite.r, cWhite.g, cWhite.b, cWhite.a);
    if( result )
        return NULL;

    SDL_SetRenderTarget(GUI_renderer, NULL);

    return drawCircle[radius];
#endif
}

SDL_Texture *getFillTexture( int radius ) {
#if defined(__WIN32__) || defined(__MACOSX__) || defined(__IPHONEOS__) || defined(__ANDROID__)
    return NULL;
#else

    // Only __EMSCRIPTEN__ and __RPI__

    if( radius <= 1 || radius >= 24 )
        return NULL;
    if( fillCircle[radius] ) {
        return fillCircle[radius];
    }

    SDL_Texture *tex = SDL_CreateTexture(GUI_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, radius*2+1, radius*2+1);
    fillCircle[radius] = tex;
    SDL_SetRenderTarget(GUI_renderer, fillCircle[radius]);
    SDL_SetTextureBlendMode(fillCircle[radius], SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GUI_renderer, 0, 0, 0, 0);
    SDL_RenderClear(GUI_renderer);

    int result = filledCircleRGBA(GUI_renderer, radius, radius, radius, cWhite.r, cWhite.g, cWhite.b, cWhite.a);

    SDL_SetRenderTarget(GUI_renderer, NULL);

    if( result )
        return NULL;
    return fillCircle[radius];
#endif
}

void GUI_FillCircle(SDL_Renderer * renderer, int cx, int cy, int radius, SDL_Color col) {
    SDL_Texture *tex = getFillTexture(radius);
    if( tex ) {
        GUI_Rect bounds = { Sint16(cx-radius), Sint16(cy-radius), Uint16(radius*2), Uint16(radius*2) };
        if (col.a != 0) {
            SDL_SetTextureColorMod(tex, col.r, col.g, col.b);
            SDL_SetTextureAlphaMod(tex, col.a);
            SDL_RenderCopy(GUI_renderer, tex, NULL, &bounds);
        }
    }
    else {
        int result = filledCircleRGBA(renderer, cx, cy, radius, col.r, col.g, col.b, col.a);
        if (result)
            GUI_Error("GUI_FillCircle", result);
    }
}

void GUI_FillCircle(int cx, int cy, int radius, SDL_Color col) {
    GUI_FillCircle(GUI_renderer, cx, cy, radius, col);
}

void GUI_DrawCircle(SDL_Renderer * renderer, int cx, int cy, int radius, SDL_Color col) {
    SDL_Texture *tex = getDrawTexture(radius);
    if( tex ) {
        GUI_Rect bounds = { Sint16(cx-radius), Sint16(cy-radius), Uint16(radius*2), Uint16(radius*2) };
        if (col.a != 0) {
            SDL_SetTextureColorMod(tex, col.r, col.g, col.b);
            SDL_SetTextureAlphaMod(tex, col.a);
            SDL_RenderCopy(GUI_renderer, tex, NULL, &bounds);
        }
    }
    else {
        int result = circleRGBA(renderer, cx, cy, radius, col.r, col.g, col.b, col.a);
        if (result)
            GUI_Error("GUI_DrawCircle", result);
    }
}

void GUI_DrawCircle(int cx, int cy, int radius, SDL_Color col) {
    GUI_DrawCircle(GUI_renderer, cx, cy, radius, col);
}

void GUI_DrawRoundRect(SDL_Renderer * renderer, int x, int y, int w, int h, int radius, SDL_Color col) {
    if( radius == w /2 ) {
        return GUI_DrawCircle( x+radius, y+radius, radius, col);
    }
    if ((radius * 2) > w)
        radius = w / 2;
    if ((radius * 2) > h)
        radius = h / 2;

    if( radius <= 2 ) {
        return GUI_DrawRect( renderer, x, y, w, h, col );
    }

    SDL_Texture *tex = getDrawTexture(radius);
    if( tex ) {
        SDL_SetTextureColorMod(tex, col.r, col.g, col.b);
        SDL_SetTextureAlphaMod(tex, col.a);

        GUI_DrawHLine( x+radius, x+w-radius, y+0, col );
        GUI_DrawHLine( x+radius, x+w-radius, y+h-1, col );
        GUI_DrawVLine( x+0, y+radius, y+h-radius, col );
        GUI_DrawVLine( x+w-1, y+radius, y+h-radius, col );

        SDL_Rect src = { 0, 0, radius, radius };
        SDL_Rect dst = { 0, 0, radius, radius };
        src.x = 0; src.y = 0; dst.x = x; dst.y = y; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
        src.x = radius+1; src.y = 0; dst.x = x + w - radius; dst.y = y; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
        src.x = 0; src.y = radius+1; dst.x = x; dst.y = h-radius; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
        src.x = radius+1; src.y = radius+1; dst.x = w-radius; dst.y = h-radius; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
    }
    else {
        int result = roundedRectangleRGBA(renderer, x, y, x + w - 1, y + h - 1, radius, col.r, col.g, col.b, col.a);

        if (result)
            GUI_Error("GUI_FillRoundRect", result);
    }

}

void GUI_DrawRoundRect(int x, int y, int w, int h, int radius, SDL_Color c) {
    GUI_DrawRoundRect(GUI_renderer, x, y, w, h, radius, c );
}

void GUI_FillRoundRect(SDL_Renderer * renderer, int x, int y, int w, int h, int radius, SDL_Color col) {
    if ((radius * 2) > w)
        radius = w / 2;
    if ((radius * 2) > h)
        radius = h / 2;

    if( radius <= 2 ) {
        return GUI_FillRect( renderer, x, y, w, h, col );
    }

    SDL_Texture *tex = getFillTexture(radius);
    if( tex ) {
        SDL_SetTextureColorMod(tex, col.r, col.g, col.b);
        SDL_SetTextureAlphaMod(tex, col.a);

        GUI_FillRect(renderer, radius, 0, w-radius*2, radius, col );
        GUI_FillRect(renderer, 0, radius, w, h-radius*2, col );
        GUI_FillRect(renderer, radius, h-radius, w-radius*2, radius, col );

        SDL_Rect src = { 0, 0, radius, radius };
        SDL_Rect dst = { 0, 0, radius, radius };
        src.x = 0; src.y = 0; dst.x = x; dst.y = y; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
        src.x = radius+1; src.y = 0; dst.x = x + w-radius; dst.y = y; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
        src.x = 0; src.y = radius+1; dst.x = x; dst.y = y + h-radius; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
        src.x = radius+1; src.y = radius+1; dst.x = x + w-radius; dst.y = y + h-radius; SDL_RenderCopy(GUI_renderer, tex, &src, &dst);
    }
    else {
        int result = roundedBoxRGBA(renderer, x, y, x + w - 1, y + h - 1, radius, col.r, col.g, col.b, col.a);

        if (result)
            GUI_Error("GUI_FillRoundRect", result);
    }
}

void GUI_FillRoundRect(int x, int y, int w, int h, int radius, SDL_Color c) {
    GUI_FillRoundRect(GUI_renderer, x, y, w, h, radius, c );
}

void filledCircleRGBAPixels(int radius, SDL_Color& circleColor, std::vector<Uint32>& pixels, Uint32& transparent)
{
    SDL_Color backgroundColor; backgroundColor.r = 0; backgroundColor.g = 0; backgroundColor.b = 0; backgroundColor.a = 0;

    SDL_Texture *tex(getFillTexture(radius));
    if(!tex) SDL_LogError(0, SDL_GetError());

    int result(0);
    result = SDL_SetRenderTarget(GUI_renderer, tex);
    if(result) SDL_LogError(0, SDL_GetError());

    Uint32 format(0);
    result = SDL_QueryTexture(tex, &format, nullptr, nullptr, nullptr);
    if(result) SDL_LogError(0, SDL_GetError());
    SDL_PixelFormat* mappingFormat(SDL_AllocFormat(format));
    if(!mappingFormat) SDL_LogError(0, SDL_GetError());
    transparent = SDL_MapRGBA(mappingFormat, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);

    Uint32 pitch((radius * 2 + 1) * sizeof (Uint32));
    SDL_Rect rect; rect.x = 0; rect.y = 0; rect.w = radius * 2 + 1; rect.h = radius * 2 + 1;
    pixels.resize(pitch/4 * (radius * 2 + 1));
    result = SDL_RenderReadPixels(GUI_renderer, &rect, format, pixels.data(), pitch);
    if(result) SDL_LogError(0, SDL_GetError());

    result = SDL_SetRenderTarget(GUI_renderer, NULL);
    if(result) SDL_LogError(0, SDL_GetError());
}

void GUI_FillRectWithGradientHorizontal(GUI_Rect* rect, SDL_Color c1, SDL_Color c2, int radius)
{
    int x(0), y(0), width(rect->w), height(rect->h);
    Uint8 r, g, b;
    SDL_Rect dest;

    Uint32 transparent(0);
    SDL_Color circleColor(cWhite);
    std::vector<Uint32> pixels;

    if(radius)
        filledCircleRGBAPixels(radius, circleColor, pixels, transparent);

    for (; x < width; ++x)
    {
        r = (c2.r * x / width) + (c1.r * (width - x) / width);
        g = (c2.g * x / width) + (c1.g * (width - x) / width);
        b = (c2.b * x / width) + (c1.b * (width - x) / width);

        dest.x = x;
        dest.y = 0;
        dest.w = 1;
        dest.h = height;

        if(radius && (dest.x <= radius))
            while(true)
            {
                if(pixels[dest.y * (radius * 2 + 1) + dest.x] != transparent)
                    break;
                ++dest.y;
                dest.h -= 2;
            }
        if(radius && (dest.x >= (width - radius)))
            while(true)
            {
                if(pixels[dest.y * (radius * 2 + 1) + dest.x - (width - radius - 1) + radius] != transparent)
                    break;
                ++dest.y;   // Not needed?!?
                dest.h -= 2;    // -- instead?!?
            }

      int result = 0;
      result |= SDL_SetRenderDrawBlendMode(GUI_renderer, (c1.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
      result |= SDL_SetRenderDrawColor(GUI_renderer, r, g, b, 255);
      result |= SDL_RenderFillRect(GUI_renderer, &dest);

      if (result)
          GUI_Error("GUI_FillRect", result);
    }
}

void GUI_FillRectWithGradientVertical(GUI_Rect* rect, SDL_Color c1, SDL_Color c2, int radius)
{
    int x(0), y(0), width(rect->w), height(rect->h);
    Uint8 r, g, b;
    SDL_Rect dest;

    Uint32 transparent(0);
    SDL_Color circleColor(cBlack);
    std::vector<Uint32> pixels;

    if(radius)
        filledCircleRGBAPixels(radius, circleColor, pixels, transparent);

    for(; y < height; ++y)
    {
        r = (c2.r * y / height) + (c1.r * (height - y) / height);
        g = (c2.g * y / height) + (c1.g * (height - y) / height);
        b = (c2.b * y / height) + (c1.b * (height - y) / height);

        dest.x = 0;
        dest.y = y;
        dest.w = width;
        dest.h = 1;

        if(radius && (dest.y <= radius))
            while(true)
            {
                if(pixels[dest.y * (radius * 2 + 1) + dest.x] != transparent)
                    break;
                ++dest.x;
                dest.w -= 2;
            }
        if(radius && (dest.y >= (height - radius - 1)))
            while(true)
            {
                if(pixels[(dest.y - (height - radius - 1) + radius) * (radius * 2 + 1) + dest.x] != transparent)
                    break;
                ++dest.x;   // Not needed?!?
                dest.w -= 2;    // -- instead?!?
            }
        int result = 0;
        result |= SDL_SetRenderDrawBlendMode(GUI_renderer, (c1.a == 255) ? SDL_BLENDMODE_NONE : SDL_BLENDMODE_BLEND);
        result |= SDL_SetRenderDrawColor(GUI_renderer, r, g, b, 255);
        result |= SDL_RenderFillRect(GUI_renderer, &dest);

        if (result)
          GUI_Error("GUI_FillRect", result);
    }
}

void GUI_FillRectWithTexture(GUI_Rect* rect, SDL_Texture* texture, int radius)
{
    int result(0);
    int w(0), h(0);
    result = SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    if(result) SDL_LogError(0, SDL_GetError());

    Uint32 transparent(0);
    SDL_Color circleColor(cBlack);
    std::vector<Uint32> pixels;

    if(radius)
        filledCircleRGBAPixels(radius, circleColor, pixels, transparent);
#ifdef DEBUG
                int lBottomIndex(0);
#endif
    SDL_Rect src; src.x = 0; src.y = 0; src.w = w; src.h = h;
    SDL_Rect dest; dest.x = 0; dest.y = 0; dest.w = src.w; dest.h = src.h;
    bool partial(false);
    for(int y(0); y < rect->h; y += h)
    {
        dest.y = y;
        for(int x(0); x < rect->w - 1;)
        {
            if(!radius)
            {
                dest.x = x;
                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
                x += w;
                continue;
            }

            while(x < radius)
            {
                src.x = x; src.y = 0; src.w = 1; src.h = h;
                dest.x = x; dest.y = y; dest.w = 1; dest.h = h;
                if(y < radius)
                    while(pixels[src.y * (radius * 2 + 1) + x] == transparent)
                    {
                        ++dest.y;
                        --dest.h;
                        ++src.y;
                        --src.h;
                    }
                if(((y + dest.y + dest.h) >= (rect->h - radius)))
                {
                    if((y + dest.y + dest.h) >= (rect->h))
                    {
                        dest.h -= (y + dest.y + dest.h) - (rect->h);
                        src.h = dest.h;
                    }
#ifdef DEBUG
                    lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + x;
#endif

                    while(pixels[(radius + radius - (rect->h - (y + + dest.y + dest.h))) * (radius * 2 + 1) + x] == transparent)
//                    while(pixels[(radius /*<- this because we check below part of pixel cirle*/ + y + src.h - (rect->h - radius)) * (radius * 2 + 1) + x] == transparent)
                    {
                        --dest.h;
                        --src.h;
                        if(!src.h)
                            break;
                    }
#ifdef DEBUG
                    lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + x;
#endif
                }
                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
                ++x;
                if(!partial) partial = true;
            }

            if(partial)
                { src.x = radius; src.y = 0; src.w = w - radius; src.h = h; }
            else
                { src.x = 0; src.y = 0; src.h = h; src.w = w; }
            dest.x = x; dest.y = y; dest.w = w; dest.h = h;

            if(partial) partial = false;

            if(x + src.w >= rect->w - radius)
            {
                partial = true;
                src.w -= (x + src.w - (rect->w - radius));
                dest.w = src.w;
                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
                x += src.w;

                src.x += src.w;
                src.w = w;
                if(src.x + 1 >= src.w)
                    src.x = 0;
                dest.x = x;
                dest.w = src.w;
            }
            else
            {
                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
                x += src.w;
                src.x = 0; src.y = 0; src.w = w; src.h = h;
                dest.x = x; dest.y = y; dest.w = w; dest.h = h;
            }

            if(partial) partial = false;

            while(((rect->w - (x + 1)) <= radius) && ((rect->w - (x + 1)) > 0))
            {
                src.y = 0; src.w = 1; src.h = h;
                dest.w = 1; dest.h = h;
                if(dest.y < radius)
                    while(pixels[dest.y * (radius * 2 + 1) + radius + radius - (rect->w - (x + 1))] == transparent)
                    {
                        ++dest.y;
                        --dest.h;
                        ++src.y;
                        --src.h;
                    }
                if(((y + dest.y + dest.h) >= (rect->h - radius)))
                {
                    if((y + dest.y + dest.h) >= (rect->h))
                    {
                        dest.h -= (y + dest.y + dest.h) - (rect->h);
                        src.h = dest.h;
                    }
#ifdef DEBUG
                    lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + radius + radius + 1 - (rect->w - (x + 1)) - 1;
#endif
                    while(pixels[(radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + radius + radius + 1 - (rect->w - (x + 1)) - 1] == transparent)
                    {
#ifdef DEBUG
                        lBottomIndex = (radius + radius - (rect->h - (y + dest.y + dest.h))) * (radius * 2 + 1) + radius + radius + 1 - (rect->w - (x + 1)) - 1;
#endif
                        --dest.h;
                        --src.h;
                        if(!dest.h)
                            break;
                    }
                }

                SDL_RenderCopy(GUI_renderer, texture, &src, &dest);
                ++dest.x;
                dest.y = y;
                dest.h = h;
                ++src.x;
                src.y = y;
                src.h = h;

                ++x;
            }
        }
    }
}

void GUI_DrawHLine( int x1, int x2, int y, SDL_Color color ) {
    Uint8 *c = (Uint8 *)&color;
    hlineRGBA( GUI_renderer, x1, x2, y, c[0], c[1], c[2], c[3]);
}

void GUI_DrawVLine( int x, int y1, int y2, SDL_Color color ) {
    Uint8 *c = (Uint8 *)&color;
    vlineRGBA( GUI_renderer, x, y1, y2, c[0], c[1], c[2], c[3] );
}
