#include <iostream>
#include <SDL.h>

#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#include <SDL_syswm.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;

float scale = 1;

const long MILLESECONDS_PER_FRAME = 1000/60;

SDL_Renderer *renderer = NULL;

bool done = false;
void doLoop();

static SDL_Texture *texture = 0;

SDL_Texture *LoadTexture( std::string imagePath )
{
    /* load the bmp */
    SDL_Surface *bmp_surface;

    SDL_Log( "Load texture: %s\n", imagePath.c_str() );
    bmp_surface = SDL_LoadBMP(imagePath.c_str());
    if (bmp_surface == NULL) {
        SDL_Log("ERROR: could not load bmp\n");
        //exit( 1 );
    }
    /* set white to transparent on the happyface */
    SDL_SetColorKey(bmp_surface, 1,
                    SDL_MapRGB(bmp_surface->format, 255, 255, 255));

    /* convert RGBA surface to texture */
    SDL_Texture *tx = SDL_CreateTextureFromSurface(renderer, bmp_surface);
    if (tx == 0) {
        SDL_Log("ERROR: could not create texture");
        exit( 1 );
    }
    SDL_SetTextureBlendMode(tx, SDL_BLENDMODE_BLEND);

    /* free up allocated memory */
    SDL_FreeSurface(bmp_surface);

    return tx;
}

int main(int argc, char *argv[]) {
    cout << "Hello, World!" << endl;
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "Portrait LandscapeLeft LandscapeRight");

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Could not initialize SDL\n");
        exit(1);
    }

    // Get Sccreen size
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        return 1;
    }
    SDL_Log("Display: %d %d\n", dm.w, dm.h);

    // Create Window
    int sx = 640, sy = 480;
    SDL_Log("picked: %d %d\n", sx, sy);

    //Now create a window with title "SDL" at 0, 0 on the screen with w:800 h:600 and show it
    // ::SDL_WINDOW_FULLSCREEN,    ::SDL_WINDOW_OPENGL,
    // ::SDL_WINDOW_HIDDEN,        ::SDL_WINDOW_BORDERLESS,
    // ::SDL_WINDOW_RESIZABLE,     ::SDL_WINDOW_MAXIMIZED,
    // ::SDL_WINDOW_MINIMIZED,     ::SDL_WINDOW_INPUT_GRABBED,
    // ::SDL_WINDOW_ALLOW_HIGHDPI.
#if defined(__IPHONEOS__) || defined(__ANDROID__)
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_BORDERLESS;
#else
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE;
#endif
    int expectedWidth = (dm.w - sx) / 2;
    int expectedHeight = (dm.h - sy) / 2;
    SDL_Window *window = SDL_CreateWindow("0020_LoadTexture", expectedWidth, expectedHeight, sx, sy, style);

    if (window == NULL) {
        SDL_Log("SDL_CreateRenderer Error\n");
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

	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    SDL_Log("given: %d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);

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

    int drawableWidth = 0;
    int drawableHeight = 0;
    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);

#ifdef __ANDROID__
    int scalex = drawableWidth / sx;
    int scaley = drawableHeight / sy;
#else
    int scalex = drawableWidth / SCREEN_WIDTH;
    int scaley = drawableHeight / SCREEN_HEIGHT;
#endif
    scale = (float)((scalex < scaley) ? scaley : scalex);
    if (scale < 1.0f) {
        scale = 1.0f;
    }

    std::string imagePath = "data/icon.bmp";
    texture = LoadTexture(imagePath);

#ifdef __EMSCRIPTEN__
    // void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
    emscripten_set_main_loop(doLoop, 0, 1);
#else
    // main loop
    Uint32 startFrame;
    Uint32 endFrame;
    int delay;

    while (!done) {
        startFrame = SDL_GetTicks();

        doLoop();

        endFrame = SDL_GetTicks();

        /* figure out how much time we have left, and then sleep */
        delay = MILLESECONDS_PER_FRAME - (endFrame - startFrame);
        if (delay < 0) {
            delay = 0;
        } else if (delay > MILLESECONDS_PER_FRAME) {
            delay = MILLESECONDS_PER_FRAME;
        }
        SDL_Delay(delay);
    }
#endif

    //Clean up our objects and quit
    SDL_DestroyTexture( texture );

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}

void render(SDL_Renderer *renderer) {
    /* fill background in with red */
    SDL_SetRenderDrawColor(renderer, 0x64, 0x95, 0xED, 255); // Corn flower blue
    SDL_RenderClear(renderer);

    //Draw the texture
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    /* update screen */
    SDL_RenderPresent(renderer);
}

void doLoop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch( event.type ) {
            case SDL_QUIT:
                done = 1;
                break;

            case SDL_WINDOWEVENT:
                switch (event.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                    SDL_Log( "Event: Window Resized: %i, %i\n", event.window.data1, event.window.data2 );
                    SCREEN_WIDTH = event.window.data1;
                    SCREEN_HEIGHT = event.window.data2;
                    break;
            }
                break;
        }
    }
    render(renderer);
}
