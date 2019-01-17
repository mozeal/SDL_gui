#include <SDL.h>
#include <iostream>
#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#include <SDL_syswm.h>
#endif
#include "SDL_gui.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;

char title[] = "SDL_gui App";
int expectedWidth = 960;
int expectedHeight = 540;

static GUI_View *topView;

int main(int argc, char *argv[]) {
    SDL_Log( "Hello, World!\n" );
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Could not initialize SDL\n");
        SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Hello, World!\n" );
        exit(1);
    }

    // Create Window
    SDL_Log("request: %d %d\n", expectedWidth, expectedHeight);

    GUI_Init(title, expectedWidth, expectedHeight);
    
    topView = GUI_createTopView("TopView", 0, 0, -1, -1);
    topView->setMargin(10, 10, 10, 10);
    
    GUI_View *vRed = GUI_View::create( topView, "Red", 10, 10, 320, 240 );
    vRed->setBackgroundColor( cRed );
    vRed->dragable = true;
    vRed->showInteract = true;
    GUI_View *vRedC1 = GUI_View::create( vRed, "Green", 40, 40, 320, 240 );
    vRedC1->setBackgroundColor( cGreen );
    vRedC1->dragable = true;
    vRedC1->showInteract = true;
    GUI_View *vRedC2 = GUI_View::create( vRed, "Yellow", 30, 60, 160, 120 );
    vRedC2->setBackgroundColor( cYellow );
    vRedC2->dragable = true;
    vRedC2->showInteract = true;

    GUI_View *vBlue = GUI_View::create( topView, "Blue", 90, 90, 320, 240 );
    vBlue->setBackgroundColor( cBlue );
    vBlue->dragable = true;
    vBlue->showInteract = true;
    GUI_View *vBlueC1 = GUI_View::create( vBlue, "Magenta", 100, 100, 320, 240 );
    vBlueC1->setBackgroundColor( cMagenta );
    vBlueC1->dragable = true;
    vBlueC1->showInteract = true;
    GUI_View *vBlueC2 = GUI_View::create( vBlue, "Green2", 10, 10, 120, 120 );
    vBlueC2->setBackgroundColor( cGreen );
    vBlueC2->dragable = true;
    vBlueC2->showInteract = true;
    vBlueC2->mouseReceive = false; // Test click through

    GUI_View *vCyan = GUI_View::create( topView, "Cyan", 270, 270, 320, 240 );
    vCyan->setBackgroundColor( cCyan );
    vCyan->dragable = true;
    vCyan->showInteract = true;
    GUI_View *vCyanC1 = GUI_View::create( vCyan, "Grey", -70, -70, 320, 240 );
    vCyanC1->setBackgroundColor( cGrey );
    vCyanC1->dragable = true;
    vCyanC1->showInteract = true;

    GUI_Run();
    GUI_Destroy();

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




