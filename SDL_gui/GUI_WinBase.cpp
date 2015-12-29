//
//  GUI_WinBase.cpp
//  iOS
//
//  Created by Panutat Tejasen on 10/16/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#include "GUI_WinBase.h"

template<class T> T* re_alloc(T* arr,int& len) { // arr only used for type inference
    T* new_arr=new T[len*2];
    memset(new_arr+len,0,sizeof(T)*len);
    for (int i=0;i<len;++i) new_arr[i]=arr[i];
    delete[] arr;
    len*=2;
    return new_arr;
}

GUI_WinBase::GUI_WinBase(GUI_WinBase *pw,const char *t,int x,int y,int dx,int dy,SDL_Color _bgcol,void (*d_cmd)(GUI_WinBase *)):
parent(pw),
children(0),
title_str(t),
topleft(x,y),
tw_area(x,y,dx,dy),
clip_area(0,0,dx,dy),
bgcol(_bgcol),
titleColor(cGrey),
hidden(false),
lst_child(-1),
end_child(5),
border(0),
selected(false),
dragging(false),
lastMousePoint(0,0),
titleTexture(NULL),
title_area(0,0,0,0),
display_cmd(d_cmd),
canMove(0),
canClose(1),
isFocus(0),
tag(0),
handle_event_cmd(NULL),
onClose(NULL),
isMainWin(false),
disable(false)
{
    if (parent) { // parent = 0 if this = topw, or if keep_on_top() will be called
        tw_area.x = topleft.x+parent->tw_area.x;
        tw_area.y = topleft.y+parent->tw_area.y;
        parent->add_child(this);
    }
};

GUI_WinBase::~GUI_WinBase() {
    while( lst_child>=0 )
        delete children[lst_child];
    
    delete[] children;
    if (parent)
        parent->remove_child(this);
    
    if( titleTexture )
        SDL_DestroyTexture( titleTexture );
    
}

void GUI_WinBase::clear(GUI_Rect *rect) {
    if (!rect)
        rect=GUI_MakeRect(0,0,tw_area.w,tw_area.h);
    if( bgcol.a != 0 ) {
        GUI_FillRect2( rect, bgcol );
    }
}

void GUI_WinBase::clear(GUI_Rect *rect, SDL_Color col) {
    GUI_FillRect2( rect, bgcol );
}

bool GUI_WinBase::close() {
    if( !canClose ) {
        hide();
        return true;
    }
    if( onClose ) {
        if( !onClose(this) )
            return false;
    }
    if( GUI_mouseCapturedWindow == this )
        GUI_mouseCapturedWindow = NULL;
    if( GUI_modalWindow == this )
        GUI_modalWindow = NULL;
    delete( this );
    return true;
}

void GUI_WinBase::hide()
{
    if( GUI_mouseCapturedWindow == this )
        GUI_mouseCapturedWindow = NULL;
    if( GUI_modalWindow == this )
        GUI_modalWindow = NULL;

    hidden = true;
}

void GUI_WinBase::show()
{
    hidden = false;
}


void GUI_WinBase::createTitleTexture( int fontSize ) {
    if( titleTexture )
        SDL_DestroyTexture( titleTexture );
    
    if( title_str && title_str[0] ) {
        //GUI_Log( title_str );
        SDL_Surface *titleSurface = TTF_RenderUTF8_Blended( GUI_fonts[fontSize-1], title_str, titleColor );
        title_area.w = titleSurface->w;
        title_area.h = titleSurface->h;
        title_area.x = (tw_area.w-titleSurface->w) / 2;
        title_area.y = (tw_area.h-titleSurface->h) / 2;
        
        titleTexture = SDL_CreateTextureFromSurface(GUI_renderer, titleSurface);
        if ( titleTexture == NULL){
            GUI_Log( "CreateTitle Texture failed.\n" );
        }
        SDL_FreeSurface(titleSurface);
    }
    else {
        SDL_Surface *titleSurface = TTF_RenderUTF8_Blended( GUI_title_font, " ", titleColor );
        title_area.h = titleSurface->h;
        SDL_FreeSurface(titleSurface);
    }
}

void GUI_WinBase::predraw()
{
    if( hidden )
        return;
    //GUI_Log( "Viewport %s: %i, %i, %i, %i\n", title_str, tw_area.x, tw_area.y, tw_area.w, tw_area.h );
    SDL_RenderSetViewport( GUI_renderer, &tw_area );

    if( parent ) {
        GUI_Rect parent_clip = GUI_Rect( parent->clip_area );
        parent_clip.x -= topleft.x;
        parent_clip.y -= topleft.y;
        SDL_IntersectRect( GUI_MakeRect(0, 0, tw_area.w, tw_area.h), &parent_clip, &clip_area );
    }
    else {
        clip_area = GUI_Rect( 0, 0, tw_area.w, tw_area.h );
    }
    SDL_RenderSetClipRect( GUI_renderer, GUI_MakeRect( clip_area.x, clip_area.y, clip_area.w, clip_area.h ) );

    clear();
}

void GUI_WinBase::draw()
{
    predraw();
    if (display_cmd)
        display_cmd( this );
    
    if( border > 0 ) {
        GUI_Rect r = GUI_Rect( border, border, tw_area.w-(2*border), tw_area.h-(2*border) );
        
        GUI_DrawRect2( GUI_MakeRect(0, 0, tw_area.w, tw_area.h), cBlack );
        SDL_IntersectRect( &r, &clip_area, &clip_area );
        
        SDL_RenderSetClipRect( GUI_renderer, GUI_MakeRect( clip_area.x, clip_area.y, clip_area.w, clip_area.h ) );
    }
}

void GUI_WinBase::drawChildren()
{
    for (int i=0;i<=lst_child;++i) {
        GUI_WinBase *child=children[i];
        
        if (!child->hidden ) {
            GUI_Rect rect = clip_area;
            rect.x += tw_area.x;
            rect.y += tw_area.y;
            if( SDL_IntersectRect( &rect, &child->tw_area, &rect ) ) {
                child->draw();
                child->drawChildren();
            }
        }
    }
}

void GUI_WinBase::add_child( GUI_WinBase *child ) {
    if (!children)
        children=new GUI_WinBase*[end_child];
    else if( lst_child==end_child-1 )
        children=re_alloc(children,end_child);
    
    children[++lst_child]=child;
}

void GUI_WinBase::remove_child( GUI_WinBase *child ) {
    for (int i=lst_child;i>=0;--i) {
        if (children[i]==child) {
            for (;i<lst_child;++i)
                children[i]=children[i+1];
            --lst_child;
            return;
        }
    }
    GUI_Log( "*** ERROR: remove_child: child %p not found",child);
}

GUI_WinBase *GUI_WinBase::hitTest(int x,int y,bool bRecursive) {
    if (hidden)
        return 0;
    
    SDL_Point pt;
    pt.x = x;
    pt.y = y;
    
    if( SDL_PointInRect( &pt, &tw_area ) ) {
        if( bRecursive ) {
            for (int i=lst_child;i>=0;--i) {      // last child first
                GUI_WinBase *wb=children[i]->hitTest(x,y);
                if (wb)
                    return wb;
            }
        }
        return this;
    }
    return 0;
}

bool GUI_WinBase::handleEvents(SDL_Event *ev) {
    if( handle_event_cmd ) {
        if( handle_event_cmd( this, ev ) )
            return true;
    }
    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            //GUI_Log( "%s Mouse Down\n", title_str );
            for (int i=lst_child;i>=0;--i) {      // last child first
                GUI_WinBase *wb=children[i]->hitTest(x, y,false);
                if( wb ) {
                    if( !wb->handleEvents( ev ) ) {
                        if( wb->canMove == 1) {
                            wb->dragging = true;
                            wb->lastMousePoint.set( x, y );
                            GUI_Log( "%s Start draging\n", wb->title_str );
                            GUI_mouseCapturedWindow = wb;
                            return true;
                        }
                    }
                    else
                        return true;
                }
            }
            return false;
        }
        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            if( dragging ) {
                dragging = false;
                GUI_mouseCapturedWindow = NULL;
                GUI_Log( "%s End draging\n", title_str );
            }
            else {
                for (int i=lst_child;i>=0;--i) {      // last child first
                    GUI_WinBase *wb=children[i]->hitTest(x, y,false);
                    if( wb ) {
                        if( wb->handleEvents( ev ) )
                            return true;
                    }
                }
            }
            return false;
        }
        case SDL_MOUSEMOTION: {
            SDL_MouseMotionEvent e = ev->motion;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            if( dragging ) {
                if( parent ) {
                    if( !parent->hitTest(x, y, false) ) {
                        return false;
                    }
                }
                int dx = x - lastMousePoint.x;
                int dy = y - lastMousePoint.y;
                lastMousePoint.set( x, y );
                move( dx, dy );
                return true;
            }
            else {
                for (int i=lst_child;i>=0;--i) {      // last child first
                    GUI_WinBase *wb=children[i]->hitTest(x, y,false);
                    if( wb ) {
                        if( wb->handleEvents( ev ) )
                            return true;
                    }
                }
            }
            return false;
        }
        default:
            for (int i=lst_child;i>=0;--i) {      // last child first
                GUI_WinBase *wb=children[i];
                if( wb ) {
                    if( wb->handleEvents( ev ) )
                        return true;
                }
            }
            break;
    }
    return false;
}

static void move_tw_area(GUI_WinBase *wb,int delta_x,int delta_y) { // recursive
    wb->tw_area.x+=delta_x;
    wb->tw_area.y+=delta_y;
    for (int i=0;i<=wb->lst_child;++i)
        move_tw_area(wb->children[i],delta_x,delta_y);
}

void GUI_WinBase::move(int dx,int dy) {
    topleft.x += dx; topleft.y += dy;
    move_tw_area(this,dx,dy);
}
