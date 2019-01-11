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

static SDL_Renderer *renderer = NULL;
static SDL_Window *window;
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
    GUI_Init("GUI_ImageView", expectedWidth, expectedHeight );
    
    topView = GUI_createTopView("TopView", 0, 0, -1, -1);
    topView->setMargin(10, 10, 10, 10);
    topView->setPadding(5, 5, 5, 5);
    
    GUI_View *vMain = GUI_View::create( topView, "Main1", 0, 0, 500, 270 );
    vMain->backgroundColor = cDarkGrey;
    vMain->setMargin( 5, 5, 5, 5 );
    vMain->setPadding( 5, 5, 5, 5 );
    vMain->dragable = true;
    
    GUI_ImageView *vRed = GUI_ImageView::create( vMain, "Top-Left", "logo.png", 0, 0, 150, 70 );
    vRed->backgroundColor = cRed;
    vRed->dragable = true;
    vRed->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRed->setMargin( 5, 5, 5, 5 );
    vRed->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vGreen = GUI_ImageView::create( vMain, "Top-Center", "logo.png", 0, 0, 150, 70 );
    vGreen->backgroundColor = cGreen;
    vGreen->dragable = true;
    vGreen->align = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreen->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreen->setMargin( 5, 5, 5, 5 );
    vGreen->setPadding( 5, 5, 5, 5 );

    GUI_ImageView *vBlue = GUI_ImageView::create( vMain, "Top-Right", "logo.png", 0, 0, 150, 70 );
    vBlue->backgroundColor = cBlue;
    vBlue->dragable = true;
    vBlue->align = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlue->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlue->setMargin( 5, 5, 5, 5 );
    vBlue->setPadding( 5, 5, 5, 5 );

    GUI_ImageView *vRed2 = GUI_ImageView::create( vMain, "Middle-Left", "logo.png", 0, 0, 150, 70 );
    vRed2->backgroundColor = cRed;
    vRed2->dragable = true;
    vRed2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRed2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRed2->setMargin( 5, 5, 5, 5 );
    vRed2->setPadding( 5, 5, 5, 5 );

    GUI_ImageView *vGreen2 = GUI_ImageView::create( vMain, "Middle-Center", "logo.png", 0, 0, 150, 70 );
    vGreen2->backgroundColor = cGreen;
    vGreen2->dragable = true;
    vGreen2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreen2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreen2->setMargin( 5, 5, 5, 5 );
    vGreen2->setPadding( 5, 5, 5, 5 );

    GUI_ImageView *vBlue2 = GUI_ImageView::create( vMain, "Middle-Right", "logo.png", 0, 0, 150, 70 );
    vBlue2->backgroundColor = cBlue;
    vBlue2->dragable = true;
    vBlue2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlue2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlue2->setMargin( 5, 5, 5, 5 );
    vBlue2->setPadding( 5, 5, 5, 5 );

    GUI_ImageView *vRed3 = GUI_ImageView::create( vMain, "Bottom-Left", "logo.png", 10, 230, 150, 70 );
    vRed3->backgroundColor = cRed;
    vRed3->dragable = true;
    vRed3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRed3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRed3->setMargin( 5, 5, 5, 5 );
    vRed3->setPadding( 5, 5, 5, 5 );

    GUI_ImageView *vGreen3 = GUI_ImageView::create( vMain, "Bottom-Center", "logo.png", 170, 230, 150, 70 );
    vGreen3->backgroundColor = cGreen;
    vGreen3->dragable = true;
    vGreen3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreen3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreen3->setMargin( 5, 5, 5, 5 );
    vGreen3->setPadding( 5, 5, 5, 5 );

    GUI_ImageView *vBlue3 = GUI_ImageView::create( vMain, "Bottom-Center", "logo.png", 330, 230, 150, 70 );
    vBlue3->backgroundColor = cBlue;
    vBlue3->dragable = true;
    vBlue3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlue3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlue3->setMargin( 5, 5, 5, 5 );
    vBlue3->setPadding( 5, 5, 5, 5 );

    GUI_View *vGrey = GUI_View::create( topView, "Gray", 520, 0, -1, -1 );
    vGrey->backgroundColor = cGrey;
    vGrey->setMargin( 5, 5, 5, 5 );
    vGrey->setPadding( 5, 5, 5, 5 );
    vGrey->dragable = true;
  
    GUI_ImageView *vRedX = GUI_ImageView::create( vGrey, "Top-Left", "logo.png", 0, 0, 0, 0 );
    vRedX->backgroundColor = cRed;
    vRedX->dragable = true;
    vRedX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRedX->setMargin( 5, 5, 5, 5 );
    vRedX->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vGreenX = GUI_ImageView::create( vGrey, "Top-Center", "logo.png", 0, 0, 0, 0 );
    vGreenX->backgroundColor = cGreen;
    vGreenX->dragable = true;
    vGreenX->align = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->setMargin( 5, 5, 5, 5 );
    vGreenX->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vBlueX = GUI_ImageView::create( vGrey, "Top-Right", "logo.png", 0, 0, 0, 0 );
    vBlueX->backgroundColor = cBlue;
    vBlueX->dragable = true;
    vBlueX->align = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->setMargin( 5, 5, 5, 5 );
    vBlueX->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vRedX2 = GUI_ImageView::create( vGrey, "Middle-Left", "logo.png", 0, 0, 0, 0 );
    vRedX2->backgroundColor = cRed;
    vRedX2->dragable = true;
    vRedX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->setMargin( 5, 5, 5, 5 );
    vRedX2->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vGreenX2 = GUI_ImageView::create( vGrey, "Middle-Center", "logo.png", 0, 0, 0, 0 );
    vGreenX2->backgroundColor = cGreen;
    vGreenX2->dragable = true;
    vGreenX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->setMargin( 5, 5, 5, 5 );
    vGreenX2->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vBlueX2 = GUI_ImageView::create( vGrey, "Middle-Right", "logo.png", 0, 0, 0, 0 );
    vBlueX2->backgroundColor = cBlue;
    vBlueX2->dragable = true;
    vBlueX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->setMargin( 5, 5, 5, 5 );
    vBlueX2->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vRedX3 = GUI_ImageView::create( vGrey, "Bottom-Left", "logo.png", 0, 0, 0, 0 );
    vRedX3->backgroundColor = cRed;
    vRedX3->dragable = true;
    vRedX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->setMargin( 5, 5, 5, 5 );
    vRedX3->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vGreenX3 = GUI_ImageView::create( vGrey, "Bottom-Center", "logo.png", 0, 0, 0, 0 );
    vGreenX3->backgroundColor = cGreen;
    vGreenX3->dragable = true;
    vGreenX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->setMargin( 5, 5, 5, 5 );
    vGreenX3->setPadding( 5, 5, 5, 5 );
    
    GUI_ImageView *vBlueX3 = GUI_ImageView::create( vGrey, "Middle-Right", "logo.png", 0, 0, 0, 0 );
    vBlueX3->backgroundColor = cBlue;
    vBlueX3->dragable = true;
    vBlueX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->setMargin( 5, 5, 5, 5 );
    vBlueX3->setPadding( 5, 5, 5, 5 );
    
    
    GUI_View *vGrey2 = GUI_View::create( topView, "Gray", 0, 290, 500, -1 );
    vGrey2->backgroundColor = cGrey;
    vGrey2->setMargin( 5, 5, 5, 5 );
    vGrey2->setPadding( 5, 5, 5, 5 );
    vGrey2->dragable = true;
    
    GUI_ImageView *vGreenXX2 = GUI_ImageView::create( vGrey2, "Middle-Center", "logo.png", 0, 0, -1, -1 );
    vGreenXX2->backgroundColor = cGreen;
    vGreenXX2->dragable = true;
    vGreenXX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenXX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenXX2->setMargin( 5, 5, 5, 5 );
    vGreenXX2->setPadding( 5, 5, 5, 5 );
    

    GUI_Run();
    GUI_Destroy();

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




