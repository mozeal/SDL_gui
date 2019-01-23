//
//  GUI_EditText.cpp
//  GUI_EditText
//
//  Created by Panutat Tejasen on 17/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_EditText.h"
#include "GUI_TextUtil.h"
#include "GUI_Utils.h"

extern int GUI_windowWidth;
extern int GUI_windowHeight;

GUI_EditText *GUI_EditText::create( GUI_View *parent, const char *title, int x, int y, int width, int height,
                                          std::function<void(GUI_View*)>callbackFunction ) {
    return new GUI_EditText( parent, title, x, y, width, height, callbackFunction );
}

GUI_EditText::GUI_EditText(GUI_View *parent, const char *title, int x, int y, int width, int height,
                           std::function<void(GUI_View*)>callbackFunction ) :
GUI_TextView(parent, title, GUI_UITextFontName.c_str(), GUI_UITextFontSize, x, y, width, height, NULL )
{
    dragable = false;
    clickable = true;
    focusable = true;
    showInteract = false;
    mouseReceive = true;
    focus_need_input = true;

    forceEmptyText = true;
    
    border = 1;
    corner = 4;
    focusBorder = 2;
    
    updateContent();
    
    textEditIndex = 0;
    if( title ) {
        textEditIndex = (int)strlen(title);
    }
    
    setTextColor(cBlack);
    setBackgroundColor(cWhite);
    setContentAlign( GUI_ALIGN_TOP | GUI_ALIGN_LEFT );
    setPadding( 4, 4, 4, 4 );
}

GUI_EditText::~GUI_EditText() {
    
}

void GUI_EditText::draw() {
    GUI_TextView::draw();
    if( isFocus() ) {
        long currentTime = SDL_GetTicks() / 500;
        
        if (currentTime & 1) {
            int w, h;
            TTF_SizeUTF8(font, title.substr(0,textEditIndex).c_str(), &w, &h );
        
            int x = _padding[3] * GUI_scale + w + contentScrollPosnX;
            SDL_SetRenderDrawColor(GUI_renderer, cBlack.r, cBlack.g, cBlack.b, cBlack.a);
            SDL_RenderDrawLine(GUI_renderer, x, _padding[0] * GUI_scale, x, rectView.h - _padding[2] * GUI_scale);
        }
    }
}

bool GUI_EditText::eventHandler(SDL_Event*event) {
    SDL_Scancode scancode;
    
    switch (event->type) {
        case SDL_KEYDOWN:
        {
            scancode = event->key.keysym.scancode;
            if (scancode == SDL_SCANCODE_BACKSPACE) {
                int l = GUI_GetPreviousUTF8Index( title, textEditIndex );
                if( l != -1 ) {
                    title.erase( l, textEditIndex-l );
                    textEditIndex = l;
                    updateContent();
                }
            } else if (scancode == SDL_SCANCODE_LEFT) {
                int l = GUI_GetPreviousMainUTF8Index( title, textEditIndex );
                if( l != -1 ) {
                    textEditIndex = l;
                    updateContent();
                }
            } else if (scancode == SDL_SCANCODE_RIGHT) {
                int l = GUI_GetNextMainUTF8Index( title, textEditIndex );
                if( l != -1 ) {
                    textEditIndex = l;
                    updateContent();
                }
            }

            break;
        }
        case SDL_TEXTINPUT:
        {
            std::string textIn = std::string(event->text.text);
            title.insert(textEditIndex, textIn);
            textEditIndex += (int)strlen(event->text.text);
            
            int w, h;
            TTF_SizeUTF8(font, title.substr(0,textEditIndex).c_str(), &w, &h );
            int x = _padding[3] * GUI_scale + w;
            if( x > rectView.w - (_padding[1] * GUI_scale) ) {
                contentScrollPosnX = (rectView.w - (_padding[1] * GUI_scale))-x-(1*GUI_scale);
                GUI_Log( "Scroll %i\n", contentScrollPosnX );
            }
            updateContent();
            
            //GUI_Log( "Text IN: %s\n", textIn.c_str() );
            
            if( callback ) {
                callback(this);
            }
            break;
        }
        case SDL_FINGERDOWN:
        {
            GUI_TextView::eventHandler(event);
            
            SDL_MouseButtonEvent e = event->button;
            
            int x = (int)(e.x*GUI_windowWidth*GUI_mouseScale);
            int y = (int)(e.y*GUI_windowHeight*GUI_mouseScale);
            if( hitTest(x, y, false) ) {
                x -= rectView.x;
                int textX = (_padding[3] * GUI_scale) + contentScrollPosnX;
                GUI_Log( "Hit edit %i\n", x - textX );
                textEditIndex = GUI_GetTextIndexFromPosition(font, title, x - textX);
                updateContent();
            }
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            GUI_TextView::eventHandler(event);
            
            SDL_MouseButtonEvent e = event->button;
            
            int x = (int)(e.x*GUI_mouseScale);
            int y = (int)(e.y*GUI_mouseScale);
            if( hitTest(x, y, false) ) {
                x -= rectView.x;
                int textX = (_padding[3] * GUI_scale) + contentScrollPosnX;
                GUI_Log( "Hit edit %i\n", x - textX );
                textEditIndex = GUI_GetTextIndexFromPosition(font, title, x - textX);
                updateContent();
            }
            break;
        }
    
        default:
        {
            return GUI_TextView::eventHandler(event);
        }
    }
    return false;
}
