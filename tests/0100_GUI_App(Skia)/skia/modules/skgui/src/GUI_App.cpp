//
//  GUI_App.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_App.h"
#if defined(SK_BUILD_FOR_ANDROID)
#include <GLES/gl.h>
#elif defined(SK_BUILD_FOR_UNIX)
#include <GL/gl.h>
#elif defined(SK_BUILD_FOR_MAC)
#include <OpenGL/gl.h>
#elif defined(SK_BUILD_FOR_IOS)
#include <OpenGLES/ES2/gl.h>
#endif

GUI_App *GUI_App::instance = NULL;

GUI_App *GUI_App::create( std::string title, int expectedWidth, int expectedHeight, int Orientation, int options ) {
#if defined( __ANDROID__ )
    int _orientation = 0; //GUI_ORIENTATION_PORTRAIT | GUI_ORIENTATION_LANDSCAPE;
    if( SDL_IsTablet() ) {
        GUI_Log( "Android: is Tablet\n" );
        _expectedWidth = 480;
        _expectedHeight = 800;
    }
    else {
        GUI_Log( "Android: is Phone\n" );
    }
#elif defined( __EMSCRIPTEN__ )
    _expectedWidth = -1;
    _expectedHeight = -1;
    int _orientation = 0;
#else
    int _orientation = GUI_ORIENTATION_PORTRAIT | GUI_ORIENTATION_LANDSCAPE;
#endif
    if( expectedWidth == 0 )
        expectedWidth = _expectedWidth;
    if( expectedHeight == 0 )
        expectedHeight = _expectedHeight;
    if( Orientation == 0 )
        Orientation = _orientation;

    if( instance ) {
        return instance;
    }
    GUI_App *app = new GUI_App( Orientation, title, expectedWidth, expectedHeight, options );
    if( app->topView == NULL ) {
        delete app;
        app = NULL;
    }
    instance = app;
    return app;
}

static void sdl_handle_error() {
    const char* error = SDL_GetError();
    SkDebugf("SDL Error: %s\n", error);
    SDL_ClearError();
}

GUI_App::GUI_App( int Orientation, std::string title, int expectedWidth, int expectedHeight, int options ) :
topView(NULL),
width(0),
height(0),
topBar(NULL),
statusBar(NULL),
contentView(NULL),
menuButton(NULL),
menuView(NULL),
menuBar(NULL),
isMenuShow(false)
{
    this->title = title;

    SDL_Log( "Hello, GUI!\n" );
    std::string orn = "";
    if( Orientation & GUI_ORIENTATION_PORTRAIT ) {
        orn = orn + "Portrait";
    }
    if( Orientation & GUI_ORIENTATION_LANDSCAPE ) {
        if( orn.length() )
            orn = orn + " ";
        orn = orn + "LandscapeLeft LandscapeRight";
    }
    SDL_SetHint(SDL_HINT_ORIENTATIONS, orn.c_str());
    
    
    // Setup SDL_GL Attribute
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#if defined(SK_BUILD_FOR_ANDROID) || defined(SK_BUILD_FOR_IOS)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    static const int kStencilBits = 8;  // Skia needs 8 stencil bits
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, kStencilBits);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    // If you want multisampling, uncomment the below lines and set a sample count
    static const int kMsaaSampleCount = 0; //4;
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, kMsaaSampleCount);
    
    

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL Init failed\n" );
        return;
    }

//    if (IMG_Init(0) != 0) {
//        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL_image Init failed\n" );
//        SDL_Quit();
//        return;
//    }
//
//    if (TTF_Init() != 0) {
//        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL_ttf Init failed\n" );
//        IMG_Quit();
//        SDL_Quit();
//        return;
//    }

    SDL_DisplayMode dm;

    if (SDL_GetDesktopDisplayMode(0, &dm) != 0) {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        dm.w = 0;
        dm.h = 0;
    }
    if( expectedWidth == -1 ) {
        expectedWidth = dm.w;
    }
    if( expectedHeight == -1 ) {
        expectedHeight = dm.h;
    }

    // Create Window and Init GUI
    GUI_Init( (options & GUI_APP_TOP_BAR) ? "" : title.c_str(), expectedWidth, expectedHeight );
    
    // Setup GL context
    GUI_glContext = SDL_GL_CreateContext(GUI_window);
    if (!GUI_glContext) {
        sdl_handle_error();
        return;
    }
    
    int success =  SDL_GL_MakeCurrent(GUI_window, GUI_glContext);
    if (success != 0) {
        sdl_handle_error();
        return;
    }
    
    int contextType, dw, dh;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &contextType);
    SDL_GL_GetDrawableSize(GUI_window, &dw, &dh);
    glViewport(0, 0, dw, dh);
    glClearColor(1, 1, 1, 1);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // setup GrContext
    auto interface = GrGLMakeNativeInterface();

    // setup contexts
    sk_sp<GrContext> grContext(GrContext::MakeGL(interface));
    SkASSERT(grContext);
    
    // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
    // render to it
    GrGLint buffer;
    GR_GL_GetIntegerv(interface.get(), GR_GL_FRAMEBUFFER_BINDING, &buffer);
    GrGLFramebufferInfo info;
    info.fFBOID = (GrGLuint) buffer;
    SkColorType colorType;
    
    uint32_t windowFormat = SDL_GetWindowPixelFormat(GUI_window);
    // TODO: the windowFormat is never any of these?
    if (SDL_PIXELFORMAT_RGBA8888 == windowFormat) {
        info.fFormat = GR_GL_RGBA8;
        colorType = kRGBA_8888_SkColorType;
    } else {
        colorType = kBGRA_8888_SkColorType;
        if (SDL_GL_CONTEXT_PROFILE_ES == contextType) {
            info.fFormat = GR_GL_BGRA8;
        } else {
            // We assume the internal format is RGBA8 on desktop GL
            info.fFormat = GR_GL_RGBA8;
        }
    }
    GrBackendRenderTarget target(dw, dh, kMsaaSampleCount, kStencilBits, info);
    
    // Setup SkSurface
    SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);
//    sk_sp<SkSurface> surface(SkSurface::MakeFromBackendRenderTarget(grContext.get(), target, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, &props));
    this->surface = SkSurface::MakeFromBackendRenderTarget(grContext.get(), target, kBottomLeft_GrSurfaceOrigin, colorType, nullptr, &props);
    

    width = GUI_windowWidth;
    height = GUI_windowHeight;

    topView = GUI_createTopView("TopView", 0, 0, -1, -1);
    topView->setMargin(0, 0, 0, 0);
    topView->setPadding(0,0,0,0);
    topView->setBackgroundColor(cCornFlowerBlue);
    topView->setLayout( GUI_LAYOUT_VERTICAL );

    if( (options & GUI_APP_TOP_BAR) || (options & GUI_APP_STATUS_BAR) ) {
        options |= GUI_APP_CONTENT_VIEW;
    }

    if( options & GUI_APP_TOP_BAR ) {
        createTopBar(options);
    }
    if( options & GUI_APP_MENUBAR ) {
        createMenuBar(options);
    }
    if( options & GUI_APP_CONTENT_VIEW ) {
        createContentView(options);
    }
    if( options & GUI_APP_STATUS_BAR ) {
        createStatusBar(options);
    }
    if( options & GUI_APP_MENU ) {
        createMenu(options);
    }
}

GUI_App::~GUI_App() {
    if (GUI_glContext) {
        SDL_GL_DeleteContext(GUI_glContext);
    }
    GUI_Destroy();
//    TTF_Quit();
//    IMG_Quit();
    SDL_Quit();
}

void GUI_App::run() {
    GUI_Run();
}

float GUI_App::getPixelScreenCoordScale(){
    // PixelRatio
    int framebufferW, framebufferH, tmpWindowW, tmpWindowH;
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    framebufferW = dims[2];
    framebufferH = dims[3];
    SDL_GetWindowSize(GUI_window, &tmpWindowW, &tmpWindowH);
    float pixelScreenCoordScale = 1.0;
    if( framebufferW != tmpWindowW){
        pixelScreenCoordScale = (float)framebufferW / (float)tmpWindowW;
        if( pixelScreenCoordScale < 1 ){
            pixelScreenCoordScale = 1;
        }
    }
    GUI_Log("pixel ratio: %.2f\n", pixelScreenCoordScale);
    return pixelScreenCoordScale;
}

void GUI_App::createTopBar( int options ) {
    topBar = GUI_TopBar::create( topView, title.c_str() );
    topBar->setSaftyPaddingFlag( 2 | 8 );
}

void GUI_App::createMenuBar( int options ) {
    menuBar = GUI_MenuBar::create( topView, title.c_str() );
}

void GUI_App::createStatusBar( int options ) {
    statusBar = GUI_StatusBar::create( topView, title.c_str() );
    topView->updateLayout();
}

void GUI_App::createContentView( int options ) {
    contentView = GUI_View::create( topView, "ContentView", 0, 0, -1, -1 );
    contentView->setBorder( 0 );
    contentView->setBackgroundColor(cWhite);
    contentView->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    contentView->setSaftyMarginFlag( 2 | 4 | 8 );
}

void GUI_App::createMenu( int options ) {
    int y = GUI_GetAppTopBarHeight()+GUI_GetStatusBarHeight();

    MenuAnimationMode mode(eFromLeft);
    if(options & GUI_APP_MENU_RIGHT)
        mode = eFromRight;
    else if(options & GUI_APP_MENU_LEFT_TOP)
        mode = eFromTop;
    else if(options & GUI_APP_MENU_LEFT_BOTTOM)
        mode = eFromBottom;
    else if(options & GUI_APP_MENU_RIGHT_TOP)
        mode = eFromTop;
    else if(options & GUI_APP_MENU_RIGHT_BOTTOM)
        mode = eFromBottom;

    menuView = GUI_Menu::create(topView, "Menu", 0, y, GUI_GetAppMenuWidth(), -1, nullptr, mode);
    menuView->setAlign( GUI_ALIGN_ABSOLUTE );
    menuView->setActivateView(menuButton);
    menuView->setBackgroundColor(cEmptyContent);

    menuView->close();

    menuButton = GUI_Button::create(topBar->contentView, NULL, kIcon_solid_bars);
    int align(GUI_ALIGN_VCENTER);
    if((options & GUI_APP_MENU_LEFT) || (options & GUI_APP_MENU_LEFT_TOP) || (options & GUI_APP_MENU_LEFT_BOTTOM))
        align |= GUI_ALIGN_LEFT;
    else
        align |= GUI_ALIGN_RIGHT;
    menuButton->setAlign(align);
    menuButton->setMargin(0, 0, 0, 5 );
    menuButton->setBackgroundColor( cClear );
    menuButton->setTextColor( cWhite );
    menuButton->setBorder( 0 );
    menuButton->showInteract = false;
    menuButton->setCallback([=](GUI_View *bt) {
        if( this->menuView->isOpen ) {
            this->menuView->close( GUI_AppMenuCollapseTime );
        }
        else {
            this->menuView->open( GUI_AppMenuCollapseTime );
        }
    });

    menuView->setActivateView(menuButton);
}
