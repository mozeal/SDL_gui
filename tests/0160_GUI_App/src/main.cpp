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

void createColumn1(GUI_View *subContentView) {
    auto column1 = GUI_View::create(subContentView, "COL2", 0, 0, 240, -1);
    column1->setLayout(GUI_LAYOUT_VERTICAL);
    column1->setMargin( 0, 0, 10, 10 );
    column1->setBackgroundColor(cClear);
    column1->border = 0;
    
    auto label1 = GUI_Label::create(column1, "Label 1");
    label1->setMargin(10, 0, 0, 0 );
    label1->setBackgroundColor(cClear);
    label1->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    
    auto list = GUI_List::create( column1, "List", 0, 0, -1, -1, [=](GUI_View *v) {
        GUI_List *l = (GUI_List *)v;
        GUI_Log( "Selected: %s\n", l->selectedItem->title.c_str() );
        label1->setTitle(l->selectedItem->title);
    } );
    list->setMargin( 10, 0, 0, 0 );
    list->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    
    list->addSimpleItem( "Simle List Item 1" );
    list->addSimpleItem( "Simle List Item 2" );
    list->addSimpleItem( "Simle List Item 3" );
    
    auto btn = GUI_Button::create( column1, "Button 1", 0, 0, 0, -1, 0 );
    btn->setMargin( 10, 0, 0, 0 );
    btn->corner = 6;
    
    auto label2 = GUI_Label::create(column1, "Label 2", 0, 0, 0, -1, 0 );
    
    label2->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    label2->setMargin(10, 0, 0, 0 );
    label2->setBackgroundColor(cGrey);
    label2->setPadding( 5, 10, 5, 10 );
    
    auto rbg = GUI_RadioButtonGroup::create( column1, "LigGroup", 0, 0, -1, 0, [=](GUI_View *v) {
        GUI_RadioButtonGroup *l = (GUI_RadioButtonGroup *)v;
        GUI_Log( "Selected: %s\n", l->selectedRadioButton->title.c_str() );
        label2->setTitle(l->selectedRadioButton->title);
    });
    rbg->border = 1;
    rbg->corner = 3;
    rbg->setPadding( 5, 10, 5, 10 );
    rbg->setMargin( 10, 0, 0, 0 );
    rbg->setBackgroundColor(cWhite);
    rbg->setLayout( GUI_LAYOUT_VERTICAL );
    
    rbg->add(GUI_RadioButton::create(NULL, "Radio 1") );
    rbg->add(GUI_RadioButton::create(NULL, "Radio 2") );
    rbg->add(GUI_RadioButton::create(NULL, "Radio 3") );
  
}

void createColumn2(GUI_View *subContentView) {
    auto column2 = GUI_View::create(subContentView, "COL2", 0, 0, 200, -1);
    column2->setLayout(GUI_LAYOUT_VERTICAL);
    column2->setMargin( 0, 0, 10, 10 );
    column2->setBackgroundColor(cClear);
    column2->border = 0;
    
    auto v1 = GUI_View::create(column2, "V1", 0, 0, -1, 0 );
    v1->setBackgroundColor(cWhite);
    v1->border = 1;
    v1->setPadding( 5, 10, 5, 10 );
    v1->setMargin( 10, 0, 0, 0 );
    v1->corner = 4;
    v1->setLayout( GUI_LAYOUT_VERTICAL );
    
    auto cb1 = GUI_CheckBox::create(v1, "Check Box 1", 0, 0, -1, 0 );
    auto cb2 = GUI_CheckBox::create(v1, "Check Box 1", 0, 0, -1, 0 );
    auto cb3 = GUI_CheckBox::create(v1, "Check Box 1", 0, 0, -1, 0 );
    

    
    auto horz1 = GUI_View::create(column2, "Horz", 0, 0, 0, 0);
    horz1->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_TOP );
    horz1->setLayout( GUI_LAYOUT_HORIZONTAL );
    horz1->setMargin( 10, 0, 0, 0 );
    horz1->setPadding( 5, 0, 5, 0 );
    
    auto bi1 = GUI_Button::create( horz1, NULL, kIcon_solid_dollar_sign );
    bi1->setMargin( 0, 5, 0, 5 );
    auto bi2 = GUI_Button::create( horz1, NULL, kIcon_solid_yen_sign );
    bi2->setMargin( 0, 5, 0, 5 );
    auto bi3 = GUI_Button::create( horz1, NULL, kIcon_solid_pound_sign );
    bi3->setMargin( 0, 5, 0, 5 );
    
    auto img = GUI_ImageView::create(column2, "Icon", "logo.png", 0, 0, -1, -1 );
    img->setMargin( 10, 0, 0, 0 );
    img->setPadding( 10, 0, 10, 0 );
    
    auto horz2 = GUI_View::create(column2, "Horz", 0, 0, -1, 0);
    horz2->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_TOP );
    horz2->setLayout( GUI_LAYOUT_HORIZONTAL );
    horz2->setMargin( 10, 0, 0, 0 );
    horz2->setPadding( 5, 0, 5, 0 );
    
    auto bi21 = GUI_Button::create( horz2, NULL, kIcon_solid_dollar_sign );
    bi21->setMargin( 0, 5, 0, 5 );
    bi21->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_TOP );
    auto bi22 = GUI_Button::create( horz2, NULL, kIcon_solid_yen_sign );
    bi22->setMargin( 0, 5, 0, 5 );
    bi22->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_TOP );
    auto bi23 = GUI_Button::create( horz2, NULL, kIcon_solid_pound_sign );
    bi23->setMargin( 0, 5, 0, 5 );
    bi23->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_TOP );

    auto btn = GUI_Button::create( column2, "Button 1", kIcon_solid_sun, 0, 0, 0, 0 );
    btn->setMargin( 10, 0, 0, 0 );
    btn->corner = 6;
    btn->setAlign(GUI_ALIGN_TOP|GUI_ALIGN_CENTER);

}

void createColumn3(GUI_View *subContentView) {
    auto column3 = GUI_View::create(subContentView, "COL2", 0, 0, -1, -1);
    column3->setLayout(GUI_LAYOUT_VERTICAL);
    column3->setMargin( 10, 10, 10, 10 );
    column3->setBackgroundColor(cWhite);
    column3->border = 1;
    column3->corner = 4;
}

void createColumn4(GUI_View *subContentView) {
    auto column4 = GUI_View::create(subContentView, "COL2", 0, 0, 0, -1);
    column4->setLayout(GUI_LAYOUT_VERTICAL);
    column4->setMargin( 10, 10, 10, 0 );
    //column4->setPadding( 5, 5, 5, 5 );
    column4->setBackgroundColor(cWhite);
    column4->border = 1;
    column4->corner = 4;
    
    auto bi21 = GUI_Button::create( column4, NULL, kIcon_solid_dollar_sign );
    bi21->setMargin( 5, 5, 5, 5 );
    bi21->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_TOP );
    auto bi22 = GUI_Button::create( column4, NULL, kIcon_solid_yen_sign );
    bi22->setMargin( 5, 5, 5, 5 );
    bi22->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_TOP );
    auto bi23 = GUI_Button::create( column4, NULL, kIcon_solid_pound_sign );
    bi23->setMargin( 5, 5, 5, 5 );
    bi23->setAlign( GUI_ALIGN_CENTER | GUI_ALIGN_TOP );
    
    column4->updateLayout();
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
        // todo
    });
    
    app->contentView->setLayout(GUI_LAYOUT_VERTICAL);
    inputBar = GUI_View::create(app->contentView, "InputBar", 0, 0, -1, 0);
    inputBar->setPadding( 5, 10, 5, 10 );
    inputBox = GUI_EditText::create(inputBar, "", 0, 0, -1, 32);
    inputBox->border = 1;
    
    auto subContentView = GUI_View::create( app->contentView, "Sub-content", 0, 0, -1, -1 );
    subContentView->setLayout(GUI_LAYOUT_HORIZONTAL );
    subContentView->border = 0;
    subContentView->setBackgroundColor(cLightGrey);
    
    createColumn1(subContentView);
    createColumn2(subContentView);
    createColumn3(subContentView);
    createColumn4(subContentView);
    
    subContentView->updateLayout();

    app->run();
    

    delete( app );

    return 0;
}




