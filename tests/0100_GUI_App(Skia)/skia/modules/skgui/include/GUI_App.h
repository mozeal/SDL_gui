//
//  GUI_App.hpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 18/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_App_hpp
#define GUI_App_hpp

#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include "skgui.h"
#include "include/core/SkCanvas.h"

#define GUI_ORIENTATION_PORTRAIT    1
#define GUI_ORIENTATION_LANDSCAPE   2

#define GUI_APP_TOP_BAR             1
#define GUI_APP_STATUS_BAR          2
#define GUI_APP_CONTENT_VIEW        4
#define GUI_APP_MENU                8
#define GUI_APP_MENUBAR             16
#define GUI_APP_MENU_LEFT           32      // Button on the left animation from the left
#define GUI_APP_MENU_LEFT_TOP       64      // Button on the left animation from the top
#define GUI_APP_MENU_LEFT_BOTTOM    128      // Button on the left animation from the bottom
#define GUI_APP_MENU_RIGHT          256     // Button on the right animation from the right
#define GUI_APP_MENU_RIGHT_TOP      512     // Button on the right animation from the top
#define GUI_APP_MENU_RIGHT_BOTTOM   1024    // Button on the right animation from the bottom

#if defined( __ANDROID__ )
static int _expectedWidth = 360;
static int _expectedHeight = 600;
#elif defined( __IPHONEOS__ )
static int _expectedWidth = 360;
static int _expectedHeight = 600;
#elif defined( __MACOSX__ )
static int _expectedWidth = 1024;
static int _expectedHeight = 768;
#elif defined( __WIN32__ )
static int _expectedWidth = 1024;
static int _expectedHeight = 768;
#elif defined( __EMSCRIPTEN__ )
static int _expectedWidth = 360;
static int _expectedHeight = 600;
#else
static int _expectedWidth = 480;
static int _expectedHeight = 800;
#endif

class GUI_View;
class GUI_TopBar;
class GUI_StatusBar;
class GUI_Button;
class GUI_Menu;
class GUI_MenuBar;

class GUI_App {
protected:
    static GUI_App *instance;
public:
    void createTopBar( int options );
    void createMenuBar( int options );
    void createStatusBar( int options );
    void createContentView( int options );
    void createMenu( int options );

    bool isMenuShow;
public:
    static GUI_App *create( std::string title, int expectedWidth=0, int expectedHeight=0, int Orientation=0, int options=0 );
    GUI_App( int Orientation, std::string title, int expectedWidth, int expectedHeight, int options=0 );
    virtual ~GUI_App();

    std::string title;
    int width;
    int height;
    GUI_View *topView;

    GUI_TopBar *topBar;
    GUI_StatusBar *statusBar;
    GUI_View *contentView;
    GUI_Button *menuButton;
    GUI_Menu *menuView;
    GUI_MenuBar *menuBar;

    sk_sp<SkSurface> surface;

    static GUI_App *getInstance() { return instance; };

    float getPixelScreenCoordScale();
    SkCanvas * getCanvas(){ return surface->getCanvas(); }

    void run();
};

#endif /* GUI_App_hpp */
