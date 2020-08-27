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
#include <SDL.h>
#include "GUI_Utils.h"

void GUI_FillRect(int x, int y, int w, int h, SkColor col);
void GUI_DrawRect(int x, int y, int w, int h, SkColor col);

void GUI_FillCircle(int cx, int cy, int radius, SkColor col);
void GUI_DrawCircle(int cx, int cy, int radius, SkColor col);

void GUI_DrawRoundRect(int x, int y, int w, int h, int radius, SkColor col);
void GUI_FillRoundRect(int x, int y, int w, int h, int radius, SkColor col);

void GUI_FillRectWithGradientHorizontal(GUI_Rect* rect, SkColor c1, SkColor c2, int radius = 0);
void GUI_FillRectWithGradientVertical(GUI_Rect* rect, SkColor c1, SkColor c2, int radius = 0);
void GUI_FillRectWithGradientHorizontal(SkRect rect, SkColor c1, SkColor c2, int radius = 0);
void GUI_FillRectWithGradientVertical(SkRect rect, SkColor c1, SkColor c2, int radius = 0);

void GUI_FillRectWithTexture(GUI_Rect* rect, SDL_Texture* texture, int radius = 0);

void GUI_DrawHLine( int x1, int x2, int y, SkColor color );
void GUI_DrawVLine( int x, int y1, int y2, SkColor color );
void GUI_DrawLine(int x1, int y1, int x2, int y2, SkColor color);

//void GUI_DrawPath(const SkPath & path,const SkPaint & paint);

#endif /* GUI_shapes_hpp */
