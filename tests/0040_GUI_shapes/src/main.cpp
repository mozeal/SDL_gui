#include <SDL.h>
#include <iostream>
#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#endif
#include "SDL_gui.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

char title[] = "0040_GUI_shapes";

int expectedWidth = 960;
int expectedHeight = 540;



int main(int argc, char *argv[]) {
    SDL_Log( "Hello, World!\n" );
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Hello, World!\n" );
        exit(1);
    }

    // Get Sccreen size
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
        SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
        exit(1);
    }
    SDL_Log("Display: %d %d\n", dm.w, dm.h);

    // Create Window
    SDL_Log("request: %d %d\n", expectedWidth, expectedHeight);

    GUI_Init(title, expectedWidth, expectedHeight);

#if defined(WIN32)
    GUI_SetWindowIcon(IDI_ICON1);
#endif

    GUI_Run(
        [](SDL_Event *ev)  -> bool {
            switch( ev->type ) {
                case GUI_EventPaint:
                    
                    int cx = GUI_windowWidth*GUI_scale/2, cy =GUI_windowHeight*GUI_scale/2;
                    
                    GUI_FillRect( cx-310, cy-17,  95, 44, cYellow );
                    GUI_DrawRect( cx-310, cy-17,  95, 44, cGreen );
                    GUI_FillRoundRect( cx-310, cy-17,  95, 44, 22, cCyan );
                    GUI_DrawRoundRect( cx-310, cy-17,  95, 44, 22, cBlack );
                    
                    GUI_FillRoundRect( cx-200, cy-32, 105, 66, 21, cCyan );
                    GUI_DrawRoundRect( cx-200, cy-32, 105, 66, 21, cBlack );
                    
                    GUI_FillRect( cx-80,  cy-40, 160, 80, cYellow );
                    GUI_DrawRect( cx-80,  cy-40, 160, 80, cGreen );
                    GUI_FillRoundRect( cx-80,  cy-40, 160, 80, 20, cCyan );
                    GUI_DrawRoundRect( cx-80,  cy-40, 160, 80, 20, cBlack );
                    
                    GUI_FillRoundRect( cx+95,  cy-32, 106, 66, 20, cCyan );
                    GUI_DrawRoundRect( cx+95,  cy-32, 106, 66, 20, cBlack );
                    
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
                    
                    break;
            }
            return false;
        }
    );
    GUI_Destroy();


    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




