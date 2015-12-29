//
//  GUI_MainWin.hpp
//  OSX
//
//  Created by Panutat Tejasen on 10/17/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#ifndef GUI_MainWin_hpp
#define GUI_MainWin_hpp

#include <stdio.h>
#include "SDL_gui.h"
#include "GUI_WinBase.h"
#include "GUI_BasicWidgets.h"

#endif /* GUI_MainWin_hpp */

struct GUI_MainWin:GUI_WinBase {
    GUI_MainWin(GUI_WinBase *parent,const char *title,int x,int y,int width,int height,SDL_Color bgcol=cRed,int _border=1,void (*client_display_cmd)(GUI_WinBase *)=NULL);
    ~GUI_MainWin();
    
    virtual void add_child(GUI_WinBase* child);
    virtual void remove_child(GUI_WinBase* child);

    virtual bool handleEvents(SDL_Event *ev);
    virtual GUI_WinBase *hitTest(int x,int y,bool bRecursive=true);

    
    GUI_WinBase *closeButton;
    GUI_WinBase *titleBar;
    GUI_WinBase *clientWin;
    
    
    void center();
    
};

struct GUI_Dialog:GUI_MainWin {
    GUI_Dialog(GUI_WinBase *parent,const char *title,int x,int y,int width,int height,SDL_Color bgcol=cDarkGrey,int _border=1,void (*client_display_cmd)(GUI_WinBase *)=NULL);
    ~GUI_Dialog();
    
    virtual void show();
};

const int max_dir = 64;
const int max_dirs = 2048;

struct GUI_FileDialog:GUI_Dialog {
    GUI_FileDialog(GUI_WinBase *parent, bool bSave, const char *ext, const char *defaultFN, bool (*file_cmd)(const char *)=NULL);
    ~GUI_FileDialog();

    const char *fileExtension;
    const char *defaultFilename;

    bool bNeedUpdate;
    char cur_wdir[512];
    char cd[512];

    char *dirName[max_dir];
    
    char dirs[max_dirs];
    char *dirList[max_dir];
    char *fileList[max_dir];
    
    GUI_Button *dirButton;
    GUI_List *dirListBox;
    GUI_List *fileListBox;
    GUI_List *menuDir;
    GUI_EditText *fileEdit;
    GUI_Button *bttnOK;
    
    bool (*file_cmd)(const char *);
    
    void setDirectory();
    
    bool bSaveDlg;
    
    virtual void draw();
};

struct GUI_TemplateDialog:GUI_Dialog {
    GUI_TemplateDialog(GUI_WinBase *parent, bool bSave, const char *ext, const char *defaultFN, bool (*file_cmd)(const char *)=NULL);
    ~GUI_TemplateDialog();
    
    const char *fileExtension;
    
    bool bNeedUpdate;
    char cur_wdir[512];
    char cd[512];
    
    char *fileList[max_dir];
    
    char dirs[max_dirs];

    GUI_List *fileListBox;
    GUI_Button *bttnOK;
    GUI_EditText *fileEdit;

    bool (*file_cmd)(const char *);
    
    void setDirectory();
    virtual void draw();
};

struct GUI_ColorDialog:GUI_Dialog {
    GUI_ColorDialog(GUI_WinBase *parent,void (*client_display_cmd)(GUI_ColorDialog *)=NULL);
    ~GUI_ColorDialog();
    
    GUI_WinBase *scrollBox;
    int activeColor;

    bool setActiveColor( SDL_Color c, bool visible=true );
    void (*selected_change)(GUI_ColorDialog *d, int pantoneCode, SDL_Color pantoneColor);
};

SDL_Color GUI_getPantoneRGB( int pantoneCode );

