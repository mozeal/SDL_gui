//
//  main.c
//  OSX
//
//  Created by Panutat Tejasen on 10/14/2558 BE.
//  Copyright © 2558 Jimmy Software Co.,Ltd. All rights reserved.
//

#include <iostream>
#include <string>
#include  <stdio.h>
#include  <stdlib.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "SDL_gui.h"
#include "GUI_TopWin.h"
#include "GUI_MainWin.h"
#include "GUI_utils.h"
#include "GUI_BasicWidgets.h"

static GUI_TopWin *topWin = NULL;
static GUI_List *menuFile = NULL;

void setActiveColor(GUI_Button*b);
void setActiveSize(GUI_Button*b);
GUI_ColorDialog *colorDlg = NULL;

const int numColor = 8;
SDL_Color mColor[numColor] = {
    { 249,191,193, 255 },
    { 112,206,226, 255 },
    { 232,117,17, 255 },
    { 249,229,91, 255 },
    { 30,181,58, 255 },
    { 232,117,17, 255 },
    { 249,229,91, 255 },
    { 30,181,58, 255 }
};
GUI_Button *c[numColor];
int activeColor = -1;

const int numSize = 3;
GUI_Button *s[numSize];
int activeSize = -1;

const int numPalette = 5;
SDL_Color mainPalette[numPalette][numColor];
int activePalette = -1;

static char preferenctPath[512];

void OpenFile( void ) {
    new GUI_FileDialog( topWin, false, "*.txt", "Untitled.txt" );
}

SDL_Texture *textureFileIcon = NULL;

void OpenColor( void ) {
    if( colorDlg == NULL ) {
        colorDlg = new GUI_ColorDialog( topWin );
        colorDlg->selected_change = [](GUI_ColorDialog *d, int pantoneCode, SDL_Color pantoneColor )
        {
            GUI_Log( "Color: %i (%i,%i,%i)\n", pantoneCode, pantoneColor.r, pantoneColor.g, pantoneColor.b );
            mainPalette[activePalette][activeColor] = pantoneColor;
            c[activeColor]->color = pantoneColor;
        };
        colorDlg->onClose = [](GUI_WinBase *w) -> bool {
            colorDlg = NULL;
            return true;
        };
    }
    else {
        if( colorDlg->isVisible() )
            colorDlg->hide();
        else
            colorDlg->show();
    }
    colorDlg->setActiveColor( mainPalette[activePalette][activeColor] );
}

void createTestWindows( int x, int y, const char *title, SDL_Color bgcol ) {
    GUI_MainWin *redWin = new GUI_MainWin(topWin, title, x, y, 480, 240+32, bgcol);
    redWin->border = 1;
}

void cellDraw( GUI_WinBase * w ) {
    GUI_ListCell *c = (GUI_ListCell *)w;
    c->predraw();
    
    if( c->selected ) {
        SDL_SetTextureColorMod( c->titleTexture, 255, 255, 255 );
        GUI_FillRect2( GUI_MakeRect(0, 0, c->tw_area.w, c->tw_area.h), cDarkGrey );
    }
    else {
        SDL_SetTextureColorMod( c->titleTexture, 0, 0, 0 );
    }
    if( c->checkable ) {
        c->title_area.x = 8+16;
    }
    else {
        c->title_area.x = 8;
    }
    SDL_RenderCopy(GUI_renderer, c->titleTexture, NULL, &c->title_area);
    
    if( c->checked ) {
        SDL_RenderCopy(GUI_renderer, GUI_checkTexture, NULL, GUI_MakeRect(3, 4, 16, 13));
    }
    
    for( int i=0; i<numColor; i++ ) {
        GUI_FillRect2( GUI_MakeRect(98+(i*16), 5, 16, 16), mainPalette[c->tag][i] );
    }
}


void savePalette( const char *fn ) {
    SDL_RWops *rw = SDL_RWFromFile(fn, "w");
    if (rw != NULL) {
        for( int i=0; i<numPalette; i++ ) {
            for( int j=0; j<numColor; j++ ) {
                Uint32 c = (mainPalette[i][j].r << 24) | (mainPalette[i][j].g << 16) | (mainPalette[i][j].b << 8) | 0xFF;
                SDL_WriteBE32(rw, c);
            }
        }
        SDL_RWclose(rw);
    }
    
}

void loadPalette( const char *fn ) {
    SDL_RWops *rw = SDL_RWFromFile(fn, "r");
    if (rw != NULL) {
        for( int i=0; i<numPalette; i++ ) {
            for( int j=0; j<numColor; j++ ) {
                Uint32 c = SDL_ReadBE32(rw);
                mainPalette[i][j] = sdl_color(c);
            }
        }
        SDL_RWclose(rw);
    }
}

Uint32 ExitCallback( Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_zero(event);
    
    event.user.type = SDL_QUIT;
    SDL_PushEvent( &event );
    return 0;
}

Uint32 SavePaletteCallback( Uint32 interval, void *param)
{
    new GUI_FileDialog( topWin, true, "*.txt", "Untitled.txt", [](const char *fn) -> bool {
        GUI_Log( "File Save Pal: %s\n", fn );
        savePalette( fn );
        return true;
    });

    return 0;
}

Uint32 LoadPaletteCallback( Uint32 interval, void *param)
{
    new GUI_FileDialog( topWin, false, "*.txt", "Untitled.txt",  [](const char *fn) -> bool {
        GUI_Log( "File Load Pal: %s\n", fn );
        loadPalette( fn );
        for( int i=0; i<numColor; i++ ) {
            c[i]->color = mainPalette[activePalette][i];
        }
        return true;
    });
    
    return 0;
}

SDL_Texture* loadTexture(const std::string &filename, SDL_Renderer *ren)
{
    std::string imagePath = GUI_getResourcePath() + filename;
    
    GUI_Log("Load:%s\n", imagePath.c_str() );
    SDL_Texture *tex = IMG_LoadTexture(ren, imagePath.c_str());
    if (tex == NULL){
        GUI_Log( "Load image %s failed.", filename.c_str() );
    }
    return tex;
}



int
main(int argc, char *argv[])
{
    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        GUI_Log("Could not initialize SDL");
    }
    if (TTF_Init() != 0){
        GUI_Log( "TTF_Init failed.");
        SDL_Quit();
        return 1;
    }
    if( GUI_Init() != 0 ) {
        GUI_Log( "GUI_Init failed." );
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    char *pref = SDL_GetPrefPath( "JimmySoftware.com", "ToolBarDialog" );
    strcpy( preferenctPath, pref );
    strcat( preferenctPath, "preference.conf" );
    
    SDL_Log( "Pref: %s\n", preferenctPath );
    if( access( preferenctPath, 0 ) != -1 ) {
        GUI_Log( "File Existed\n" );
        loadPalette( preferenctPath );
    } else {
        GUI_Log( "File not existed\n" );
        for( int i=0; i<numPalette; i++ ) {
            for( int j=0; j<numColor; j++ ) {
                mainPalette[i][j] = mColor[j];
            }
        }
    }
    activePalette = 0;
    
    //int sx = 720, sy = 405;
#if __IPHONEOS__ or __ANDROID__
    int sx = 640, sy = 360;
#else
    int sx = 640, sy = 360;
#endif
    SDL_Window *window = GUI_CreateWindow( "GUI", sx, sy );
    if (window == NULL) {
        fatalError("GUI_CreateWindow Error");
        return 1;
    }
    
    SCREEN_WIDTH = GUI_windowWidth;
    SCREEN_HEIGHT = GUI_windowHeight;
    
    topWin=new GUI_TopWin( "TopWin", GUI_Rect(0,0,GUI_windowWidth,GUI_windowHeight),0,
       []( GUI_WinBase *tw ) {
           //tw->clear();
       }
    );
    
    textureFileIcon = loadTexture( "icon_file.png", GUI_renderer );

    
    GUI_WinBase *topBar = new GUI_WinBase( topWin, "TopBar", 0, 0, GUI_windowWidth, 48, cDarkGrey );
    if( topBar ) {
        GUI_Button *bttnFile = new GUI_Button( topBar, "F", 8, 8, 32, 32, cGrey,
            [](GUI_Button *bt) {
                if( menuFile == NULL ) {
                    GUI_Log( "create new MenuFile\n" );
                    static const char *menuText[] = {
                        "File", "Open", "Save", "Exit", NULL
                    };
                    menuFile = new GUI_List( topWin, menuText, 4, bt->tw_area.x, bt->tw_area.y+bt->tw_area.w+10, 240, 0, 0, true,
                        [](GUI_List *l,const char *sel, int index) {
                            switch (index) {
                                case 1:
                                    OpenFile();
                                    break;
                                case 3:
                                    SDL_AddTimer( 100, ExitCallback, NULL );
                                    break;
                                default:
                                    break;
                            }
                            menuFile = NULL;
                        }
                    );
                    menuFile->title_str = "Menu File";
                    menuFile->onClose = [] (GUI_WinBase *) -> bool {
                        menuFile = NULL;
                        return true;
                    };
                }
            }
        );
        bttnFile->texImage = textureFileIcon;
        bttnFile->color = cClear;
        
        new GUI_Button( topBar, "C", 48, 8, 32, 32, cGrey,
            [](GUI_Button *bt) {
                OpenColor();
            }
        );
        
        GUI_WinBase *colorBox = new GUI_WinBase( topBar, "สี", 88, 6, 32*9+18, 36, cClear,
                                                [](GUI_WinBase *wb) {
                                                    GUI_FillRoundRect( 0, 0, wb->tw_area.w, wb->tw_area.h, wb->tw_area.h/2, sdl_color( 0x484848ff ) );
                                                }
                                                );
        if( colorBox ) {
            for( int i=0; i<numColor; i++ ) {
                c[i] = new GUI_Button( colorBox, "", 6+32*i, 3, 31, 31, mainPalette[activePalette][i], setActiveColor );
                c[i]->tag = i;
                c[i]->radius = 15;
                c[i]->border = 1;
            }
            
            setActiveColor( c[0] );
            
            GUI_WinBase *paletteButton = new GUI_WinBase( colorBox, "Palette", 6+32*numColor+5, 3+8, 25, 15, cClear,
                [](GUI_WinBase *w)
                {
                    //GUI_DrawRect( GUI_MakeRect(0, 0, w->tw_area.w, w->tw_area.h), cBlack );
                    SDL_RenderCopy(GUI_renderer, GUI_dropdownTexture, NULL, GUI_MakeRect(1, 1, w->tw_area.w-2, w->tw_area.h-2));
                }
            );
            paletteButton->handle_event_cmd = [](GUI_WinBase *w, SDL_Event* ev ) -> bool {
                switch (ev->type) {
                    case SDL_MOUSEBUTTONUP: {
                        static const char *menuPalette[] = {
                            "Palette1", "Palette2", "Palette3", "Palette4", "Palette5", "---", "Load", "Save", NULL
                        };

                        //GUI_Log( "Palette\n" );
                        GUI_List *listPal = new GUI_List( topWin, menuPalette, 8, w->tw_area.x, w->tw_area.y+w->tw_area.h+18, 240, 0, 0, true,
                                     [](GUI_List* l,const char*sz,int n) {
                                         if( n < numPalette ) {
                                             activePalette = n;
                                             l->setCheck( n );
                                             for( int i=0; i<numColor; i++ ) {
                                                 c[i]->color = mainPalette[n][i];
                                             }
                                         }
                                         else if( n == numPalette + 1 ) {
                                             SDL_AddTimer( 100, LoadPaletteCallback, NULL );
                                         }
                                         else if( n == numPalette + 2 ) {
                                             SDL_AddTimer( 100, SavePaletteCallback, NULL );
                                         }
                                     });
                        for( int i=0; i<numPalette; i++ ) {
                            GUI_ListCell *cell = listPal->getCell( i );
                            cell->checkable = true;
                            cell->display_cmd = cellDraw;
                        }
                        listPal->setCheck( activePalette );
                    }
                }
                return false;
            };

        }
    }
    
    GUI_WinBase *sizeBox = new GUI_WinBase( topBar, "สี", 402, 6, 32*3+6, 36, cClear,
                                            [](GUI_WinBase *wb) {
                                                GUI_FillRoundRect( 0, 0, wb->tw_area.w, wb->tw_area.h, 10, sdl_color( 0x484848ff ) );
                                            }
                                            );
    if( sizeBox ) {
        for( int i=0; i<numSize; i++ ) {
            s[i] = new GUI_Button( sizeBox, "", 3+32*i, 3, 30, 30, cClear, setActiveSize );
            s[i]->tag = i;
            s[i]->radius = 5;
            s[i]->border = 1;
            s[i]->display_cmd = [](GUI_WinBase *w) {
                int bw = 12 + w->tag * 6;
                int cw = 0;
                if( w->tag == 1 )
                    cw -= 3;
                GUI_FillRect( cw+(w->tw_area.w-bw)/2, (w->tw_area.h-bw)/2, bw, bw, cGrey );
                GUI_DrawRect( cw+(w->tw_area.w-bw)/2, (w->tw_area.h-bw)/2, bw, bw, ((GUI_Button*)w)->borderColor );
            };
        }
        setActiveSize( s[0] );
    }
    
    GUI_WinBase *bottomBar = new GUI_WinBase( topWin, "BottomBar", 0, GUI_windowHeight-48, GUI_windowWidth, 48, cDarkGrey );


    //createTestWindows( 20, 20, "Red", cRed );

    
    GUI_Run();
    
    savePalette( preferenctPath );
    
    
    
    /* shutdown SDL */
    GUI_Quit();
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}

void setActiveColor(GUI_Button*b)
{
    if( activeColor == b->tag ) {
        OpenColor();
    }
    else {
        activeColor = b->tag;
        for( int i=0; i<numColor; i++ ) {
            if( c[i]->tag == activeColor )
                c[i]->borderColor = cWhite;
            else
                c[i]->borderColor = cBlack;
        }
        if( colorDlg && colorDlg->isVisible() ) {
            colorDlg->setActiveColor( mainPalette[activePalette][activeColor] );
        }
    }
}

void setActiveSize(GUI_Button*b)
{
    activeSize = b->tag;
    for( int i=0; i<numSize; i++ ) {
        if( s[i]->tag == activeSize )
            s[i]->borderColor = cWhite;
        else
            s[i]->borderColor = cBlack;
    }
}
