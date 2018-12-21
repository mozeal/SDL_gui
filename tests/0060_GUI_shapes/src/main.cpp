#include <SDL.h>
#include <iostream>
#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#include <SDL_syswm.h>
#endif
#include "SDL_gui.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

char title[] = "SDL_gui App";
int expectedWidth = 1024;
int expectedHeight = 768;

static SDL_Renderer *renderer = NULL;
static SDL_Window *window;


int main(int argc, char *argv[]) {
    SDL_Log( "Hello, World!\n" );
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Hello, World!\n" );
        exit(1);
    }

    // Get Sccreen size
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        exit(1);
    }
    SDL_Log("Display: %d %d\n", dm.w, dm.h);

    // Create Window
    SDL_Log("request: %d %d\n", expectedWidth, expectedHeight);

    //Now create a window with title "SDL" at 0, 0 on the screen with w:800 h:600 and show it
    // ::SDL_WINDOW_FULLSCREEN,    ::SDL_WINDOW_OPENGL,
    // ::SDL_WINDOW_HIDDEN,        ::SDL_WINDOW_BORDERLESS,
    // ::SDL_WINDOW_RESIZABLE,     ::SDL_WINDOW_MAXIMIZED,
    // ::SDL_WINDOW_MINIMIZED,     ::SDL_WINDOW_INPUT_GRABBED,
    // ::SDL_WINDOW_ALLOW_HIGHDPI.
#if defined(__IPHONEOS__) || defined(__ANDROID__) || defined(__EMSCRIPTEN__)
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_BORDERLESS;
    int cx = 0;
    int cy = 0;
#else
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE;
    int cx = (dm.w - expectedWidth) / 2;
    int cy = (dm.h - expectedHeight) / 2;
#endif
    window = SDL_CreateWindow(title, cx, cy, expectedWidth, expectedHeight, style);

    if (window == NULL) {
        printf("SDL_CreateRenderer Error\n");
        exit(1);
    }

#if defined(WIN32)
	HICON hicon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	SDL_SysWMinfo info = {0};
	SDL_VERSION(&info.version);
	SDL_GetWindowWMInfo(window, &info);
	HWND hwnd = info.info.win.window;
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hicon);
	SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hicon);
#endif



    //Create a renderer that will draw to the window, -1 specifies that we want to load whichever
    //video driver supports the flags we're passing
    //Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
    //SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
    //synchornized with the monitor's refresh rate
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_Log("SDL_CreateRenderer Error\n");
        exit(1);
    }

    GUI_Init(window, renderer);
    GUI_Run(
        [](SDL_Event *ev)  -> bool {
            switch( ev->type ) {
                case GUI_EventPaint:
                    int cx = GUI_windowWidth*GUI_scale/2, cy =GUI_windowHeight*GUI_scale/2;
                    
                    GUI_FillRect( cx-310, cy-17,  95, 44, cYellow );
                    GUI_DrawRect( cx-310, cy-17,  95, 44, cGreen );
                    GUI_FillRoundRect( cx-310, cy-17,  95, 44, 22, cCyan );
                    GUI_DrawRoundRect( cx-310, cy-17,  95, 44, 22, cBlack );
                    
                    GUI_FillRoundRect( cx-200, cy-32, 105, 66, 21, cCyan );
                    GUI_DrawRoundRect( cx-200, cy-32, 105, 66, 21, cBlack );
                    
                    GUI_FillRect( cx-80,  cy-40, 160, 80, cYellow );
                    GUI_DrawRect( cx-80,  cy-40, 160, 80, cGreen );
                    GUI_FillRoundRect( cx-80,  cy-40, 160, 80, 20, cCyan );
                    GUI_DrawRoundRect( cx-80,  cy-40, 160, 80, 20, cBlack );
                    
                    GUI_FillRoundRect( cx+95,  cy-32, 106, 66, 20, cCyan );
                    GUI_DrawRoundRect( cx+95,  cy-32, 106, 66, 20, cBlack );
                    
                    GUI_FillRoundRect( cx+215, cy-17,  95, 44, 22, cCyan );
                    GUI_DrawRoundRect( cx+215, cy-17,  95, 44, 22, cBlack );
                    
                    GUI_FillRect( cx-45, cy-145, 90, 90, cYellow );
                    GUI_DrawRect( cx-45, cy-145, 90, 90, cGreen );
                    
                    
                    
                    GUI_FillCircle( cx, cy-100, 45, cCyan );
                    GUI_DrawCircle( cx, cy-100, 45, cBlack );
                    int r = 45;
                    int ccx = 0;
                    for( int i=0; i<6; i++ ) {
                        ccx += r;
                        r -= 7;
                        ccx += r+6;
                        //GUI_Log( "R: %i\n", r );
                        GUI_FillCircle( cx+ccx, cy-100, r, cCyan );
                        GUI_DrawCircle( cx+ccx, cy-100, r, cBlack );
                        GUI_FillCircle( cx-ccx, cy-100, r, cCyan );
                        GUI_DrawCircle( cx-ccx, cy-100, r, cBlack );
                    }
                    
                    GUI_FillRect( cx-310, cy-17+100,  95, 44, cCyan );
                    GUI_DrawRect( cx-310, cy-17+100,  95, 44, cBlack );
                    GUI_FillRect( cx-200, cy-32+100, 105, 66, cCyan );
                    GUI_DrawRect( cx-200, cy-32+100, 105, 66, cBlack );
                    GUI_FillRect( cx-80,  cy-40+100, 160, 80, cCyan );
                    GUI_DrawRect( cx-80,  cy-40+100, 160, 80, cBlack );
                    GUI_FillRect( cx+95,  cy-32+100, 106, 66, cCyan );
                    GUI_DrawRect( cx+95,  cy-32+100, 106, 66, cBlack );
                    GUI_FillRect( cx+215, cy-17+100,  95, 43, cCyan );
                    GUI_DrawRect( cx+215, cy-17+100,  95, 43, cBlack );
                    
                    break;
            }
            return false;
        }
    );
    GUI_Destroy();
    
    //Clean up our objects and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




