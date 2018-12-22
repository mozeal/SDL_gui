//
//  GUI_View.cpp
//  GUI_View
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_View.h"
#include "GUI_Utils.h"
#include "GUI_shapes.h"

extern int GUI_physicalWindowWidth;
extern int GUI_physicalWindowHeight;

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern SDL_Renderer *GUI_renderer;
extern float GUI_scale;

GUI_View *GUI_View::createView( GUI_View *parent, const char *title, int x, int y, int width, int height,
                               std::function<bool(SDL_Event* ev)>userEventHandler) {
    GUI_View *view = new GUI_View( parent, title, x, y, width, height, userEventHandler );
    return view;
}

GUI_View::GUI_View( GUI_View *p, const char *t, int x, int y, int w, int h,
                   std::function<bool(SDL_Event* ev)>userEventHandler):
parent(p),
children(0),
topLeft(x * GUI_scale, y * GUI_scale),
rectView(x * GUI_scale, y * GUI_scale, w == -1 ? w : w * GUI_scale, h == -1 ? h : h * GUI_scale),
rectClip(0,0,0,0),
backgroundColor(cWhite),
borderColor(cBlack),
border(1),
corner(0),
padding{0,0,0,0},
margin{0,0,0,0},
layout(GUI_LAYOUT_ABSOLUTE),
align(GUI_ALIGN_LEFT | GUI_ALIGN_TOP),
_hidden(false),
_disable(false),
_uiready(true),
_focus(false)
{
    ox = x;
    oy = y;
    ow = w;
    oh = h;
    
    if (t) {
        title = std::string(t);
    }
    if (parent) {
        parent->add_child(this);
    }
}

GUI_View::~GUI_View() {
    children.clear();
}

bool GUI_View::eventHandler(SDL_Event*event) {
    if (user_events_handler) {
        if (user_events_handler(event))
            return true;;
    }
    switch( event->type ) {
        case GUI_EventPaint:
            predraw();
            draw();
            postdraw();
            break;
        default:
            break;
    }
    for (std::vector<GUI_View *>::iterator it = children.begin() ; it != children.end(); ++it) {
        GUI_View *child = *it;
        child->eventHandler(event);
    }

    return false;
}

void GUI_View::add_child(GUI_View *child) {
    child->parent = this;
    
    children.push_back(child);
    
    updateLayout();
}

void GUI_View::remove_child(GUI_View *child) {
    for (std::vector<GUI_View *>::iterator it = children.begin() ; it != children.end(); ++it) {
        if( child == *it ) {
            children.erase( it );
            break;
        }
    }
}

void GUI_View::remove_all_children() {
    children.clear();
}


void GUI_View::predraw() {
    if (isVisible() == false)
        return;
    
    if (parent) {
        GUI_Rect parent_clip = GUI_Rect(parent->rectClip);
        parent_clip.x -= topLeft.x;
        parent_clip.y -= topLeft.y;
        SDL_IntersectRect(GUI_MakeRect(0, 0, rectView.w, rectView.h), &parent_clip, &rectClip);
        
        // Bug of SDL
        if (rectClip.w < 0)
            rectClip.w = 0;
        if (rectClip.h < 0)
            rectClip.h = 0;
    } else {
        rectClip = GUI_Rect(0, 0, rectView.w, rectView.h);
    }
    
#ifdef __EMSCRIPTENx__
    SDL_RenderSetViewport(GUI_renderer, GUI_MakeRect(rectView.x, GUI_windowHeight*GUI_scale - rectView.y - rectView.h, rectView.w, rectView.h));
#else
    SDL_RenderSetViewport(GUI_renderer, GUI_MakeRect(rectView.x, rectView.y, rectView.w, rectView.h));
#endif
    
#ifdef __EMSCRIPTENx__
    float magic_y = GUI_windowHeight*GUI_scale - rectView.y - rectView.h;
    
    SDL_RenderSetClipRect(GUI_renderer, GUI_MakeRect(rectView.x + rectClip.x,
                                                     0 - magic_y + rectClip.y,
                                                     rectClip.w,
                                                     rectClip.h));
#else
    SDL_RenderSetClipRect(GUI_renderer, GUI_MakeRect(rectClip.x,
                                                     rectClip.y,
                                                     rectClip.w,
                                                     rectClip.h));
#endif
    clear();
}

void GUI_View::draw() {

}

void GUI_View::postdraw() {
    if (border > 0) {
        GUI_Rect r = GUI_Rect(border, border, rectView.w - (2 * border), rectView.h - (2 * border));
        
        if (corner) {
            GUI_DrawRoundRect(0, 0, rectView.w, rectView.h, corner * GUI_scale, borderColor);
        } else {
            GUI_DrawRect(0, 0, rectView.w, rectView.h, borderColor);
        }
        
        SDL_IntersectRect(&r, &rectClip, &rectClip);
        
#ifdef __EMSCRIPTENx__
        float magic_y = GUI_windowHeight*GUI_scale - rectView.y - rectView.h;
        
        SDL_RenderSetClipRect(GUI_renderer, GUI_MakeRect(rectView.x + rectClip.x,
                                                         0 - magic_y + rectClip.y,
                                                         rectClip.w,
                                                         rectClip.h));
#else
        SDL_RenderSetClipRect(GUI_renderer, GUI_MakeRect(rectClip.x,
                                                         rectClip.y,
                                                         rectClip.w,
                                                         rectClip.h));
#endif
    }
}

void GUI_View::clear(GUI_Rect *rect) {
    if (!rect)
        rect = GUI_MakeRect(0, 0, rectView.w, rectView.h);
    
    if (backgroundColor.a != 0) {
        if (corner != 0) {
            GUI_FillRoundRect(rect->x, rect->y, rect->w, rect->h, corner * GUI_scale, backgroundColor);
        } else {
            GUI_FillRect(rect->x, rect->y, rect->w, rect->h, backgroundColor);
        }
    }
}

void GUI_View::updateLayout() {
    if (layout == GUI_LAYOUT_ABSOLUTE) {
        bool sz_changed = false;
        for (std::vector<GUI_View *>::iterator it = children.begin() ; it != children.end(); ++it) {
            GUI_View *child = *it;
            
            if (child->isVisible() == false)
                continue;
            
            if (child->align & GUI_ALIGN_RIGHT) {
                child->topLeft.x = rectView.w - child->rectView.w - (child->margin[1] + padding[1]) * GUI_scale;
                child->rectView.x = rectView.x + child->topLeft.x;
            } else if (child->align & GUI_ALIGN_CENTER) {
                child->topLeft.x = (rectView.w - child->rectView.w) / 2;
                child->rectView.x = rectView.x + child->topLeft.x;
            } else {
                if ((child->align & GUI_ALIGN_ABSOLUTE) == 0) { // Left Align || ow == 0
                    if (child->ox == 0) {
                        child->topLeft.x = (child->margin[3] + padding[3]) * GUI_scale;
                    } else {
                        child->topLeft.x = child->ox * GUI_scale;
                    }
                }
                
                child->rectView.x = rectView.x + child->topLeft.x;
                
                if (child->ow == -1) {
                    child->rectView.w = rectView.w - child->topLeft.x - (child->margin[1] + padding[1]) * GUI_scale;
                    //child->updateSubLayout();
                }
            }
            
            if (ow == 0) {
                int wwx = (child->rectView.w + (padding[1] + padding[3] + child->margin[1] + child->margin[3])*GUI_scale);
                
                if (wwx > rectView.w) {
                    rectView.w = wwx;
                    sz_changed = true;
                }
            }
            
            if (child->align & GUI_ALIGN_BOTTOM) {
                child->topLeft.y = rectView.h - child->rectView.h - (child->margin[2] + padding[2]) * GUI_scale;
                child->rectView.y = rectView.y + child->topLeft.y;
            } else if (child->align & GUI_ALIGN_VCENTER) {
                child->topLeft.y = (rectView.h - child->rectView.h) / 2;
                child->rectView.y = rectView.y + child->topLeft.y;
            } else {
                if ((child->align & GUI_ALIGN_ABSOLUTE) == 0) { // Top Align || oh == 0
                    if (child->oy == 0) {
                        child->topLeft.y = (child->margin[0] + padding[0]) * GUI_scale;
                    } else {
                        child->topLeft.y = child->oy * GUI_scale;
                    }
                }
                
                child->rectView.y = rectView.y + child->topLeft.y;
                
                if (child->oh == -1) {
                    child->rectView.h = rectView.h - child->topLeft.y - (child->margin[2] + padding[2]) * GUI_scale;
                    //child->updateSubLayout();
                }
            }
            
            if (oh == 0) {
                int hhx = (child->rectView.h + (padding[0] + padding[2] + child->margin[0] + child->margin[2])*GUI_scale);
                
                if (hhx > rectView.h) {
                    rectView.h = hhx;
                    sz_changed = true;
                }
            }
            
            if (ow == 0 || oh == 0) {
                if (sz_changed) {
                    int oww = ow;
                    int ohh = oh;
                    ow = rectView.w;
                    oh = rectView.h;
                    //updateSubLayout();
                    
                    if (align & (GUI_ALIGN_CENTER | GUI_ALIGN_RIGHT | GUI_ALIGN_BOTTOM | GUI_ALIGN_VCENTER)) {
                        if (parent) {
                            parent->updateLayout();
                        }
                    }
                    
                    ow = oww;
                    oh = ohh;
                }
            }
            
            child->updateLayout();
        }
    }
}
