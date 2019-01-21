//
//  GUI_TextView.cpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_TextView.h"
#include "GUI_Fonts.h"

GUI_TextView *GUI_TextView::create( GUI_View *parent, const char *title, const char *fontname, int fontsize, int x, int y, int width, int height,
                                     std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_TextView(parent, title, fontname, fontsize, x, y, width, height, userEventHandler );
}

GUI_TextView::GUI_TextView( GUI_View *parent, const char *title, const char *fontname, int fontsize, int x, int y, int width, int height,
                             std::function<bool(SDL_Event* ev)>userEventHandler):
GUI_ImageView( parent, title, NULL, x, y, width, height, userEventHandler),
font(NULL),
forceEmptyText(false)
{
    mouseReceive = false;
    
    std::string fn;
    
    if( fontname ) {
        fn = fontname;
    }
    else {
        fn = GUI_UITextFontName;
    }
    int fs = fontsize;
    if( fs == 0 ) {
        fs = GUI_UIIconFontSize;
    }

    std::string fontPath = std::string("data/")+fn;
    font = GUI_Fonts::getFont(fn, fontsize);
    
    if (!font) {
        GUI_Log("font-spec %s not found\n", fontPath.c_str());
        return;
    }
    
    updateContent();
}

void GUI_TextView::updateContent() {
    //We need to first render to a surface as that's what TTF_RenderText returns, then
    //load that surface into a texture
    SDL_Surface *surf;
    
    if( title.length() > 0 )
        surf = TTF_RenderUTF8_Blended(font, title.c_str(), cWhite);
    else
        surf = TTF_RenderUTF8_Blended(font, " ", cWhite);
    if (surf == NULL){
        GUI_Log("Could not create text surface\n");
        return;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(GUI_renderer, surf);
    if (texture == NULL){
        GUI_Log("Could not create text texture\n");
        return;
    }
    image.setTexture(texture);
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    
    updateSize();
    if( parent ) {
        parent->updateLayout();
    }
    else {
        updateLayout();
    }
}

void GUI_TextView::updateSize() {
    GUI_ImageView::updateSize();
    if( forceEmptyText ) {
        if( ow == 0 ) {
            rectView.w = 1 + (_padding[1] + _padding[3]) * GUI_scale;
        }
        if( oh == 0 ) {
            rectView.h = TTF_FontHeight(font) + (_padding[0] + _padding[2]) * GUI_scale;
        }
    }
}

GUI_TextView::~GUI_TextView() {
    
}

void GUI_TextView::setTextColor( SDL_Color c ) {
    _textColor = c;
    colorMod = c;
}

GUI_FPSView *GUI_FPSView::create( GUI_View *parent, int x, int y, int width, int height ) {
    return new GUI_FPSView( parent, x, y, width, height );
}

GUI_FPSView::GUI_FPSView(GUI_View *parent, int x, int y, int width, int height) :
GUI_TextView( parent, NULL, GUI_UITextFontName.c_str(), GUI_UITextFontSize, x, y, width, height ),
frame_count(0),
timer_start(0)
{
    border = 0;
    setBackgroundColor(cClear);
}

GUI_FPSView::~GUI_FPSView() {
    
}

void GUI_FPSView::update() {
    if( timer_start == 0 ) {
        timer_start = SDL_GetTicks();
        return;
    }
    frame_count++;
    Uint32 duration = SDL_GetTicks() - timer_start;
    
    if (frame_count % 10 == 0) {
        float fps = (float)frame_count / duration * 1000.0f;
        char fps_text[16];
        sprintf(fps_text, "%0.2f fps", fps);
        setTitle(fps_text);
    }
}
