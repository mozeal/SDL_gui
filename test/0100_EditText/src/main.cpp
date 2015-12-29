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
#include "GUI_BasicWidgets.h"
#include "GUI_utils.h"

static GUI_TopWin *topWin = NULL;
static GUI_MainWin *mainWin = NULL;
static GUI_MainWin *dialog = NULL;


int main(int argc, char *argv[])
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
    
    int sx = 360, sy = 640;
    

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
    
    topWin=new GUI_TopWin("TOP",GUI_Rect(0,0,GUI_windowWidth,GUI_windowHeight),0,
       []( GUI_WinBase *tw ) {
           //tw->clear();
       }
    );
    
    mainWin = new GUI_MainWin(topWin, "สวัสดีจียูไอ", 0, 0, GUI_windowWidth, GUI_windowHeight, cRed, 0,
        []( GUI_WinBase *w ) {
        }
    );
    mainWin->closeButton->hide();
    
    GUI_Label *label = new GUI_Label( mainWin, "Please enter text:", 10, 10 );
    GUI_EditText *edit = new GUI_EditText( mainWin, "Test Edit Text", 10, 35, GUI_windowWidth-20,
                                          []( GUI_EditText *bt ) {
                                              GUI_Log( "Button: %s\n", bt->title_str );
                                          }
                                          );
    GUI_Button *button = new GUI_Button( mainWin, "ปุ่มกด", 10, 80, 0, 0, cBlue,
        []( GUI_Button *bt ) {
            GUI_Log( "Button: %s\n", bt->title_str );
            if( !dialog ) {
                int w = GUI_windowWidth - 40;
                int h = w * 3/4;
                dialog = new GUI_MainWin( mainWin, "Dialog", 20, (GUI_windowHeight-h)/2, w, h, cGreen );
                dialog->canClose = false;
                
                GUI_Label *label = new GUI_Label( dialog, "Content", 10, 30, w-20 );
            }
            else {
                dialog->show();
            }
        }
    );
    button->tw_area.x = (GUI_windowWidth-button->tw_area.w)/2;

    GUI_Run();
    
    /* shutdown SDL */
    GUI_Quit();
    TTF_Quit();
    SDL_Quit();
    
    return 0;
    
}
