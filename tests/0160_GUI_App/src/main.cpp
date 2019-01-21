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

void createColumn1() {
    auto column2 = GUI_View::create(app->contentView, "COL2", 0, 0, 240, -1);
    column2->setLayout(GUI_LAYOUT_VERTICAL);
    column2->setMargin( 0, 0, 10, 10 );
    column2->setBackgroundColor(cClear);
    column2->border = 0;
    
    auto label5 = GUI_Label::create(column2, " ");
    label5->setMargin(10, 0, 0, 0 );
    label5->setBackgroundColor(cClear);
    
    auto list = GUI_List::create( column2, "List", 0, 0, -1, -1, [=](GUI_View *v) {
        GUI_List *l = (GUI_List *)v;
        GUI_Log( "Selected: %s\n", l->selectedItem->title.c_str() );
        label5->setTitle(l->selectedItem->title);
    } );
    list->setMargin( 10, 0, 0, 0 );
    list->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    
    list->addSimpleItem( "Simle List Item 1" );
    list->addSimpleItem( "Simle List Item 2" );
    list->addSimpleItem( "Simle List Item 3" );
}

int main(int argc, char *argv[]) {
    app = GUI_App::create( GUI_ORIENTATION_LANDSCAPE, "SDL-GUI App", expectedWidth, expectedHeight,
                          GUI_APP_TOP_BAR | GUI_APP_STATUS_BAR | GUI_APP_MENU | GUI_APP_MENUBAR );
    if( app == NULL ) {
        exit( 1 );
    }
    topView = app->topView;
    
    app->menuBar->addSimpleMenu( "File" );
    app->menuBar->addSimpleMenu( "Edit" );
    app->menuBar->addSimpleMenu( "View" );
    app->menuBar->addSimpleMenu( "Format" );
    app->menuBar->addSimpleMenu( "Help" );

    app->menuView->setCallback( [=](GUI_View *v) {
        GUI_Menu *l = (GUI_Menu *)v;
        GUI_Log( "%s\n", l->selectedItem->title.c_str() );
        l->close( GUI_AppMenuCollapseTime );
    } );
    
    app->menuView->addSimpleMenu( "About app...", true );
    app->menuView->addSimpleMenu( "Simle List Item 2" );
    app->menuView->addSimpleMenu( "Simle List Item 3", true );
    app->menuView->addSimpleMenu( "Quit", true );
    
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
    
    createColumn1();

    app->run();
    

    delete( app );

    return 0;
}




