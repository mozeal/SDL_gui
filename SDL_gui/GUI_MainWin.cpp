//
//  GUI_MainWin.cpp
//  OSX
//
//  Created by Panutat Tejasen on 10/17/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h> // for chdir(), getcwd()
#include <dirent.h> // for DIR, dirent, opendir()
#include <stdlib.h> // for abs(), exit()
#include <pwd.h>
#include "pantone.h"

extern "C" char __iOS_DOCUMENTS_FOLDER[];

#include "GUI_MainWin.h"
#include "GUI_BasicWidgets.h"


static int titleBarSize = 32;
static int titleBarGap = 0;

GUI_MainWin::GUI_MainWin(GUI_WinBase *pw,const char *t,int x,int y,int dx,int dy,SDL_Color _bgcol,int _border,void (*client_d_cmd)(GUI_WinBase *)):
GUI_WinBase(pw,t,x,y,dx,dy,_bgcol),
titleBar(0),
closeButton(0),
clientWin(0)
{
    border = _border;
    canMove = 2;
    isMainWin = true;
    
#if __IPHONEOS__
    titleBarGap = 0;
    titleBarSize += titleBarGap;
#endif
    
    titleBar = new GUI_WinBase( this, t, border, border, dx-(2*border), titleBarSize-border, _bgcol,
                               []( GUI_WinBase *w ) {
                                   GUI_DrawLine( 0, w->tw_area.h-1, w->tw_area.w, w->tw_area.h-1, cBlack );
                                   GUI_Rect r = w->title_area;
                                   r.y += titleBarGap;
                                   SDL_RenderCopy(GUI_renderer, w->titleTexture, NULL, &r);
                               });
    titleBar->titleColor = cWhite;
    titleBar->createTitleTexture();
    titleBar->title_area.y -= titleBarGap / 2;
    
    closeButton = new GUI_WinBase( titleBar, "Close", dx-24-border, (titleBarSize-15)/2-border, 16, 15, cWhite,
        [](GUI_WinBase *w)
        {
            GUI_DrawRect2( GUI_MakeRect(0, 0, w->tw_area.w, w->tw_area.h), cBlack );
            SDL_RenderCopy(GUI_renderer, GUI_crossTexture, NULL, GUI_MakeRect(1, 1, w->tw_area.w-2, w->tw_area.h-2));
        }
    );
    
    clientWin = new GUI_WinBase( this, "Client", border, titleBarSize, dx-(2*border), dy-titleBarSize-border, cWhite, client_d_cmd );
}

GUI_MainWin::~GUI_MainWin()
{
    
}

void GUI_MainWin::add_child( GUI_WinBase *child ) {
    if( !clientWin ) {
        GUI_WinBase::add_child( child );
        return;
    }
    clientWin->add_child( child );
    child->parent = clientWin;
    child->tw_area.x = child->topleft.x+clientWin->tw_area.x;
    child->tw_area.y = child->topleft.y+clientWin->tw_area.y;
}

void GUI_MainWin::remove_child( GUI_WinBase *child ) {
    if( child == titleBar ) {
        GUI_WinBase::remove_child( child );
        return;
    }
    clientWin->remove_child( child );
}

GUI_WinBase *GUI_MainWin::hitTest(int x,int y,bool bRecursive) {
    return GUI_WinBase::hitTest(x, y, bRecursive);
}

bool GUI_MainWin::handleEvents(SDL_Event *ev) {
    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            if( titleBar != NULL ) {
                GUI_WinBase *wHit =  titleBar->hitTest(x, y,true);
                if( wHit == titleBar ) {
                    if( canMove == 2 ) {
                        dragging = true;
                        lastMousePoint.set( x, y );
                        GUI_mouseCapturedWindow = this;
                    }
                    return true;
                }
                else {
                    if( wHit ) {
                        return wHit->handleEvents(ev);
                    }
                }
            }
            GUI_WinBase *wHit =  clientWin->hitTest(x, y,false);
            if( wHit ) {
                return wHit->handleEvents(ev);
            }
            
            return false;
        }
        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent e = ev->button;
            int x = e.x*GUI_mouseScale/GUI_scale;
            int y = e.y*GUI_mouseScale/GUI_scale;
            if( titleBar != NULL ) {
                GUI_WinBase *wHit =  titleBar->hitTest(x, y,true);
                if( wHit == closeButton ) {
                    if( canClose ) {
                        close();
                        return true;
                    }
                    else {
                        hide();
                    }
                    return true;
                }

            }
        }
            
        default: {
            return GUI_WinBase::handleEvents(ev);
        }
    }
    return false;
}

void GUI_MainWin::center()
{
    int dx = ((parent->tw_area.w-tw_area.w)/2)-tw_area.x;
    int dy = ((parent->tw_area.h-tw_area.h)/2)-tw_area.y;
    
    move( dx, dy );
}

GUI_Dialog::GUI_Dialog(GUI_WinBase *pw,const char *t,int x,int y,int w,int h,SDL_Color c,int b,void (*cm)(GUI_WinBase *)):
GUI_MainWin( pw, t, x, y, w, h, c, b, cm )
{
    center();
    GUI_modalWindow = this;
}

GUI_Dialog::~GUI_Dialog() {
    
}

void GUI_Dialog::show()
{
    GUI_modalWindow = this;
    GUI_WinBase::show();
}

void GUI_FileDialog::setDirectory()
{
    const char *_docDir = GUI_getDocumentDirectory();
    strcpy( cur_wdir, _docDir );
    chdir( cur_wdir );

}

GUI_FileDialog::GUI_FileDialog(GUI_WinBase *pw, bool bSave, const char *ext, const char *defaultFN, bool (*cmd)(const char *)):
GUI_Dialog( pw, "", 0, 0, 480, 244 ),
dirButton(NULL),
menuDir(NULL),
dirListBox(NULL),
fileListBox(NULL),
fileEdit(NULL),
bttnOK(NULL),
bSaveDlg(bSave),
file_cmd(cmd),
fileExtension(ext),
defaultFilename(defaultFN)
{
    setDirectory();
    bNeedUpdate = true;
}

GUI_FileDialog::~GUI_FileDialog()
{
    
}

void GUI_FileDialog::draw()
{
    if( bNeedUpdate ) {
        bNeedUpdate = false;
        
        GUI_Log( ">> Dir: %s\n", cur_wdir );
        
        const char *tt;
        if( bSaveDlg ) {
            tt = "Save";
        }
        else {
            tt = "Open";
        }
        if( fileEdit ) {
            delete fileEdit;
        }
        fileEdit = new GUI_EditText( this, "", 5, 3, tw_area.w-60 );
        if( defaultFilename )
            fileEdit->setText( defaultFilename );
        
        if( bttnOK ) {
            delete bttnOK;
        }
        bttnOK = new GUI_Button( this, tt, tw_area.w-52, 4, 48, 30, cGrey,
                                [](GUI_Button *b) {
                                    GUI_FileDialog *dlg = (GUI_FileDialog *)b->parent->parent;
                                    if( dlg->fileEdit->title_str && dlg->fileEdit->title_str[0] ) {
                                        //getcwd(dlg->cur_wdir,512);
                                        strcat(dlg->cur_wdir, "/" );
                                        strcat(dlg->cur_wdir, dlg->fileEdit->title_str );
                                        if( dlg->fileExtension ) {
                                            if( strcmp( &dlg->cur_wdir[strlen(dlg->cur_wdir)-strlen(dlg->fileExtension)], dlg->fileExtension ) ) {
                                                strcat( dlg->cur_wdir, dlg->fileExtension );
                                            }
                                        }
                                        GUI_Log( "File: %s\n",dlg->cur_wdir );
                                        if( dlg->file_cmd ) {
                                            if( dlg->file_cmd( dlg->cur_wdir ) )
                                                dlg->close();
                                        }
                                    }
                                });
        if( !bSaveDlg ) {
            fileEdit->disable = true;
            fileEdit->titleColor = cGrey;
        }
        
        DIR *dp;
        if (!(dp=opendir(cur_wdir))) {
            GUI_Log("wdir not accessable");
            return;
        }
        dirent *dir;

        int str_index = 0;
        int str_count = 0;
        int file_count = 0;
        while ((dir=readdir(dp))!=0) {
            if (!dir->d_ino) {
                GUI_Log("d_ino?");
                continue;
            }
            switch (dir->d_type) {
                case DT_DIR:
                    //GUI_Log( "[%s]\n", dir->d_name );
                    if( dir->d_name[0] != '.' ) {
                        if( str_index + strlen(dir->d_name) < max_dirs-1 ) {
                            dirList[str_count++] = &dirs[str_index];
                            strcpy( &dirs[str_index], dir->d_name );
                            str_index += strlen(dir->d_name)+1;
                        }
                    }
                    break;
                case DT_REG:
                    GUI_Log( "%s\n", dir->d_name );
                    if( dir->d_name[0] != '.' ) {
                        if( str_index + strlen(dir->d_name) < max_dirs-1 ) {
                            if( fileExtension == NULL || (strlen( dir->d_name ) >= strlen( fileExtension) ) ) {
                                if( fileExtension == NULL || !strcmp( dir->d_name +strlen( dir->d_name)-strlen( fileExtension ), fileExtension ) ) {
                                    fileList[file_count++] = &dirs[str_index];
                                    strcpy( &dirs[str_index], dir->d_name );
                                    str_index += strlen(dir->d_name)+1;
                                }
                                else {
                                    GUI_Log( "no %s %s\n", dir->d_name, fileExtension );
                                }
                            }
                            else {
                                GUI_Log( "Short %i %i\n", strlen( &dirs[str_index]), strlen( fileExtension) );
                            }
                        }
                    }
                    break;
                case DT_LNK:
                    GUI_Log( "%s\n", dir->d_name );
                    break;
            }
            if( str_count >= max_dir-1 )
                break;
            if( file_count >= max_dir-1 )
                break;
        }
        dirList[str_count] = NULL;
        fileList[file_count] = NULL;
        closedir( dp );
        
        int by = 38;
        if( bSaveDlg ) {
            by = 38;
        }
        
        if( dirListBox ) {
            delete dirListBox;
        }
        dirListBox = new GUI_List( this, (const char **)dirList, str_count, -1, by-1, 180+2, this->tw_area.h-titleBarSize+1-by, 0, false,
            [](GUI_List *l,const char *sel, int index) {
                //GUI_Log( "Selected: %s %i %i\n", sel, index, l->selectedIndex);

                if( index == l->selectedIndex ) {
                    GUI_FileDialog *dlg = (GUI_FileDialog *)l->parent->parent;
                    getcwd(dlg->cur_wdir,512);
                    strcat( dlg->cur_wdir, "/" );
                    strcat( dlg->cur_wdir, sel );
                    chdir( dlg->cur_wdir );
                    dlg->bNeedUpdate = true;

                }
            }
        );
        dirListBox->title_str = "Dir List Box";
        
        if( fileListBox ) {
            delete fileListBox;
        }
        fileListBox = new GUI_List( this, (const char **)fileList, file_count, 180, by-1, tw_area.w-180-1, this->tw_area.h-titleBarSize+1-by, 0, false,
                                  [](GUI_List *l,const char *sel, int index) {
                                      if( index == l->selectedIndex ) {
                                          GUI_FileDialog *dlg = (GUI_FileDialog *)l->parent->parent;
                                          //getcwd(dlg->cur_wdir,512);
                                          //strcat( dlg->cur_wdir, "/" );
                                          //strcat( dlg->cur_wdir, sel );
                                          //GUI_Log( "File: %s\n", dlg->cur_wdir );
                                          if( dlg->fileEdit ) {
                                              dlg->fileEdit->setText( sel );
                                          }
                                          
                                      }
                                  }
                                  );
        fileListBox->title_str = "File List Box";
        
        int fc = 0;
        char cwd[512];
        strcpy( cwd, cur_wdir );
        for( long i=strlen( cwd )-1; i>=0; i-- ) {
            if( cwd[i] == '/' ) {
                dirName[fc++] = &cwd[i+1];
                cwd[i] = 0;
                if( fc >= max_dir-1 )
                    break;
            }
        }
        dirName[fc] = NULL;
        if( dirButton ) {
            delete dirButton;
        }
        
        dirButton = new GUI_Button( titleBar, dirName[0], 10, 0, 180, 0, cClear,
                                               [](GUI_Button *bt) {
                                                   GUI_FileDialog *dlg = (GUI_FileDialog *)bt->parent->parent;
                                                   if( dlg->menuDir->isVisible() )
                                                       dlg->menuDir->hide();
                                                   else
                                                       dlg->menuDir->show();
                                               }
                                               );
        dirButton->title_area.x = 10;
        
        
        menuDir = new GUI_List( this, (const char **)&dirName[1], fc-1,
                               10, 1, 160, 0, 0, true,
                               [](GUI_List *l,const char *sel, int index) {
                                   //GUI_Log( "Selected: %s %i\n", sel, index);
                                   
                                   GUI_FileDialog *dlg = (GUI_FileDialog *)l->parent->parent;
                                   //getcwd(dlg->cur_wdir,512);
                                   int fc = 0;
                                   for( long i=strlen( dlg->cur_wdir )-1; i>=0; i-- ) {
                                       if( dlg->cur_wdir[i] == '/' ) {
                                           dlg->cur_wdir[i] = 0;
                                           fc++;
                                           if( fc >= index+1 )
                                               break;
                                       }
                                   }
                                   //GUI_Log( "Dir: %s\n", dlg->cur_wdir );
                                   chdir( dlg->cur_wdir );
                                   dlg->bNeedUpdate = true;
                               }
                               );
        menuDir->canClose = false;
        menuDir->title_str = "Menu Dir";
        menuDir->hide();
        


    }
    GUI_Dialog::draw();
}

GUI_TemplateDialog::~GUI_TemplateDialog()
{
    
}


void GUI_TemplateDialog::setDirectory()
{
    
#if __ANDROID__
	strcpy( cur_wdir, "/mnt/sdcard/PhaThai/Templates" );
#else
    char *base_path = SDL_GetBasePath();
    const char *data_path;
    if (base_path) {
        data_path = base_path;
    } else {
        data_path = SDL_strdup("./");
    }
    
    
    strcpy( cur_wdir, data_path );
    strcat( cur_wdir, "data/Templates" );
#endif

    
}

GUI_TemplateDialog::GUI_TemplateDialog(GUI_WinBase *pw, bool bSave, const char *ext, const char *defaultFN, bool (*cmd)(const char *)):
GUI_Dialog( pw, "Templates", 0, 0, 480, 244 ),
fileListBox(NULL),
bttnOK(NULL),
fileEdit(NULL),
file_cmd(cmd),
fileExtension(ext)
{
    setDirectory();
    bNeedUpdate = true;
}

void GUI_TemplateDialog::draw()
{
    if( bNeedUpdate ) {
        bNeedUpdate = false;
        
        GUI_Log( ">> Dir: %s\n", cur_wdir );
        if( fileEdit ) {
            delete fileEdit;
        }
        fileEdit = new GUI_EditText( this, "", 5, 3, tw_area.w-60 );
        
        if( bttnOK ) {
            delete bttnOK;
        }
        bttnOK = new GUI_Button( this, "Open", tw_area.w-52, 4, 48, 30, cGrey,
                                [](GUI_Button *b) {
                                    GUI_TemplateDialog *dlg = (GUI_TemplateDialog *)b->parent->parent;
                                    if( dlg->fileEdit->title_str && dlg->fileEdit->title_str[0] ) {
                                        //getcwd(dlg->cur_wdir,512);
                                        strcat(dlg->cur_wdir, "/" );
                                        strcat(dlg->cur_wdir, dlg->fileEdit->title_str );
                                        GUI_Log( "File: %s\n",dlg->cur_wdir );
                                        if( dlg->file_cmd ) {
                                            if( dlg->file_cmd( dlg->cur_wdir ) )
                                                dlg->close();
                                        }
                                    }
                                });
        
        DIR *dp;
        if (!(dp=opendir(cur_wdir))) {
            GUI_Log("wdir not accessable");
            return;
        }
        dirent *dir;
        
        int str_index = 0;
        int str_count = 0;
        int file_count = 0;
        while ((dir=readdir(dp))!=0) {
            if (!dir->d_ino) {
                GUI_Log("d_ino?");
                continue;
            }
            switch (dir->d_type) {
                case DT_DIR:
                    //GUI_Log( "[%s]\n", dir->d_name );
                    break;
                case DT_REG:
                    GUI_Log( "%s\n", dir->d_name );
                    if( dir->d_name[0] != '.' ) {
                        if( str_index + strlen(dir->d_name) < max_dirs-1 ) {
                            if( fileExtension == NULL || (strlen( dir->d_name ) >= strlen( fileExtension) ) ) {
                                if( fileExtension == NULL || !strcmp( dir->d_name +strlen( dir->d_name)-strlen( fileExtension ), fileExtension ) ) {
                                    fileList[file_count++] = &dirs[str_index];
                                    strcpy( &dirs[str_index], dir->d_name );
                                    str_index += strlen(dir->d_name)+1;
                                }
                                else {
                                    GUI_Log( "no %s %s\n", dir->d_name, fileExtension );
                                }
                            }
                            else {
                                GUI_Log( "Short %i %i\n", strlen( &dirs[str_index]), strlen( fileExtension) );
                            }
                        }
                    }
                    break;
                case DT_LNK:
                    GUI_Log( "%s\n", dir->d_name );
                    break;
            }
            if( str_count >= max_dir-1 )
                break;
            if( file_count >= max_dir-1 )
                break;
        }
        fileList[file_count] = NULL;
        closedir( dp );
        
        int by = 38;
        if( fileListBox ) {
            delete fileListBox;
        }
        fileListBox = new GUI_List( this, (const char **)fileList, file_count, 180, by-1, tw_area.w-180-2, this->tw_area.h-titleBarSize+1-by-1, 0, false,
                                   [](GUI_List *l,const char *sel, int index) {
                                       if( index == l->selectedIndex ) {
                                           GUI_TemplateDialog *dlg = (GUI_TemplateDialog *)l->parent->parent;
                                           //getcwd(dlg->cur_wdir,512);
                                           //strcat( dlg->cur_wdir, "/" );
                                           //strcat( dlg->cur_wdir, sel );
                                           //GUI_Log( "File: %s\n", dlg->cur_wdir );
                                           if( dlg->fileEdit ) {
                                               dlg->fileEdit->setText( sel );
                                           }
                                           
                                       }
                                   }
                                   );
        fileListBox->title_str = "File List Box";

    }
    GUI_Dialog::draw();
    
}


static int cellWidth = 24;
static int cellHeight = 24;

static int cellPerRow = 24;
static int numRow = ((num_pantone+cellPerRow-1)/cellPerRow);

GUI_ColorDialog::GUI_ColorDialog(GUI_WinBase *pw,void (*client_display_cmd)(GUI_ColorDialog *)):
GUI_Dialog( pw, "", 0, 0, 578, 256 ),
selected_change(NULL),
activeColor(-1)
{
    num_pantone = num_pantone;
    
    static int scrollWidth = cellWidth * cellPerRow;
    static int scrollHeight = cellHeight * numRow;
    
    scrollBox = new GUI_WinBase( this->clientWin, "Scroll", 0, 0, scrollWidth, scrollHeight, cDarkGrey,
        [](GUI_WinBase *w) {
            for( int i=0; i<num_pantone; i++ ) {
                int nxx = i % cellPerRow;
                int nyy = i / cellPerRow;
                
                if( nyy + cellHeight < 0 )
                    continue;
                if( nyy > w->tw_area.h )
                    break;
                GUI_FillRect(1+nxx*cellWidth, 1+nyy*cellHeight, cellWidth-2, cellHeight-2, pantone_color[i] );
                GUI_ColorDialog *cd = (GUI_ColorDialog *)w->parent->parent;
                if( i == cd->activeColor ) {
                    GUI_DrawRect( 1+nxx*cellWidth, 1+nyy*cellHeight, cellWidth-2, cellHeight-2, cWhite );
                    GUI_DrawRect( nxx*cellWidth, nyy*cellHeight, cellWidth, cellHeight, cWhite );
                }
            }
        }
    );
    scrollBox->handle_event_cmd = [](GUI_WinBase *w, SDL_Event* ev ) -> bool {
        
        static int lastMouseX;
        static int lastMouseY;
        static bool bMousePress = false;
        static long mouseClickTime;
        static bool dragging = false;
        
        switch (ev->type) {
            case SDL_MOUSEBUTTONDOWN: {
                SDL_MouseButtonEvent e = ev->button;
                int x = e.x*GUI_mouseScale/GUI_scale;
                int y = e.y*GUI_mouseScale/GUI_scale;
                lastMouseX = x;
                lastMouseY = y;
                //GUI_Log( "Last: %i, %i\n", lastMouseX, lastMouseY );
                bMousePress = true;
                mouseClickTime = SDL_GetTicks();
                GUI_mouseCapturedWindow = w;
                return true;
            }
            case SDL_MOUSEMOTION: {
                SDL_MouseButtonEvent e = ev->button;
                int x = e.x*GUI_mouseScale/GUI_scale;
                int y = e.y*GUI_mouseScale/GUI_scale;
                //GUI_Log( "Last2: %i, %i\n", lastMouseX, lastMouseY );
                if( bMousePress ) {
                    if( abs(x-lastMouseX) > 5 || abs(y-lastMouseY) > 5 ) {
                        bMousePress = false;
                        dragging = true;
                    }
                }
                if( dragging ) {
                    int diffY = y - lastMouseY;
                    w->move( 0, diffY );
                    if( w->tw_area.y-w->parent->tw_area.y > 0 )
                    {
                        w->move(0, w->parent->tw_area.y-w->tw_area.y );
                    }
                    if( w->tw_area.y+w->tw_area.h < w->parent->tw_area.y+w->parent->tw_area.h ) {
                        w->move(0, (w->parent->tw_area.y+w->parent->tw_area.h)-(w->tw_area.y+w->tw_area.h) );
                    }
                    lastMouseX = x;
                    lastMouseY = y;
                }
                return true;
            }
            case SDL_MOUSEBUTTONUP: {
                if( dragging ) {
                    dragging = false;
                }
                GUI_mouseCapturedWindow = NULL;
                if( !bMousePress )
                    break;
                SDL_MouseButtonEvent e = ev->button;
                int x = e.x*GUI_mouseScale/GUI_scale;
                int y = e.y*GUI_mouseScale/GUI_scale;
                //GUI_Log( "Last2: %i, %i\n", lastMouseX, lastMouseY );
                if( abs(x-lastMouseX) > 5 || abs(y-lastMouseY) > 5 )
                    break;
                
                y -= (w->tw_area.y);
                x -= (w->tw_area.x);
                
                int color_index = (y/cellHeight) * cellPerRow + (x/cellWidth);
                //GUI_Log( "Color: %i\n", color_index );
                int pcode = pantone_code[color_index];
                SDL_Color pcolor = pantone_color[color_index];
                GUI_ColorDialog *cd = (GUI_ColorDialog *)w->parent->parent;
                cd->setActiveColor( pcolor, false );
                if( cd->selected_change ) {
                    cd->selected_change( cd, pcode, pcolor );
                }

                bMousePress = false;
                return true;
            }
        }
        return false;
    };
}

GUI_ColorDialog::~GUI_ColorDialog()
{
    
}

bool GUI_ColorDialog::setActiveColor( SDL_Color c, bool visible )
{
    for( int i=0; i<num_pantone; i++ ) {
        if( pantone_color[i].r == c.r &&
            pantone_color[i].g == c.g &&
            pantone_color[i].b == c.b &&
            pantone_color[i].a == c.a ) {
            activeColor = i;
            if( visible ) {
                int y = (i / cellPerRow) - 2;
                if( y < 0 )
                    y = 0;
                y = 0-(y*cellHeight);
                scrollBox->move( 0, y-(scrollBox->tw_area.y-scrollBox->parent->tw_area.y) );
                
                if( scrollBox->tw_area.y-scrollBox->parent->tw_area.y > 0 )
                {
                    scrollBox->move(0, scrollBox->parent->tw_area.y-scrollBox->tw_area.y );
                }
                if( scrollBox->tw_area.y+scrollBox->tw_area.h < scrollBox->parent->tw_area.y+scrollBox->parent->tw_area.h ) {
                    scrollBox->move(0, (scrollBox->parent->tw_area.y+scrollBox->parent->tw_area.h)-(scrollBox->tw_area.y+scrollBox->tw_area.h) );
                }

            }
            static char str[128];
            sprintf( str, "Pantone: %i", pantone_code[i] );
            titleBar->title_str = str;
            titleBar->createTitleTexture();
            return true;
        }
    }
    return false;
}


SDL_Color GUI_getPantoneRGB( int pantoneCode )
{
    int l = sizeof( pantone_code ) / sizeof( pantone_code[0] );
    for( int i=0; i<l; i++ ) {
        if( pantone_code[i] == pantoneCode )
            return pantone_color[i];
    }
    return sdl_color( 0 );
}


