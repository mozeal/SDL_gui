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
    
    if (TTF_Init() != 0) {
        GUI_Log("TTF_Init failed.");
        SDL_Quit();
        return 1;
    }

    // Create Window
    SDL_Log("request: %d %d\n", expectedWidth, expectedHeight);
    GUI_Init("GUI_TextView", expectedWidth, expectedHeight );
    
    topView = GUI_createTopView("TopView", 0, 0, -1, -1);
    topView->setMargin(0, 0, 0, 0);
    topView->setPadding(5, 5, 5, 5);
    topView->setLayout( GUI_LAYOUT_VERTICAL );
    
    auto label = GUI_Label::create(topView, "This label for visible testing");
    
    auto button1 = GUI_Button::create(topView, "Visible");
    button1->setBackgroundColor(cCyan);

    GUI_Run();
    GUI_Destroy();

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




