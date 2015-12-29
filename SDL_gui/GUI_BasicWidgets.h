//
//  GUI_BasicWidgets.hpp
//  OSX
//
//  Created by Panutat Tejasen on 11/1/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#ifndef GUI_BasicWidgets_hpp
#define GUI_BasicWidgets_hpp
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <SDL.h>
#include "SDL_gui.h"
#include "GUI_WinBase.h"

struct GUI_Label:GUI_WinBase {
    GUI_Label( GUI_WinBase *parent, const char *title, int x=0, int y=0, int width=0, SDL_Color color=cBlack, int align=0, int fontSize=3 );
    ~GUI_Label();
    
    int format;
    int padding;
    
    virtual void draw();
};

struct GUI_Button:GUI_WinBase {
    void (*cmd)(GUI_Button*);

    GUI_Button( GUI_WinBase *parent, const char *title, int x=0, int y=0, int width=0, int height=0, SDL_Color color=cBlue, void (*cmd)(GUI_Button*)=NULL );
    virtual ~GUI_Button();
    
    int padding;
    bool isDown;
    int radius;
    SDL_Color color;
    SDL_Color borderColor;
    SDL_Texture *texImage;
    
    virtual void draw();
    virtual bool handleEvents( SDL_Event *ev );
};

struct GUI_EditText:GUI_WinBase {
    void (*cmd)(GUI_EditText*);
    
    GUI_EditText( GUI_WinBase *parent, const char *text, int x=0, int y=0, int width=0, void (*cmd)(GUI_EditText*)=NULL );
    ~GUI_EditText();
    
    int padding;
    
    std::string text;
    int text_index;
    
    virtual void draw();
    virtual bool handleEvents( SDL_Event *ev );
    
    void setText( const char *t );
};

struct GUI_ListCell:GUI_WinBase {
    GUI_ListCell( GUI_WinBase *parent, const char *text, int x, int y, int width, int height, bool checkable=false );
    ~GUI_ListCell();
    
    virtual void draw();
    
    bool checkable;
    bool checked;
    bool separator;
};

struct GUI_List:GUI_WinBase {
    void (*cmd)(GUI_List*,const char *sel,int index);
    
    GUI_List( GUI_WinBase *parent, const char **text, int num, int x=0, int y=0, int width=0, int height=0, int cellHeight= 0, bool popup=false, void (*cmd)(GUI_List*,const char*,int)=NULL );
    ~GUI_List();
    
    bool popupMode;
    int padding;
    int selectedIndex;
    int corner;
    bool border;
    bool scrollable;
    int cellHeight;
    SDL_TimerID timerID;
    
    GUI_WinBase *scrollPad;
    
    const char **menuText;
    std::vector<GUI_ListCell *> vMenu;
    int numCells;
    
    int setCheck( int tag );
    
    void selectChanged( int n );
    
    int lastMouseX;
    int lastMouseY;
    
    bool bMousePress;
    Uint32 mouseClickTime;
    
    virtual void hide();
    virtual void show();
    
    virtual void draw();
    virtual bool handleEvents( SDL_Event *ev );
    
    GUI_ListCell *getCell( int tag );
};

#endif /* GUI_BasicWidgets_hpp */
