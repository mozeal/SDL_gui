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
#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#include <SDL_syswm.h>
#endif
#include "SDL_gui.h"

#define GUI_ORIENTATION_PORTRAIT    1
#define GUI_ORIENTATION_LANDSCAPE   2

#define GUI_APP_TOP_BAR           1
#define GUI_APP_STATUS_BAR          2
#define GUI_APP_CONTENT_VIEW        4
#define GUI_APP_MENU                8

class GUI_View;
class GUI_TopBar;
class GUI_StatusBar;
class GUI_Button;

class GUI_App {
protected:
    void createTopBar( int options );
    void createStatusBar( int options );
    void createContentView( int options );
    void createMenu( int options );
    
    bool isMenuShow;
public:
    static GUI_App *create( int Orientation, std::string title, int expectedWidth, int expectedHeight, int options=0 );
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
    GUI_View *menuView;
    
    void run();
};

#endif /* GUI_App_hpp */
