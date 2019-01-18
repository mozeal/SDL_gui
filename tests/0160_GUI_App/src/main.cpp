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
                          GUI_APP_TOP_BAR );
    if( app == NULL ) {
        exit( 1 );
    }
    topView = app->topView;
    
    
    /*
    auto column1 = GUI_View::create(topView, "COL1", 0, 0, -1, -1);
    column1->setLayout(GUI_LAYOUT_VERTICAL);
    column1->corner = 3;
    column1->setMargin(10,10,10,10);
    column1->setPadding( 5, 0, 5, 0 );
    column1->click_to_top = false;
    */



    app->run();
    

    delete( app );

    return 0;
}




