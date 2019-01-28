//
//  GUI_shapes.hpp
//  GUI_shapes
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_shapes_hpp
#define GUI_shapes_hpp

#include <stdio.h>

void GUI_FillRect(int x, int y, int w, int h, SDL_Color col);
void GUI_DrawRect(int x, int y, int w, int h, SDL_Color col);

void GUI_FillCircle(int cx, int cy, int radius, SDL_Color col);
void GUI_DrawCircle(int cx, int cy, int radius, SDL_Color col);

void GUI_DrawRoundRect(int x, int y, int w, int h, int radius, SDL_Color col);
void GUI_FillRoundRect(int x, int y, int w, int h, int radius, SDL_Color col);

void GUI_DrawHLine( int x1, int x2, int y, SDL_Color color );
void GUI_DrawVLine( int x, int y1, int y2, SDL_Color color );

#endif /* GUI_shapes_hpp */
