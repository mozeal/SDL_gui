//
//  SDL_gui.cpp
//  TestSDLimage
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "SDL_gui.h"

int GUI_physicalWindowWidth = 0;
int GUI_physicalWindowHeight = 0;

int GUI_windowWidth = 0;
int GUI_windowHeight = 0;

float GUI_scale = 1;

static bool done = false;
SDL_Renderer *GUI_renderer = NULL;
SDL_Window *GUI_window = NULL;
GUI_View *GUI_topView = NULL;

static std::function<bool(SDL_Event* ev)> user_handle_events = NULL;

const long MILLESECONDS_PER_FRAME = 1000/60;
static Uint32 timer_start = 0;
static float frameCount = 0;


static void GUI_Loop();
static void handle_events(SDL_Event *ev);

int GUI_Init( SDL_Window *in_window, SDL_Renderer *in_renderer ) {
    GUI_window = in_window;
    GUI_renderer = in_renderer;
    
    SDL_GetWindowSize(GUI_window, &GUI_windowWidth, &GUI_windowHeight);
    SDL_Log("given: %d %d\n", GUI_windowWidth, GUI_windowHeight);
    
    SDL_GL_GetDrawableSize(GUI_window, &GUI_physicalWindowWidth, &GUI_physicalWindowHeight);
    SDL_Log( "Drawable: %i %i\n", GUI_physicalWindowWidth, GUI_physicalWindowHeight );
    
#ifdef __ANDROID__
    // Android always get fullscreen with no retina
    int scalex = drawableWidth / expectedWidth;
    int scaley = drawableHeight / expectedHeight;
#else
    int scalex = GUI_physicalWindowWidth / GUI_windowWidth;
    int scaley = GUI_physicalWindowHeight / GUI_windowHeight;
#endif
    
    GUI_scale = (float)((scalex < scaley) ? scaley : scalex);
    if (GUI_scale < 1.0f) {
        GUI_scale = 1.0f;
    }
    SDL_Log( "Scale: %0.2f\n", GUI_scale );
#ifdef __ANDROID__
    SCREEN_WIDTH = drawableWidth / scale;
    SCREEN_HEIGHT = drawableHeight / scale;
#endif
    SDL_Log("virtual: %d %d\n", GUI_windowWidth, GUI_windowHeight);
    
    return 0;
}

void GUI_Run(std::function<bool(SDL_Event* ev)> user_handle_ev) {
    user_handle_events = user_handle_ev;
    
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





void GUI_Error(const char* fn, int result) {
    GUI_Log("SDL_gui ERROR: \"%s\" error: %x (%d).\n", fn, result, result);
}

static void GUI_Render() {
    /* fill background in with red */
    SDL_SetRenderDrawColor(GUI_renderer, 0x64, 0x95, 0xED, 255); // Corn flower blue
    SDL_RenderClear(GUI_renderer);
    
    /* update screen */
    SDL_RenderPresent(GUI_renderer);
}

static void GUI_Loop() {
#ifdef __EMSCRIPTEN__
    SDL_Rect rect = {0, 0, GUI_physicalWindowWidth, GUI_physicalWindowHeight};
    SDL_RenderSetViewport(GUI_renderer, &rect);
    SDL_RenderSetClipRect(GUI_renderer, &rect);
    
    SDL_SetRenderDrawColor(GUI_renderer, 0x64, 0x95, 0xed, 0xff);
    SDL_RenderClear(GUI_renderer);
#else
    SDL_RenderSetViewport(GUI_renderer, NULL);
    SDL_RenderSetClipRect(GUI_renderer, NULL);
    
    SDL_SetRenderDrawColor(GUI_renderer, 0x64, 0x95, 0xed, 0xff);
    SDL_RenderClear(GUI_renderer);
#endif
    
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
                        GUI_windowWidth = event.window.data1;
                        GUI_windowHeight = event.window.data2;

                        int drawableWidth = 0;
                        int drawableHeight = 0;
                        SDL_GL_GetDrawableSize(GUI_window, &drawableWidth, &drawableHeight);
                        SDL_Log( "Drawable: %i %i\n", drawableWidth, drawableHeight );

                        #ifdef __ANDROID__
                        // Android always get fullscreen with no retina
                        int scalex = drawableWidth / expectedWidth;
                        int scaley = drawableHeight / expectedHeight;
                        #else
                        int scalex = drawableWidth / GUI_windowWidth;
                        int scaley = drawableHeight / GUI_windowHeight;
                        #endif
                        GUI_scale = (float)((scalex < scaley) ? scaley : scalex);
                        if (GUI_scale < 1.0f) {
                         GUI_scale = 1.0f;
                        }
                        SDL_Log( "Scale: %0.2f\n", GUI_scale );
                        #ifdef __ANDROID__
                        SCREEN_WIDTH = drawableWidth / scale;
                        SCREEN_HEIGHT = drawableHeight / scale;
                        #endif
                        SDL_Log("virtual: %d %d\n", GUI_windowWidth, GUI_windowHeight);
                        
                        
                        break;
                }
                break;
        }
        handle_events(&event);
    }
    event.type = GUI_EventPaint;
    handle_events(&event);
    
    /* update screen */
    SDL_RenderPresent(GUI_renderer);
    
    Uint32 duration = SDL_GetTicks()-timer_start;
    float fps = frameCount / duration * 1000.0f;
    //printf( "FPS: %0.2f\n", fps );
}

static void handle_events(SDL_Event *ev) {
    if (user_handle_events) {
        if (user_handle_events(ev))
            return;
    }
    if( GUI_topView ) {
        if( GUI_topView->eventHandler(ev) )
            return;
    }
}

GUI_View *GUI_createTopView(const char* t, int x, int y, int w, int h,
                             std::function<bool(SDL_Event* ev)>userEventHandler) {
    if (GUI_topView) {
        GUI_Log("GUI_TopView existed");
        printf("ERROR: GUI_TopView existed.");
        exit(1);
    }
    
    GUI_topView = GUI_View::createView(NULL,  t, x, y, w, h, userEventHandler);
    
    if (w == -1) {
        w = GUI_windowWidth - x;
    }
    
    if (h == -1) {
        h = GUI_windowHeight - y;
    }
    
    //GUI_topView->resize(w, h);
    //GUI_topView->setMargin(0, 0, 0, 0);
    //GUI_topView->setPadding(0, 0, 0, 0);
    //GUI_topView->setBorder(0);
    //GUI_topView->bgcol = cWhite;
    
    return GUI_topView;
}


