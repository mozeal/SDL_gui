//
//  GUI_App.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_App.h"

GUI_App *GUI_App::instance = NULL;

GUI_App *GUI_App::create( int Orientation, std::string title, int expectedWidth, int expectedHeight, int options ) {
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

    SDL_Log( "Hello, World!\n" );
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
    
    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL Init failed\n" );
        return;
    }
    
    if (IMG_Init(0) != 0) {
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL_image Init failed\n" );
        SDL_Quit();
        return;
    }
    
    if (TTF_Init() != 0) {
        SDL_LogError( SDL_LOG_CATEGORY_SYSTEM, "SDL_ttf Init failed\n" );
        IMG_Quit();
        SDL_Quit();
        return;
    }
    
    GUI_Init( (options & GUI_APP_TOP_BAR) ? "" : title.c_str(), expectedWidth, expectedHeight );
    
    width = GUI_windowWidth;
    height = GUI_windowHeight;
    
    topView = GUI_createTopView("TopView", 0, 0, -1, -1);
    topView->setMargin(0, 0, 0, 0);
    topView->setPadding(0,0,0,0);
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
    GUI_Destroy();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void GUI_App::run() {
    GUI_Run();
}

void GUI_App::createTopBar( int options ) {
    topBar = GUI_TopBar::create( topView, title.c_str() );
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
    contentView->setBorder( 1 );
    contentView->setBackgroundColor(cEmptyContent);
    contentView->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
}

void GUI_App::createMenu( int options ) {
    int y = GUI_AppTopBarHeight;
#if defined( __IPHONEOS__ )
    y += getiOSStatusBarHeight();
#endif
    
    menuView = GUI_Menu::create(topView, "Menu", 0, y, GUI_AppMenuWidth, -1);
    menuView->setAlign( GUI_ALIGN_ABSOLUTE );
    menuView->setActivateView(menuButton);
    menuView->setBackgroundColor(cDarkGrey);
    
    menuView->close();
    
    menuButton = GUI_Button::create(topBar->contentView, NULL, kIcon_solid_bars);
    menuButton->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    menuButton->setMargin(0, 0, 0, 10 );
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
