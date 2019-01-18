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
    
    auto menu = GUI_Menu::create( app->menuView, "Menu", 0, 0, -1, 0, [=](GUI_View *v) {
        GUI_Menu *l = (GUI_Menu *)v;
    } );
    menu->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    
    menu->addSimpleMenu( "About app...", true );
    menu->addSimpleMenu( "Simle List Item 2" );
    menu->addSimpleMenu( "Simle List Item 3", true );
    menu->addSimpleMenu( "Quit", true );

    app->run();
    

    delete( app );

    return 0;
}




