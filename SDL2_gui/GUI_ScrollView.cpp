//
//  GUI_ScrollView.cpp
//  GUI_App
//
//  Created by Panutat Tejasen on 26/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_ScrollView.h"

GUI_ScrollView *GUI_ScrollView::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                       std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_ScrollView(parent, title, x, y, width, height, callbackFunction );
}

GUI_ScrollView::GUI_ScrollView(GUI_View *parent, const char *title, int x, int y, int width, int height,
               std::function<void(GUI_View*)>callbackFunction ) :
GUI_View( parent, title, x, y, width, height ),
scrollContent(NULL)
{
    setLayout(GUI_LAYOUT_ABSOLUTE);
    border = 0;
    
    scrollContent = GUI_View::create(this, "ScrollContent", 0, 0, -1, 0);
    scrollContent->setLayout(GUI_LAYOUT_VERTICAL);
    scrollContent->setBackgroundColor(cClear);
    scrollContent->dragable = true;
    scrollContent->drag_limit = true;
    scrollContent->dragMinX = 0;
    scrollContent->dragMaxX = 0;
    scrollContent->dragMinY = 0-(scrollContent->getHeight()-getHeight());
    scrollContent->dragMaxY = 0;
    scrollContent->border = 0;

}

GUI_ScrollView::~GUI_ScrollView() {
    
}

void GUI_ScrollView::updateLayout() {
    GUI_View::updateLayout();
    if( scrollContent ) {
        scrollContent->dragMinY = 0-(scrollContent->getHeight()-getHeight());
    }
}
