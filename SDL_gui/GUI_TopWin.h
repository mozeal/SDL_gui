//
//  GUI_TopWin.hpp
//  OSX
//
//  Created by Panutat Tejasen on 10/16/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#ifndef GUI_TopWin_hpp
#define GUI_TopWin_hpp

#include <stdio.h>
#include "SDL_gui.h"
#include "GUI_WinBase.h"


struct GUI_TopWin:GUI_WinBase {
    GUI_TopWin(const char* title,GUI_Rect rect,Uint32 init_flag=0,void (*disp_cmd)(GUI_WinBase *)=NULL);
    
    virtual void draw();
    virtual bool handleEvents( SDL_Event *ev );

    void refresh(GUI_Rect *rect=0);
};

extern GUI_TopWin *GUI_topWin;

#endif /* GUI_TopWin_hpp */
