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
static GUI_View *topView;

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
    
    topView = GUI_createTopView("TopView", 0, 0, -1, -1);
    topView->setMargin(10, 10, 10, 10);
    
    GUI_View *vRed = GUI_View::createView( topView, "Red", 10, 10, 320, 240 );
    vRed->backgroundColor = cRed;
    GUI_View *vRedC1 = GUI_View::createView( vRed, "Green", 40, 40, 320, 240 );
    vRedC1->backgroundColor = cGreen;
    GUI_View *vRedC2 = GUI_View::createView( vRed, "Yellow", 30, 60, 160, 120 );
    vRedC2->backgroundColor = cYellow;

    GUI_View *vBlue = GUI_View::createView( topView, "Blue", 90, 90, 320, 240 );
    vBlue->backgroundColor = cBlue;
    GUI_View *vBlueC1 = GUI_View::createView( vBlue, "Magenta", 100, 100, 320, 240 );
    vBlueC1->backgroundColor = cMagenta;

    GUI_View *vCyan = GUI_View::createView( topView, "Cyan", 270, 270, 320, 240 );
    vCyan->backgroundColor = cCyan;
    GUI_View *vCyanC1 = GUI_View::createView( vCyan, "Grey", -70, -70, 320, 240 );
    vCyanC1->backgroundColor = cGrey;
    
    GUI_Run();
    GUI_Destroy();
    
    //Clean up our objects and quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




