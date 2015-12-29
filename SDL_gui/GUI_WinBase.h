//
//  GUI_WinBase.hpp
//  iOS
//
//  Created by Panutat Tejasen on 10/16/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#ifndef GUI_WinBase_hpp
#define GUI_WinBase_hpp

#include <stdio.h>
#include "SDL_gui.h"

struct GUI_WinBase {
private:
    bool hidden;
public:
    GUI_WinBase *parent;
    GUI_WinBase **children;
    const char *title_str;
    GUI_Point topleft; // relative to parent window
    GUI_Rect tw_area;  // relative to top window
    GUI_Rect clip_area; // clip rec
    SDL_Color bgcol;
    SDL_Color titleColor;
    int border;
    bool selected;
    bool dragging;
    GUI_Point lastMousePoint;
    SDL_Texture *titleTexture;
    GUI_Rect title_area;
    
    bool    canClose;
    int     canMove;
    bool    disable;
    
    bool isFocus;
    bool isMainWin;

    int tag;
    
    int lst_child,end_child;
    
    void (*display_cmd)(GUI_WinBase *);
    GUI_WinBase(GUI_WinBase *parent,const char *title,int x,int y,int width,int height,SDL_Color bgcol,void (*disp_cmd)(GUI_WinBase *)=NULL);
    virtual ~GUI_WinBase();

    virtual void add_child(GUI_WinBase* child);
    virtual void remove_child(GUI_WinBase* child);
    
    bool (*handle_event_cmd)(GUI_WinBase *w, SDL_Event *ev );
    bool (*onClose)(GUI_WinBase *w);
    virtual bool handleEvents( SDL_Event *ev );
    virtual void predraw();
    virtual void draw();
    void drawChildren();

    virtual GUI_WinBase *hitTest(int x,int y,bool bRecursive=true);

    void clear( GUI_Rect *rect = 0 );
    void clear( GUI_Rect*, SDL_Color col);
    
    void createTitleTexture( int fontSize=3 );

    //void set_parent( GUI_WinBase *pw );
    
    
    //void set_title(const char* new_t,bool _upd=true);
    virtual void hide();
    virtual void show();
    virtual void move(int dx,int dy);  // move win
    virtual bool close();
    
    bool isVisible() { return !hidden; };
    
    int inline getWidth() { return (tw_area.w); };
    int inline getHeight() { return (tw_area.h); };
};

#endif /* GUI_WinBase_hpp */
