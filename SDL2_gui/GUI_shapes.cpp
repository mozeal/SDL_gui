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
}

SDL_Texture *getFillTexture( int radius ) {
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

void GUI_DrawHLine( int x1, int x2, int y, SDL_Color color ) {
    Uint8 *c = (Uint8 *)&color;
    hlineRGBA( GUI_renderer, x1, x2, y, c[0], c[1], c[2], c[3]);
}

void GUI_DrawVLine( int x, int y1, int y2, SDL_Color color ) {
    Uint8 *c = (Uint8 *)&color;
    vlineRGBA( GUI_renderer, x, y1, y2, c[0], c[1], c[2], c[3] );
}
