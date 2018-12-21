//
//  GUI_shapes.cpp
//  GUI_shapes
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "SDL_gui.h"
#include "GUI_shapes.h"

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

void GUI_FillCircle(SDL_Renderer * renderer, int cx, int cy, int radius, SDL_Color col) {
    int result = filledCircleRGBA(renderer, cx, cy, radius, col.r, col.g, col.b, col.a);
    if (result)
        GUI_Error("GUI_FillCircle", result);
}

void GUI_FillCircle(int cx, int cy, int radius, SDL_Color col) {
    GUI_FillCircle(GUI_renderer, cx, cy, radius, col);
}

void GUI_DrawCircle(SDL_Renderer * renderer, int cx, int cy, int radius, SDL_Color col) {
    int result = circleRGBA(renderer, cx, cy, radius, col.r, col.g, col.b, col.a);
    if (result)
        GUI_Error("GUI_DrawCircle", result);
}

void GUI_DrawCircle(int cx, int cy, int radius, SDL_Color col) {
    GUI_DrawCircle(GUI_renderer, cx, cy, radius, col);
}

void GUI_DrawRoundRect(SDL_Renderer * renderer, int x, int y, int w, int h, int radius, SDL_Color col) {
    if ((radius * 2) > w)
        radius = w / 2;
    if ((radius * 2) > h)
        radius = h / 2;
    int result = roundedRectangleRGBA(renderer, x, y, x + w - 1, y + h - 1, radius, col.r, col.g, col.b, col.a);
    
    if (result)
        GUI_Error("GUI_FillRoundRect", result);
}

void GUI_DrawRoundRect(int x, int y, int w, int h, int radius, SDL_Color c) {
    GUI_DrawRoundRect(GUI_renderer, x, y, w, h, radius, c );
}

void GUI_FillRoundRect(SDL_Renderer * renderer, int x, int y, int w, int h, int radius, SDL_Color col) {
    if ((radius * 2) > w)
        radius = w / 2;
    if ((radius * 2) > h)
        radius = h / 2;
    
    int result = roundedBoxRGBA(renderer, x, y, x + w - 1, y + h - 1, radius, col.r, col.g, col.b, col.a);
    
    if (result)
        GUI_Error("GUI_FillRoundRect", result);
}

void GUI_FillRoundRect(int x, int y, int w, int h, int radius, SDL_Color c) {
    GUI_FillRoundRect(GUI_renderer, x, y, w, h, radius, c );
}
