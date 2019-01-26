//
//  GUI_Slider.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 25/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Slider.h"
#include "GUI_Config.h"
#include "GUI_shapes.h"

GUI_ProgressBar *GUI_ProgressBar::create( GUI_View *parent, const char *title, float min, float max, float val,
                               int x, int y, int width, int height,
                               std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_ProgressBar( parent, title, min, max, val, x, y, width, height, callbackFunction );
}

GUI_ProgressBar::GUI_ProgressBar(GUI_View *parent, const char *title, float min, float max, float val,
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

GUI_ProgressBar::~GUI_ProgressBar() {
    
}

void GUI_ProgressBar::draw() {
    int h = rectView.h;
    int w = value / (maxValue-minValue) * (rectView.w - rectView.h);
    GUI_FillRect(h/2, 0, w, h, GUI_AppTopBarColor);
    if( value > minValue ) {
        GUI_FillCircle( h/2, h/2, h/2, GUI_AppTopBarColor);
    }
    if( value == maxValue ) {
        GUI_FillCircle( rectView.w-h/2, h/2, h/2, GUI_AppTopBarColor);
    }
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
    setLayout(GUI_LAYOUT_ABSOLUTE);
    

    int h = height;
    if( h == 0 ) {
        h = 24;
    }
    
    valueBar = GUI_ProgressBar::create(this, "value", min, max, val, 0, 0, -1, 0);
    valueBar->setMargin( 0, (h-8)/2, 0, (h-8)/2 );
    valueBar->setAlign( GUI_ALIGN_LEFT | GUI_ALIGN_VCENTER );

    
    
    indicator = GUI_View::create( this, "Ind", 0, 0, h, h );
    indicator->corner = h / 2;
    indicator->dragable = true;
    indicator->drag_limit = true;
    indicator->dragMinX = 0;
    indicator->dragMaxX = width-h;
    indicator->dragMinY = 0;
    indicator->dragMaxY = 0;
    indicator->callback_on_drag = true;
    indicator->setAlign(GUI_ALIGN_ABSOLUTE);
    indicator->setCallback([=](GUI_View *v) {
        this->value = min + ((float)((indicator->topLeft.x/GUI_scale) * (max-min)) / (float)(indicator->dragMaxX-indicator->dragMinX));
        valueBar->value = this->value;
        if( callback ) {
            callback(this);
        }
    });
    int xx = value / (max-min) * (float)(indicator->dragMaxX-indicator->dragMinX);
    indicator->setAbsolutePosition( getAbsolutePosition().x+xx, getAbsolutePosition().y );
}

GUI_Slider::~GUI_Slider() {
    
}
