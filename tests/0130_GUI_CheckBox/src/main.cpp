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
GUI_Label *label2;

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
    topView->setPadding(10,10,10,10);
    topView->setLayout( GUI_LAYOUT_HORIZONTAL );
    
    auto column1 = GUI_View::create(topView, NULL, 0, 0, 250, -1);
    column1->setLayout(GUI_LAYOUT_VERTICAL);
    column1->corner = 4;
    column1->setPadding( 5, 0, 5, 0 );
    
    label = GUI_Label::create(column1, "This label for visible testing");
    label->setMargin(0, 0, 0, 10 );
    
    auto button1 = GUI_Button::create(column1, "Visible", kIcon_solid_eye, 0, 0, 120, 0, [=](GUI_View *bt) {
        label->setVisible(true);
        topView->updateLayout();
    } );
    button1->setBackgroundColor(cCyan);
    button1->setMargin( 10, 10, 10, 10 );
    
    auto button2 = GUI_Button::create(column1, "Hidden", kIcon_solid_eye_slash, 0, 0, 120, 0, [=](GUI_View *bt) {
        label->setVisible(false);
        topView->updateLayout();
    } );
    button2->setBackgroundColor(cBlue);
    button2->setTextColor(cWhite);
    button2->setMargin( 0, 10, 10, 10 );
    
    auto button3 = GUI_Button::create(column1, "Toggle", kIcon_solid_exchange_alt, 0, 0, 120, 0, [=](GUI_View *bt) {
        label->setVisible(!label->isVisible());
        topView->updateLayout();
    } );
    button3->setBackgroundColor(cWhite);
    button3->setTextColor(cBlack);
    button3->setMargin( 0, 10, 10, 10 );

    
    auto button4 = GUI_Button::create(column1, "Enable", kIcon_solid_thumbs_up, 0, 0, 120, 0, [=](GUI_View *bt) {
    } );
    button4->setBackgroundColor(cWhite);
    button4->setTextColor(cBlack);
    button4->setMargin( 10, 10, 10, 10 );

    auto button5 = GUI_Button::create(column1, "Toggle", kIcon_solid_exchange_alt, 0, 0, 120, 0, [=](GUI_View *bt) {
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

    label2 = GUI_Label::create(column1, "This label for checkbox testing");
    label2->setMargin(0, 0, 0, 10 );
    
    auto checkbox1 = GUI_CheckBox::create( column1, "Check Box 1", 0, 0, 140, 0, [=](GUI_View *v) {
        GUI_CheckBox *cb = (GUI_CheckBox *)v;
        
        label2->setVisible(cb->isChecked());
        column1->updateLayout();
    });
    checkbox1->setMargin( 10, 10, 10, 10 );


    
    auto label3 = GUI_Label::create(column1, "This label for radio testing");
    label3->setMargin(0, 0, 0, 10 );
    
    auto radio1 = GUI_RadioButton::create( column1, "Radio Button 1", 0, 0, 140, 0, [=](GUI_View *v) {
        GUI_RadioButton *rb = (GUI_RadioButton *)v;
        
        label3->setVisible(rb->isChecked());
        column1->updateLayout();
    });
    radio1->setMargin( 10, 10, 10, 10 );
    
    auto button6 = GUI_Button::create(column1, "Toggle", kIcon_solid_exchange_alt, 0, 0, 120, 0, [=](GUI_View *bt) {
        checkbox1->setEnable(!checkbox1->isEnable());
        radio1->setEnable(!radio1->isEnable());
        if( checkbox1->isEnable() ) {
            checkbox1->setTitle("Enable");
        }
        else {
            checkbox1->setTitle("Disabled");
        }
    } );
    button6->setBackgroundColor(cRed);
    button6->setTextColor(cWhite);
    button6->setMargin( 0, 10, 10, 10 );
    
    
    
    
    auto column2 = GUI_View::create(topView, NULL, 0, 0, 220, 0);
    column2->setLayout(GUI_LAYOUT_VERTICAL);
    column2->corner = 4;
    column2->setMargin( 0, 0, 0, 10 );
    column2->setPadding( 5, 0, 5, 0 );

    
    auto panel1 = GUI_View::create( column2, NULL, 0, 0, -1, 0);
    panel1->setLayout(GUI_LAYOUT_HORIZONTAL);
    panel1->setBackgroundColor(cClear);
    panel1->border = 0;
    
        auto label4 = GUI_Label::create(panel1, "Selection: ");
        label4->setMargin(0, 0, 0, 10 );
        auto labelSelected = GUI_Label::create(panel1, "0");
        labelSelected->setMargin(0, 0, 0, 0 );
    
    auto radioGroup1 = GUI_RadioButtonGroup::create(column2, NULL, 0, 0, -1, 0, [=](GUI_View *v) {
        GUI_RadioButtonGroup *rg = (GUI_RadioButtonGroup *)v;
        GUI_RadioButton *sel = rg->selectedRadioButton;
        labelSelected->setTitle(sel->title);
    });
    radioGroup1->setPadding( 10, 10, 10, 10 );
    radioGroup1->setLayout( GUI_LAYOUT_VERTICAL );
    
    radioGroup1->add( GUI_RadioButton::create( radioGroup1, "Radio Button 1", 0, 0, 140, 0) );
    radioGroup1->add( GUI_RadioButton::create( radioGroup1, "Radio Button 2", 0, 0, 140, 0) );
    radioGroup1->add( GUI_RadioButton::create( radioGroup1, "Radio Button 3", 0, 0, 140, 0) );
    radioGroup1->add( GUI_RadioButton::create( radioGroup1, "Radio Button 4", 0, 0, 140, 0) );
    radioGroup1->add( GUI_RadioButton::create( radioGroup1, "Radio Button 5", 0, 0, 140, 0) );
    

    
    GUI_Run();
    GUI_Destroy();

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}




