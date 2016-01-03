//
//  main.c
//  OSX
//
//  Created by Panutat Tejasen on 10/14/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_gui.h"
#include "GUI_TopWin.h"
#include "GUI_MainWin.h"
#include "GUI_utils.h"

static GUI_TopWin *topWin = NULL;

void createTestWindows( int x, int y, const char *title, SDL_Color bgcol ) {
    GUI_MainWin *redWin = new GUI_MainWin(topWin, title, x, y, 320, 240+32, bgcol);
    redWin->border = 1;
    
    GUI_WinBase *greenWin = new GUI_WinBase(redWin, "Green", 20, 20, 280, 200, cGreen);
    greenWin->border = 1;
    greenWin->canMove = true;

    GUI_WinBase *whiteWin = new GUI_WinBase(redWin, "White", 10, 10, 120, 90, cWhite);
    whiteWin->border = 1;
    whiteWin->canMove = true;
    

    
    GUI_WinBase *blueWin = new GUI_WinBase(greenWin, "Blue", 20, 20, 240, 160, cBlue);
    blueWin->border = 1;
    blueWin->canMove = true;
    
    GUI_WinBase *greyWin = new GUI_WinBase(greenWin, "Grey", 10, 10, 120, 90, cGrey);
    greyWin->border = 1;
    greyWin->canMove = true;
    
    GUI_WinBase *yellowWin = new GUI_WinBase(blueWin, "Yellow", 10, 10, 120, 90, cYellow);
    yellowWin->border = 1;
    yellowWin->canMove = true;
    
    GUI_WinBase *cyanWin = new GUI_WinBase(blueWin, "Cyan", 20, 20, 120, 90, cCyan);
    cyanWin->border = 1;
    cyanWin->canMove = true;

}

int
main(int argc, char *argv[])
{
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        GUI_Log("Could not initialize SDL");
    }
    if (TTF_Init() != 0){
        GUI_Log( "TTF_Init failed.");
        SDL_Quit();
        return 1;
    }
    if( GUI_Init() != 0 ) {
        GUI_Log( "GUI_Init failed." );
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    int sx = 720, sy = 405;
    SDL_Window *window = GUI_CreateWindow( "GUI", sx, sy );
    if (window == NULL) {
        fatalError("GUI_CreateWindow Error");
        return 1;
    }
    
    SCREEN_WIDTH = GUI_windowWidth;
    SCREEN_HEIGHT = GUI_windowHeight;
    
    topWin=new GUI_TopWin( "TopWin", GUI_Rect(10,10,GUI_windowWidth-20,GUI_windowHeight-20),0,
       []( GUI_WinBase *tw ) {
           //tw->clear();
       }
    );
    
    createTestWindows( 20, 20, "Red", cRed );
    createTestWindows( 420, 60, "Dark", cDarkGrey );
    createTestWindows( 220, 225, "Magenta", cMagenta );
    
    GUI_Run();
    
    /* shutdown SDL */
    GUI_Quit();
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
