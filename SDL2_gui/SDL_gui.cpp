//
//  SDL_gui.cpp
//  TestSDLimage
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "SDL_gui.h"

static int SCREEN_WIDTH = 0;
static int SCREEN_HEIGHT = 0;

static int drawableWidth = 0;
static int drawableHeight = 0;

static float scale = 1;

static bool done = false;
static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;

const long MILLESECONDS_PER_FRAME = 1000/60;
static Uint32 timer_start = 0;
static float frameCount = 0;

void GUI_Loop();

int GUI_Init( SDL_Window *in_window, SDL_Renderer *in_renderer ) {
    window = in_window;
    renderer = in_renderer;
    
    SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
    SDL_Log("given: %d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
    SDL_Log( "Drawable: %i %i\n", drawableWidth, drawableHeight );
    
#ifdef __ANDROID__
    // Android always get fullscreen with no retina
    int scalex = drawableWidth / expectedWidth;
    int scaley = drawableHeight / expectedHeight;
#else
    int scalex = drawableWidth / SCREEN_WIDTH;
    int scaley = drawableHeight / SCREEN_HEIGHT;
#endif
    
    scale = (float)((scalex < scaley) ? scaley : scalex);
    if (scale < 1.0f) {
        scale = 1.0f;
    }
    SDL_Log( "Scale: %0.2f\n", scale );
#ifdef __ANDROID__
    SCREEN_WIDTH = drawableWidth / scale;
    SCREEN_HEIGHT = drawableHeight / scale;
#endif
    SDL_Log("virtual: %d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    
    return 0;
}

void GUI_Run(std::function<bool(SDL_Event* ev)> user_handle_ev) {
#ifdef __EMSCRIPTEN__
    // void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);
    emscripten_set_main_loop(GUI_Loop, 0, 1);
#else
    timer_start = SDL_GetTicks();
    // main loop
    Uint32 startFrame = 0;
    Uint32 endFrame = 0;
    int delay;
    
    //Uint32 timer_start = SDL_GetTicks();
    while (!done) {
        startFrame = SDL_GetTicks();
        GUI_Loop();
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
}

void GUI_Destroy() {
    
}

void GUI_Render() {
    /* fill background in with red */
    SDL_SetRenderDrawColor(renderer, 0x64, 0x95, 0xED, 255); // Corn flower blue
    SDL_RenderClear(renderer);
    
    /* update screen */
    SDL_RenderPresent(renderer);
}

void GUI_Loop() {
    frameCount++;
    
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
                     
                     int drawableWidth = 0;
                     int drawableHeight = 0;
                     SDL_GL_GetDrawableSize(window, &drawableWidth, &drawableHeight);
                     SDL_Log( "Drawable: %i %i\n", drawableWidth, drawableHeight );
                     
                     #ifdef __ANDROID__
                     // Android always get fullscreen with no retina
                     int scalex = drawableWidth / expectedWidth;
                     int scaley = drawableHeight / expectedHeight;
                     #else
                     int scalex = drawableWidth / SCREEN_WIDTH;
                     int scaley = drawableHeight / SCREEN_HEIGHT;
                     #endif
                     scale = (float)((scalex < scaley) ? scaley : scalex);
                     if (scale < 1.0f) {
                     scale = 1.0f;
                     }
                     SDL_Log( "Scale: %0.2f\n", scale );
                     #ifdef __ANDROID__
                     SCREEN_WIDTH = drawableWidth / scale;
                     SCREEN_HEIGHT = drawableHeight / scale;
                     #endif
                     SDL_Log("virtual: %d %d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
                     
                    
                    break;
            }
                break;
        }
    }
    GUI_Render();
    
    Uint32 duration = SDL_GetTicks()-timer_start;
    float fps = frameCount / duration * 1000.0f;
    //printf( "FPS: %0.2f\n", fps );
}
