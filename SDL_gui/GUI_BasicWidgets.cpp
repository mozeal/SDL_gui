//
//  GUI_BasicWidgets.cpp
//  OSX
//
//  Created by Panutat Tejasen on 11/1/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "GUI_BasicWidgets.h"

GUI_Label::GUI_Label( GUI_WinBase *pw, const char *t, int x, int y, int width, SDL_Color color, int align, int fontSize  ):
GUI_WinBase(pw,t,x,y,0,0,cClear),
format(align),
padding(5)
{
    titleColor = color;
    createTitleTexture( fontSize );
    
    if( width == 0 ) {
        tw_area.w = title_area.w;
        title_area.x = 0;
        tw_area.x = parent->tw_area.x;
        if( format == 0 )
            tw_area.x += x;
        else if( format == 1 ) {
            tw_area.x += x-(title_area.w)/2;
        }
        else if( format == 2 ) {
            tw_area.x += x-title_area.w;
        }

    }
    else {
        tw_area.w = width;
        if( format == 0 )
            title_area.x = padding;
        else if( format == 1 ) {
            title_area.x = (tw_area.w-title_area.w)/2;
        }
        else if( format == 2 ) {
            title_area.x = (tw_area.w-title_area.w)-padding;
        }
    }
    title_area.y = 0;
    tw_area.h = title_area.h;
    clip_area.w = title_area.w;
    clip_area.h = title_area.h;
}

GUI_Label::~GUI_Label()
{
    
}

void GUI_Label::draw()
{
    GUI_WinBase::draw();
    SDL_RenderCopy(GUI_renderer, titleTexture, NULL, &title_area);
}

GUI_Button::GUI_Button( GUI_WinBase *pw, const char *t, int x, int y, int w, int h, SDL_Color col, void (*c)(GUI_Button*) ):
GUI_WinBase(pw,t,x,y,0,0,cClear),
cmd(c),
padding(5),
isDown(false),
radius(5),
color(col),
borderColor(cBlack),
texImage(NULL)
{
    titleColor = cWhite;
    createTitleTexture();
    
    if( w == 0 ) {
        tw_area.w = title_area.w+padding*4;
        title_area.x = padding*2;
    }
    else {
        tw_area.w = w;
        title_area.x = (tw_area.w-title_area.w)/2;
    }
    if( h == 0 ) {
        tw_area.h = title_area.h+padding*2;
        title_area.y = padding;
    }
    else {
        tw_area.h = h;
        title_area.y = (tw_area.h-title_area.h)/2;
    }
    clip_area.w = title_area.w+padding*4;
    clip_area.h = title_area.h+padding*2;
}

GUI_Button::~GUI_Button()
{
    if( texImage ) {
        SDL_DestroyTexture( texImage );
    }
}

void GUI_Button::draw()
{
    GUI_WinBase::predraw();
    if( color.a != 0 ) {
        if( isDown ) {
            GUI_FillRoundRect( 1, 1, tw_area.w-2, tw_area.h-2, radius-2, color );
            if( border )
                GUI_DrawRoundRect( 1, 1, tw_area.w-2, tw_area.h-2, radius-2, borderColor );
        }
        else {
            GUI_FillRoundRect( 0, 0, tw_area.w, tw_area.h, radius, color );
            if( border )
                GUI_DrawRoundRect( 0, 0, tw_area.w, tw_area.h, radius, borderColor );
        }
    }

    if( texImage ) {
        int w, h;
        SDL_QueryTexture(texImage, NULL, NULL, &w, &h);
        if( isDown )
        {
            w -= 2;
            h -= 2;
        }
        
        SDL_RenderCopy(GUI_renderer, texImage, NULL, GUI_MakeRect((tw_area.w-w)/2, (tw_area.h-h)/2, w, h));
    }
    else {
        SDL_RenderCopy(GUI_renderer, titleTexture, NULL, &title_area);
    }
    if( display_cmd ) {
        display_cmd( this );
    }
}

bool GUI_Button::handleEvents(SDL_Event *ev) {
    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            isDown = true;
            isFocus = true;
            GUI_mouseCapturedWindow = (SDL_Window *)this;
            return true;
        }
        case SDL_MOUSEMOTION: {
            if( isFocus ) {
                SDL_MouseButtonEvent e = ev->button;
                int x = e.x*GUI_mouseScale/GUI_scale;
                int y = e.y*GUI_mouseScale/GUI_scale;
                
                if( hitTest(x, y) ) {
                    isDown = true;
                    return true;
                }
                else {
                    isDown = false;
                }
            }
            return false;
        }
        case SDL_MOUSEBUTTONUP: {
            if( isFocus ) {
                GUI_mouseCapturedWindow = NULL;
                SDL_MouseButtonEvent e = ev->button;
                int x = e.x*GUI_mouseScale/GUI_scale;
                int y = e.y*GUI_mouseScale/GUI_scale;
                
                isDown = false;
                isFocus = false;
                if( hitTest(x, y) ) {
                    if( cmd ) {
                        cmd( this );
                        return true;
                    }
                }
            }
            return false;
        }
        default: {
            for (int i=lst_child;i>=0;--i) {      // last child first
                GUI_WinBase *wb=children[i];
                if( wb ) {
                    if( wb->handleEvents( ev ) )
                        return true;
                }
            }
        }
    }
    return false;
}

GUI_EditText::GUI_EditText( GUI_WinBase *pw, const char *t, int x, int y, int width, void (*c)(GUI_EditText*) ):
GUI_WinBase(pw,t,x,y,0,0,cClear),
cmd(c),
padding(5),
text(t),
text_index(0)
{
    titleColor = cBlack;
    createTitleTexture();
    
    title_area.x = padding;
    title_area.y = padding;
    if( width == 0 ) {
        tw_area.w = title_area.w+padding*2;
    }
    else {
        tw_area.w = width;
    }
    tw_area.h = title_area.h+padding*2;
    clip_area.w = title_area.w+padding*2;
    clip_area.h = title_area.h+padding*2;
}

GUI_EditText::~GUI_EditText()
{
    
}

void GUI_EditText::setText( const char *t )
{
    text = t;
    text_index = 0;
    title_str = text.c_str();
    createTitleTexture();
    title_area.x = padding;
    title_area.y = padding;
}


void GUI_EditText::draw()
{
    GUI_WinBase::draw();
    
    
    GUI_DrawRect2( GUI_MakeRect( 0, 0, tw_area.w, tw_area.h ), cGrey );
    SDL_RenderCopy(GUI_renderer, titleTexture, NULL, &title_area);
    
    long currentTime = SDL_GetTicks()/500;
    if( isFocus && (currentTime & 1) ) {
        int w, h;
        
        TTF_SizeUTF8( GUI_title_font, text.substr(0,text_index).c_str(), &w, &h );
        int x = padding+w;
        GUI_DrawLine( x, padding, x, tw_area.h-padding, cBlue );
    }
}

bool GUI_EditText::handleEvents(SDL_Event *ev) {
    SDL_Scancode scancode;      /* scancode of last key we pushed */

    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            if( this->disable )
                return false;
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            if( isFocus ) {
                if( !hitTest(x, y) ) {
                    //isFocus = false;
                    //GUI_mouseCapturedWindow = NULL;
                    //SDL_StopTextInput();
                    return true;
                }
            }
            else {
                isFocus = true;
                GUI_mouseCapturedWindow = (SDL_Window *)this;
                SDL_StartTextInput();
            }
            if( isFocus ) {
                int xx = padding+tw_area.x;
                char cc[4];
                cc[3] = 0;
                int j=0;
                for( int i=0; i<text.length(); i++ ) {
                    unsigned char c = text.at(i);
                    int ww, hh;
                    if( (c & 0x80) == 0 ) {
                        cc[j++] = c;
                    }
                    else if( (c & 0xc0) == 0xc0 ) {
                        cc[j++] = c;
                        c = text.at(i+j);
                        while( (c & 0xC0) == 0x80 ) {
                            cc[j++] = c;
                            c = text.at(i+j);
                        }
                    }
                    cc[j] = 0;
                    TTF_SizeUTF8( GUI_title_font, text.substr(0,i+j).c_str(), &ww, &hh );
                    if( j == 3 ) {
                        //GUI_Log( "%i %i %02X %02X %02X: %i %i\n", i, j, (int)(unsigned char)cc[0], (int)(unsigned char)cc[1], (int)(unsigned char)cc[2], xx+ww, x );
                    }
                    else {
                        //GUI_Log( "%i %i %s: %i %i\n", i, j, cc, xx+ww, x );
                    }
                    if( (xx+ww) > x+4 ) {
                        text_index = i;
                        break;
                    }
                    else {
                        text_index = i+1;
                    }
                    i += j-1;
                    //xx += ww;
                    j = 0;
                }
                return true;
            }
            return false;
        }
        case SDL_MOUSEBUTTONUP: {
            if( this->disable )
                return false;
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            if( isFocus ) {
                if( !hitTest(x, y) ) {
                    isFocus = false;
                    GUI_mouseCapturedWindow = NULL;
                    SDL_StopTextInput();
                    //return true;
                }
            }
            return false;
        }
        case SDL_KEYDOWN: {
            if( isFocus == false )
                return false;
            scancode = ev->key.keysym.scancode;
            if (scancode == SDL_SCANCODE_BACKSPACE) {
                /* if user hit delete, delete the last character */
                int oi = text_index;
                while( text_index > 0 ) {
                    text_index--;
                    int c = (text.at(text_index) & 0xC0);
                    if( c != 0x80 ) {
                        break;
                    }
                }
                if( oi != text_index ) {
                    text = text.substr(0,text_index)+text.substr(oi);
                    title_str = text.c_str();
                    createTitleTexture();
                    title_area.x = padding;
                    title_area.y = padding;
                }
            }
            return true;
        }
        case SDL_TEXTINPUT: {
            if( isFocus == false )
                return false;
            text.insert( text_index, std::string( ev->text.text ) );
            text_index += strlen(ev->text.text);
            title_str = text.c_str();
            createTitleTexture();
            title_area.x = padding;
            title_area.y = padding;
            
            return true;
        }
            
        default:
            break;
    }
    return true;
}

GUI_ListCell::GUI_ListCell( GUI_WinBase *pw, const char *t, int x, int y, int w, int h, bool chk ):
GUI_WinBase(pw,t,x,y,w,h,cClear),
checkable( chk ),
checked(false),
separator(false)
{
    if( !strcmp( t, "---" ) ) {
        separator = true;
    }
    else {
        titleColor = cWhite;
        createTitleTexture();
        title_area.x = 8;
    }
}

GUI_ListCell::~GUI_ListCell()
{
    
}

void GUI_ListCell::draw()
{
    GUI_WinBase::draw();

    if( !display_cmd ) {
        if( separator ) {
            GUI_DrawLine(0, tw_area.h/2, tw_area.w, tw_area.h/2, cBlack );
            return;
        }
        if( selected ) {
            SDL_SetTextureColorMod( titleTexture, 255, 255, 255 );
            GUI_FillRect2( GUI_MakeRect(0, 0, tw_area.w, tw_area.h), cDarkGrey );
        }
        else {
            SDL_SetTextureColorMod( titleTexture, 0, 0, 0 );
        }
        if( checkable ) {
            title_area.x = 8+16;
        }
        else {
            title_area.x = 8;
        }
        SDL_RenderCopy(GUI_renderer, titleTexture, NULL, &title_area);

        if( checked ) {
            SDL_RenderCopy(GUI_renderer, GUI_checkTexture, NULL, GUI_MakeRect(3, 4, 16, 13));
        }
    }
}


/*
void GUI_ListCell::handleEvents(SDL_Event *ev) {
    SDL_Scancode scancode;      
 
    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            GUI_List *p = (GUI_List *)parent;
            p->selectChanged(tag);
        }
    }
}
*/

GUI_List::GUI_List( GUI_WinBase *pw, const char **t, int num,  int x, int y, int w, int h, int ch, bool pu, void (*c)(GUI_List*,const char *,int) ):
GUI_WinBase(pw,"List",x,y,w,h,cClear),
cmd(c),
padding(4),
popupMode(pu),
selectedIndex(-1),
corner(0),
border(true),
bMousePress(false),
scrollPad(NULL),
numCells(num),
scrollable(false),
cellHeight(ch),
timerID(-1)
{
    for( int i=0; i<15; i++ ) {
        if( t[i] && t[i][0] ) {
            //GUI_Log( "%i: %s\n", i+1, t[i] );
        }
        else {
            break;
        }
    }

    tw_area.w = w;
    menuText = t;
    //GUI_Log( "items: %i\n", sizeof( menuText ) );
    
    
    int cch;
    if( cellHeight == 0 )
        cch = GUI_nominal_leading;
    else
        cch = cellHeight;

    scrollPad = new GUI_WinBase( this, "ScrollPad", 0, 0, tw_area.w, numCells*cch+padding*2,cClear );
    
    int i=0;
    for( i=0; i<numCells; i++ ) {
        if( menuText[i] && menuText[i][0] ) {
            GUI_ListCell *cell = new GUI_ListCell( scrollPad, menuText[i], padding, padding+i*cch, tw_area.w-padding*2, cch, false );
            cell->tag = i;
            vMenu.push_back( cell );
        }
        else {
            break;
        }
    }
    if( h == 0 )
        tw_area.h = cch * i + padding*2;
    
    if( scrollPad->tw_area.h > tw_area.h ) {
        scrollable = true;
    }
    if( popupMode ) {
        GUI_mouseCapturedWindow = this;
    }

}

GUI_List::~GUI_List()
{
    if( timerID != -1 )
    {
        SDL_RemoveTimer( timerID );
        timerID = -1;
    }
    vMenu.clear();
}

GUI_ListCell *GUI_List::getCell( int tag )
{
    for( int i=0; i<vMenu.size(); i++ ) {
        GUI_ListCell *cell = vMenu.at(i);
        if( cell->tag == tag ) {
            return cell;
        }
    }
    return NULL;
}

int GUI_List::setCheck( int tag )
{
    int ret = -1;
    for( int i=0; i<vMenu.size(); i++ ) {
        GUI_ListCell *cell = vMenu.at(i);
        if( cell->tag == tag ) {
            cell->checked = true;
            ret = i;
        }
        else {
            cell->checked = false;
        }
    }
    return ret;
}

void GUI_List::draw()
{
    GUI_WinBase::draw();
    
    if( corner ) {
        GUI_FillRoundRect( 0, 0, tw_area.w, tw_area.h, 8, cGrey );
    }
    else {
        GUI_FillRect2(GUI_MakeRect(0, 0, tw_area.w, tw_area.h), cWhite );
        if( border ) {
            GUI_DrawRect2(GUI_MakeRect(0, 0, tw_area.w, tw_area.h), cBlack );
        }
    }
    for( int i=0; i<vMenu.size(); i++ ) {
        GUI_ListCell *cell = vMenu.at(i);
        if( i == selectedIndex ) {
            cell->selected = true;
        }
        else {
            cell->selected = false;
        }
    }
}

Uint32 GUI_List_TimerCallback( Uint32 interval, void *param)
{
    GUI_List *l = (GUI_List *)param;
    l->timerID = -1;
    l->disable = false;
    //GUI_Log( "Enable\n" );

    SDL_Event event;
    SDL_zero(event);
    
    event.user.type = GUI_LISTSELECTED;
    event.user.data1 = param;
    SDL_PushEvent(&event);
    
    return 0;
}


void GUI_List::selectChanged( int n )
{
    //SDL_Log( "Select: %i\n", n );
    selectedIndex = n;
    static int p;
    p = n;
    disable = true;
    if( timerID != -1 ) {
        SDL_RemoveTimer( timerID );
    }
    
    /*
    if( cmd ) {
        cmd( this, menuText[selectedIndex], selectedIndex );
    }
    disable = false;
    */
    
    timerID = SDL_AddTimer( 100, GUI_List_TimerCallback, this );
}

bool GUI_List::handleEvents(SDL_Event *ev) {
    //SDL_Scancode scancode;
    if( disable ) {
        GUI_Log( "Reject\n" );
        return false;
    }
    
    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            lastMouseX = x;
            lastMouseY = y;
            if( popupMode ) {
                if( !this->hitTest(x, y) ) {
                    //close();
                    return true;
                }
            }
            GUI_Log( "Last: %i, %i\n", lastMouseX, lastMouseY );
            bMousePress = true;
            mouseClickTime = SDL_GetTicks();
            if( !popupMode ) {
                if( scrollable )
                    GUI_mouseCapturedWindow = this;
            }
            GUI_WinBase *w = scrollPad->hitTest(x, y);
            if( w && w->parent == scrollPad ) {
                selectedIndex = w->tag;
                this->draw();
            }

            return true;
        }
        case SDL_MOUSEMOTION: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            //GUI_Log( "Last2: %i, %i\n", lastMouseX, lastMouseY );
            if( bMousePress ) {
                if( abs(x-lastMouseX) > 5 || abs(y-lastMouseY) > 5 ) {
                    if( scrollable ) {
                        bMousePress = false;
                        dragging = true;
                    }
                }
            }
            if( dragging ) {
                int diffY = y - lastMouseY;
                scrollPad->move( 0, diffY );
                if( scrollPad->tw_area.y-scrollPad->parent->tw_area.y > 0 )
                {
                    scrollPad->move(0, scrollPad->parent->tw_area.y-scrollPad->tw_area.y );
                }
                if( scrollPad->tw_area.y+scrollPad->tw_area.h < scrollPad->parent->tw_area.y+tw_area.h ) {
                    scrollPad->move(0, (scrollPad->parent->tw_area.y+tw_area.h)-(scrollPad->tw_area.y+scrollPad->tw_area.h) );
                }
                lastMouseX = x;
                lastMouseY = y;
            }
            else {
                GUI_WinBase *w = scrollPad->hitTest(x, y);
                if( w && w->parent == scrollPad ) {
                    //GUI_Log( "Tag: %i\n", w->tag );
                    selectedIndex = w->tag;
                    this->draw();
                }
            }
            return true;
        }
        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;

            if( dragging ) {
                dragging = false;
            }
            if( !popupMode ) {
                GUI_mouseCapturedWindow = NULL;
            }
            else {
                if( !this->hitTest(x, y) ) {
                    close();
                    return true;
                }
            }
            if( !bMousePress )
                break;
            //GUI_Log( "Last2: %i, %i\n", lastMouseX, lastMouseY );
            if( abs(x-lastMouseX) > 5 || abs(y-lastMouseY) > 5 ) {
                if( scrollable ) {
                    break;
                }
            }
            
            GUI_WinBase *w = scrollPad->hitTest(x, y);
            if( w && w->parent == scrollPad ) {
                //GUI_Log( "Tag: %i (%s)\n", w->tag, this->title_str );
                selectChanged( w->tag );
                this->draw();
            }
            bMousePress = false;
            return true;
        }
        case GUI_LISTSELECTED: {
            if( ev->user.data1 == this ) {
                if( cmd ) {
                    cmd( this, menuText[selectedIndex], selectedIndex );
                }
                if( popupMode ) {
                    close();
                }
                return true;
            }
        }
        default:
            break;
    }
    return false;
}

void GUI_List::hide()
{
    if( popupMode ) {
        GUI_mouseCapturedWindow = NULL;
    }
    GUI_WinBase::hide();
}

void GUI_List::show() {
    if( popupMode ) {
        GUI_mouseCapturedWindow = this;
    }
    GUI_WinBase::show();
}

