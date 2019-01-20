#include <SDL.h>
#include <iostream>
#include <string>
#include <sstream>
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
    app = GUI_App::create( GUI_ORIENTATION_LANDSCAPE, "Mouse & Finger Test", expectedWidth, expectedHeight,
                          0 );
    if( app == NULL ) {
        exit( 1 );
    }
    topView = app->topView;
    topView->setLayout( GUI_LAYOUT_VERTICAL );
    topView->setPadding( 10, 10, 10, 10 );
    
    auto mousePanel = GUI_View::create(topView, "Mouse", 0, 0, -1, 0 );
    mousePanel->setLayout( GUI_LAYOUT_HORIZONTAL );
    mousePanel->setPadding( 5, 5, 5, 5 );
    mousePanel->corner = 4;

    auto mouseDownLabel = GUI_Label::create( mousePanel, "", 0, 0, 0, 200, 0 );
    mouseDownLabel->setMargin( 5, 5, 5, 5 );
    mouseDownLabel->setContentAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    auto mouseMotionLabel = GUI_Label::create( mousePanel, "", 0, 0, 0, 200, 0 );
    mouseMotionLabel->setMargin( 5, 5, 5, 5 );
    mouseMotionLabel->setContentAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    auto mouseUpLabel = GUI_Label::create( mousePanel, "", 0, 0, 0, 200, 0 );
    mouseUpLabel->setMargin( 5, 5, 5, 5 );
    mouseUpLabel->setContentAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    
    auto touchPanel = GUI_View::create(topView, "Touch", 0, 0, -1, 0 );
    touchPanel->setLayout( GUI_LAYOUT_HORIZONTAL );
    touchPanel->setPadding( 5, 5, 5, 5 );
    touchPanel->setMargin( 10, 0, 0, 0);
    touchPanel->corner = 4;
    
    auto touchDownLabel = GUI_Label::create( touchPanel, "", 0, 0, 0, 200, 0 );
    touchDownLabel->setMargin( 5, 5, 5, 5 );
    touchDownLabel->setContentAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    auto touchMotionLabel = GUI_Label::create( touchPanel, "", 0, 0, 0, 200, 0 );
    touchMotionLabel->setMargin( 5, 5, 5, 5 );
    touchMotionLabel->setContentAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );
    auto touchUpLabel = GUI_Label::create( touchPanel, "", 0, 0, 0, 200, 0 );
    touchUpLabel->setMargin( 5, 5, 5, 5 );
    touchUpLabel->setContentAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );

    auto edit = GUI_EditText::create(topView, "", 0, 0, -1, 0 );
    edit->setPadding( 10, 10, 10, 10 );
    edit->setMargin( 10, 0, 0, 0);


    topView->setUserEventHandler( [=](SDL_Event* event) -> bool {

        switch( event->type ) {

           case SDL_MOUSEBUTTONDOWN:
           {
               SDL_Log( "SDL_MOUSEBUTTONDOWN\n" );
               SDL_MouseButtonEvent e = event->button;

               int x = (int)(e.x*GUI_mouseScale);
               int y = (int)(e.y*GUI_mouseScale);

               std::ostringstream stringStream;
               stringStream << "Mouse Down " << x << ", " << y;
               std::string str = stringStream.str();
               mouseDownLabel->setTitle(str);
               mousePanel->updateLayout();
               break;
           }

           case SDL_MOUSEMOTION:
           {
#ifndef __ANDROID__
               GUI_Log( "SDL_MOUSEMOTION\n" );
               SDL_MouseButtonEvent e = event->button;

               int x = (int)(e.x*GUI_mouseScale);
               int y = (int)(e.y*GUI_mouseScale);

               std::ostringstream stringStream;
               stringStream << "Mouse Motion " << x << ", " << y;
               std::string str = stringStream.str();
               mouseMotionLabel->setTitle(str);
               mousePanel->updateLayout();
#endif
               break;
           }

           case SDL_MOUSEBUTTONUP:
           {
               SDL_Log( "SDL_MOUSEBUTTONUP\n" );
               SDL_MouseButtonEvent e = event->button;

               int x = (int)(e.x*GUI_mouseScale);
               int y = (int)(e.y*GUI_mouseScale);

               std::ostringstream stringStream;
               stringStream << "Mouse Up " << x << ", " << y;
               std::string str = stringStream.str();
               mouseUpLabel->setTitle(str);
               mousePanel->updateLayout();
               break;
           }

           case SDL_FINGERDOWN:
           {
               SDL_Log( "SDL_FINGERDOWN\n" );

               SDL_TouchFingerEvent e = event->tfinger;
               SDL_FingerID fid = e.fingerId;

               int x = (int)(e.x*GUI_windowWidth*GUI_mouseScale);
               int y = (int)(e.y*GUI_windowHeight*GUI_mouseScale);

               int n = SDL_GetNumTouchFingers(fid);

               std::ostringstream stringStream;
               stringStream << "Finger Down (" << n << ") " << x << ", " << y;
               std::string str = stringStream.str();
               touchDownLabel->setTitle(str);
               touchPanel->updateLayout();

               break;
           }

           case SDL_FINGERMOTION:
           {
#ifndef __ANDROID__
               SDL_Log( "SDL_FINGERMOTION\n" );

               SDL_TouchFingerEvent e = event->tfinger;


               int x = (int)(e.x*GUI_windowWidth*GUI_mouseScale);
               int y = (int)(e.y*GUI_windowHeight*GUI_mouseScale);



               std::ostringstream stringStream;
               stringStream << "Finger Motion " << x << ", " << y;
               std::string str = stringStream.str();
               SDL_Log( "%s\n", str.c_str() );
               touchMotionLabel->setTitle(str);
               touchPanel->updateLayout();
#endif
               break;
           }

           case SDL_FINGERUP:
           {
               SDL_Log( "SDL_FINGERUP\n" );
               SDL_TouchFingerEvent e = event->tfinger;
               SDL_FingerID fid = e.fingerId;

               int x = (int)(e.x*GUI_windowWidth*GUI_mouseScale);
               int y = (int)(e.y*GUI_windowHeight*GUI_mouseScale);

               int n = SDL_GetNumTouchFingers(fid);

               std::ostringstream stringStream;
               stringStream << "Finger Down (" << n << ") " << x << ", " << y;
               std::string str = stringStream.str();
               touchUpLabel->setTitle(str);
               touchPanel->updateLayout();
               break;
           }

           default:
               break;
       }

        return false;
    });


    app->run();
    

    delete( app );

    return 0;
}




