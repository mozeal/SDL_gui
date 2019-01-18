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

static GUI_App *app;
static GUI_View *topView;

int main(int argc, char *argv[]) {
    app = GUI_App::create( GUI_ORIENTATION_LANDSCAPE, "SDL-GUI App", expectedWidth, expectedHeight,
                          GUI_APP_TOP_BAR | GUI_APP_STATUS_BAR | GUI_APP_MENU );
    if( app == NULL ) {
        exit( 1 );
    }
    topView = app->topView;
    
    app->run();
    

    delete( app );

    return 0;
}




