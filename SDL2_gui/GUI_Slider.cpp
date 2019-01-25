//
//  GUI_Slider.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 25/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Slider.h"
#include "GUI_Config.h"

GUI_ValueBar *GUI_ValueBar::create( GUI_View *parent, const char *title, float min, float max, float val,
                               int x, int y, int width, int height,
                               std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_ValueBar( parent, title, min, max, val, x, y, width, height, callbackFunction );
}

GUI_ValueBar::GUI_ValueBar(GUI_View *parent, const char *title, float min, float max, float val,
                       int x, int y, int width, int height,
                       std::function<void(GUI_View*)>callbackFunction ) :
GUI_View(parent, title, x, y, width, height ),
valueView(NULL)
{
    minValue = min;
    maxValue = max;
    value = val;
    
    int h = height;
    if( h == 0 ) {
        h = 8;
    }
    corner = h/2;
    setBackgroundColor(cGrey);
    
    valueView = GUI_View::create(this, "value", 0, 0, 0, h);
    valueView->setBackgroundColor( GUI_AppTopBarColor );
    valueView->corner = h/2;
}

GUI_ValueBar::~GUI_ValueBar() {
    
}


// ------------------------------------------------------------------------------------------------

GUI_Slider *GUI_Slider::create( GUI_View *parent, const char *title, float min, float max, float val,
                          int x, int y, int width, int height,
                               std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_Slider( parent, title, min, max, val, x, y, width, height, callbackFunction );
}

GUI_Slider::GUI_Slider(GUI_View *parent, const char *title, float min, float max, float val,
           int x, int y, int width, int height,
                       std::function<void(GUI_View*)>callbackFunction ) :
    GUI_View(parent, title, x, y, width, height ),
    valueBar(NULL)
{
    minValue = min;
    maxValue = max;
    value = val;
    border = 0;
    

    int h = height;
    if( h == 0 ) {
        h = 24;
    }
    
    valueBar = GUI_ValueBar::create(this, "value", min, max, val, 0, 0, -1, 0);
    valueBar->setMargin( 0, (h-8)/2, 0, (h-8)/2 );
    valueBar->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );

    
    
    indicator = GUI_View::create( this, "Ind", 0, 0, h, h );
    indicator->corner = h / 2;
}

GUI_Slider::~GUI_Slider() {
    
}
