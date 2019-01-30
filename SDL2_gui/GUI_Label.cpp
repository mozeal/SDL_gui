//
//  GUI_Label.cpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 13/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_Label.h"

GUI_Label *GUI_Label::create( GUI_View *parent, const char *title, uint16_t unicode, int x, int y, int width, int height,
                             std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_Label(parent, title, unicode, x, y, width, height, userEventHandler );
}

GUI_Label::GUI_Label(GUI_View *parent, const char *title, uint16_t unicode, int x, int y, int width, int height,
                     std::function<bool(SDL_Event* ev)>userEventHandler) :
GUI_View( parent, title, x, y, width, height, userEventHandler),
iconView( NULL ),
textView( NULL )
{
    _backgroundColor = cWhite;
    setBorder( 0 );
    dragable = false;
    mouseReceive = false;
    setLayout( GUI_LAYOUT_HORIZONTAL );
    setMargin( 0, 0, 0, 0 );
    setPadding( 0, 0, 0, 0 );
    
    if( unicode != 0 ) {
        iconView = GUI_IconView::create(this, unicode, GUI_GetUIIconFontName().c_str(), GUI_GetUIIconFontSize());
        iconView->setAlign( (width == 0 ? GUI_ALIGN_LEFT : GUI_ALIGN_CENTER) | (height == 0 ? GUI_ALIGN_TOP : GUI_ALIGN_VCENTER) );
        iconView->setBorder( 0 );
        iconView->setBackgroundColor( cClear );
        iconView->setColor( cBlack );
        if( title ) {
            iconView->setMargin( 0, 3, 0, 0 );
        }
        else {
            iconView->setMargin( 0, 0, 0, 0 );
        }
    }

    if( title ) {
        textView = GUI_TextView::create(this, title, GUI_GetUITextFontName().c_str(), GUI_GetUITextFontSize());
        textView->setAlign( (width == 0 ? GUI_ALIGN_LEFT : GUI_ALIGN_CENTER) | (height == 0 ? GUI_ALIGN_TOP : GUI_ALIGN_VCENTER) );
        textView->setBorder( 0 );
        textView->setBackgroundColor( cClear );
        textView->setColor( cBlack );
        textView->setMargin( 0, 0, 0, 3 );
    }

    setLayout( GUI_LAYOUT_HORIZONTAL );
    updateSize();
    
    /*
    if( parent ) {
        parent->updateLayout();
    }
    else {
        updateLayout();
    }
    */
}

GUI_Label::~GUI_Label() {
    
}

void GUI_Label::setTitle( std::string t ) {
    title = std::string(t);
    if( textView ) {
        textView->setTitle(t);
    }
    updateLayout();
}

void GUI_Label::setIcon( uint16_t unicode ) {
    if( iconView ) {
        iconView->setIcon( unicode );
    }
    updateLayout();
}

void GUI_Label::setTextColor( SDL_Color c ) {
    _textColor = c;
    if( textView ) {
        textView->setTextColor(c);
    }
    if( iconView ) {
        iconView->setTextColor(c);
    }
}

void GUI_Label::setLayout( int l ) {
    GUI_View::setLayout(l);
    if( _layout == GUI_LAYOUT_HORIZONTAL ) {
        int icm = 0;
        int txm = 0;
        
        if( iconView && textView ) {
            if( iconView->rectView.h > textView->rectView.h ) {
                txm = (iconView->rectView.h - textView->rectView.h) / (2 * GUI_scale);
            }
            else {
                icm = (textView->rectView.h - iconView->rectView.h) / (2 * GUI_scale);
            }
        }
        
        if( iconView ) {
            iconView->setMargin( icm, textView ? 4 : 0, icm, 0 );
        }
        if( textView ) {
            textView->setMargin( txm, 0, txm, iconView ? 4 : 0 );
        }
    }
    else if( _layout == GUI_LAYOUT_VERTICAL ) {
        if( iconView ) {
            iconView->setMargin( 0, 0, textView ? 4 : 0, 0 );
        }
        if( textView ) {
            textView->setMargin( iconView ? 4 : 0, 0, 0, 0 );
        }
    }
}

void GUI_Label::setContentAlign( int a ) {
    GUI_View::setContentAlign(a);
    if( iconView ) {
        iconView->setAlign( a );
    }
    if( textView ) {
        textView->setAlign( a );
    }
}


