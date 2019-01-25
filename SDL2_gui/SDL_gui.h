//
//  SDL_gui.hpp
//  TestSDLimage
//
//  Created by Panutat Tejasen on 21/12/2561 BE.
//  Copyright © 2561 Jimmy Software Co., Ltd. All rights reserved.
//

#ifndef SDL_gui_hpp
#define SDL_gui_hpp

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#if defined(WIN32)
#include "..\_VisualC\WinApp\WinApp\resource.h"
#include <SDL_syswm.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "SDL2_gfxPrimitives.h"

#include "GUI_App.h"
#include "GUI_Config.h"
#include "GUI_shapes.h"
#include "GUI_Utils.h"
#include "GUI_Fonts.h"
#include "GUI_View.h"
#include "GUI_image.h"
#include "GUI_ImageView.h"
#include "GUI_TextView.h"
#include "GUI_IconView.h"
#include "GUI_Label.h"
#include "GUI_Button.h"
#include "GUI_CheckBox.h"
#include "GUI_RadioButton.h"
#include "GUI_RadioButtonGroup.h"
#include "GUI_List.h"
#include "GUI_EditText.h"
#include "GUI_TextUtil.h"
#include "GUI_TopBar.h"
#include "GUI_StatusBar.h"
#include "GUI_Menu.h"
#include "GUI_MenuBar.h"
#include "GUI_PopupView.h"
#include "GUI_MessageBox.h"
#include "GUI_Switch.h"
#include "GUI_ComboBox.h"

int GUI_Init( const char* title, int expectedWidth, int expectedHeight );
void GUI_Run(std::function<bool(SDL_Event *ev)>handle_ev = NULL);
void GUI_Destroy();

extern SDL_Renderer *GUI_renderer;
extern SDL_Window *GUI_window;
extern GUI_View *GUI_topView;
//extern GUI_View * GUI_mouseCapturedView;

extern int GUI_physicalWindowWidth;
extern int GUI_physicalWindowHeight;

extern int GUI_windowWidth;
extern int GUI_windowHeight;

extern float GUI_scale;
extern float GUI_mouseScale;

extern void GUI_updateScaleParameters();

GUI_View *GUI_createTopView(const char* t, int x, int y, int w, int h,
                            std::function<bool(SDL_Event* ev)>userEventHandler = NULL);
void GUI_SetMouseCapture( GUI_View *v );
GUI_View * GUI_GetMouseCapture();


#endif /* SDL_gui_hpp */
