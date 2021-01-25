//
//  GUI_IconView.cpp
//  GUI_TextView
//
//  Created by Panutat Tejasen on 12/1/2562 BE.
//  Copyright © 2562 Jimmy Software Co., Ltd. All rights reserved.
//

#include "GUI_IconView.h"
#include "GUI_Fonts.h"
#include "skgui.h"
#include "include/core/SkSurface.h"
#include "include/core/SkImage.h"
#include "include/core/SkPaint.h"

GUI_IconView *GUI_IconView::create( GUI_View *parent, uint16_t unicode, const char *fontname, int fontsize, int x, int y, int width, int height,
                                   std::function<bool(SDL_Event* ev)>userEventHandler ) {
    return new GUI_IconView(parent, unicode, fontname, fontsize, x, y, width, height, userEventHandler );
}

GUI_IconView::GUI_IconView(GUI_View *parent, uint16_t unicode, const char *fontname, int fontsize, int x, int y, int width, int height,
                           std::function<bool(SDL_Event* ev)>userEventHandler) :
GUI_TextView(parent, NULL, fontname, fontsize, x, y, width, height, userEventHandler )
{
    mouseReceive = false;

    setIcon(unicode);
}

GUI_IconView::~GUI_IconView() {
    
}

void GUI_IconView::updateContent() {
//    SDL_Texture *texture = createTextureFormUnicode( icon );
//    if (texture == NULL){
//        GUI_Log("Could not create icon texture\n");
//        return;
//    }
//    image.setTexture(texture);
    
    auto img = createImageFromUnicode( icon );
    if(img==NULL){
        GUI_Log("Could not create icon image\n");
        return;
    }
    image.setImage(img);
    
    updateSize();
    if( parent ) {
        parent->updateLayout();
    }
    else {
        updateLayout();
    }
}

void GUI_IconView::setIcon( uint16_t unicode ) {
    icon = unicode;
    updateContent();
}

//SDL_Texture* GUI_IconView::createTextureFormUnicode(Uint16 unicode, SDL_Rect* rect) {
////    if (font) {
////        SDL_Surface* surf = TTF_RenderGlyph_Blended(font, unicode, cWhite);
////        SDL_Texture *tex = SDL_CreateTextureFromSurface(GUI_renderer, surf);
////
////        if (rect != NULL) {
////            rect->x = rect->y = 0;
////            rect->w = surf->w;
////            rect->h = surf->h;
////        }
////
////        SDL_FreeSurface(surf);
////        return tex;
////    }
//
//    return NULL;
//}

static void sk_release_direct_surface_storage(void* pixels, void* context){
    if (pixels == context) {
        SkDebugf("expected release context\n");
    }
    sk_free(pixels);
}

static sk_sp<SkSurface> MakeRasterDirectReleaseProc(int w, int h){
    SkImageInfo imageInfo = SkImageInfo::Make(w, h, SkColorType::kRGBA_8888_SkColorType, SkAlphaType::kUnpremul_SkAlphaType);
    size_t pixelLen = w * h * 4; // // it's 8888, so 4 bytes per pixel
    void* pixelPtr = sk_malloc_throw(pixelLen); //sk_malloc_canfail(pixelLen);
    if(pixelPtr){
        sk_sp<SkSurface> surface(SkSurface::MakeRasterDirectReleaseProc(imageInfo, pixelPtr, w*4,
                                                                        sk_release_direct_surface_storage, pixelPtr));
        return surface;
    }
    return nullptr;
}

sk_sp<SkImage> GUI_IconView::createImageFromUnicode(Uint16 unicode, GUI_Rect* rect){
    SkUnichar c = unicode;
    uint16_t g = font.unicharToGlyph(c);
    
    SkPath path;
    font.getPath(g, &path);
    SkRect bounds = path.getBounds();
    if(rect){
        rect->x = bounds.x();
        rect->y = bounds.y();
        rect->w = bounds.width();
        rect->h = bounds.height();
    }
    auto surface = MakeRasterDirectReleaseProc(bounds.width(), bounds.height());
    auto canvas = surface->getCanvas();
    canvas->translate(-bounds.x(), -bounds.y());
    SkPaint p;
    canvas->clear(cClear);
    canvas->drawPath(path, p);
    canvas->flush();
    return surface->makeImageSnapshot();
}

bool GUI_IconView::eventHandler(SDL_Event*event) {
    switch (event->type) {
        case GUI_FontChanged:
        {
            std::string fn;
            
            fn = _fontName;
            
            int fs = _fontSize;
            if( fs == 0 ) {
                fs = GUI_GetUIIconFontSize();
            }
            
            std::string fontPath = std::string("fonts/")+fn;
            font = GUI_Fonts::getFont(fn, fs);
            
            updateContent();
            updateSize();
            
            break;
        }
        default:
        {
            return GUI_ImageView::eventHandler(event);
        }
    }
    return false;
}
