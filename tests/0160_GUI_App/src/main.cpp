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

static GUI_View *inputBar;
static GUI_EditText *inputBox;

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
    
    auto addButton = GUI_Button::create(app->topBar, NULL, kIcon_solid_plus);
    addButton->setAlign( GUI_ALIGN_RIGHT | GUI_ALIGN_VCENTER );
    addButton->setMargin(0, 10, 0, 0 );
    addButton->setCallback([=](GUI_View *bt) {
    });
    
    app->contentView->setLayout(GUI_LAYOUT_VERTICAL);
    inputBar = GUI_View::create(app->contentView, "InputBar", 0, 0, -1, 0);
    inputBar->setPadding( 5, 10, 5, 10 );
    inputBox = GUI_EditText::create(inputBar, "", 0, 0, -1, 32);
    inputBox->border = 0;

    app->run();
    

    delete( app );

    return 0;
}




