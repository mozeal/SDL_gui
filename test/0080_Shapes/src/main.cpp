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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static GUI_TopWin *topWin = NULL;

int
main(int argc, char *argv[])
{
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        GUI_Log("Could not initialize SDL");
    }
    //Also need to init SDL_ttf
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
    
    int sx = 720, sy = 480;
    SDL_Window *window = GUI_CreateWindow( "GUI", sx, sy );
    if (window == NULL){
        fatalError("GUI_CreateWindow Error");
        return 1;
    }
    SCREEN_WIDTH = GUI_windowWidth;
    SCREEN_HEIGHT = GUI_windowHeight;
    
    SDL_Renderer *renderer = GUI_renderer;
    if (renderer == NULL){
        fatalError("SDL_CreateRenderer Error");
        return 1;
    }
    
    
    topWin=new GUI_TopWin("Hello",GUI_Rect(0,0,GUI_windowWidth,GUI_windowHeight),0,
        []( GUI_WinBase *tw ) {
        }
    );
    
    GUI_MainWin *mainWin = new GUI_MainWin(topWin, "Shapes", 20, 20, GUI_windowWidth-40, GUI_windowHeight-40, cRed, 1,
        []( GUI_WinBase *w ) {
            int cx = w->getWidth()/2, cy = w->getHeight()/2;
            
            GUI_FillRect( cx-310, cy-17,  95, 44, cYellow );
            GUI_DrawRect( cx-310, cy-17,  95, 44, cGreen );
            GUI_FillRoundRect( cx-310, cy-17,  95, 44, 22, cCyan );
            GUI_DrawRoundRect( cx-310, cy-17,  95, 44, 22, cBlack );

            //GUI_FillRect( cx-200, cy-32, 105, 66, cYellow );
            //GUI_DrawRect( cx-200, cy-32, 105, 66, cGreen );
            GUI_FillRoundRect( cx-200, cy-32, 105, 66, 21, cCyan );
            GUI_DrawRoundRect( cx-200, cy-32, 105, 66, 21, cBlack );

            GUI_FillRect( cx-80,  cy-40, 160, 80, cYellow );
            GUI_DrawRect( cx-80,  cy-40, 160, 80, cGreen );
            GUI_FillRoundRect( cx-80,  cy-40, 160, 80, 20, cCyan );
            GUI_DrawRoundRect( cx-80,  cy-40, 160, 80, 20, cBlack );

            //GUI_FillRect( cx+95,  cy-32, 106, 66, cYellow );
            //GUI_DrawRect( cx+95,  cy-32, 106, 66, cGreen );
            GUI_FillRoundRect( cx+95,  cy-32, 106, 66, 20, cCyan );
            GUI_DrawRoundRect( cx+95,  cy-32, 106, 66, 20, cBlack );

            //GUI_FillRect( cx+215, cy-17,  95, 44, cYellow );
            //GUI_DrawRect( cx+215, cy-17,  95, 44, cGreen );
            GUI_FillRoundRect( cx+215, cy-17,  95, 44, 22, cCyan );
            GUI_DrawRoundRect( cx+215, cy-17,  95, 44, 22, cBlack );

            GUI_FillRect( cx-45, cy-145, 90, 90, cYellow );
            GUI_DrawRect( cx-45, cy-145, 90, 90, cGreen );
            GUI_FillCircle( cx, cy-100, 45, cCyan );
            GUI_DrawCircle( cx, cy-100, 45, cBlack );
            int r = 45;
            int ccx = 0;
            for( int i=0; i<6; i++ ) {
                ccx += r;
                r -= 7;
                ccx += r+6;
                //GUI_Log( "R: %i\n", r );
                GUI_FillCircle( cx+ccx, cy-100, r, cCyan );
                GUI_DrawCircle( cx+ccx, cy-100, r, cBlack );
                GUI_FillCircle( cx-ccx, cy-100, r, cCyan );
                GUI_DrawCircle( cx-ccx, cy-100, r, cBlack );
            }
            GUI_FillRect( cx-310, cy-17+100,  95, 44, cCyan );
            GUI_DrawRect( cx-310, cy-17+100,  95, 44, cBlack );
            GUI_FillRect( cx-200, cy-32+100, 105, 66, cCyan );
            GUI_DrawRect( cx-200, cy-32+100, 105, 66, cBlack );
            GUI_FillRect( cx-80,  cy-40+100, 160, 80, cCyan );
            GUI_DrawRect( cx-80,  cy-40+100, 160, 80, cBlack );
            GUI_FillRect( cx+95,  cy-32+100, 106, 66, cCyan );
            GUI_DrawRect( cx+95,  cy-32+100, 106, 66, cBlack );
            GUI_FillRect( cx+215, cy-17+100,  95, 43, cCyan );
            GUI_DrawRect( cx+215, cy-17+100,  95, 43, cBlack );
        }
    );
    
    mainWin->canClose = false;
    mainWin->canMove = false;

    GUI_Run();
    
    /* shutdown SDL */
    GUI_Quit();
    TTF_Quit();
    SDL_Quit();
    
    return 0;
    
}
