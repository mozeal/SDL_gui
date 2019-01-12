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

void createAbsoluteViews();
void createHorizontalViews();
void createVerticalViews();

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
    topView->setMargin(10, 10, 10, 10);
    topView->setPadding(5, 5, 5, 5);

    createAbsoluteViews();
    createHorizontalViews();
    createVerticalViews();

    GUI_Run();
    GUI_Destroy();

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}


void createAbsoluteViews() {
    GUI_View *vGrey = GUI_View::create( topView, "Gray", 0, 0, 500, 400 );
    vGrey->backgroundColor = cGrey;
    vGrey->setMargin( 5, 5, 5, 5 );
    vGrey->setPadding( 5, 5, 5, 5 );
    vGrey->layout = GUI_LAYOUT_ABSOLUTE;
    vGrey->dragable = true;
    
    GUI_TextView *vRedX = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX->backgroundColor = cRed;
    vRedX->dragable = true;
    vRedX->align = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRedX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRedX->setMargin( 5, 5, 5, 5 );
    vRedX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX->backgroundColor = cGreen;
    vGreenX->dragable = true;
    vGreenX->align = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->setMargin( 5, 5, 5, 5 );
    vGreenX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX->backgroundColor = cBlue;
    vBlueX->dragable = true;
    vBlueX->align = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->setMargin( 5, 5, 5, 5 );
    vBlueX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vRedX2 = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX2->backgroundColor = cRed;
    vRedX2->dragable = true;
    vRedX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->setMargin( 5, 5, 5, 5 );
    vRedX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX2 = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX2->backgroundColor = cGreen;
    vGreenX2->dragable = true;
    vGreenX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->setMargin( 5, 5, 5, 5 );
    vGreenX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX2 = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX2->backgroundColor = cBlue;
    vBlueX2->dragable = true;
    vBlueX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->setMargin( 5, 5, 5, 5 );
    vBlueX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vRedX3 = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX3->backgroundColor = cRed;
    vRedX3->dragable = true;
    vRedX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->setMargin( 5, 5, 5, 5 );
    vRedX3->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX3 = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX3->backgroundColor = cGreen;
    vGreenX3->dragable = true;
    vGreenX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->setMargin( 5, 5, 5, 5 );
    vGreenX3->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX3 = GUI_TextView::create( vGrey, "ไก่กี่ปีเป่าปี่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX3->backgroundColor = cBlue;
    vBlueX3->dragable = true;
    vBlueX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->setMargin( 5, 5, 5, 5 );
    vBlueX3->setPadding( 5, 5, 5, 5 );
}

void createHorizontalViews() {
    GUI_View *vGrey = GUI_View::create( topView, "Gray", 50, 50, 500, 400 );
    vGrey->backgroundColor = cGrey;
    vGrey->setMargin( 5, 5, 5, 5 );
    vGrey->setPadding( 5, 5, 5, 5 );
    vGrey->layout = GUI_LAYOUT_HORIZONTAL;
    vGrey->dragable = true;
    
    GUI_TextView *vRedX = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX->backgroundColor = cRed;
    vRedX->dragable = true;
    vRedX->align = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRedX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRedX->setMargin( 5, 5, 5, 5 );
    vRedX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX->backgroundColor = cGreen;
    vGreenX->dragable = true;
    vGreenX->align = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->setMargin( 5, 5, 5, 5 );
    vGreenX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX->backgroundColor = cBlue;
    vBlueX->dragable = true;
    vBlueX->align = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->setMargin( 5, 5, 5, 5 );
    vBlueX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vRedX2 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX2->backgroundColor = cRed;
    vRedX2->dragable = true;
    vRedX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->setMargin( 5, 5, 5, 5 );
    vRedX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX2 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX2->backgroundColor = cGreen;
    vGreenX2->dragable = true;
    vGreenX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->setMargin( 5, 5, 5, 5 );
    vGreenX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX2s = GUI_TextView::create( vGrey, "ไก่กา", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX2s->backgroundColor = cGreen;
    vGreenX2s->dragable = true;
    vGreenX2s->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2s->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2s->setMargin( 5, 5, 5, 5 );
    vGreenX2s->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX2 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX2->backgroundColor = cBlue;
    vBlueX2->dragable = true;
    vBlueX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->setMargin( 5, 5, 5, 5 );
    vBlueX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vRedX3 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX3->backgroundColor = cRed;
    vRedX3->dragable = true;
    vRedX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->setMargin( 5, 5, 5, 5 );
    vRedX3->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX3 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX3->backgroundColor = cGreen;
    vGreenX3->dragable = true;
    vGreenX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->setMargin( 5, 5, 5, 5 );
    vGreenX3->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX3 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX3->backgroundColor = cBlue;
    vBlueX3->dragable = true;
    vBlueX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->setMargin( 5, 5, 5, 5 );
    vBlueX3->setPadding( 5, 5, 5, 5 );
}

void createVerticalViews() {
    GUI_View *vGrey = GUI_View::create( topView, "Gray", 100, 100, 500, 400 );
    vGrey->backgroundColor = cGrey;
    vGrey->setMargin( 5, 5, 5, 5 );
    vGrey->setPadding( 5, 5, 5, 5 );
    vGrey->layout = GUI_LAYOUT_VERTICAL;
    vGrey->dragable = true;
    
    GUI_TextView *vRedX = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX->backgroundColor = cRed;
    vRedX->dragable = true;
    vRedX->align = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRedX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_LEFT;
    vRedX->setMargin( 5, 5, 5, 5 );
    vRedX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX->backgroundColor = cGreen;
    vGreenX->dragable = true;
    vGreenX->align = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_CENTER;
    vGreenX->setMargin( 5, 5, 5, 5 );
    vGreenX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX->backgroundColor = cBlue;
    vBlueX->dragable = true;
    vBlueX->align = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->contentAlign = GUI_ALIGN_TOP | GUI_ALIGN_RIGHT;
    vBlueX->setMargin( 5, 5, 5, 5 );
    vBlueX->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vRedX2 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX2->backgroundColor = cRed;
    vRedX2->dragable = true;
    vRedX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_LEFT;
    vRedX2->setMargin( 5, 5, 5, 5 );
    vRedX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX2 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX2->backgroundColor = cGreen;
    vGreenX2->dragable = true;
    vGreenX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2->setMargin( 5, 5, 5, 5 );
    vGreenX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX2s = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX2s->backgroundColor = cGreen;
    vGreenX2s->dragable = true;
    vGreenX2s->align = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2s->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_CENTER;
    vGreenX2s->setMargin( 5, 5, 5, 5 );
    vGreenX2s->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX2 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX2->backgroundColor = cBlue;
    vBlueX2->dragable = true;
    vBlueX2->align = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->contentAlign = GUI_ALIGN_VCENTER | GUI_ALIGN_RIGHT;
    vBlueX2->setMargin( 5, 5, 5, 5 );
    vBlueX2->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vRedX3 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vRedX3->backgroundColor = cRed;
    vRedX3->dragable = true;
    vRedX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_LEFT;
    vRedX3->setMargin( 5, 5, 5, 5 );
    vRedX3->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vGreenX3 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vGreenX3->backgroundColor = cGreen;
    vGreenX3->dragable = true;
    vGreenX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_CENTER;
    vGreenX3->setMargin( 5, 5, 5, 5 );
    vGreenX3->setPadding( 5, 5, 5, 5 );
    
    GUI_TextView *vBlueX3 = GUI_TextView::create( vGrey, "ไก่", "Kanit-Light.ttf", 16, 0, 0, 0, 0 );
    vBlueX3->backgroundColor = cBlue;
    vBlueX3->dragable = true;
    vBlueX3->align = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->contentAlign = GUI_ALIGN_BOTTOM | GUI_ALIGN_RIGHT;
    vBlueX3->setMargin( 5, 5, 5, 5 );
    vBlueX3->setPadding( 5, 5, 5, 5 );
}
