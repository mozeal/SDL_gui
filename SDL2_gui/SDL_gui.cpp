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

int GUI_expectedWidth = 0;
int GUI_expectedHeight = 0;

float GUI_scale = 1;
float GUI_mouseScale = 1;

static bool done = false;
SDL_Renderer *GUI_renderer = NULL;
SDL_Window *GUI_window = NULL;
GUI_View *GUI_topView = NULL;
GUI_View * _GUI_mouseCapturedView = NULL;

static std::function<bool(SDL_Event* ev)> user_handle_events = NULL;

const long MILLESECONDS_PER_FRAME = 1000/60;
static Uint32 timer_start = 0;
static float frameCount = 0;


static void GUI_Loop();
static void handle_events(SDL_Event *ev);

static std::vector<GUI_UserMessage> user_message_queue;

static int statusBarHeight = 0;

int GUI_Init( const char* title, int expectedWidth, int expectedHeight ) {
    // Get Sccreen size
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        exit(1);
    }
    SDL_Log("Display: %d %d\n", dm.w, dm.h);
    
    //Now create a window with title "SDL" at 0, 0 on the screen with w:800 h:600 and show it
    // ::SDL_WINDOW_FULLSCREEN,    ::SDL_WINDOW_OPENGL,
    // ::SDL_WINDOW_HIDDEN,        ::SDL_WINDOW_BORDERLESS,
    // ::SDL_WINDOW_RESIZABLE,     ::SDL_WINDOW_MAXIMIZED,
    // ::SDL_WINDOW_MINIMIZED,     ::SDL_WINDOW_INPUT_GRABBED,
    // ::SDL_WINDOW_ALLOW_HIGHDPI.
#if defined(__IPHONEOS__)
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI;
    int cx = 0;
    int cy = 0;
#elif defined(__ANDROID__)
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE; // SDL_WINDOW_RESIZABLE to enable sensor orientation
    int cx = 0;
    int cy = 0;
#elif defined(__EMSCRIPTEN__)
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_BORDERLESS;
    int cx = 0;
    int cy = 0;
#else
    int style = SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE;
    int cx = (dm.w - expectedWidth) / 2;
    int cy = (dm.h - expectedHeight) / 2;
#endif
    GUI_window = SDL_CreateWindow(title, cx, cy, expectedWidth, expectedHeight, style);

    if (GUI_window == NULL) {
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
    GUI_renderer = SDL_CreateRenderer(GUI_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (GUI_renderer == NULL) {
        SDL_Log("SDL_CreateRenderer Error\n");
        exit(1);
    }


    
    SDL_GetWindowSize(GUI_window, &GUI_windowWidth, &GUI_windowHeight);
    SDL_Log("given: %d %d\n", GUI_windowWidth, GUI_windowHeight);

    if( expectedWidth )
        GUI_expectedWidth = expectedWidth;
    else
        GUI_expectedWidth = GUI_windowWidth;

    if( expectedHeight )
        GUI_expectedHeight = expectedHeight;
    else
        GUI_expectedHeight = GUI_windowHeight;

    SDL_Log( "!!! Expected: %i %i\n", GUI_expectedWidth, GUI_expectedHeight );
    GUI_updateScaleParameters();
    
    return 0;
}

void GUI_updateScaleParameters() {
    SDL_GL_GetDrawableSize(GUI_window, &GUI_physicalWindowWidth, &GUI_physicalWindowHeight);
    SDL_Log( "Drawable: %i %i\n", GUI_physicalWindowWidth, GUI_physicalWindowHeight );

    int scalex = 1;
    int scaley = 1;
#ifdef __ANDROID__
    SDL_Log( "Expected: %i %i\n", GUI_expectedWidth, GUI_expectedHeight );
    // Android always get fullscreen with no retina

    if( ((GUI_physicalWindowWidth > GUI_physicalWindowHeight) && (GUI_expectedWidth < GUI_expectedHeight)) ||
            ((GUI_physicalWindowWidth < GUI_physicalWindowHeight) && (GUI_expectedWidth > GUI_expectedHeight)) ) {
        int t = GUI_expectedWidth;
        GUI_expectedWidth = GUI_expectedHeight;
        GUI_expectedHeight = t;
    }
    SDL_Log( "Expected: %i %i\n", GUI_expectedWidth, GUI_expectedHeight );
    scalex = GUI_physicalWindowWidth / GUI_expectedWidth;
    scaley = GUI_physicalWindowHeight / GUI_expectedHeight;

    SDL_Log( "Calc scale: %i %i\n", scalex, scaley );
#else
    scalex = GUI_physicalWindowWidth / GUI_windowWidth;
    scaley = GUI_physicalWindowHeight / GUI_windowHeight;
#endif
    
    GUI_scale = (float)((scalex < scaley) ? scaley : scalex);
    if (GUI_scale < 1.0f) {
        GUI_scale = 1.0f;
    }
    SDL_Log( "Scale: %0.2f\n", GUI_scale );
    SDL_Log( "Mouse Scale: %0.2f\n", GUI_mouseScale );
#if defined(__ANDROID__) 
    GUI_windowWidth = GUI_physicalWindowWidth / GUI_scale;
    GUI_windowHeight = GUI_physicalWindowHeight / GUI_scale;
    GUI_mouseScale = GUI_scale;
    //GUI_mouseScale = 1.0;
#else
    if( GUI_windowWidth * GUI_scale != GUI_physicalWindowWidth ||
        GUI_windowHeight * GUI_scale != GUI_physicalWindowHeight ) {
        
        GUI_windowWidth = GUI_physicalWindowWidth / GUI_scale;
        GUI_windowHeight = GUI_physicalWindowHeight / GUI_scale;
    }
    GUI_mouseScale = GUI_scale;
#endif
    SDL_Log("virtual: %d %d\n", GUI_windowWidth, GUI_windowHeight);
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
    SDL_DestroyRenderer(GUI_renderer);
    SDL_DestroyWindow(GUI_window);
}

void GUI_Error(const char* fn, int result) {
    GUI_Log("SDL_gui ERROR: \"%s\" error: %x (%d).\n", fn, result, result);
}

static void GUI_Loop() {
    if( GUI_View::closeQueue.size() ) {
        for (std::vector<GUI_View *>::iterator it = GUI_View::closeQueue.end()-1 ; it >= GUI_View::closeQueue.begin(); --it) {
            GUI_View *child = *it;
            GUI_View::closeQueue.erase(it);
            delete(child);
        }
        GUI_View::closeQueue.clear();
    }
    
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
    
    if (user_message_queue.size() > 0) {
        GUI_UserMessage user_msg = user_message_queue.front();
        
        SDL_UserEvent user_event;
        user_event.type = user_msg.message_id;
        user_event.windowID = user_msg.parameter1;
        user_event.code = user_msg.parameter2;
        user_event.data1 = user_msg.extra_parameter1;
        user_event.data2 = user_msg.extra_parameter2;
        
        GUI_Log( "Dispatch: %i\n", user_event.type );
        handle_events((SDL_Event *)&user_event);
        
        user_message_queue.erase(user_message_queue.begin());
    }
    
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
                        //GUI_Log( "Event: Window Resized: %i, %i\n", event.window.data1, event.window.data2 );
                        break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        GUI_Log( "Event: Window size changed: %i, %i\n", event.window.data1, event.window.data2 );
#ifdef __ANDROID__
                        int ww = GUI_physicalWindowWidth;
                        int hh = GUI_physicalWindowHeight;
#endif
                        GUI_windowWidth = event.window.data1;
                        GUI_windowHeight = event.window.data2;
#ifdef __ANDROID__
                        if( ((GUI_windowWidth > GUI_windowHeight) && (ww < hh)) ||
                                ((GUI_windowWidth < GUI_windowHeight) && (ww > hh))) {
                            int t = ww;
                            ww = hh;
                            hh = t;
                        }
                        if( statusBarHeight == 0 ) {
                            statusBarHeight = (GUI_windowHeight-hh) > 0 ? 20 : 0;
                            GUI_Log( "Status Bar Height %i (%i-%i)\n", statusBarHeight, GUI_windowHeight, hh );
                        }
#endif
                        GUI_updateScaleParameters();

                        GUI_Fonts::clear();
                        GUI_PostMessage( GUI_FontChanged, 0, 0, NULL, NULL );
                        
                        GUI_PostMessage( GUI_UpdateSize, 0, 0, NULL, NULL );
                        
                        if( GUI_topView ) {
                            GUI_topView->updateLayout();
                        }
                        break;
                }
                break;
        }
        handle_events(&event);
    }


    event.type = GUI_EventUpdate;
    handle_events(&event);
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
    if (_GUI_mouseCapturedView) {
        switch (ev->type) {
            case SDL_FINGERDOWN:
            case SDL_MOUSEBUTTONDOWN:
                if (_GUI_mouseCapturedView->eventHandler(ev))
                    return;
                break;
            case SDL_FINGERMOTION:
            case SDL_MOUSEMOTION:
                if (_GUI_mouseCapturedView->eventHandler(ev))
                    return;
                break;
            case SDL_FINGERUP:
            case SDL_MOUSEBUTTONUP:
                if (_GUI_mouseCapturedView->eventHandler(ev))
                    return;
                break;
        }
    }
    if( GUI_topView ) {
        if( GUI_topView->eventHandler(ev) )
            return;
    }
}

void GUI_SetMouseCapture( GUI_View *v ) {
    if( v ) {
        if( v->isMouseCapturing )
            return;
    }
    if( _GUI_mouseCapturedView ) {
        _GUI_mouseCapturedView->isMouseCapturing = false;
        if( !v ) {
            //GUI_Log( "Kill capture %s\n", _GUI_mouseCapturedView->title.c_str());
        }
    }
    _GUI_mouseCapturedView = v;
    if( _GUI_mouseCapturedView ) {
        _GUI_mouseCapturedView->isMouseCapturing = true;
        GUI_Log( "Capture to %s\n", v->title.c_str());
    }
    else {
        // GUI_Log( "NO capture\n" );
    }
}

GUI_View * GUI_GetMouseCapture() {
    return _GUI_mouseCapturedView;
}

GUI_View *GUI_createTopView(const char* t, int x, int y, int w, int h,
                             std::function<bool(SDL_Event* ev)>userEventHandler) {
    if (GUI_topView) {
        GUI_Log("GUI_TopView existed");
        printf("ERROR: GUI_TopView existed.");
        exit(1);
    }
    
    GUI_topView = GUI_View::create(NULL,  t, x, y, w, h, userEventHandler);
    
    GUI_topView->focusable = true;
    
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

bool GUI_PostMessage(Uint32 msg, int param1, int param2, void *extra1, void *extra2) {
    GUI_UserMessage *user_msg = new GUI_UserMessage();
    user_msg->message_id = msg;
    user_msg->parameter1 = param1;
    user_msg->parameter2 = param2;
    user_msg->extra_parameter1 = extra1;
    user_msg->extra_parameter2 = extra2;
    
    user_message_queue.push_back(*user_msg);
    
    return true;
}

int GUI_GetStatusBarHeight() {
#if defined(__IPHONEOS__)
    return getiOSStatusBarHeight();
#endif
    return statusBarHeight;
}
