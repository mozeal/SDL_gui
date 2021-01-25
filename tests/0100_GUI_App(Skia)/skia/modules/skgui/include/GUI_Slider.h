//
//  GUI_Slider.hpp
//  GUI_App
//
//  Created by Panutat Tejasen on 25/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_Slider_hpp
#define GUI_Slider_hpp

#include <stdio.h>
#include "GUI_View.h"

class GUI_ProgressBar : public GUI_View {
    GUI_View *valueView;
public:
    static GUI_ProgressBar *create( GUI_View *parent, const char *title, float min, float max, float val,
                              int x, int y, int width, int height,
                              std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_ProgressBar(GUI_View *parent, const char *title, float min, float max, float val,
               int x, int y, int width, int height,
               std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_ProgressBar();
    
    float minValue;
    float maxValue;
    float value;
    
    virtual void draw();
};

class GUI_Slider : public GUI_View {
protected:
    GUI_View *indicator;
    GUI_ProgressBar *valueBar;
public:
    static GUI_Slider *create( GUI_View *parent, const char *title, float min, float max, float val,
                              int x, int y, int width, int height,
                              std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_Slider(GUI_View *parent, const char *title, float min, float max, float val,
               int x, int y, int width, int height,
               std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_Slider();
    
    float minValue;
    float maxValue;
    float value;
};

#endif /* GUI_Slider_hpp */
