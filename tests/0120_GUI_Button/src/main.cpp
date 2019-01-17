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

GUI_Label *label;

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
    
    label = GUI_Label::create(topView, "This label for visible testing");
    
    auto button1 = GUI_Button::create(topView, "Visible", kIcon_solid_eye, 0, 0, 120, 0, [=](GUI_View *v) {
        //GUI_Button *bt = (GUI_Button *)v;
        label->setVisible(true);
        topView->updateLayout();
    } );
    button1->setBackgroundColor(cCyan);
    button1->setMargin( 10, 10, 10, 10 );
    
    auto button2 = GUI_Button::create(topView, "Hidden", kIcon_solid_eye_slash, 0, 0, 120, 0, [=](GUI_View *v) {
        label->setVisible(false);
        topView->updateLayout();
    } );
    button2->setBackgroundColor(cBlue);
    button2->setTextColor(cWhite);
    button2->setMargin( 0, 10, 10, 10 );
    
    auto button3 = GUI_Button::create(topView, "Toggle", kIcon_solid_exchange_alt, 0, 0, 120, 0, [=](GUI_View *v) {
        label->setVisible(!label->isVisible());
        topView->updateLayout();
    } );
    button3->setBackgroundColor(cWhite);
    button3->setTextColor(cBlack);
    button3->setMargin( 0, 10, 10, 10 );

    
    auto button4 = GUI_Button::create(topView, "Enable", kIcon_solid_thumbs_up, 0, 0, 120, 0, [=](GUI_View *v) {
    } );
    button4->setBackgroundColor(cWhite);
    button4->setTextColor(cBlack);
    button4->setMargin( 10, 10, 10, 10 );

    auto button5 = GUI_Button::create(topView, "Toggle", kIcon_solid_exchange_alt, 0, 0, 120, 0, [=](GUI_View *v) {
        button4->setEnable(!button4->isEnable());
        if( button4->isEnable() ) {
            button4->setTitle("Enable");
            button4->setIcon( kIcon_solid_thumbs_up );
        }
        else {
            button4->setTitle("Disabled");
            button4->setIcon( kIcon_solid_thumbs_down );
        }
    } );
    button5->setBackgroundColor(cRed);
    button5->setTextColor(cWhite);
    button5->setMargin( 0, 10, 10, 10 );

    
    
    GUI_Run();
    GUI_Destroy();

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




