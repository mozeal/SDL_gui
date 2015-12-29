//
//  SDL_gui.hpp
//  OSX
//
//  Created by Jimmy on 10/16/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#ifndef SDL_gui_hpp
#define SDL_gui_hpp

#include <stdio.h>
#include "SDL.h"
#include "SDL_ttf.h"

#define MAX(a,b) ((a) > (b) ? a : b)
#define MIN(a,b) ((a) < (b) ? a : b)

#define GUI_FONTPATH "data/DroidSans.ttf"
#define GUI_FONTPATH_MONO "data/DroidSansMono.ttf"

const int GUI_nominal_font_size=14;
const int GUI_nominal_leading=28;
extern TTF_Font *GUI_def_font, *GUI_title_font, *GUI_mono_font;

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern SDL_Window *GUI_window;
extern SDL_Renderer *GUI_renderer;

//extern SDL_Surface *GUI_screenSurface;
//extern SDL_Texture *GUI_screenTexture;

extern void *GUI_mouseCapturedWindow;
extern void *GUI_modalWindow;

extern int GUI_scale;
extern float GUI_mouseScale;

int GUI_Init( void );
SDL_Window* GUI_CreateWindow(const char *title="GUI", int sx=0, int sy=0);
void GUI_Quit( void );
void GUI_Log( const char * format, ... );
void GUI_Run( bool (*user_handle_ev)(SDL_Event *) = NULL );

struct GUI_Point {
    short x, y;
    GUI_Point();
    GUI_Point(short x,short y);
    void set(short x,short y);
    bool operator==(GUI_Point b);
    bool operator!=(GUI_Point b);
};

struct GUI_Rect:SDL_Rect {
    GUI_Rect();
    GUI_Rect(Sint16 x,Sint16 y,Uint16 dx,Uint16 dy);
    void set(Sint16 x,Sint16 y,Uint16 dx,Uint16 dy);
};

extern const SDL_Color cGrey0, cGrey1, cGrey2, cGrey3, cGrey4, cGrey5, cGrey6, cGrey7, cGrey8;
extern SDL_Color cClear, cNop, cWhite, cBlack, cGrey, cRed, cGreen, cBlue, cCyan, cMagenta, cYellow, cDarkGrey, cLightGrey;
extern SDL_Color cBorder, cBackground, cForeground, cPointer;

GUI_Rect *GUI_MakeRect(int x,int y,int w,int h);

void GUI_DrawLine( int x1, int y1, int x2, int y2, SDL_Color col);

void GUI_DrawRect2( GUI_Rect *rect, SDL_Color col);
void GUI_FillRect2( GUI_Rect *rect, SDL_Color col);

inline void GUI_DrawRect( int x, int y, int xc, int yc, SDL_Color col) {
    GUI_DrawRect2( GUI_MakeRect(x, y, xc, yc), col );
};
inline void GUI_FillRect( int x, int y, int xc, int yc, SDL_Color col) {
    GUI_FillRect2( GUI_MakeRect(x, y, xc, yc), col );
};

void GUI_DrawCircle( int xc, int yc, int rad, SDL_Color col );
void GUI_FillCircle( int xc, int yc, int rad, SDL_Color col );

void GUI_DrawRoundRect( int x, int y, int xc, int yc, int radius, SDL_Color col );
void GUI_FillRoundRect( int x, int y, int xc, int yc, int radius, SDL_Color col );

SDL_Texture *GUI_createPixmap(const char* pm_data[]);
SDL_Color sdl_color(Uint32 c);

extern SDL_Texture *GUI_crossTexture;
extern SDL_Texture *GUI_dropdownTexture;
extern SDL_Texture *GUI_checkTexture;
extern TTF_Font *GUI_fonts[3];


extern  Uint32  GUI_EventID;

extern const char *GUI_getDocumentDirectory();


#define GUI_LISTSELECTED    SDL_USEREVENT+1

#endif /* SDL_gui_hpp */
