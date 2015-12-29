//
//  SDL_gui.cpp
//  OSX
//
//  Created by Jimmy on 10/16/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#include "SDL_gui.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif
#include "GUI_TopWin.h"
#include "GUI_WinBase.h"
#include "xpm.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h> // for chdir(), getcwd()
#include <dirent.h> // for DIR, dirent, opendir()
#include <stdlib.h> // for abs(), exit()
#include <pwd.h>

extern "C" char __iOS_DOCUMENTS_FOLDER[];


static const int MILLESECONDS_PER_FRAME = 1000.0/15.0;       /* about 60 frames per second */


int GUI_physicalWindowWidth = 0;
int GUI_physicalWindowHeight = 0;

int GUI_windowWidth = 0;
int GUI_windowHeight = 0;

int GUI_scale = 1;
float GUI_mouseScale = 1.0;

bool GUI_running = false; // set true by GUI_Run()
static bool quit=false;

static SDL_mutex *mtx=SDL_CreateMutex();
static SDL_cond *cond=SDL_CreateCond();

SDL_Window *GUI_window = NULL;
SDL_Renderer *GUI_renderer = NULL;

void *GUI_mouseCapturedWindow = NULL;
void *GUI_modalWindow = NULL;

#ifdef __ANDROID__
static const char* LOGNAME = "SDL_gui";
#endif

static int GUI_initialized = 0;

TTF_Font *GUI_small_font = NULL;
TTF_Font *GUI_def_font = NULL;
TTF_Font *GUI_title_font = NULL;
TTF_Font *GUI_mono_font = NULL;

TTF_Font *GUI_fonts[3];

SDL_Texture *GUI_crossTexture;
SDL_Texture *GUI_dropdownTexture;
SDL_Texture *GUI_checkTexture;


//SDL_Surface *GUI_screenSurface = NULL;
//SDL_Texture *GUI_screenTexture = NULL;

static int char_wid=0;   // -> 7
Uint32 GUI_EventID = SDL_USEREVENT;

int GUI_Init( void )
{
    int status = 0;
    
    if( !GUI_initialized ) {
        GUI_EventID = SDL_RegisterEvents( 1024 );
        if( GUI_EventID != SDL_USEREVENT ) {
            GUI_Log( "User event ERROR.\n" );
            exit( 1 );
        }
    }
    if ( status == 0 ) {
        ++GUI_initialized;
    }
    return status;
}

SDL_Window* GUI_CreateWindow(const char *title, int sx, int sy)
{
    if( GUI_window ) {
        GUI_Log("GUI_window existed.");
        return GUI_window;
    }

    if( !(GUI_title_font = TTF_OpenFont( GUI_FONTPATH, GUI_nominal_font_size+2 )) )  // fontpath from config.h
    {
        GUI_Log( "font-spec %s not found", GUI_FONTPATH );
        return NULL;
    }
    if( !(GUI_def_font = TTF_OpenFont( GUI_FONTPATH, GUI_nominal_font_size )) )
    {
        GUI_Log( "font-spec %s not found", GUI_FONTPATH) ;
        return NULL;
    }
    if( !(GUI_small_font = TTF_OpenFont( GUI_FONTPATH, GUI_nominal_font_size-3 )) )
    {
        GUI_Log( "font-spec %s not found", GUI_FONTPATH) ;
        return NULL;
    }
    if( !(GUI_mono_font = TTF_OpenFont( GUI_FONTPATH_MONO, GUI_nominal_font_size )) )
    {
        GUI_Log( "font-spec %s not found", GUI_FONTPATH_MONO) ;
        return NULL;
    }
    TTF_SizeText(GUI_mono_font,"1",&char_wid,0); // minimum mono char width
    
    GUI_fonts[0] = GUI_small_font;
    GUI_fonts[1] = GUI_def_font;
    GUI_fonts[2] = GUI_title_font;
    
    int osx = sx;
    int osy = sy;
    GUI_Log("requested: %d %d\n", sx, sy);
    
#if __IPHONEOS__ or __ANDROID__
    if( sx == 0 || sy == 0 ) {
        sx = 720;
        sy = 480;
    }
    int landscape = 0;
    if( sx > sy )
        landscape = 1;
    int modes = SDL_GetNumDisplayModes(0);
    sx = 0;
    sy = 0;
    GUI_Log( "  Modes available: %i\n", modes );
    
    for (int i = 0; i < modes; i++)
    {
        SDL_DisplayMode mode;
        SDL_GetDisplayMode(0, i, &mode);
        GUI_Log( "    :Mode #%i: %i, %i\n", i+1, mode.w, mode.h );
        if (landscape ? mode.w > sx : mode.h > sy)
        {
            sx = mode.w;
            sy = mode.h;
        }
    }
#else
    if( sx == 0 || sy == 0 ) {
        sx = 720;
        sy = 405;
    }
#endif
    GUI_Log("picked: %d %d\n", sx, sy);
    
#if __IPHONEOS__ or __ANDROID__
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_BORDERLESS;
#else
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI;
#endif
    
    //Now create a window with title "SDL" at 0, 0 on the screen with w:800 h:600 and show it
    GUI_window = SDL_CreateWindow("SDL", 0, 0, sx, sy, style );
    if (GUI_window == NULL){
        GUI_Log("GUI_CreateRenderer Error");
        return NULL;
    }
    int gx, gy;
    SDL_GetWindowSize( GUI_window, &gx, &gy );
    GUI_Log("given: %d %d\n", gx, gy);
    

    
    //Create a renderer that will draw to the window, -1 specifies that we want to load whichever
    //video driver supports the flags we're passing
    //Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
    //SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
    //synchornized with the monitor's refresh rate
    GUI_renderer = SDL_CreateRenderer(GUI_window, -1, 0);
    if (GUI_renderer == NULL){
        GUI_Log("GUI_CreateRenderer Error");
        GUI_Quit();
        return NULL;
    }
    SDL_GL_GetDrawableSize( GUI_window, &GUI_physicalWindowWidth, &GUI_physicalWindowHeight );
    GUI_Log("drawable: %d %d\n", GUI_physicalWindowWidth, GUI_physicalWindowHeight);
    
    GUI_mouseScale = (float)GUI_physicalWindowWidth/(float)gx;
    
    
    int scalex = GUI_physicalWindowWidth / osx;
    int scaley = GUI_physicalWindowHeight / osy;
    GUI_scale = (scalex < scaley) ? scaley : scalex;
    if( GUI_scale < 1 )
        GUI_scale = 1;
    
    
    
    GUI_windowWidth = (GUI_physicalWindowWidth) / GUI_scale;
    GUI_windowHeight = (GUI_physicalWindowHeight) / GUI_scale;
    GUI_Log( "Scale: %i\n", GUI_scale );
    GUI_Log("virtual: %d %d\n", GUI_windowWidth, GUI_windowHeight);
    
    
    // Select the color for drawing. It is set to red here.
    SDL_SetRenderDrawColor(GUI_renderer, 0, 0, 0, 255);
    SDL_RenderClear(GUI_renderer);
    SDL_RenderPresent(GUI_renderer);
    
    SDL_RenderSetScale( GUI_renderer, GUI_scale, GUI_scale );
    
    //GUI_screenSurface = SDL_CreateRGBSurface(0, GUI_physicalWindowWidth, GUI_physicalWindowHeight, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    //GUI_screenTexture = SDL_CreateTexture( GUI_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, GUI_physicalWindowWidth, GUI_physicalWindowHeight );

    GUI_crossTexture = GUI_createPixmap( cross_xpm );
    GUI_dropdownTexture = GUI_createPixmap( dropdown_xpm );
    GUI_checkTexture = GUI_createPixmap( check_xpm );
    
    return GUI_window;
}

void GUI_Quit( void )
{
    if( GUI_topWin ) {
        delete GUI_topWin;
        GUI_topWin = NULL;
    }
    if( GUI_window ) {
        SDL_DestroyWindow(GUI_window);
        GUI_window = NULL;
    }
    if( GUI_renderer ) {
        SDL_DestroyRenderer(GUI_renderer);
        GUI_renderer = NULL;
    }
}

void GUI_Log( const char * format, ... )
{
#ifdef __ANDROID__
    char buffer[512];
    va_list args;
    va_start (args, format);
    vsprintf (buffer,format, args);
    va_end (args);
    
    __android_log_print(ANDROID_LOG_VERBOSE, LOGNAME, buffer, 1 );
#else
    va_list args;
    va_start (args, format);
    vprintf (format, args);
    va_end (args);
#endif
}

GUI_Rect::GUI_Rect() { x=y=w=h=0; }
GUI_Rect::GUI_Rect(Sint16 _x,Sint16 _y,Uint16 _dx,Uint16 _dy) { x=_x; y=_y; w=_dx; h=_dy; }
void GUI_Rect::set(Sint16 _x,Sint16 _y,Uint16 _dx,Uint16 _dy) { x=_x; y=_y; w=_dx; h=_dy; }

GUI_Point::GUI_Point(short x1,short y1):x(x1),y(y1) { }
GUI_Point::GUI_Point():x(0),y(0) { }
void GUI_Point::set(short x1,short y1) { x=x1; y=y1; }
bool GUI_Point::operator==(GUI_Point b) { return x==b.x && y==b.y; }
bool GUI_Point::operator!=(GUI_Point b) { return x!=b.x || y!=b.y; }

GUI_Rect *GUI_MakeRect(int x,int y,int w,int h) { // creates pointer to Rect. Each source file gets its own instance.
    static GUI_Rect rp_rect;
    rp_rect.set(x,y,w,h);
    return &rp_rect;
}

int send_ticks(void* data) {  // keep alive
    while (!quit) {
        SDL_CondSignal(cond);
        SDL_Delay(20);
    }
    return 0;
}

bool (*user_handle_events)(SDL_Event *);

static void handle_events(SDL_Event *ev) {
    if( GUI_mouseCapturedWindow ) {
        switch (ev->type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONUP:
                ((GUI_WinBase *)GUI_mouseCapturedWindow)->handleEvents(ev);
                return;
        }
    }
    if( GUI_modalWindow ) {
        switch (ev->type) {
            case SDL_MOUSEBUTTONDOWN: {
                SDL_MouseButtonEvent e = ev->button;
                int x = e.x*GUI_mouseScale/GUI_scale;
                int y = e.y*GUI_mouseScale/GUI_scale;
                GUI_WinBase *w = ((GUI_WinBase *)GUI_modalWindow);
                if( !w->hitTest(x, y) ) {
                    return;
                }
                else {
                    ((GUI_WinBase *)GUI_modalWindow)->handleEvents(ev);
                    return;
                }
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                SDL_MouseButtonEvent e = ev->button;
                int x = e.x*GUI_mouseScale/GUI_scale;
                int y = e.y*GUI_mouseScale/GUI_scale;
                GUI_WinBase *w = ((GUI_WinBase *)GUI_modalWindow);
                if( !w->hitTest(x, y) ) {
                    w->close();
                    return;
                }
                else {
                    ((GUI_WinBase *)GUI_modalWindow)->handleEvents(ev);
                    return;
                }
            }
        }

    }
    
    if( GUI_topWin ) {
        if( GUI_topWin->handleEvents( ev ) )
            return;
    }
    if( user_handle_events ) {
        if( user_handle_events( ev ) )
            return;
    }
    
}

void GUI_Run(bool (*user_handle_ev)(SDL_Event *)) {
    user_handle_events = user_handle_ev;
    
    GUI_running=true;
    
    SDL_Event ev;
    
    Uint32 startFrame;
    Uint32 endFrame;
    int delay;

    startFrame = SDL_GetTicks();

    quit=false;
    while (!quit) {
        while (SDL_PollEvent(&ev)) {
            if (ev.type==SDL_QUIT) {
                quit=true;
                break;
            }
            if (ev.type == SDL_KEYDOWN) {
                if (ev.key.keysym.sym == SDLK_AC_BACK ) {
                    quit=true;
                    break;
                }
            }

            handle_events(&ev);
        }
 
        SDL_RenderSetScale( GUI_renderer, GUI_scale, GUI_scale );
        SDL_RenderSetViewport( GUI_renderer, NULL );
        SDL_RenderSetClipRect( GUI_renderer, NULL );
        SDL_SetRenderDrawColor(GUI_renderer, 0x64, 0x95, 0xed, 0xff);
        SDL_RenderClear( GUI_renderer );
        
        if( GUI_topWin ) {
            GUI_topWin->draw();
        }
        
        SDL_RenderPresent(GUI_renderer);

        endFrame = SDL_GetTicks();
        
        /* figure out how much time we have left, and then sleep */
        Uint32 frameTime = (endFrame - startFrame);
        if( frameTime <= 0 ) {
            frameTime = 1;
        }
        
        delay = MILLESECONDS_PER_FRAME - frameTime;
        if (delay > 0) {
            SDL_Delay(delay);
        }
        startFrame = endFrame;
    }
    GUI_running = false;
    GUI_Log( "GUI stop.\n" );
}

void GUI_RendererError( void )
{
    GUI_Log( "Uninitialized Renderer access denied.\n" );
}

char _docDir[512];
const char *GUI_getDocumentDirectory()
{
#if __IPHONEOS__
    strcpy( _docDir, __iOS_DOCUMENTS_FOLDER );
#elif __MACOSX__
    struct passwd *pwx = getpwuid(getuid());
    
    const char *homedir = pwx->pw_dir;
    
    strcpy( _docDir, homedir );
#elif __ANDROID__
    strcpy( _docDir, "/mnt/sdcard/PhaThai" );
#else
    getcwd(_docDir,512);
#endif
    return _docDir;
}

SDL_Color sdl_color(Uint32 c) {
    SDL_Color sdl_c;
    sdl_c.r=c>>24 & 0xff;
    sdl_c.g=c>>16 & 0xff;
    sdl_c.b=c>>8 & 0xff;
    sdl_c.a=0xff;
    return sdl_c;
}

static const Uint32
pixdepth=16,
foreground= 0xcdba96ff,
background= 0x6495EDFF,
red       = 0xff0000ff,
green     = 0x00ff00ff,
dark      = 0x303030ff,
grey0     = 0x606060ff,
grey1     = 0x707070ff,
grey2     = 0x909090ff,
grey3     = 0xa0a0a0ff,
grey4     = 0xb0b0b0ff,
grey8     = 0xe9e9e9ff,
white     = 0xffffffff;

const SDL_Color
cGrey0       = sdl_color(grey0),
cGrey1       = sdl_color(0x808080ff),
cGrey2       = sdl_color(grey2),
cGrey3       = sdl_color(grey3),
cGrey4       = sdl_color(grey4),
cGrey5       = sdl_color(0xc0c0c0ff),
cGrey6       = sdl_color(0xd0d0d0ff),
cGrey7       = sdl_color(0xe0e0e0ff),
cGrey8       = sdl_color(grey8);

SDL_Color
cClear      = { 0,0,0,0 },
cNop        = { 0,0,0,1 },
cWhite      = sdl_color(white),
cBlack      = sdl_color(0xff),
cGrey       = cGrey5,
cRed        = sdl_color(red),
cGreen      = sdl_color(green),
cBlue       = sdl_color(0xffff),
cCyan       = sdl_color(0xffffff),
cMagenta    = sdl_color(0xff00ffff),
cYellow     = sdl_color(0xffff00ff),
cDarkGrey   = sdl_color(grey1),
cLightGrey  = cGrey7;

SDL_Color
cBorder     = cGrey0,
cBackground = sdl_color(background),
cForeground = sdl_color(foreground),
cPointer    = sdl_color(0xff7d7dff);

static void mapsympixel(SDL_Renderer *rend,int x, int y, int xc, int yc, int symnum) {
    switch(symnum){
        case 8:	// symmetric in all octents
            SDL_RenderDrawPoint(rend,y+xc, x+yc);
            SDL_RenderDrawPoint(rend,y+xc, -x+yc);
            SDL_RenderDrawPoint(rend,-y+xc, x+yc);
            SDL_RenderDrawPoint(rend,-y+xc, -x+yc);
            
        case 4 :	// symmetric in all quadrants
            SDL_RenderDrawPoint(rend,-x+xc, -y+yc);
            SDL_RenderDrawPoint(rend,-x+xc, y+yc);
            
        case 2 :	// symmetric about only X-axis
            SDL_RenderDrawPoint(rend,x+xc, -y+yc);
            SDL_RenderDrawPoint(rend,x+xc, y+yc);
            break;
            
        case 1 :	// symmetric about only Y-axis
            SDL_RenderDrawPoint(rend,-x+xc, y+yc);
            SDL_RenderDrawPoint(rend,x+xc, y+yc);
    }
}

static void mapsympixel2(SDL_Renderer *rend,int x, int y, int xc, int yc, int d, int symnum) {
    SDL_RenderDrawPoint( rend,  x+xc,  y+yc );
    SDL_RenderDrawPoint( rend, -x+xc,  y+yc );
    SDL_RenderDrawPoint( rend,  x+xc, -y+yc );
    SDL_RenderDrawPoint( rend, -x+xc, -y+yc );
    
    SDL_RenderDrawPoint( rend,  y+xc+d,  x+yc-d );
    SDL_RenderDrawPoint( rend, -y+xc-d,  x+yc-d );
    SDL_RenderDrawPoint( rend,  y+xc+d, -x+yc+d );
    SDL_RenderDrawPoint( rend, -y+xc-d, -x+yc+d );
    
}

void GUI_DrawLine( int x1, int y1, int x2, int y2, SDL_Color col) {
    if( !GUI_renderer ) {
        GUI_RendererError();
        return;
    }
    SDL_SetRenderDrawColor(GUI_renderer,col.r,col.g,col.b,0xff);
    SDL_RenderDrawLine(GUI_renderer, x1, y1, x2, y2 );
}

void GUI_DrawRect2( GUI_Rect *rect, SDL_Color col) {
    if( !GUI_renderer ) {
        GUI_RendererError();
        return;
    }
    SDL_SetRenderDrawColor(GUI_renderer,col.r,col.g,col.b,0xff);
    SDL_RenderDrawRect(GUI_renderer,rect);
}

void GUI_FillRect2( GUI_Rect *rect, SDL_Color col) {
    if( !GUI_renderer ) {
        GUI_RendererError();
        return;
    }
    SDL_SetRenderDrawColor(GUI_renderer,col.r,col.g,col.b,0xff);
    SDL_RenderFillRect(GUI_renderer,rect);
}



SDL_Texture *GUI_createPixmap(const char* pm_data[]) {
    int i,dx,dy,nr_col;
    char ch;
    sscanf(pm_data[0],"%d %d %d",&dx,&dy,&nr_col);
    SDL_Surface *pm=SDL_CreateRGBSurface(0,dx,dy,pixdepth,0,0,0,0);
    SDL_Renderer *rend=SDL_CreateSoftwareRenderer(pm);
    SDL_SetColorKey(pm,SDL_TRUE,0);
    //printf("dx=%d dy=%d nr_col=%d\n",dx,dy,nr_col);
    SDL_FillRect(pm,0,0); // default = None
    struct ColSym {
        char ch;
        Uint32 col;
        bool none;
    };
    ColSym *colsym=new ColSym[nr_col];
    for (i=0;i<nr_col;++i) {
        Uint32 col;
        bool none=false;
        char s[10];
        sscanf(pm_data[i+1],"%c c %c%6s",&colsym[i].ch,&ch,s); // expected "* c #606060" or "* c None"
        if (ch=='#')
            sscanf(s,"%x",&col);
        else if (ch=='N' && !strcmp(s,"one")) {
            col=0; none=true;
        }
        else {
            printf("unexpected char '%c' in xpm\n",ch); col=0;
        }
        if ((col & 0xff) < 8 && (col>>8 & 0xff) < 8 && (col>>16 & 0xff) < 8)  // to prevent transparent color
            col |= 8;
        colsym[i].col= col<<8 | 0xff;
        colsym[i].none=none;
    }
    for (int col=0;col<nr_col;++col) {
        ch=colsym[col].ch;
        SDL_Color scol=sdl_color(colsym[col].col);
        SDL_SetRenderDrawColor(rend,scol.r,scol.g,scol.b,0xFF);
        bool none=colsym[col].none;
        for (int y=0;y<dy;++y)
            for (int x=0;x<dx;++x)
                if (pm_data[y+nr_col+1][x]==ch && !none)
                    SDL_RenderDrawPoint(rend,x,y);
    }
    delete[] colsym;
    
    SDL_Texture *tex = SDL_CreateTextureFromSurface(GUI_renderer, pm );
    SDL_FreeSurface( pm );
    
    return tex;
}

/*
 * This is a 32-bit pixel function created with help from this
 * website: http://www.libsdl.org/intro.en/usingvideo.html
 *
 * You will need to make changes if you want it to work with
 * 8-, 16- or 24-bit surfaces.  Consult the above website for
 * more information.
 */
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
    *(Uint32 *)target_pixel = pixel;
}

void draw_line(SDL_Surface *surface, int x1, int y1, int x2, int y2, Uint32 pixel)
{
    if( x1 == x2 ) {
        for( int y=y1; y<=y2; y++ ) {
            Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x1 * 4;
            *(Uint32 *)target_pixel = pixel;
        }
    }
    else if( y1 == y2 ) {
        for( int x=x1; x<=x2; x++ ) {
            Uint8 *target_pixel = (Uint8 *)surface->pixels + y1 * surface->pitch + x * 4;
            *(Uint32 *)target_pixel = pixel;
        }
    }
}

static void srf_mapsympixel(SDL_Surface *srf,int x, int y, int xc, int yc, int symnum, Uint32 pixel) {
    switch(symnum){
        case 8:	// symmetric in all octents
            set_pixel(srf, y+xc,    x+yc,   pixel);
            set_pixel(srf, y+xc,   -x+yc-1, pixel);
            set_pixel(srf,-y+xc-1,  x+yc,   pixel);
            set_pixel(srf,-y+xc-1, -x+yc-1, pixel);
            
        case 4 :	// symmetric in all quadrants
            set_pixel(srf,-x+xc-1, -y+yc-1, pixel);
            set_pixel(srf,-x+xc-1, y+yc, pixel);
            
        case 2 :	// symmetric about only X-axis
            set_pixel(srf,x+xc, -y+yc-1, pixel);
            set_pixel(srf,x+xc, y+yc, pixel);
            break;
            
        case 1 :	// symmetric about only Y-axis
            set_pixel(srf,-x+xc-1, y+yc, pixel);
            set_pixel(srf,x+xc, y+yc, pixel);
    }
}

void GUI_FillCircle( int xc, int yc, int radius, SDL_Color col )
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif
    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, radius*2, radius*2, 32, rmask, gmask, bmask, amask);
    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
 
    
    int x, y,
    p = 0-radius;
    Uint32 uc = SDL_MapRGBA( surface->format, col.r, col.g, col.b, col.a );
    int xcc = radius;
    int ycc = radius;
    for( x=0, y=radius-1, p=2-radius; x<=y; x++) {
        draw_line( surface,  x+xcc,   -y+ycc-1,  x+xcc,   y+ycc, uc );
        draw_line( surface, -x+xcc-1, -y+ycc-1, -x+xcc-1, y+ycc, uc );
        draw_line( surface, -y+xcc-1, -x+ycc-1, -y+xcc-1, x+ycc, uc );
        draw_line( surface,  y+xcc,   -x+ycc-1,  y+xcc,   x+ycc, uc );
        if (p>=0) {
            p += (2*x - 2*y + 1);
            y--;
        }
        else
            p += (2*x + 3);
    }
    
    
    SDL_Texture *tx = SDL_CreateTextureFromSurface(GUI_renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_RenderCopy( GUI_renderer, tx, NULL, GUI_MakeRect(xc-radius, yc-radius, radius*2, radius*2));
    SDL_DestroyTexture( tx );
}

void GUI_DrawCircle( int xc, int yc, int radius, SDL_Color col )
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif
    SDL_SetRenderDrawColor( GUI_renderer, col.r, col.g, col.b, 0xff );

    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, radius*2, radius*2, 32, rmask, gmask, bmask, amask);
    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
    
    
    int x, y,
    p = 0-radius;
    Uint32 uc = SDL_MapRGBA( surface->format, col.r, col.g, col.b, col.a );
    for( x=0, y=radius-1, p=2-radius; x<=y; x++) {
        srf_mapsympixel( surface, x, y, radius, radius, 8, uc );
        if (p>=0) {
            p += (2*x - 2*y + 1);
            y--;
        }
        else
            p += (2*x + 3);
    }
    
    
    SDL_Texture *tx = SDL_CreateTextureFromSurface(GUI_renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_RenderCopy( GUI_renderer, tx, NULL, GUI_MakeRect(xc-radius, yc-radius, radius*2, radius*2));
    SDL_DestroyTexture( tx );
}

static void srf_mapsympixel2(SDL_Surface *srf,int x, int y, int w, int h, int xc, int yc, int symnum, Uint32 pixel) {
    switch(symnum){
        case 8:	// symmetric in all octents
            set_pixel(srf, y+xc+w,    x+yc+h,   pixel);
            set_pixel(srf, y+xc+w,   -x+yc-1, pixel);
            set_pixel(srf,-y+xc-1,  x+yc+h,   pixel);
            set_pixel(srf,-y+xc-1, -x+yc-1, pixel);
            
        case 4 :	// symmetric in all quadrants
            set_pixel(srf,-x+xc-1, -y+yc-1, pixel);
            set_pixel(srf,-x+xc-1, y+yc+h, pixel);
            
        case 2 :	// symmetric about only X-axis
            set_pixel(srf,x+xc+w, -y+yc-1, pixel);
            set_pixel(srf,x+xc+w, y+yc+h, pixel);
            break;
            
        case 1 :	// symmetric about only Y-axis
            set_pixel(srf,-x+xc-1, y+yc, pixel);
            set_pixel(srf,x+xc+w, y+yc, pixel);
    }
}

void GUI_DrawRoundRect( int _x, int _y, int w, int h, int radius, SDL_Color col )
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif
    SDL_SetRenderDrawColor( GUI_renderer, col.r, col.g, col.b, 0xff );
    
    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
    
    int x, y,
    p = 0-radius;
    Uint32 uc = SDL_MapRGBA( surface->format, col.r, col.g, col.b, col.a );
    for( x=0, y=radius-1, p=2-radius; x<=y; x++) {
        srf_mapsympixel2( surface, x, y, w-radius-radius, h-radius-radius, radius, radius, 8, uc );
        if (p>=0) {
            p += (2*x - 2*y + 1);
            y--;
        }
        else
            p += (2*x + 3);
    }
    draw_line( surface, radius, 0, w-radius, 0, uc );
    draw_line( surface, radius, h-1, w-radius, h-1, uc );
    draw_line( surface, 0, radius, 0, h-radius, uc );
    draw_line( surface, w-1, radius, w-1, h-radius, uc );
    
    SDL_Texture *tx = SDL_CreateTextureFromSurface(GUI_renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_RenderCopy( GUI_renderer, tx, NULL, GUI_MakeRect(_x, _y, w, h));
    SDL_DestroyTexture( tx );
}


void GUI_FillRoundRect( int _x, int _y, int w, int h, int radius, SDL_Color col )
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif
    SDL_SetRenderDrawColor( GUI_renderer, col.r, col.g, col.b, 0xff );
    
    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
    if(surface == NULL) {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));

    int x, y,
    p = 0-radius;
    Uint32 uc = SDL_MapRGBA( surface->format, col.r, col.g, col.b, col.a );
    int xcc = radius;
    int ycc = radius;
    for( x=0, y=radius-1, p=2-radius; x<=y; x++) {
        draw_line( surface,  x+xcc+w-radius-radius,   -y+ycc-1,  x+xcc+w-radius-radius,   y+ycc+h-radius-radius, uc );
        draw_line( surface, -x+xcc-1, -y+ycc-1, -x+xcc-1, y+ycc+h-radius-radius, uc );
        draw_line( surface, -y+xcc-1, -x+ycc-1, -y+xcc-1, x+ycc+h-radius-radius, uc );
        draw_line( surface,  y+xcc+w-radius-radius,   -x+ycc-1,  y+xcc+w-radius-radius,   x+ycc+h-radius-radius, uc );
        if (p>=0) {
            p += (2*x - 2*y + 1);
            y--;
        }
        else
            p += (2*x + 3);
    }
    
    SDL_FillRect( surface, GUI_MakeRect( radius, 0, w-radius-radius, h), uc);
    
    
    SDL_Texture *tx = SDL_CreateTextureFromSurface(GUI_renderer, surface);
    SDL_FreeSurface(surface);
    
    SDL_RenderCopy( GUI_renderer, tx, NULL, GUI_MakeRect(_x, _y, w, h));
    SDL_DestroyTexture( tx );
}
