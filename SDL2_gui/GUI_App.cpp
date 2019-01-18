//
//  GUI_App.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_App.h"


GUI_App *GUI_App::create( int Orientation, std::string title, int expectedWidth, int expectedHeight, int options ) {
    GUI_App *app = new GUI_App( Orientation, title, expectedWidth, expectedHeight, options );
    if( app->topView == NULL ) {
        delete app;
        app = NULL;
    }
    return app;
}

GUI_App::GUI_App( int Orientation, std::string title, int expectedWidth, int expectedHeight, int options ) :
topView(NULL),
width(0),
height(0)
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
    
    if( options & GUI_APP_TOP_BAR ) {
        createTopBar();
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

void GUI_App::createTopBar() {
    topBar = GUI_TopBar::create( topView, title.c_str() );
}
