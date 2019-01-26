//
//  GUI_ScrollView.hpp
//  GUI_App
//
//  Created by Panutat Tejasen on 26/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef GUI_ScrollView_hpp
#define GUI_ScrollView_hpp

#include <stdio.h>
#include "GUI_View.h"

class GUI_ScrollView: public GUI_View {
protected:

public:
    static GUI_ScrollView *create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                              std::function<void(GUI_View*)>callbackFunction = NULL );
    GUI_ScrollView(GUI_View *parent, const char *title, int x, int y, int width, int height,
               std::function<void(GUI_View*)>callbackFunction = NULL );
    virtual ~GUI_ScrollView();
    
    GUI_View *scrollContent;
    
    virtual void updateLayout();
};

#endif /* GUI_ScrollView_hpp */
